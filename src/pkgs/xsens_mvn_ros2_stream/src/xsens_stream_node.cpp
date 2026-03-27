// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#include <chrono>
#include <memory>
#include <string>

#include <diagnostic_msgs/msg/diagnostic_status.hpp>
#include <rclcpp/time.hpp>
#include <rclcpp/rclcpp.hpp>
#include <tf2_eigen/tf2_eigen.hpp>
#include <xsens_mvn_ros2_common/skeleton_publisher.hpp>
#include <xsens_mvn_ros2_common/xsens_model.hpp>
#include <xsens_mvn_ros2_stream/xsens_stream_client.hpp>
#include <xsens_mvn_ros2_stream/xsens_stream_node.hpp>

namespace xsens_mvn_ros2
{

XsensStreamNode::XsensStreamNode()
: rclcpp_lifecycle::LifecycleNode("xsens_mvn_ros2_stream_node")
{
}

XsensStreamNode::~XsensStreamNode()
{
  RCLCPP_INFO_STREAM(this->get_logger(), "Shutting down.");
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensStreamNode::on_configure(const rclcpp_lifecycle::State &)
{
  m_paramListener = std::make_shared<xsens_stream_node::ParamListener>(
    get_node_parameters_interface());

  m_comPublisher = create_publisher<geometry_msgs::msg::Point>("com", rclcpp::SystemDefaultsQoS());

  m_tfBroadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
  m_skeletonPub = std::make_unique<SkeletonPublisher>(*this, *m_tfBroadcaster);

  m_diagnosticUpdater = std::make_unique<diagnostic_updater::Updater>(this);
  m_diagnosticUpdater->setHardwareID("Xsens MVN Stream");
  m_diagnosticUpdater->add("Stream status", this, &XsensStreamNode::diagnosticsCallback);
  m_diagnosticTimer = create_wall_timer(
    std::chrono::seconds(1), [this]() {m_diagnosticUpdater->force_update();});

  initialize_xsens_stream_client();

  RCLCPP_INFO(get_logger(), "Configured. Waiting for activation...");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensStreamNode::on_activate(const rclcpp_lifecycle::State &)
{
  m_comPublisher->on_activate();
  m_skeletonPub->activate();

  const auto params = m_paramListener->get_params();
  m_updateTimer = create_wall_timer(
    std::chrono::milliseconds(static_cast<int>(1000 / params.update_frequency)),
    [this]() {
      if ((!m_xsensClient || !m_xsensClient->isActive()) &&
      !initialize_xsens_stream_client())
      {
        return;
      }
      publish();
    });

  RCLCPP_INFO(get_logger(), "Activated.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensStreamNode::on_deactivate(const rclcpp_lifecycle::State &)
{
  if (m_updateTimer) {
    m_updateTimer->cancel();
    m_updateTimer.reset();
  }
  m_comPublisher->on_deactivate();
  m_skeletonPub->deactivate();

  RCLCPP_INFO(get_logger(), "Deactivated.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensStreamNode::on_cleanup(const rclcpp_lifecycle::State &)
{
  m_updateTimer.reset();
  m_diagnosticTimer.reset();
  m_diagnosticUpdater.reset();
  m_xsensClient.reset();
  m_skeletonPub.reset();
  m_tfBroadcaster.reset();
  m_comPublisher.reset();
  m_paramListener.reset();
  m_lastDataTimeNs = 0;

  RCLCPP_INFO(get_logger(), "Cleaned up.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensStreamNode::on_shutdown(const rclcpp_lifecycle::State &)
{
  m_updateTimer.reset();
  m_diagnosticTimer.reset();
  m_diagnosticUpdater.reset();
  m_xsensClient.reset();
  m_skeletonPub.reset();
  m_tfBroadcaster.reset();
  m_comPublisher.reset();
  m_paramListener.reset();

  RCLCPP_INFO(get_logger(), "Shut down.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
XsensStreamNode::on_error(const rclcpp_lifecycle::State &)
{
  m_updateTimer.reset();
  m_diagnosticTimer.reset();
  m_diagnosticUpdater.reset();
  m_xsensClient.reset();
  m_skeletonPub.reset();
  m_tfBroadcaster.reset();
  m_comPublisher.reset();
  m_paramListener.reset();
  m_lastDataTimeNs = 0;

  RCLCPP_ERROR(get_logger(), "Error encountered, returning to unconfigured.");
  return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
}

bool XsensStreamNode::initialize_xsens_stream_client()
{
  // Reset a dead client so a fresh one is created below.
  if (m_xsensClient && !m_xsensClient->isActive()) {
    RCLCPP_WARN(get_logger(), "Stream client lost connection. Reconnecting...");
    m_xsensClient.reset();
  }
  if (!m_xsensClient) {
    try {
      auto client =
        std::make_unique<XsensStreamClient>(get_logger(), m_paramListener->get_params().udp_port);
      if (!client->init()) {
        RCLCPP_WARN_THROTTLE(
          this->get_logger(), *get_clock(), 5000, "Xsens client initialization failed; retrying.");
        return false;
      }
      m_xsensClient = std::move(client);
    } catch (const std::exception & err) {
      RCLCPP_WARN_THROTTLE(
        this->get_logger(), *get_clock(), 5000, "Could not set up Xsens client: %s", err.what());
      return false;
    }
  }
  return true;
}

void XsensStreamNode::publish()
{
  auto stamp = now();
  bool has_com = m_comPublisher->get_subscription_count() > 0;

  auto segments = m_xsensClient->getSegments();
  auto joint_vec = m_xsensClient->getJoints();
  m_lastDataTimeNs = m_xsensClient->lastDataTimeNs();

  m_skeletonPub->publishLinkStates(stamp, segments);
  m_skeletonPub->publishJointStates(stamp, joint_vec);

  if (has_com) {
    auto com = m_xsensClient->getCOM();
    if (com) {
      m_comPublisher->publish(toMsg(*com));
    }
  }
}

void XsensStreamNode::diagnosticsCallback(diagnostic_updater::DiagnosticStatusWrapper & stat)
{
  if (!m_xsensClient) {
    stat.summary(
      diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Stream client not initialized.");
    return;
  }

  if (!m_xsensClient->isActive()) {
    stat.summary(
      diagnostic_msgs::msg::DiagnosticStatus::ERROR, "Stream client disconnected.");
    return;
  }

  // Base status: connected and running.
  stat.summary(
    diagnostic_msgs::msg::DiagnosticStatus::OK, "Stream client connected and receiving data.");

  stat.add("UDP Port", m_paramListener->get_params().udp_port);

  stat.add("Links", static_cast<int>(m_xsensClient->getSegments().size()));
  stat.add("Joints", static_cast<int>(m_xsensClient->getJoints().size()));

  // Staleness check: warn if no data arrived within 2× the expected period.
  if (m_lastDataTimeNs > 0) {
    auto now_ns = std::chrono::steady_clock::now().time_since_epoch().count();
    auto elapsed_ms = (now_ns - m_lastDataTimeNs) / 1'000'000;
    stat.add("Last data received (ms ago)", static_cast<int>(elapsed_ms));
    int expected_period_ms = 1000 / m_paramListener->get_params().update_frequency;
    if (elapsed_ms > 2 * expected_period_ms) {
      stat.mergeSummary(
        diagnostic_msgs::msg::DiagnosticStatus::WARN,
        "No data received in the last " + std::to_string(elapsed_ms) + " ms.");
    }
  }
}

}  // namespace xsens_mvn_ros2

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<xsens_mvn_ros2::XsensStreamNode>();
  rclcpp::executors::SingleThreadedExecutor exec;
  exec.add_node(node->get_node_base_interface());
  exec.spin();
  rclcpp::shutdown();
  return 0;
}

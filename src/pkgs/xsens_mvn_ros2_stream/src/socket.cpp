// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
#include "xsens_mvn_ros2_stream/socket.hpp"

#include <cerrno>
#include <cstring>

#include <rclcpp/logging.hpp>

namespace
{
auto & socket_logger()
{
  static auto logger = rclcpp::get_logger("xsens_mvn_sdk.socket");
  return logger;
}
}  // namespace

Socket::Socket(IpProtocol protocol)
: m_protocol(protocol)
{
  if (m_protocol == IP_UDP) {
    m_sock = socket(AF_INET, SOCK_DGRAM, 0);
  } else if (m_protocol == IP_TCP) {
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
  } else {
    throw std::invalid_argument("Unknown protocol");
  }

  if (m_sock < 0) {
    throw std::runtime_error(
      std::string("Failed to create socket: ") + strerror(errno));
  }

  // Allow rapid port reuse after restart (avoids TIME_WAIT blocking bind).
  int reuse = 1;
  setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

  // Set a receive timeout so read() doesn't block indefinitely.
  // This is critical for allowing clean shutdown when no data arrives.
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

Socket::~Socket()
{
  close(m_sock);
}

bool Socket::bind(uint16_t port)
{
  if (port < 2000) {
    throw std::invalid_argument("Port number should be bigger than 2000");
  }

  m_my_addr = {};

  m_my_addr.sin_family = AF_INET;
  m_my_addr.sin_addr.s_addr = INADDR_ANY;
  m_my_addr.sin_port = htons(port);

  if (::bind(m_sock, reinterpret_cast<struct sockaddr *>(&m_my_addr), sizeof(m_my_addr)) < 0) {
    RCLCPP_ERROR(
      socket_logger(),
      "Socket binding failed on port %u: %s. "
      "Check: (1) Is another process already using this port? "
      "(2) Do you have permission to bind to this port?",
      port, strerror(errno));
    return false;
  }

  return true;
}

int Socket::read(char * buffer, size_t size)
{
  int n = -1;
  memset(buffer, 0, size);

  if (m_protocol == IP_UDP) {
    n = static_cast<int>(recvfrom(
      m_sock, buffer, size, 0,
      reinterpret_cast<struct sockaddr *>(&m_addr_from),
      &fromlen));
  } else {
    throw std::runtime_error("TCP read is not implemented");
  }
  return n;
}

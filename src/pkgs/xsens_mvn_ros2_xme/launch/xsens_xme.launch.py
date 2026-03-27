# Copyright (c) 2026, Xsens Technologies B.V.
# SPDX-License-Identifier: BSD-3-Clause
import launch
from launch.actions import (
    DeclareLaunchArgument,
    EmitEvent,
    RegisterEventHandler,
)
from launch.conditions import IfCondition
from launch.substitutions import (
    EnvironmentVariable,
    LaunchConfiguration,
    PathJoinSubstitution,
)
from launch_ros.actions import LifecycleNode
from launch_ros.event_handlers import OnStateTransition
from launch_ros.events.lifecycle import ChangeState
from launch_ros.substitutions import FindPackageShare

import lifecycle_msgs.msg


def generate_launch_description():
    namespace_arg = DeclareLaunchArgument(
        'namespace', default_value='')
    auto_activate_arg = DeclareLaunchArgument(
        'auto_activate', default_value='true')
    model_name_arg = DeclareLaunchArgument(
        'model_name', default_value='skeleton')

    xme_node = LifecycleNode(
        package='xsens_mvn_ros2_xme',
        executable='xsens_mvn_ros2_xme_node',
        name='xsens_mvn_ros2_xme_node',
        namespace=LaunchConfiguration('namespace'),
        output='screen',
        parameters=[
            PathJoinSubstitution([
                FindPackageShare('xsens_mvn_ros2_xme'),
                'config', 'xsens_xme_node.yaml']),
            PathJoinSubstitution([
                FindPackageShare('xsens_mvn_ros2_xme'),
                'config', 'body_dimensions.yaml']),
            {
                'model_name': LaunchConfiguration('model_name'),
                'settingsPath': FindPackageShare('xsens_xme_sdk'),
                'userPath': FindPackageShare('xsens_xme_sdk'),
                'logPath': PathJoinSubstitution([
                    EnvironmentVariable('HOME'), '.ros', 'log']),
            },
        ],
    )

    configure_event = EmitEvent(
        condition=IfCondition(LaunchConfiguration('auto_activate')),
        event=ChangeState(
            lifecycle_node_matcher=launch.events.matches_action(xme_node),
            transition_id=lifecycle_msgs.msg.Transition.TRANSITION_CONFIGURE,
        )
    )

    activate_handler = RegisterEventHandler(
        condition=IfCondition(LaunchConfiguration('auto_activate')),
        event_handler=OnStateTransition(
            target_lifecycle_node=xme_node,
            goal_state='inactive',
            handle_once=True,
            entities=[
                EmitEvent(
                    event=ChangeState(
                        lifecycle_node_matcher=launch.events.matches_action(
                            xme_node),
                        transition_id=lifecycle_msgs.msg.Transition.TRANSITION_ACTIVATE,
                    )
                ),
            ],
        )
    )

    return launch.LaunchDescription([
        namespace_arg,
        auto_activate_arg,
        model_name_arg,
        xme_node,
        activate_handler,
        configure_event,
    ])

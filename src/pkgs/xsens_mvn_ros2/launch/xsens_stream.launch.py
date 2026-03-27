# Copyright (c) 2026, Xsens Technologies B.V.
# SPDX-License-Identifier: BSD-3-Clause
import launch
from launch.actions import (
    DeclareLaunchArgument,
    IncludeLaunchDescription,
    SetEnvironmentVariable,
)
from launch.conditions import IfCondition
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    namespace_arg = DeclareLaunchArgument(
        'namespace', default_value='')
    auto_activate_arg = DeclareLaunchArgument(
        'auto_activate', default_value='true')
    discovery_range_arg = DeclareLaunchArgument(
        'discovery_range', default_value='LOCALHOST')
    launch_rviz_arg = DeclareLaunchArgument(
        'launch_rviz', default_value='true')
    launch_description_arg = DeclareLaunchArgument(
        'launch_description', default_value='true')
    model_name_arg = DeclareLaunchArgument(
        'model_name', default_value='skeleton')
    rviz_config_arg = DeclareLaunchArgument(
        'rviz_config_file',
        default_value=PathJoinSubstitution([
            FindPackageShare('xsens_mvn_ros2_description'),
            'config', 'xsens_visualization.rviz']))

    set_discovery = SetEnvironmentVariable(
        'ROS_AUTOMATIC_DISCOVERY_RANGE',
        LaunchConfiguration('discovery_range'))

    stream_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                FindPackageShare('xsens_mvn_ros2_stream'),
                'launch', 'xsens_stream.launch.py'])),
        launch_arguments={
            'namespace': LaunchConfiguration('namespace'),
            'auto_activate': LaunchConfiguration('auto_activate'),
            'model_name': LaunchConfiguration('model_name'),
        }.items(),
    )

    rviz_node = Node(
        condition=IfCondition(LaunchConfiguration('launch_rviz')),
        package='rviz2',
        executable='rviz2',
        name='xsens_rviz',
        output='screen',
        arguments=['-d', LaunchConfiguration('rviz_config_file')],
    )

    description_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                FindPackageShare('xsens_mvn_ros2_description'),
                'launch', 'description.launch.py'])),
        condition=IfCondition(LaunchConfiguration('launch_description')),
        launch_arguments={
            'namespace': LaunchConfiguration('namespace'),
            'auto_activate': LaunchConfiguration('auto_activate'),
            'model_name': LaunchConfiguration('model_name'),
        }.items(),
    )

    return launch.LaunchDescription([
        namespace_arg,
        auto_activate_arg,
        discovery_range_arg,
        launch_rviz_arg,
        launch_description_arg,
        model_name_arg,
        rviz_config_arg,
        set_discovery,
        stream_launch,
        rviz_node,
        description_launch,
    ])

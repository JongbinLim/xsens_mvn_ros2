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
from launch.substitutions import (
    EnvironmentVariable,
    LaunchConfiguration,
    PathJoinSubstitution,
)
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
    settings_path_arg = DeclareLaunchArgument(
        'settings_path',
        default_value=FindPackageShare('xsens_xme_sdk'))
    user_path_arg = DeclareLaunchArgument(
        'user_path',
        default_value=FindPackageShare('xsens_xme_sdk'))
    log_path_arg = DeclareLaunchArgument(
        'log_path',
        default_value=PathJoinSubstitution([
            EnvironmentVariable('HOME'), '.ros', 'log']))
    rviz_config_arg = DeclareLaunchArgument(
        'rviz_config_file',
        default_value=PathJoinSubstitution([
            FindPackageShare('xsens_mvn_ros2_description'),
            'config', 'xsens_visualization.rviz']))

    set_discovery = SetEnvironmentVariable(
        'ROS_AUTOMATIC_DISCOVERY_RANGE',
        LaunchConfiguration('discovery_range'))

    xme_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                FindPackageShare('xsens_mvn_ros2_xme'),
                'launch', 'xsens_xme.launch.py'])),
        launch_arguments={
            'namespace': LaunchConfiguration('namespace'),
            'auto_activate': LaunchConfiguration('auto_activate'),
            'model_name': LaunchConfiguration('model_name'),
            'settings_path': LaunchConfiguration('settings_path'),
            'user_path': LaunchConfiguration('user_path'),
            'log_path': LaunchConfiguration('log_path'),
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
        settings_path_arg,
        user_path_arg,
        log_path_arg,
        rviz_config_arg,
        set_discovery,
        xme_launch,
        rviz_node,
        description_launch,
    ])

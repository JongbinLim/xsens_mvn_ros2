# Copyright (c) 2026, Xsens Technologies B.V.
# SPDX-License-Identifier: BSD-3-Clause
import launch
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    namespace_arg = DeclareLaunchArgument(
        'namespace', default_value='')
    auto_activate_arg = DeclareLaunchArgument(
        'auto_activate', default_value='true')
    model_name_arg = DeclareLaunchArgument(
        'model_name', default_value='skeleton')

    description_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                FindPackageShare('xsens_mvn_ros2_description'),
                'launch', 'description.launch.py'])),
        launch_arguments={
            'namespace': LaunchConfiguration('namespace'),
            'auto_activate': LaunchConfiguration('auto_activate'),
            'model_name': LaunchConfiguration('model_name'),
        }.items(),
    )

    return launch.LaunchDescription([
        namespace_arg,
        auto_activate_arg,
        model_name_arg,
        description_launch,
    ])

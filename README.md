# Xsens MVN ROS2 Driver

ROS2 driver for Xsens motion capture suits. Supports two operational modes: receiving streamed data from Xsens MVN Studio over UDP, or connecting directly to the suit hardware via the Xsens XME SDK.

---

## Table of Contents

- [Overview](#overview)
- [Package Structure](#package-structure)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Lifecycle Management](#lifecycle-management)
- [Architecture](#architecture)
- [Nodes](#nodes)
- [Custom Messages](#custom-messages)
- [Configuration](#configuration)
- [URDF and Visualization](#urdf-and-visualization)
- [Using XML Launch Files](#using-xml-launch-files)
- [Code Style Guidelines](#code-style-guidelines)
- [Testing](#testing)
- [Contributing](#contributing)

---

## Overview

The driver publishes skeletal kinematics as ROS2 topics and TF transforms, making Xsens MVN motion data directly usable in any ROS2 application. All nodes are implemented as **ROS2 managed (lifecycle) nodes**, providing explicit control over initialization, activation, and shutdown. Two complementary modes are available:

| Mode | Node | When to use |
|------|------|-------------|
| **Stream** | `xsens_mvn_ros2_stream_node` | MVN Studio (Windows) is running and streaming data over UDP |
| **XME** | `xsens_mvn_ros2_xme_node` | Direct Linux connection to the suit hardware (no Windows PC needed) |

A third node — the **URDF publisher** — dynamically generates a body-proportioned robot description from the live TF data, enabling accurate mesh-scaled visualization in RViz.

**ROS Distribution:** Jazzy Jalisco
**Build system:** colcon / ament_cmake
**Language:** C++17

---

## Package Structure

```
xsens_mvn_ros2/                      <- workspace root
├── src/
│   ├── pkgs/
│   │   ├── xsens_mvn_msgs/          <- custom ROS2 message/service/action definitions
│   │   ├── xsens_mvn_ros2/          <- meta-package (launch files, config, exec_depend on all below)
│   │   ├── xsens_mvn_ros2_common/   <- shared library: SkeletonPublisher, xsens_model, IMotionCaptureSource
│   │   ├── xsens_mvn_ros2_stream/   <- stream node: UDP client for MVN Studio
│   │   ├── xsens_mvn_ros2_xme/      <- XME node: direct hardware via XME SDK
│   │   └── xsens_mvn_ros2_description/ <- URDF publisher node, meshes, xacro, rviz config
│   └── deps/
│       ├── xsens_mvn_sdk/            <- custom MVN UDP protocol parser (ament package)
│       └── xsens_xme_sdk/            <- vendored XME SDK binaries and headers
├── examples/
│   └── xme_cpp/                      <- XME SDK example projects (outside colcon workspace)
└── colcon_defaults.yaml              <- workspace build defaults
```

### Package Dependency Graph

```
xsens_mvn_ros2 (meta-package)
├── xsens_mvn_ros2_stream
│   ├── xsens_mvn_ros2_common
│   │   └── xsens_mvn_msgs
│   └── xsens_mvn_sdk
├── xsens_mvn_ros2_xme
│   ├── xsens_mvn_ros2_common
│   │   └── xsens_mvn_msgs
│   └── xsens_xme_sdk
└── xsens_mvn_ros2_description
    └── xsens_mvn_msgs
```

### Key Components

| Component | Package | Type | Purpose |
|-----------|---------|------|---------|
| `xsens_mvn_ros2_stream_node` | `xsens_mvn_ros2_stream` | Lifecycle Node | UDP client for MVN Studio streaming |
| `xsens_mvn_ros2_xme_node` | `xsens_mvn_ros2_xme` | Lifecycle Node | Direct XME SDK hardware interface |
| `xsens_mvn_ros2_urdf_publisher_node` | `xsens_mvn_ros2_description` | Lifecycle Node | Dynamic, body-scaled URDF generation |
| `SkeletonPublisher` | `xsens_mvn_ros2_common` | Library | TF broadcasting and topic publishing (shared by stream and XME) |
| `IMotionCaptureSource` | `xsens_mvn_ros2_common` | Interface | Abstract interface for dependency injection |
| `HumanDataHandler` | `xsens_mvn_ros2_stream` | Library | Thread-safe skeleton data container |
| `XsensStreamClient` | `xsens_mvn_ros2_stream` | Library | UDP socket and MVN protocol handling |

---

## Installation

### Prerequisites

- **OS:** Ubuntu 24.04 LTS
- **ROS2:** [Jazzy Jalisco](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debians.html) (ros-base or desktop)

Install ROS2 and development tools:

```bash
sudo apt-get update
sudo apt-get install ros-jazzy-ros-base ros-dev-tools
```

For XME mode (direct hardware connection), the XME SDK is vendored in `src/deps/xsens_xme_sdk/` — no additional SDK installation is needed. However, a **valid XME SDK license** is required on the host machine. Without it, the XME node will fail during configuration with a `"License system not constructed"` error. The Linux license manager can be downloaded from the [Xsens Software & Documentation](https://www.xsens.com/support/software-documentation) page. Contact Xsens for license information.

### Clone and Build

```bash
# 1. Clone the repository
git clone <repository-url>
cd xsens_mvn_ros2

# 2. Initialize submodules (if any)
git submodule update --init --recursive

# 3. Source ROS2
source /opt/ros/jazzy/setup.bash

# 4. Install ROS dependencies
sudo rosdep init   # only needed once per system
rosdep update
rosdep install -y -i --from-paths $(colcon list --paths-only)

# 5. Build
colcon build

# 6. Source the workspace
source install/setup.bash
```

### Build Options

The `colcon_defaults.yaml` in the repo root pre-configures common settings:

| Setting | Default | Description |
|---------|---------|-------------|
| Build type | `Debug` | CMake build type |
| Optimization | `-O2` | Compiler optimization level |
| Code coverage | `--coverage` | gcov instrumentation enabled |
| Testing | `ON` | Build and run tests |
| Install mode | Symlink | Faster iteration during development |

To build in Release mode without coverage:

```bash
colcon build --cmake-args -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-O2
```

### Verify the Installation

```bash
# Run tests
source install/setup.bash
colcon test
colcon test-result --verbose

# Check that nodes are available
ros2 pkg executables xsens_mvn_ros2_stream
ros2 pkg executables xsens_mvn_ros2_xme
ros2 pkg executables xsens_mvn_ros2_description
```

### Docker (ROS 2 Jazzy alongside a Humble host)

The repository includes a Jazzy container configuration. It keeps the Xsens
workspace independent from a ROS 2 Humble installation on the host while using
host networking so DDS and MVN UDP traffic can reach the container.
The vendored XME SDK is x86-64 only, so direct XME mode requires an amd64 Linux
host (or emulation, which is not recommended for real-time capture).

For the complete Korean walkthrough—including Docker GUI access, Sentinel
HASP/LDK installation, XME-SDK license activation, persistent container reuse,
hardware checks, and calibration—see
[Docker XME setup (Korean)](docs/docker_xme_setup_ko.md).

Build the image:

```bash
docker compose build xsens-jazzy
```

Open a sourced Jazzy shell:

```bash
docker compose run --rm xsens-jazzy
```

Run the MVN Studio UDP mode:

```bash
docker compose run --rm xsens-jazzy \
  ros2 launch xsens_mvn_ros2 xsens_stream.launch.py \
  launch_rviz:=false discovery_range:=SUBNET
```

Run the direct XME hardware mode when Sentinel licensing is provided by the
host or a network license server:

```bash
docker compose run --rm xsens-jazzy \
  ros2 launch xsens_mvn_ros2 xsens_xme.launch.py \
  launch_rviz:=false discovery_range:=SUBNET \
  user_path:=/var/lib/xsens log_path:=/var/log/xsens
```

`/var/lib/xsens` and `/var/log/xsens` are backed by named Docker volumes so
calibration data and logs survive container removal. The Compose service also
passes `/dev/bus/usb` and udev information into the container.

Set `ROS_DOMAIN_ID` to the same value used by the host Humble project:

```bash
ROS_DOMAIN_ID=7 docker compose run --rm xsens-jazzy
```

Standard messages such as `sensor_msgs/JointState` and TF can be consumed
directly by a Humble process when DDS discovery and the domain ID match. A
Humble consumer of `xsens_mvn_msgs/LinkStateArray` must also build the matching
`xsens_mvn_msgs` interface definition.

For RViz, permit the local container to use the X server and omit
`launch_rviz:=false`:

```bash
xhost +si:localuser:root
docker compose run --rm xsens-jazzy \
  ros2 launch xsens_mvn_ros2 xsens_xme.launch.py \
  discovery_range:=SUBNET \
  user_path:=/var/lib/xsens log_path:=/var/log/xsens
```

Revoke the permission afterward with `xhost -si:localuser:root`.

> **XME licensing:** The vendored SDK does not include the Sentinel HASP/LDK
> runtime or an activated XME license. The runtime may be provided by the host
> over host networking or installed in a persistent container. Do not use an
> ephemeral `--rm` container for container-local activation. See the linked
> Docker XME setup guide for the procedure used in this workspace.

---

## Quick Start

### Stream Mode (MVN Studio)

Configure Xsens MVN Studio to stream data via UDP, then launch:

```bash
ros2 launch xsens_mvn_ros2 xsens_stream.launch.py
```

The Python launch file automatically configures and activates the lifecycle node. RViz and the URDF publisher are included by default. To disable:

```bash
ros2 launch xsens_mvn_ros2 xsens_stream.launch.py launch_rviz:=false launch_description:=false
```

### XME Mode (Direct Hardware)

> **XME SDK License:** A valid license must be present on the host machine. If the node fails during configuration with `"License system not constructed"`, the license is missing or invalid. Download the Linux license manager from the [Xsens Software & Documentation](https://www.xsens.com/support/software-documentation) page to activate your license.

> **Before launching:** Edit `src/pkgs/xsens_mvn_ros2_xme/config/body_dimensions.yaml` to match your subject's measurements (all values in metres). Accurate body dimensions are required for correct biomechanical solving. See the [Body Dimensions](#body-dimensions) section for all available parameters.

With the suit connected (Awinda or USB), launch:

```bash
ros2 launch xsens_mvn_ros2 xsens_xme.launch.py
```

**Perform calibration** (N-Pose):

```bash
ros2 action send_goal /xsens_mvn_ros2_xme_node/perform_calibration \
  xsens_mvn_msgs/action/PerformCalibration \
  "{pose: 0, output_file_name: 'my_calibration.mvn'}"
```

**Load an existing calibration**:

```bash
ros2 service call /xsens_mvn_ros2_xme_node/set_calibration_path \
  xsens_mvn_msgs/srv/LoadCalibrationPath \
  "{calibration_file_path: '/path/to/calibration.mvn'}"
```

**Monitor hardware diagnostics**:

```bash
# Command line
ros2 topic echo /diagnostics

# Or with the RQT monitor
ros2 run rqt_robot_monitor rqt_robot_monitor
```

---

## Lifecycle Management

All three nodes are implemented as **ROS2 managed (lifecycle) nodes** (`rclcpp_lifecycle::LifecycleNode`). This provides explicit control over node state: initialization, activation, deactivation, and cleanup can be triggered independently via the standard lifecycle interface.

### State Machine

```
    ┌──────────────┐
    │ Unconfigured │ ◄── on_cleanup ── ┐
    └──────┬───────┘                   │
           │ on_configure              │
           ▼                           │
    ┌──────────────┐                   │
    │   Inactive   │ ──────────────────┘
    └──────┬───────┘
           │ on_activate
           ▼
    ┌──────────────┐
    │    Active    │
    └──────┬───────┘
           │ on_deactivate
           ▼
    ┌──────────────┐
    │   Inactive   │
    └──────────────┘
```

### What Happens in Each State

| Callback | Stream Node | XME Node | URDF Publisher |
|----------|-------------|----------|----------------|
| **on_configure** | Creates params, TF broadcaster, SkeletonPublisher, diagnostics, UDP client | Creates params, license, XmeControl, hardware scan (30 s timeout), publishers, services, action server | Creates params, TF buffer/listener, publisher, republish service, diagnostics |
| **on_activate** | Starts the polling timer (begins publishing) | Lifecycle publishers become active (SDK callbacks begin producing output) | Starts fast-poll timer to compute scales and publish URDF |
| **on_deactivate** | Stops the polling timer (UDP client stays alive) | Stops publishing; aborts in-progress calibration | Stops timers, resets published flag |
| **on_cleanup** | Destroys UDP client, resets all publishers and diagnostics | Removes SDK callback handler, disconnects hardware, terminates SDK, resets all ROS objects | Destroys TF listener/buffer, publishers, service, diagnostics |

### Default Behaviour with Python Launch Files

The provided Python launch files (`.launch.py`) automatically configure and activate each node at startup by default. From the user's perspective, the node starts publishing immediately — the same behaviour as a regular `rclcpp::Node`.

```bash
# These handle configure + activate automatically:
ros2 launch xsens_mvn_ros2 xsens_stream.launch.py
ros2 launch xsens_mvn_ros2 xsens_xme.launch.py
ros2 launch xsens_mvn_ros2_description description.launch.py
```

### Starting in Unconfigured State

Set `auto_activate:=false` to launch all nodes in the `unconfigured` state. This is useful for staged bringup, debugging, or when you want full manual control over the lifecycle:

```bash
# Nodes start but remain unconfigured — no hardware scan, no publishing
ros2 launch xsens_mvn_ros2 xsens_xme.launch.py auto_activate:=false

# In another terminal, transition nodes manually when ready
ros2 lifecycle set /xsens_mvn_ros2_xme_node configure
ros2 lifecycle set /xsens_mvn_ros2_xme_node activate
ros2 lifecycle set /xsens_urdf_publisher configure
ros2 lifecycle set /xsens_urdf_publisher activate
```

The `auto_activate` argument is forwarded to all included launch files (e.g. the URDF publisher), so all nodes in the launch graph respect it.

### Manual Lifecycle Control

Regardless of `auto_activate`, you can always manage lifecycle transitions manually using `ros2 lifecycle`:

```bash
# Check current state
ros2 lifecycle get /xsens_mvn_ros2_stream_node

# Step through transitions manually
ros2 lifecycle set /xsens_mvn_ros2_stream_node configure
ros2 lifecycle set /xsens_mvn_ros2_stream_node activate

# Pause publishing (deactivate) without disconnecting
ros2 lifecycle set /xsens_mvn_ros2_stream_node deactivate

# Resume publishing
ros2 lifecycle set /xsens_mvn_ros2_stream_node activate

# Full teardown and return to unconfigured
ros2 lifecycle set /xsens_mvn_ros2_stream_node deactivate
ros2 lifecycle set /xsens_mvn_ros2_stream_node cleanup

# Reconfigure with new parameters
ros2 lifecycle set /xsens_mvn_ros2_stream_node configure
ros2 lifecycle set /xsens_mvn_ros2_stream_node activate
```

> **Note:** Services and action servers on the XME node reject requests when the node is not in the `active` state.

---

## Architecture

### High-Level Overview

The driver follows a layered architecture with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────────────────┐
│                          ROS2 Interface Layer                       │
│   ┌──────────────────┐  ┌──────────────────┐  ┌─────────────────┐  │
│   │  Stream Node     │  │  XME Node        │  │  URDF Publisher │  │
│   │  (xsens_mvn_     │  │  (xsens_mvn_     │  │  (xsens_mvn_    │  │
│   │   ros2_stream)   │  │   ros2_xme)      │  │   ros2_         │  │
│   └────────┬─────────┘  └────────┬─────────┘  │   description)  │  │
│            │                     │             └────────┬────────┘  │
│            │                     │                      │           │
│   ┌────────▼─────────────────────▼──────────┐          │           │
│   │         SkeletonPublisher               │◄─────────┘           │
│   │         (xsens_mvn_ros2_common)         │    reads /tf         │
│   └────────┬────────────────────────────────┘                      │
│            │  IMotionCaptureSource interface                       │
├────────────┼───────────────────────────────────────────────────────┤
│            │           Data Source Layer                            │
│   ┌────────▼─────────┐  ┌──────────────────┐                      │
│   │ XsensStreamClient│  │ XmeControl       │                      │
│   │ + HumanDataHandler│  │ (XME SDK)        │                      │
│   └────────┬─────────┘  └────────┬─────────┘                      │
│            │                     │                                  │
├────────────┼─────────────────────┼──────────────────────────────────┤
│            │       Protocol / Hardware Layer                        │
│   ┌────────▼─────────┐  ┌──────────────────┐                      │
│   │ xsens_mvn_sdk    │  │ xsens_xme_sdk    │                      │
│   │ (UDP parser)     │  │ (vendored binary) │                      │
│   └──────────────────┘  └──────────────────┘                      │
└─────────────────────────────────────────────────────────────────────┘
```

**Key design principles:**

- **Lifecycle nodes** — All nodes inherit from `rclcpp_lifecycle::LifecycleNode`, enabling explicit control over initialization, activation, and shutdown. Publishers use `LifecyclePublisher` which automatically gates output based on the node's state.
- **Dependency injection** — Both stream and XME nodes use `SkeletonPublisher` for all ROS publishing. The data source is decoupled from the publishing logic via the `IMotionCaptureSource` interface.
- **Thread safety** — `HumanDataHandler` provides a thread-safe container between the UDP receive thread and the ROS timer callback. XME SDK callbacks are similarly synchronized. The XME node removes the SDK callback handler before destroying ROS objects during cleanup to prevent use-after-free.
- **Generated parameters** — All nodes use `generate_parameter_library` for type-safe, validated parameters with zero manual `declare_parameter` calls.
- **Diagnostics-first** — Every node publishes to `/diagnostics` via `diagnostic_updater::Updater` at 1 Hz, making health monitoring uniform.

### Data Flow — Stream Mode

```
Xsens MVN Studio (Windows, UDP)
        │
        ▼
XsensStreamClient (UDP socket, background thread)
        │  MVN protocol datagrams
        ▼
ParserManager (xsens_mvn_sdk)
        │  parsed kinematics
        ▼
HumanDataHandler (thread-safe data container)
        │
        ▼
SkeletonPublisher
    ├──> /tf                  (TF2 transforms per segment)
    ├──> ~/joint_states       (Euler angles)
    ├──> ~/link_states        (full kinematic state)
    └──> ~/com                (centre of mass)
```

### Data Flow — XME Mode

```
Xsens Suit Hardware (Awinda / USB)
        │
        ▼
XmeControl (Xsens XME SDK)
        │  callbacks
        ▼
XsensXmeNode
    ├── CalibrationModule  ──> ~/perform_calibration (action server)
    │                      ──> ~/set_calibration_path (service)
    ├── DiagnosticsModule  ──> /diagnostics
    └── SkeletonPublisher
            ├──> /tf
            ├──> ~/joint_states
            ├──> ~/link_states
            └──> ~/com
```

### Data Flow — URDF Publisher

```
TF frames (published by either stream or XME node)
        │  measure joint distances
        ▼
XsensUrdfPublisherNode
        │  scale mesh per segment
        ▼
/robot_description  (std_msgs/String, transient-local QoS)
```

### Skeleton Definition

The driver uses a 24-segment humanoid model defined in `xsens_model.hpp`:

```
pelvis
  ├── l5 -> l3 -> t12 -> t8
  │              └── neck -> head
  │              ├── left_shoulder  -> left_upper_arm  -> left_forearm  -> left_hand
  │              └── right_shoulder -> right_upper_arm -> right_forearm -> right_hand
  ├── left_upper_leg  -> left_lower_leg  -> left_foot  -> left_toe
  └── right_upper_leg -> right_lower_leg -> right_foot -> right_toe
```

Frame names are prefixed with the `model_name` parameter (default: `skeleton`), e.g. `skeleton_pelvis`.

### Parameter System

Each node uses a **generated typed `ParamListener`** — no raw `declare_parameter` calls anywhere:

| YAML definition file | Generated class | Used by |
|---------------------|----------------|---------|
| `xsens_stream_node_parameters.yaml` | `xsens_stream_node::ParamListener` | Stream node |
| `xsens_xme_node_parameters.yaml` | `xsens_xme_node::ParamListener` | XME node |
| `body_dimensions_parameters.yaml` | `body_dimensions::ParamListener` | XME node (in common pkg) |
| `xsens_urdf_publisher_parameters.yaml` | `xsens_urdf_publisher::ParamListener` | URDF publisher |

The XME node additionally uses `add_on_set_parameters_callback` for runtime hardware-side updates (sampleRate, suitConfiguration, bodyDimension.*, biomechanicalModel).

### Error Handling

The driver uses three complementary layers for reporting errors:

**Layer 1 — ROS2 Logging** (`ros2 topic echo /rosout`)

| Situation | Macro | Behaviour |
|-----------|-------|-----------|
| Unrecoverable — node cannot function | `RCLCPP_ERROR` | Logged once; node does not retry |
| Transient — retrying automatically | `RCLCPP_WARN_THROTTLE` (5 s) | Suppressed between retries |
| Expected degradation (low battery, stale data) | `RCLCPP_WARN` | Logged once per state change |
| Normal lifecycle events | `RCLCPP_INFO` | Logged once per event |
| High-frequency data paths | `RCLCPP_DEBUG` | Never used at INFO or above on hot paths |

Library code uses the same macros via an injected `rclcpp::Logger` so all messages appear under the owning node's logger name.

**Layer 2 — Diagnostics** (`ros2 topic echo /diagnostics`)

All three nodes publish to `/diagnostics` via `diagnostic_updater::Updater` (1 Hz):

| Node | Hardware ID | Key/value fields |
|------|-------------|-----------------|
| Stream | `Xsens MVN Stream` | UDP Port, Links, Joints, Last data received (ms ago) |
| XME | `Xsens MVN XME` | Total Sensors, Battery level (%), Radio Quality (%), Calibration quality, Last Error |
| URDF Publisher | `Xsens URDF Publisher` | Last published (s ago) |

Status levels: `OK` = nominal, `WARN` = degraded but running, `ERROR` = not functional.

**Layer 3 — Service/Action Response Fields**

| Operation | Type | Error field |
|-----------|------|-------------|
| Load calibration file | `LoadCalibrationPath` srv | `bool success` + `string message` |
| Perform calibration | `PerformCalibration` action | `string calibration_result`; `abort()`/`canceled()` |
| Set parameters | `SetParameters` | `bool successful` + `string reason` |
| Republish URDF | `std_srvs/Trigger` srv | `bool success` + `string message` |

---

## Nodes

For full parameter and topic reference, see [`src/pkgs/xsens_mvn_ros2/README.md`](src/pkgs/xsens_mvn_ros2/README.md).

### xsens_mvn_ros2_stream_node

UDP client for Xsens MVN Studio streaming mode. Lifecycle node.

**Launch:** `ros2 launch xsens_mvn_ros2 xsens_stream.launch.py`
**Config:** `src/pkgs/xsens_mvn_ros2_stream/config/xsens_stream_node.yaml`

Key parameters: `udp_port` (default `9763`), `model_name`, `reference_frame`, `update_frequency`.

### xsens_mvn_ros2_xme_node

Direct hardware interface via XME SDK. Provides calibration actions, live diagnostics, and configurable body dimensions. Lifecycle node. Services and actions reject requests when the node is not active.

**Launch:** `ros2 launch xsens_mvn_ros2 xsens_xme.launch.py`
**Config:** `src/pkgs/xsens_mvn_ros2_xme/config/xsens_xme_node.yaml` and `config/body_dimensions.yaml`

Key parameters: `awindaChannel`, `sampleRate` (default `240 Hz`), `biomechanicalModel` (`Legacy` / `Female` / `Male`), `suitConfiguration`.

### xsens_mvn_ros2_urdf_publisher_node

Reads live TF data to measure subject body proportions and publishes a scaled URDF to `/robot_description`. Lifecycle node.

**Launch:** Included automatically in `xsens_stream.launch.py` and `xsens_xme.launch.py` when `launch_description:=true`.
**Config:** `src/pkgs/xsens_mvn_ros2_description/config/xsens_urdf_publisher_node.yaml`

Service `~/republish_urdf` (`std_srvs/Trigger`) forces immediate re-publication. Rejects requests when the node is not active.

---

## Custom Messages

### LinkState.msg

Full kinematic state for a single body segment.

```
std_msgs/Header header
geometry_msgs/Pose  pose   # position + orientation in reference frame
geometry_msgs/Twist twist  # linear and angular velocity
geometry_msgs/Accel accel  # linear and angular acceleration
```

### LinkStateArray.msg

```
std_msgs/Header header
xsens_mvn_msgs/LinkState[] states
```

### LoadCalibrationPath.srv

```
# Request
string calibration_file_path
---
# Response
string message
bool   success
```

### PerformCalibration.action

```
# Goal
int8 NPOSE = 0
int8 TPOSE = 1
int8   pose             # 0 = N-Pose, 1 = T-Pose
string output_file_name # must include .mvn extension
---
# Result
string calibration_result
---
# Feedback
string status           # real-time progress updates
```

---

## Configuration

### Body Dimensions

Edit `src/pkgs/xsens_mvn_ros2_xme/config/body_dimensions.yaml` to match your subject. All values are in metres.

```yaml
/**:
  ros__parameters:
    bodyDimension:
      bodyHeight:      1.85
      footSize:        0.31
      shoulderHeight:  1.44
      shoulderWidth:   0.38
      elbowSpan:       0.90
      wristSpan:       1.50
      armSpan:         1.78
      hipHeight:       1.07
      hipWidth:        0.32
      kneeHeight:      0.59
      ankleHeight:     0.13
      shoeSoleHeight:  0.00
```

These values are used by the XME node for biomechanical solving. The URDF publisher derives body proportions independently from live TF data.

### Awinda Channel

If you experience wireless interference, set a fixed channel in `xsens_xme_node.yaml`:

```yaml
awindaChannel: 15   # range 11-25; -1 = auto
```

---

## URDF and Visualization

The URDF publisher generates a body-proportioned skeleton by measuring TF distances between adjacent joints and scaling each mesh accordingly:

| Segment group | Scale axis | Reference distance |
|---------------|------------|-------------------|
| Pelvis (width) | Y | Bilateral hip-joint distance / 0.16 m |
| Spine / legs | Z | TF segment length / neutral mesh length |
| Arms / shoulders | Y | TF segment length / neutral mesh length |
| Feet | Uniform | Ankle-to-ball-of-foot distance / 0.1526 m |

Scales are clamped to the range [0.5, 2.0]. The static template is `src/pkgs/xsens_mvn_ros2_description/urdf/humanoid.urdf.xacro`; meshes are in `urdf/meshes/neutral/`.

**RViz configuration** — a pre-built workspace is available at:
`src/pkgs/xsens_mvn_ros2_description/config/xsens_visualization.rviz`

---

## Using XML Launch Files

The original XML launch files are still included alongside the Python ones. They start the lifecycle nodes in the **unconfigured** state, so you must manage state transitions manually. This is useful for debugging, staged bringup, or integration into a larger launch system that manages lifecycle externally.

### Available XML Launch Files

| Package | File | Description |
|---------|------|-------------|
| `xsens_mvn_ros2_stream` | `xsens_stream.launch.xml` | Stream node + optional RViz + optional URDF publisher |
| `xsens_mvn_ros2_xme` | `xsens_xme.launch.xml` | XME node + optional RViz + optional URDF publisher |
| `xsens_mvn_ros2_description` | `description.launch.xml` | URDF publisher standalone |
| `xsens_mvn_ros2` (meta) | `xsens_stream.launch.xml` | Wrapper that includes the stream package launch |
| `xsens_mvn_ros2` (meta) | `xsens_xme.launch.xml` | Wrapper that includes the XME package launch |
| `xsens_mvn_ros2` (meta) | `description.launch.xml` | Wrapper that includes the description package launch |

### Example: Stream Mode with XML

```bash
# 1. Launch the node (starts in unconfigured state)
ros2 launch xsens_mvn_ros2 xsens_stream.launch.xml

# 2. In another terminal, configure and activate the stream node
ros2 lifecycle set /xsens_mvn_ros2_stream_node configure
ros2 lifecycle set /xsens_mvn_ros2_stream_node activate

# 3. Configure and activate the URDF publisher (if launch_description:=true)
ros2 lifecycle set /xsens_urdf_publisher configure
ros2 lifecycle set /xsens_urdf_publisher activate
```

### Example: XME Mode with XML

```bash
# 1. Launch the node
ros2 launch xsens_mvn_ros2 xsens_xme.launch.xml

# 2. Configure and activate (hardware scan happens during configure)
ros2 lifecycle set /xsens_mvn_ros2_xme_node configure
ros2 lifecycle set /xsens_mvn_ros2_xme_node activate

# 3. Activate the URDF publisher
ros2 lifecycle set /xsens_urdf_publisher configure
ros2 lifecycle set /xsens_urdf_publisher activate
```

### Example: Pause and Resume

The XML launch approach is useful when you want to pause and resume data publishing without restarting the node:

```bash
# Pause publishing (stream client stays connected, XME hardware stays connected)
ros2 lifecycle set /xsens_mvn_ros2_stream_node deactivate

# Resume publishing
ros2 lifecycle set /xsens_mvn_ros2_stream_node activate
```

### Python Launch Arguments

| Argument | Default | Description |
|----------|---------|-------------|
| `auto_activate` | `true` | Automatically configure and activate all lifecycle nodes on startup. Set to `false` for manual lifecycle control. |
| `namespace` | `""` | ROS2 namespace prefix for all nodes, topics, and services. Useful for multi-suit setups. |
| `model_name` | `"skeleton"` | Passed to node and URDF publisher |
| `launch_rviz` | `true` | Whether to start RViz |
| `launch_description` | `true` | Whether to start the URDF publisher node |
| `discovery_range` | `LOCALHOST` | DDS discovery scope |
| `rviz_config_file` | built-in | Path to a custom RViz config |

### XML Launch Arguments

The XML launches accept the same arguments as the Python versions, except `auto_activate` and `namespace` (XML launches always start in the unconfigured state with no namespace):

| Argument | Default | Description |
|----------|---------|-------------|
| `model_name` | `"skeleton"` | Passed to node and URDF publisher |
| `launch_rviz` | `true` | Whether to start RViz |
| `launch_description` | `true` | Whether to start the URDF publisher node |
| `discovery_range` | `LOCALHOST` | DDS discovery scope |
| `rviz_config_file` | built-in | Path to a custom RViz config |

---

## Code Style Guidelines

### C++ Standard and Compiler Flags

- **Standard:** C++17
- **Compiler warnings:** `-Wall -Wextra -Wpedantic` (enforced in every `CMakeLists.txt`)
- **Linker safety:** `-Wl,--no-undefined` (catches missing symbols at link time)

### Naming Conventions

| Element | Convention | Example |
|---------|-----------|---------|
| **Files** | `snake_case` with `.hpp`/`.cpp` | `skeleton_publisher.hpp`, `xsens_stream_node.cpp` |
| **Classes / Structs / Enums** | `PascalCase` | `SkeletonPublisher`, `SegmentKinematics`, `ConfigurationResult` |
| **Functions / Methods** | `camelCase` | `initializeXsensStreamClient()`, `publishLinkStates()`, `diagnosticsCallback()` |
| **Member variables** | `m_` prefix + `camelCase` | `m_tfBroadcaster`, `m_paramListener`, `m_lastDataTimeNs` |
| **Constants** | `k` prefix + `PascalCase` | `kParent`, `kEmpty` |
| **Namespaces** | `snake_case` | `xsens_mvn_ros2`, `xsens_xme_ros2` |
| **ROS2 topics / services** | `snake_case` | `~/joint_states`, `~/perform_calibration` |
| **TF frame names** | `{model_name}_{segment}` | `skeleton_pelvis`, `skeleton_left_hand` |

### Header Files

- Use **`#pragma once`** for include guards.
- Prefer **angle-bracket includes** (`<package/header.hpp>`) for installed package headers.
- Group includes in order: standard library, third-party (Eigen, Boost), ROS2, project headers.

### Documentation

- Use **`///`** (Doxygen-style) doc comments on public APIs (classes, methods, structs).
- Add **`@param`** and **`@return`** annotations for non-trivial interfaces.
- Inline comments only where logic is not self-evident.

### ROS2-Specific Patterns

- **Use lifecycle nodes** (`rclcpp_lifecycle::LifecycleNode`) for all nodes. Perform resource allocation in `on_configure`, start publishing in `on_activate`, and release resources in `on_cleanup`. Use `LifecyclePublisher` instead of `rclcpp::Publisher`.
- **No raw `declare_parameter` calls.** Use `generate_parameter_library` with a YAML definition.
- **Inject `rclcpp::Logger`** into library classes — do not create nodes just for logging.
- **Publish diagnostics** via `diagnostic_updater::Updater` on every node.
- **Use `RCLCPP_WARN_THROTTLE`** (5 s) for transient/retrying conditions — never spam logs.
- **Guard services and actions** with a lifecycle state check (`get_current_state().id() == PRIMARY_STATE_ACTIVE`) to reject requests when the node is not active.

### CMake Patterns

- Every package uses `ament_cmake` as the build type.
- Shared libraries are named `${PROJECT_NAME}_ComponentName` (e.g. `xsens_mvn_ros2_stream_HumanDataHandler`).
- Use `BUILD_INTERFACE` / `INSTALL_INTERFACE` generator expressions for include directories.
- Export targets via `ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)`.
- Generated parameter libraries are declared with `generate_parameter_library()` and linked as a regular CMake target.

### Linting

All packages declare `ament_lint_auto` + `ament_lint_common` as test dependencies. This enables the standard ROS2 linting suite automatically:

- **ament_copyright** — license header check (**currently disabled** — `set(ament_cmake_copyright_FOUND TRUE)` in each `CMakeLists.txt` until the project license is finalized; re-enable by removing that line and adding the appropriate copyright/license headers to all source files)
- **ament_cppcheck** — static analysis
- **ament_cpplint** — Google C++ style subset
- **ament_flake8** — Python linting (launch files)
- **ament_lint_cmake** — CMakeLists.txt linting
- **ament_xmllint** — XML validation (package.xml, launch files)
- **ament_uncrustify** — C++ code formatting (the default ROS2 formatter; no custom `.clang-format` overrides)

---

## Testing

### Test Framework

Unit tests use **Google Test** (`ament_cmake_gtest`). The `colcon_defaults.yaml` configures sequential test execution with up to 3 retries on failure.

### Existing Tests

| Package | Test | What it covers |
|---------|------|---------------|
| `xsens_mvn_ros2_common` | `test_xsens_model` | `xmeSegmentToCanonical`, `kineticParent` lookups |
| `xsens_mvn_ros2_common` | `test_skeleton_publisher` | POD types, arm correction functions, frame naming |
| `xsens_mvn_ros2_common` | `test_skeleton_publisher_node` | TF publishing, relative transforms via lifecycle node (configure + activate) |
| `xsens_mvn_ros2_stream` | `test_human_data_handler` | `setLink`/`getLink` roundtrip, COM, thread safety |
| `xsens_mvn_ros2_description` | `test_segment_defs` | No null parents, no duplicates, positive reference lengths |
| `xsens_mvn_ros2_xme` | Stub only | Hardware-in-the-loop not available without suit |

### Running Tests

```bash
source /opt/ros/jazzy/setup.bash
source install/setup.bash

# Run all tests
colcon test

# Run tests for a specific package
colcon test --packages-select xsens_mvn_ros2_stream

# View results
colcon test-result --verbose
```

---

## Contributing

### Branching

Feature branches follow the pattern `feature/<ticket>_<description>` (e.g. `feature/2588_create_ros_complient_error_handling`).

### Adding a New Package

1. Create the package under `src/pkgs/`.
2. Add it to the `packages-up-to` list in `colcon_defaults.yaml`.
3. Add `ament_lint_auto` and `ament_lint_common` as test dependencies in `package.xml`.
4. Add `ament_lint_auto_find_test_dependencies()` in your `CMakeLists.txt` testing block.
5. If the meta-package should depend on it, add an `<exec_depend>` to `src/pkgs/xsens_mvn_ros2/package.xml`.

---

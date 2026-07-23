# Xsens XME ROS 2 Jazzy Docker 설정

이 문서는 Ubuntu 22.04 및 ROS 2 Humble 호스트에서 Xsens XME 노드를
Ubuntu 24.04/ROS 2 Jazzy Docker 컨테이너로 실행하기 위해 진행한 설정을
정리한다.

## 구성 개요

```text
Ubuntu 22.04 호스트 (ROS 2 Humble)
├── Docker Engine + Compose
├── X11/XWayland
└── Xsens USB 장비
    ├── Xsens Awinda Station (2639:0101)
    └── Xsens MTw            (2639:0200)
          │
          │ USB 및 udev 전달
          ▼
Ubuntu 24.04 컨테이너 (ROS 2 Jazzy)
├── vendored XME SDK
├── Sentinel HASP/LDK runtime
├── Xsens Linux License Manager
├── xsens_mvn_ros2_xme
└── RViz2
```

XME SDK의 공유 라이브러리는 Linux amd64 전용이다. 현재 Compose 구성도
`platform: linux/amd64`로 고정되어 있다.

## 현재 작업 체크포인트

현재까지 확인된 상태:

- Docker/Jazzy 이미지 빌드 완료
- 컨테이너 GUI용 X11/`DISPLAY` 구성 완료
- 호스트와 컨테이너에서 Awinda Station 및 MTw USB 인식 확인
- Linux License Manager와 Sentinel runtime 다운로드 완료
- `xsens-xme-license` 영구 컨테이너 생성
- 호스트 Downloads가 컨테이너 `/downloads`에 read-only로 마운트됨
- Sentinel `.deb`가 컨테이너 `/tmp`에 복사됨

현재 컨테이너의 `/tmp`에서 이어서 실행할 명령:

```bash
apt-get install -y /tmp/xsens-h*.deb
/etc/init.d/aksusbd start
```

그다음 이 문서의 **6. Xsens Linux License Manager 실행 및 활성화** 절차로
진행한다.

## 1. Docker 이미지 빌드

레포 루트에서 실행한다.

```bash
cd ~/xsens/xsens_mvn_ros2
docker compose build xsens-jazzy
```

이미지의 entrypoint가 다음 환경을 자동으로 source한다.

```text
/opt/ros/jazzy/setup.bash
/opt/xsens_ws/install/setup.bash
```

## 2. 필요한 Xsens 라이선스 파일

Xsens 공식 Software & Documentation 페이지의 **Xsens Humanoid** 영역에서
다음 Linux 파일을 다운로드한다.

```text
Xsens-License-Manager-2026.6.0.appimage
xsens-hasp-runtime_10.21.1-1_amd64.deb
```

다운로드 위치:

```text
/home/jongbin/Downloads
```

레포에 포함된 XME SDK를 사용하므로 아래 파일은 추가 설치할 필요가 없다.

```text
xsens-xme-sdk_2026.6.0-1_amd64.deb
```

라이선스 키가 일반 Analyze/Animate 키가 아니라 `XME-SDK` entitlement를
포함하는지 확인해야 한다. 키 문자열 자체는 문서나 레포에 기록하지 않는다.

## 3. Docker GUI 권한 허용

Compose 설정은 호스트의 `DISPLAY`와 `/tmp/.X11-unix`를 컨테이너에
전달한다. 컨테이너를 실행하기 전에 호스트에서 root 컨테이너의 X11 접근을
허용한다.

```bash
xhost +si:localuser:root
```

작업이 끝난 뒤에는 권한을 회수한다.

```bash
xhost -si:localuser:root
```

## 4. Downloads를 마운트한 영구 컨테이너 생성

Sentinel과 License Manager를 컨테이너 내부에 설치하므로 `--rm`을 사용하지
않는다. `--rm`으로 실행하면 컨테이너 종료 시 설치 내용과 활성화 정보가
삭제될 수 있다.

호스트에서 실행:

```bash
cd ~/xsens/xsens_mvn_ros2

docker compose run \
  --name xsens-xme-license \
  -v "$HOME/Downloads:/downloads:ro" \
  xsens-jazzy
```

마운트 의미:

```text
호스트:    /home/jongbin/Downloads
컨테이너: /downloads
권한:      read-only
```

컨테이너 내부에서 파일을 확인한다.

```bash
ls -lh /downloads
```

## 5. Sentinel HASP/LDK runtime 설치

파일명을 직접 다시 입력할 때 발생할 수 있는 오타를 피하기 위해 glob을
사용한다.

컨테이너 내부에서:

```bash
cp /downloads/xsens-h*.deb /tmp/
apt-get update
apt-get install -y /tmp/xsens-h*.deb
```

`apt-get update` 전에 `apt install vim` 등을 실행하면 패키지 목록이 비어
있어 `Unable to locate package`가 발생할 수 있다.

Sentinel daemon을 시작한다.

```bash
/etc/init.d/aksusbd start
```

프로세스를 확인한다.

```bash
ps aux | grep -E 'aksusbd|hasplmd'
```

Sentinel Admin Control Center는 다음 주소에서 확인할 수 있다.

```text
http://localhost:1947
```

Compose가 host network를 사용하므로 호스트 브라우저에서도 같은 주소로
접근할 수 있다.

## 6. Xsens Linux License Manager 실행 및 활성화

AppImage를 읽기 전용 마운트 밖으로 복사하고 실행 권한을 부여한다.

```bash
cp /downloads/Xsens-License-Manager-*.appimage \
  /opt/xsens-license-manager.appimage

chmod +x /opt/xsens-license-manager.appimage
```

FUSE 장치 없이 AppImage를 실행한다.

```bash
/opt/xsens-license-manager.appimage \
  --appimage-extract-and-run
```

GUI가 sandbox 오류로 종료되는 경우:

```bash
/opt/xsens-license-manager.appimage \
  --appimage-extract-and-run \
  --no-sandbox
```

GUI에서 Xsens가 제공한 `XME-SDK` product key를 활성화한다. 활성화 후
`http://localhost:1947`의 `Sentinel Keys`, `Products` 또는 `Features`
화면에서 Xsens/XME 관련 항목을 확인한다.

## 7. XME 노드와 RViz 실행

같은 컨테이너에서 실행한다.

```bash
ros2 launch xsens_mvn_ros2 xsens_xme.launch.py \
  launch_rviz:=true \
  discovery_range:=SUBNET \
  user_path:=/var/lib/xsens \
  log_path:=/var/log/xsens
```

RViz 없이 먼저 확인하려면:

```bash
ros2 launch xsens_mvn_ros2 xsens_xme.launch.py \
  launch_rviz:=false \
  discovery_range:=SUBNET \
  user_path:=/var/lib/xsens \
  log_path:=/var/log/xsens
```

정상 동작 시 예상되는 주요 로그:

```text
XME Version: ...
Waiting for hardware connection...
Hardware is ready
Activated.
```

## 8. 토픽 확인

XME launch를 실행한 상태에서 두 번째 Jazzy 컨테이너 셸을 연다.

```bash
cd ~/xsens/xsens_mvn_ros2
docker compose run --rm xsens-jazzy
```

컨테이너 내부에서:

```bash
ros2 node list
ros2 topic list
ros2 topic hz /tf
ros2 topic echo /diagnostics
ros2 topic hz /xsens_mvn_ros2_xme_node/joint_states
ros2 topic echo /xsens_mvn_ros2_xme_node/link_states --once
```

실제 토픽 이름이 다르면 `ros2 topic list`에 출력된 이름을 사용한다.

## 9. 캘리브레이션

N-pose calibration:

```bash
ros2 action send_goal \
  /xsens_mvn_ros2_xme_node/perform_calibration \
  xsens_mvn_msgs/action/PerformCalibration \
  "{pose: 0, output_file_name: 'my_calibration.mvn'}" \
  --feedback
```

저장된 calibration 불러오기:

```bash
ros2 service call \
  /xsens_mvn_ros2_xme_node/set_calibration_path \
  xsens_mvn_msgs/srv/LoadCalibrationPath \
  "{calibration_file_path: '/var/lib/xsens/my_calibration.mvn'}"
```

`/var/lib/xsens`와 `/var/log/xsens`는 Compose named volume에 연결되어
컨테이너와 별도로 유지된다.

## 10. 기존 라이선스 컨테이너 재진입

컨테이너에서 `exit`한 뒤에는 새 컨테이너를 만들지 않고 기존 컨테이너를
다시 시작한다.

```bash
xhost +si:localuser:root
docker start -ai xsens-xme-license
```

컨테이너가 재시작되면 Sentinel daemon을 다시 시작해야 할 수 있다.

```bash
/etc/init.d/aksusbd start
```

그 후 XME launch를 다시 실행한다.

## 11. Humble 호스트와 ROS 통신

호스트 Humble 프로젝트와 컨테이너 Jazzy의 `ROS_DOMAIN_ID`를 동일하게
설정한다.

```bash
ROS_DOMAIN_ID=7 docker compose run --rm xsens-jazzy
```

`/tf` 및 `sensor_msgs/msg/JointState` 같은 동일한 표준 인터페이스는 DDS를
통해 교환할 수 있다. Humble 프로젝트에서
`xsens_mvn_msgs/msg/LinkStateArray`를 구독하려면 Humble 워크스페이스에도
동일한 `xsens_mvn_msgs` 정의를 빌드해야 한다.

## 12. 문제 해결

### `Sentinel protection key was not found (H0007)`

XME 노드가 하드웨어 검색을 시작하기 전에 라이선스 초기화에서 종료된
상태다. 다음을 확인한다.

```bash
/etc/init.d/aksusbd start
ps aux | grep -E 'aksusbd|hasplmd'
```

그리고 `http://localhost:1947`에서 XME-SDK feature가 보이는지 확인한다.

### `Hardware connection timeout after 30 seconds`

라이선스 확인은 통과했지만 XME가 장비에 연결하지 못한 상태다. 호스트에서:

```bash
lsusb | grep -i xsens
```

현재 확인된 USB 장비는 다음과 같다.

```text
2639:0101 Xsens Awinda Station
2639:0200 Xsens MTw
```

컨테이너는 Compose를 통해 `/dev/bus/usb`와 `/run/udev`를 전달받는다.

### `Unable to locate package`

컨테이너 이미지는 apt 목록을 제거한 상태로 배포되므로 먼저 실행한다.

```bash
apt-get update
```

### Downloads 파일을 찾지 못함

`/downloads`는 자동으로 보이지 않는다. 컨테이너 생성 시 다음 마운트가
필요하다.

```bash
-v "$HOME/Downloads:/downloads:ro"
```

실행 중인 컨테이너에는 새로운 bind mount를 추가할 수 없으므로 마운트가
빠진 컨테이너는 종료하고 올바른 옵션으로 새로 생성한다.

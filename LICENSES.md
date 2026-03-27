# License Overview

This document provides an overview of all licenses used throughout the
xsens_mvn_ros2 repository.

## Project License

| Item | Details |
|------|---------|
| License | BSD-3-Clause |
| Copyright | (c) 2026, Xsens Technologies B.V. |
| File | [LICENSE](LICENSE) |

Applies to all source code under `src/pkgs/` (except where noted below).

## Per-Package Licenses

### Our Packages (`src/pkgs/`)

| Package | License (package.xml) | LICENSE File | Copyright |
|---------|-----------------------|--------------|-----------|
| xsens_mvn_ros2 | BSD-3-Clause | Root [LICENSE](LICENSE) | Xsens Technologies B.V. |
| xsens_mvn_ros2_common | BSD-3-Clause | Root [LICENSE](LICENSE) | Xsens Technologies B.V. |
| xsens_mvn_ros2_stream | BSD-3-Clause | Root [LICENSE](LICENSE) | Xsens Technologies B.V. |
| xsens_mvn_ros2_xme | BSD-3-Clause | Root [LICENSE](LICENSE) | Xsens Technologies B.V. |
| xsens_mvn_ros2_description | BSD-3-Clause | Root [LICENSE](LICENSE) | Xsens Technologies B.V. |
| xsens_mvn_msgs | BSD | [src/pkgs/xsens_mvn_msgs/LICENSE](src/pkgs/xsens_mvn_msgs/LICENSE) | 2023 HRII Lab (MIT) |

### Third-Party Dependencies (`src/deps/`)

| Package | License (package.xml) | LICENSE File | Copyright |
|---------|-----------------------|--------------|-----------|
| xsens_mvn_sdk | BSD | [src/deps/xsens_mvn_sdk/LICENSE](src/deps/xsens_mvn_sdk/LICENSE) | Public domain (Unlicense) |
| xsens_xme_sdk | Apache-2.0 | None | Xsens (vendored binary SDK) |

## Source File Headers

All source files authored by Xsens under `src/pkgs/` carry the following
header (C++ and Python variants):

```
// Copyright (c) 2026, Xsens Technologies B.V.
// SPDX-License-Identifier: BSD-3-Clause
```

### Exceptions

| File | License | Origin |
|------|---------|--------|
| `src/pkgs/xsens_mvn_ros2_stream/include/xsens_mvn_ros2_stream/socket.hpp` | Unlicense (public domain) | Inherited from xsens_mvn_sdk |
| `src/deps/xsens_xme_sdk/include/xstypes/pstdint.h` | BSD-3-Clause | Paul Hsieh (2005-2007) |

## Third-Party License Details

### xsens_mvn_sdk — Unlicense

The MVN UDP protocol parser SDK is released into the public domain under the
[Unlicense](https://unlicense.org). All header and source files under
`src/deps/xsens_mvn_sdk/` contain the Unlicense notice. This is compatible
with the project's BSD-3-Clause license.

### xsens_xme_sdk — Apache-2.0

The XME hardware SDK is a vendored binary SDK licensed under
[Apache-2.0](https://www.apache.org/licenses/LICENSE-2.0). Apache-2.0 is
compatible with BSD-3-Clause.

### xsens_mvn_msgs — MIT

The custom message definitions package retains its original
[MIT](https://opensource.org/licenses/MIT) license from the HRII Lab import.
MIT is compatible with BSD-3-Clause.

### pstdint.h — BSD-3-Clause

A portable stdint.h implementation by Paul Hsieh, bundled within the XME SDK.
Licensed under BSD-3-Clause.

## License Compatibility

All third-party licenses in this repository (Unlicense, MIT, Apache-2.0,
BSD-3-Clause) are permissive and compatible with the project's BSD-3-Clause
license.

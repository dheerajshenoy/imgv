# imgv
Minimal image viewer

# Installation

1. Install the following dependencies:

    - Qt6
    - ImageMagick
    - cmake & ninja (for building)

2. Run the following commands to clone the repo and build imgv

```bash
git clone github.com/dheerajshenoy/imgv
cd imgv
mkdir build && cd build
cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug
ninja
sudo ninja install
```

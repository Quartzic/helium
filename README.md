# Description
Helium allows you to quickly extract cropped faces from photos, which is useful for profile pictures, ID cards, and other situations in which you need standardization.

# Usage
Run `helium` to see the available functions.

Run `helium crop-faces <arguments>` to crop a photo.

Run `helium crop-faces --help` to see available arguments.

# Build
```
git clone https://github.com/Quartzic/helium.git
cd helium
cmake --build .
```

# Requirements
To compile from source, this project requires [OpenCV](https://github.com/opencv/opencv), [Boost](https://github.com/boostorg/boost), and [CMake](https://cmake.org) 3.17 or greater.

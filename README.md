# Description
Helium allows you to quickly extract cropped faces from photos, which is useful for profile pictures, ID cards, and other situations in which you need standardization.

# Usage
Run `helium` to see the available arguments.

Run `helium --input <photo_path>` to crop a photo using the defaults.

# Build
```
git clone https://github.com/Quartzic/helium.git
cd helium
cmake . && cmake --build .
```

# Requirements
To compile from source, this project requires [OpenCV](https://github.com/opencv/opencv), [Boost](https://github.com/boostorg/boost), and [CMake](https://cmake.org) 3.17 or greater.

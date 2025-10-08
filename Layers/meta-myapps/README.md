# Yocto Application Recipes: CMake and g++

This layer contains two simple examples of how to build and install an application in Yocto:
one using **CMake** and one using a direct **g++ compile** step.

## Structure
```
meta-myproject/
├── recipes-apps/
│   ├── hello-cmake/
│   │   ├── files/
│   │   │   ├── hello.cpp
│   │   │   └── CMakeLists.txt
│   │   └── hello-cmake_1.0.bb
│   └── hello-gpp/
│       ├── files/
│       │   └── hello.cpp
│       └── hello-gpp_1.0.bb
└── LICENSE
```

## 1️⃣ hello-cmake
- Uses the **CMake** build system (inherits `cmake`).
- Automatically detects source files from `CMakeLists.txt`.
- Produces `/usr/bin/hello_cmake` in the target image.

## 2️⃣ hello-gpp
- Builds directly using g++ with a manual `do_compile()` task.
- Simpler, faster to prototype.
- Produces `/usr/bin/hello_gpp`.

## Adding the Layer
In your `bblayers.conf`:
```
BBLAYERS += "path/to/meta-myproject"
```

## Building
To build either app as part of your image:
```
bitbake hello-cmake
bitbake hello-gpp
```

To include them in an image:
Add to `IMAGE_INSTALL` in your image recipe:
```
IMAGE_INSTALL:append = " hello-cmake hello-gpp"
```

Then rebuild your image:
```
bitbake core-image-weston
```

Each app will be available in `/usr/bin` on your target device.

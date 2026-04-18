# Building Revive

For your convenience, there is a script to help retrieve and set up
dependencies in the project root. This script will also attempt to build OpenXR
and Revive.

Instructions for manual build are below.

## Manual

Before the project can be built, you must retrieve some external dependencies through [vcpkg](https://docs.microsoft.com/en-us/cpp/build/vcpkg).

In bash for Windows:

```
git clone git@github.com:microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
```

Now that vcpkg has been installed we need to install the dependencies and integrate with Visual Studio 2019

```
vcpkg install --triplet x86-windows-static detours openxr-loader glfw3
vcpkg install --triplet x64-windows-static detours openxr-loader glfw3
vcpkg integrate install
```

Now we're ready to clone the Revive repository and set up vendored dependencies.

```
cd ..
git clone --recursive git@github.com:LibreVR/Revive.git
```

Download the Oculus SDK for Windows
[here](https://developer.oculus.com/downloads/package/oculus-sdk-for-windows/)
and place it in `Revive/Externals/'. Then:

```
cd Revive/Externals
unzip ovr_sdk_win_<version>.zip
```

The Revive, ReviveXR and ReviveInjector projects can then build normally in Visual Studio 2019.

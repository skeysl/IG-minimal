# IG-minimal

Minimal project to reproduce Vulkan present barrier problem.

## General System Requirements

* Windows system. 
* Three Quadro RTX graphic cards. 
* Each Quadro RTX card is having one output monitor connected.
* All three cards are synchronized via Quadro Sync II.
* Display synchronization is enabled via Nvidia Control Panel.

## Specific System Configuration

* AMD Epyc 72F3
* Supermicro H12SSL-i
* 64GB RAM
* 3x Quadro RTX A5500 + Quadro Sync II
* Windows 10, driver 527.27

## Bulding

Latest Vulkan SDK (1.3.239.0) has to present on the system.

```
mkdir build && cd build
conan install ..
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
cmake --build . --config Release
```

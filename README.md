# IG-minimal

Minimal project to reproduce Vulkan present barrier problem.

## System Configuration

* AMD Epyc 72F3
* Supermicro H12SSL-i
* 64GB RAM
* 3x Quadro RTX A5500 + Quadro Sync II
* Each Quadro RTX card is having one output monitor connected.
* Display synchronization is enabled via Nvidia Control Panel.
* Windows 10, driver 528.02

<img width="75%" alt="Sync" src="https://user-images.githubusercontent.com/7813849/218439066-1b4d8092-58c3-46a4-9330-d74b56aa6297.png">

## Prerequisites

* Visual Studio 2022 with bundled Cmake (3.24)
* Conan 1.53.0
* Latest Vulkan SDK (1.3.239.0) has to be present on the system.

## Building

```
mkdir build && cd build
conan install ..
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
cmake --build . --config Release
```

## Project structure

### ig
Header only library with simple utilities (timer and logger).

### igx 
Very simple RAII wrappes of Vulkan and OpenGL primitives to simplify lifetime management. 

### igui 
Classes for opening win32 windows, managing message loop, creating of OpenGL render context with correct GPU affinity, setting up swap groups and swap barriers

### vksync 

Test application creates three rendering threads (one thread per GPU). Each thread creates one fullscreen window on the selected monitor and renders a rotating triangle. 

## Renderers

There are three renderers implemented in vksync application - Vulkan renderer, OpenGL renderer and renderer using Vulkan/OpenGL interop. For project simplicity active renderer is hardcoded and called from main function.

### Vulkan renderer 

One vulkan instance is created. In each thread after the window is created, a logical device is created for the selected GPU. In case of nv_present_barrier extension being supported this feature is enabled when a logical device is created. Later the Vulkan surface is constructed and a swapchain is created (fifo). This swapchain is instrued to use present barriers upon construction (via VkSwapchainPresentBarrierCreateInfoNV). Later the actual triangle renderer is created. While rendering common swapchain acquire/present methods are used. Command buffers are recycled in common round robin fashion. There is no additional synchronization between those three threads (CPU or GPU).

Rendering works only when two GPUs are used. 

![PXL_20230210_075045848](https://user-images.githubusercontent.com/7813849/218450974-47921cf6-6d6f-4efa-b093-e46e10a7e233.jpg)

When three GPUs are used application hungs at the startup:

![PXL_20230210_075105851](https://user-images.githubusercontent.com/7813849/218450897-09e2ccf4-6268-48a9-aff7-b4e2a9bcca49.jpg)

Without a present barrier it's possible to have an application running. Rendering is not fluent. After observing Nsight Systems capture of vksync it's visible that vkQueuePresent on any GPU is blocking vkQueueSubmits on other's GPU: 

![image](https://user-images.githubusercontent.com/7813849/218451970-8b3e80de-0e07-4c83-b043-e26d96e8c5f1.png)

### OpenGL renderer

When render window is being created, render context is created for correct GPU affinity and DC is added to swap group and binded to swap barrier. 

Rendering on all three GPUs works. 

![PXL_20230210_074723135](https://user-images.githubusercontent.com/7813849/218451151-26a54312-a948-4da8-a296-3cb37faf6c96.jpg)

### Vulkan/OpenGL interop renderer

In this case Vulkan renderer created for specific GPU renders into texture shared with OpenGL via interop mechanism. OpenGL is used only to present rendered texture via fullscreen window with active swap group/barrier. 

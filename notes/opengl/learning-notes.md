# OpenG

## Graphics Pipeline

The job of the graphics pipeline is to transform 3D coordinates into 2D coordinates, first.

Then, those 2D coordinates are turned into actual colored pixels.

A modern graphics pipeline is a series of steps that have highly specialized functions, where each step takes as input the output of the previous step, and outputs data for the next step to handle.

Usually, each step can also be executed in parallel. That's why GPUs today have thousands of processing cores to quickly process your data.

A processing core runs a small program on the GPU, and these small programs are called **shaders**.

The language shaders are written in depends on the graphics API. For OpenGL, it is the *OpenGL Shading Language (GLSL)*.



# OpenGL

A modern OpenGL learning project built with C++ using GLFW, GLEW, CMake, and GLSL.

## About

This repository contains my hands-on journey of learning OpenGL and real-time graphics programming using modern C++.

The project is being developed incrementally, starting from the fundamentals of creating an OpenGL application and gradually exploring the OpenGL rendering pipeline, buffers, shaders, and other graphics concepts.

The main focus is on understanding how OpenGL works internally and implementing the concepts through practical C++ code.

## Learning Source

This project is primarily based on the **Welcome to OpenGL** YouTube playlist by The Cherno.

The concepts are being implemented step by step while developing this project.

## Technologies

- C++
- OpenGL
- GLSL
- GLFW
- GLEW
- CMake
- Ninja
- Visual Studio

## Current Progress

- [x] GLFW window creation
- [x] OpenGL context creation
- [x] GLEW initialization
- [x] Vertex Buffer Object (VBO)
- [x] Vertex Array Object (VAO)
- [x] Vertex attribute configuration
- [x] Vertex shader
- [x] Fragment shader
- [x] External shader file
- [x] Shader file parsing
- [x] Shader compilation
- [x] Shader program creation
- [x] Shader program linking
- [x] Triangle rendering

## Project Structure

```text
OpenGL/
│
├── CMakeLists.txt
├── CMakePresets.json
├── .gitignore
├── README.md
│
├── src/
│   └── main.cpp
│
└── res/
    └── shaders/
        └── Basic.shader


## Shaders

The project uses an external shader file:

```text
res/shaders/Basic.shader
```

The file contains both the vertex and fragment shaders separated using shader markers:

```glsl
#shader vertex

#shader fragment
```

The C++ application:

1. Reads the shader file.
2. Separates the vertex and fragment shader source.
3. Compiles the shaders.
4. Creates an OpenGL shader program.
5. Links the shaders.
6. Uses the shader program for rendering.

## Building the Project

### Requirements

- Visual Studio with C++ development tools
- CMake
- Ninja
- Git
- Windows x64

### Configure

From the project root:

```bash
cmake --preset x64-debug
```

### Build

```bash
cmake --build out/build/x64-debug
```

### Run

After a successful build, run the generated executable from the build output directory.

## Dependencies

The project currently uses:

- GLFW
- GLEW

Dependencies are managed using CMake `FetchContent`.

They are not stored directly in this repository. CMake downloads and builds the required dependencies during the project configuration and build process.

## Development Environment

```text
Operating System : Windows
Language         : C++
Graphics API     : OpenGL
Window Library   : GLFW
Extension Loader : GLEW
Build System     : CMake
Build Tool       : Ninja
IDE              : Visual Studio
Architecture     : x64
```

## Status

 **In Development**

This project is actively evolving as I progress through the OpenGL learning material and implement each concept in code.
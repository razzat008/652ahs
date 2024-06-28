# 652ahs

This is an end-semester project done in 1st year 2nd semester at Kathmandu University.

# Building

## Prerequisites

- CMake
- SQLite3
- OpenGL

## Setting up

- To clone the repo and initialize all submodules, run:

```bash
git clone --recurse-submodules https://github.com/razzat008/652ahs
```

- `cd` into **652ahs** and

```bash
cmake -S . -B build/
```

- `cd` into **build/** and run the generated **MakeFile**

```bash
make
```

and to run the obtained binary

```bash
./652ahs
```

_Below is the rough demonstration of the filetree of the project_

```
652ahs/
├── CMakeLists.txt
├── CMakeLists.txt
├── src/
│   └── main.cpp
│   └── ...
│   └── ...
└── libs/
│   └── imgui/
│       ├── backends/
│       │   ├── imgui_impl_glfw.cpp
│       │   ├── imgui_impl_glfw.h
│       │   ├── imgui_impl_opengl3.cpp
│       │   └── imgui_impl_opengl3.h
│       │   └── ...
│       │   └── ...
│       ├── imgui.cpp
│       ├── imgui.h
│       ├── imgui_demo.cpp
│       ├── imgui_draw.cpp
│       ├── imgui_internal.h
│       ├── imgui_widgets.cpp
│       ├── imstb_rectpack.h
│       ├── imstb_textedit.h
│       └── imstb_truetype.h
│       └── ...
└── ...
```

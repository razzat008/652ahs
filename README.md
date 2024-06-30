# 652ahs

This project is an end-semester project done in 1st year 2nd semester at Kathmandu University.

# Building

## Prerequisites

- CMake
- OpenGL
- SQLite3\
  Can be installed with\

```bash
#Arch Based systems:
sudo pacman -S sqlite3

#Debian Based systems:
sudo apt install sqlite3
```


## Setting up

- Clone the repository

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
├── src/
│   ├── main.cpp
│   └── ...
└── libs/
│   ├── glfw/
│   │   ├── docs
│   │   ├── examples/
│   │   └── ...
│   └── imgui/
│       ├── backends/
│       │   ├── imgui_impl_glfw.cpp
│       │   ├── imgui_impl_glfw.h
│       │   └── ...
│       ├── imgui.cpp
│       ├── imgui.h
│       └── ...
└── ...
```

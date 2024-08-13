# 652ahs

This is an end-semester project done in 1st year 2nd semester at Kathmandu University.

# Building

## Prerequisites

- CMake
- OpenGL\
  Can be installed with

```bash
#Debian Based systems:
sudo apt install mesa-utils xorg-dev libxkbcommon-dev libgl1-mesa-dev libglu1-mesa-dev libglfw3-dev   
```

- SQLite3\
  Can be installed with

```bash
#Arch Based systems:
sudo pacman -S sqlite3

#Debian Based systems:
sudo apt install libsqlite3-dev
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

## Checkout

- [boot.dev](https://blog.boot.dev/cryptography/how-sha-2-works-step-by-step-sha-256/) to learn more on how the algorithm works
- [this too](https://gist.github.com/hak8or/8794351) 

# 🧭 Pathfinding Visualizer

A simple C++ program to visualize pathfinding algorithms like BFS and DFS using SDL2. Designed for educational and demonstration purposes.

## ✨ Features

- Grid-based pathfinding visualizer
- Interactive placement of:
  - Start and End nodes
  - Walls/barriers
- BFS (Breadth-First Search) and DFS (Depth-First Search) algorithms
- Real-time animation using SDL2
- Simple and modular structure for easy extension

## 🛠 Dependencies

Make sure you have the following SDL2 libraries installed:

- SDL2 >= 2.0.22
- SDL2_ttf >= 2.20.2 (for text rendering)

### On MSYS2 (Windows)

```bash
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf
```

### On Ubuntu/Debian (Linux)

```bash
sudo apt install libsdl2-dev libsdl2-ttf-dev
```

## 🏗️ Build Instructions

This project uses a `Makefile` and a `config.mk` for easy compilation.

### 1. Clone the repo

```bash
git clone https://github.com/Mubaris2/Path-Visualizer.git
cd path-visualizer
```

### 2. Edit `config.mk`

Set your SDL2 include and lib paths:

```make
# Example config.mk
SDL2_CFLAGS := -IC:/msys64/mingw64/include/SDL2
SDL2_LIBS := -LC:/msys64/mingw64/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
```

### 3. Compile

```bash
make
```

## 🎮 How to Use

- Left-click to place Walls
- Right-click to unplace Walls
- Press:
  - `s` to place Start
  - `e` to place Destination
  - `ESC` to quit
- Click:
  - `BFS` to start **BFS**
  - `DFS` to start **DFS**
  - `Reset` to clear grid

## 📁 Project Structure

```
.
├── src/
│   ├── main.cpp
│   └── algorithms.cpp
├── fonts/
│   └── ...
├── include/
│   ├── globals.hpp
│   └── algorithms.hpp
├── bin/
│   └── ...
├── config.mk
├── Makefile
└── README.md

# 🎮 MazeGame - Console-Based Maze Adventure

<div align="center">

![C++](https://img.shields.io/badge/C%2B%2B-14-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)


*Escape procedurally generated mazes, collect power-ups, and outsmart your biggest threat – the Minotaur.*

[Features](#-features) • [Installation](#-installation) • [Usage](#-usage) • [Technical Details](#-technical-details) • [Project Structure](#-project-structure)

</div>

---

## 📋 About

MazeGame is a console-based adventure game developed as part of the **Object-Oriented Programming 2** course at the **Faculty of Technical Sciences, University of Novi Sad**. The project demonstrates advanced C++ concepts including OOP design patterns, procedural generation algorithms, and real-time console manipulation.

Navigate through procedurally generated mazes while avoiding a dangerous Minotaur, collecting power-ups, and racing to reach the exit. Each maze is unique, ensuring a fresh challenge every time you play!

## ✨ Features

### 🗺️ Procedural Maze Generation

- **DFS-based Algorithm**: Utilizes Depth-First Search with backtracking for maze generation
- **Guaranteed Solvability**: Every maze has a valid path from entrance to exit
- **Dynamic Sizing**: Configurable maze dimensions (minimum 15x15)
- **Smart Exit Placement**: Ensures the exit is always reachable even with obstacles

### 🎯 Gameplay Mechanics

- **Minotaur's Condition**: Minotaur is always positioned in the maze so that there is a way around it
- **Strategic Power-Ups**: Four unique power-ups that alter gameplay dynamics
- **Real-Time Updates**: Smooth console rendering without screen flickering
- **Win/Lose Conditions**: Multiple outcome scenarios based on player performance

### 💎 Power-Up System

| Power-Up | Effect | Duration |
|----------|--------|----------|
| ⚔️ **Sword** | Instantly defeats the Minotaur upon contact | 3 turns |
| 🛡️ **Shield** | Protects from Minotaur attacks | 3 turns |
| 🔨 **Hammer** | Break through internal walls (not outer boundaries) | 3 turns |
| 🌫️ **Fog of War** | Reduces visibility to a 3x3 area around the player | 3 turns |

### 🎨 Technical Highlights

- **Custom Console Rendering**: Uses Windows Console API for flicker-free updates
- **Memory Management**: Proper dynamic memory allocation and deallocation
- **Modular Architecture**: Clean separation between game logic, rendering, and maze generation
- **Object-Oriented Design**: Inheritance, polymorphism, and encapsulation throughout

## 🚀 Installation

### Prerequisites
- **Operating System**: Windows 10 or later
- **Compiler**: Visual Studio 2019 or later with C++14 support
- **Build Tools**: MSVC v142 or later

### Build Instructions

1. **Clone the repository**
   ```bash
   git clone https://github.com/0vertake/MazeGame.git
   cd MazeGame
   ```
2. **Open in Visual Studio**
   - Open `MazeGame.sln` in Visual Studio
   - Ensure the solution configuration is set to `Debug` or `Release`

3. **Build the solution**
   - Press `Ctrl + Shift + B` or
   - Navigate to `Build > Build Solution`

4. **Run the game**
   ```bash
   MazeGame.exe <rows> <cols> <numberOfPowerUps>
   ```

### Quick Start Examples
```bash
# Generate a 20x20 maze with 5 power-ups
MazeGame.exe 20 20 5

# Generate a 30x30 maze with 10 power-ups
MazeGame.exe 30 30 10
```

## 🎮 Usage

### Command Line Arguments
| Parameter | Description | Constraints |
|-----------|-------------|-------------|
| `rows` | Height of the maze | Minimum: 15 |
| `cols` | Width of the maze | Minimum: 15 |
| `numberOfPowerUps` | Number of power-ups spawned | Recommended: 3-10 |

### Controls
- **Arrow Keys** (↑ ↓ ← →): Move the robot
- **Q**: Quit the game

### Game Symbols
| Symbol | Meaning |
|--------|---------|
| `R` | Robot (Player) |
| `M` | Minotaur (Enemy) |
| `#` | Wall |
| `.` | Open path |
| `P` | Power-up |
| `U` | Entrance |
| `I` | Exit |

### Gameplay Tips
1. **Plan Your Route**: The Minotaur moves after every player move
2. **Power-Up Strategy**: Save defensive power-ups for dangerous encounters
3. **Wall Breaking**: Use the Hammer wisely to create shortcuts
4. **Fog Navigation**: The Fog of War affects visibility but can confuse the Minotaur's behavior

## 🔧 Technical Details

### Architecture
```
MazeGame/
├── MazeGame/          
│   ├── Main.cpp            # Entry point
│   ├── GameLogic.*         # Core game loop and input handling
│   ├── MazeMatrix.*        # Game state management
│   ├── Cell.*              # Individual maze cell representation
│   ├── PowerUp.*           # Abstract power-up base class
│   └── PowerUps.h          # Concrete power-up implementations
└── MazeLayout/             
    └── MazeLayout.*        # DFS-based procedural generation
```

### Key Algorithms

#### 1. Maze Generation (DFS with Backtracking)
```cpp
function generateMaze(rows, cols):
    grid = createInitialGrid(rows, cols)
    stack = []
    current = startCell
    while unvisitedCells exist:
        neighbors = getUnvisitedNeighbors(current)
        if neighbors not empty:
            next = random(neighbors)
            removeWall(current, next)
            stack.push(current)
            current = next
        else:
            current = stack.pop()
```

#### 2. Minotaur Placement Validation
- Places Minotaur randomly
- Runs DFS to verify exit is still reachable
- Blocks cells adjacent to Minotaur from pathfinding
- Repeats until valid placement found


### Design Patterns Used
- **Strategy Pattern**: Power-up system with polymorphic behavior
- **Template Method**: Game loop structure
- **Factory Pattern**: Random power-up generation
- **Singleton-like**: Static game logic runner

### Memory Management
- Dynamic 2D array allocation for maze matrix
- Proper cleanup in destructors
- Smart power-up lifecycle management
- No memory leaks (verified with Visual Studio diagnostics)

## 📊 Project Structure
```
.
├── MazeGame/
│   ├── Cell.cpp/h
│   ├── GameLogic.cpp/h
│   ├── Main.cpp
│   ├── MazeMatrix.cpp/h
│   ├── PowerUp.h
│   └── PowerUps.h
├── MazeLayout/
│   ├── MazeLayout.cpp/h
│   ├── pch.cpp/h
│   └── framework.h
└── README.md
```

## 🧪 Testing

The game has been tested with:
- ✅ Maze sizes from 15x15 to 100x100
- ✅ Various power-up quantities (0-50)
- ✅ Edge cases (even/odd dimensions)
- ✅ Memory leak detection
- ✅ Long gameplay sessions (1000+ moves)

## 🎓 Academic Context

**Course**: Object-Oriented Programming 2  
**Institution**: Faculty of Technical Sciences, University of Novi Sad  
**Academic Year**: 2024/2025  

**Key Concepts Demonstrated**:
- Object-Oriented Design (Inheritance, Polymorphism, Encapsulation)
- Dynamic Memory Management
- Algorithm Design (DFS, Pathfinding)
- Real-time Console Manipulation
- Game State Management

---
## 📝 License
This project is licensed under the MIT License.

## 👤 Author
**Miloš Jovanović**  
🎓 Faculty of Technical Sciences, University of Novi Sad  
🔗 [GitHub](https://github.com/0vertake), [LinkedIn](https://www.linkedin.com/in/milos-jovanovic-se/)

**⭐ Star this repository if you found it interesting!**

Made with ❤️ for OOP2 course

</div>

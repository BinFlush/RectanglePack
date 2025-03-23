# Rectangle Packing Simulation

This project is a simulation of the packing algorithms described in the paper "Shelf algorithms for two-dimensional packing problems" by Brenda S. Baker and Jerald S. Schwartz, published in the SIAM Journal on Computing, 1983. The application allows users to visualize different packing algorithms and interactively add rectangles to the packing simulation.

## Features

- **Interactive Drawing**: Users can draw rectangles in a designated window using the mouse.
- **Packing Algorithms**: Implements NFDH, FFDH, NFSr, and FFSr.
- **Real-time Visualization**: Visualizes the packing process in real-time as rectangles are added. Rectangles can be undone using the backspace key.

## Setup Instructions

### Prerequisites

- Ensure you have SFML installed. On Ubuntu or Debian-based systems, you can install it using:
  ```bash
  sudo apt-get install libsfml-dev
  ```

### Clone the Repository

```bash
git clone https://github.com/yourusername/rectangle-packing.git
cd rectangle-packing
```

### Build the Project

To build the project, use the following command:

```bash
g++ -o RectanglePack main.cpp FFDH.cpp FFS.cpp NFDH.cpp NFS.cpp -lsfml-graphics -lsfml-window -lsfml-system
```

### Run the Application

After building the project, you can run the application using:

```bash
./RectanglePack 0.5
```
Replace `0.5` with the desired packing ratio r (0<r<1).

## Usage

1. **Drawing Rectangles**:
   - Use the left mouse button to draw rectangles in the "DrawWindow" window.
   - The dimensions of the drawn rectangles are used to simulate the packing algorithms.

2. **Undo Rectangles**:
   - Press the backspace key to undo the last drawn rectangle.


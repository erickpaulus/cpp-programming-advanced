# Snake Game (SFML + C++)

## Requirements
- A C++ compiler (e.g., `g++`, MSVC, etc.)
- SFML installed  
  🔗 Download SFML(https://www.sfml-dev.org/download.php)
- SFML and Visual Studio instalation (https://www.sfml-dev.org/tutorials/3.0/getting-started/visual-studio/#installing-sfml)
## Features
- Real-time graphics with smooth snake movement
- Food collection, scoring system, and snake growth
- Wall collision and self-collision detection
- ⬆️⬇️⬅️➡️ Arrow key controls

## Setup Instructions

### If you're using `g++` on Windows:

```sh
g++ -std=c++11 snake.cpp -o snake -lsfml-graphics -lsfml-window -lsfml-system
```

Make sure SFML is installed and linked properly. If needed, I can help you with a setup guide for your platform or IDE (Visual Studio, Code::Blocks, etc.).

Next to do:
- Add sound using sf::Sound
- Add a score display
- Add levels and increasing speed
- Add walls or obstacles
- Multiplayer with separate keys

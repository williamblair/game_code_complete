# [Game Coding Complete, Fourth Edition](https://www.mcshaffry.com/GameCode/)
## SDL/OpenGL Implementation

## Linux Build

Dependencies (Ubuntu/Debian):

```bash
sudo apt-get install gcc g++ libglew-dev libsdl2-dev libbullet-dev libassimp-dev
```

Build:

```bash
cd <dir>
make -j4
```

## Windows Build

Requires Visual Studio/MSVC. On my system I have the dependencies installed in a
folder specified in `MakeWindows.bat` In each directory. Once GLEW, SDL2, Bullet,
and Assimp are installed, open the Visual Studio developer command prompt
and run `MakeWindows.bat` in the given directory.



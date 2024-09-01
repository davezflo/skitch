# Skitch
Skitch is an experimental interpreted programming language that allows you to script simple, interactive 3d objects and environments. It was written in C++ and utilizes [Simple Direct Media Layer - SDL2](https://www.libsdl.org/). 

Skitch is a work-in-progress, and it's gotten to the point that I want to make it available to others and invite contribution. 

## Building Skitch

git clone locally to skitch (i.e. ~/skitch)

```bash
git clone https://github.com/davezflo/skitch.git
```

### Linux
Get the latest version of SDL2 and install it by following the instructions in INSTALL.txt.

```bash
cd ~/skitch
make
cd skitches
../bin/skitch test.txt
```
By running this, you should see a spinning, multi-colored pyramid appear in a new window. We run from the skitches directory so that dependent skitches can be found.

### Mac
Get the latest version of SDL2 and install it by following the instructions in INSTALL.txt. Although there is a .dmg available, I recommend using the .tar.gz (just tar -xzf -C ~/sdl2/). Then follow the INSTALL.txt instructions followed by:

```bash
cd ~/skitch
make
cd skitches
../bin/skitch test.txt
```
By running this, you should see a spinning, multi-colored pyramid appear in a new window. We run from the skitches directory so that dependent skitches can be found.

### Windows
I will be adding more information on how to get this working in Windows later. I'll need to move to CMake for one thing. Generally, though, it should be straight-forward:

1. Create a CMakeLists.txt from the makefile (chatGPT does this nicely).
2. Install CMake
3. Install VS 2022 or better
4. CMake SDL
5. CMake skitch
6. Ensure the sdl2.dll is in the same directory as the skitch.exe and then run the test.txt (this must always be run from the skitches directory so that dependent skitch files can be found).

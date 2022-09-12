# Advanced Computer Graphics

This program has the following dependencies:

 1. OpenGL
 2. GLEW
 3. GLFW
 4. FreeImage
 5. Assimp

## Dependencies
OpenGL should already exist on your platform.
Assimp source code is included in this repository and will be built the first time you build this project.

**on Mac:**

You can install the remaining three dependencies using homebrew:
```
brew install glew
brew install glfw3
brew install freeimage
```

**on Windows:**

You don't need to install any external library since all dependencies are included in this repository.

**on Linux:**

You can install the remaining three dependencies using apt-get:
```
sudo apt-get install libglew-dev
sudo apt-get install libglfw3-dev
sudo apt-get install libfreeimage3 libfreeimage-dev
```

## Build
After installing the dependencies you should be able to use cmake to build the project. Make sure that the build folder is inside the project folder:

```
[project root]
	|-- build
	|-- CMake
	|-- doc
	|-- external
	|-- res
	|-- shader
	|-- src
	
```
## Run
When running the project, the output should look like [root]/doc/starting_scene.png (the Menu might be collapsed)
![Starting Scene](./doc/starting_scene.png)

## Note
Contact us if you have any problem building the project.
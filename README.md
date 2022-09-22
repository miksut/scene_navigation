# Computer Graphics: Scene Navigation

This repository hosts an implementation of two cameras: ArcBall and Fly-over. The project has been completed as part of a lecture offered by the [Visualization and MultiMedia Lab](https://www.ifi.uzh.ch/en/vmml/teaching.html) in the Department of Informatics at the University of Zurich, Switzerland.

---
## Project Description
In order to draw a 3D scene consisting of 3D objects, several transformations are needed. These transformations change the coordinate system of the vertices. Starting in the model coordinate system, a vertex gets then gradually transformed to the world coordinate system, the camera coordinate system, and finally to the projection coordinate system. The camera specification, that consists of multiple parameters, is responsible for the last two transformations. In an application, the user should be able to manipulate some of these parameters such as the position and the orientation of the camera. The idea is to design different mappings between keyboard and mouse inputs to camera movements. Such a mapping affects how conveniently and quickly a user reaches the position and orientation they want. 

In this project, two well-known cameras and their associated mappings are implemented: ArcBall and Fly-over. The implementation takes place in the `Camera` class. The two functions `mouseDrag` and `mouseScroll` rececive the inputs and update the camera parameters as well as the relevant matrices. The application builds upon a framework provided by the Visualization and MultiMedia Lab.

---

## Installation and Setup
This program has the following dependencies:

 1. OpenGL
 2. GLEW
 3. GLFW
 4. FreeImage
 5. Assimp

### Dependencies
OpenGL should already exist on the platform. The Assimp source code is included in this repository and will be built the first time when building this project.

**on Mac:**

Install the remaining three dependencies using homebrew:
```
brew install glew
brew install glfw3
brew install freeimage
```

**on Windows:**
All dependencies are included in this repository. When running CMake, choose "Visual Studio xx WIN64" or choose "Visual Studio xx" as the target platform. In the second case, make sure to definitely choose x64 in the next field.

**on Linux:**

Install the remaining three dependencies using apt-get:
```
sudo apt-get install libglew-dev
sudo apt-get install libglfw3-dev
sudo apt-get install libfreeimage3 libfreeimage-dev
```

### Build
After installing the dependencies, use CMake to build the project. Make sure that the build folder is inside the project folder:

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
---

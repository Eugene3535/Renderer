# Renderer
Development of a simple cross-platform renderer for visualizing 2D and 3D primitives, shapes, tile maps, meshes, textured models. 
OpenGL 4.6 core profile is used. The project is created for educational purposes only

Project build: requires CMake version at least 3.16, additional dependencies: GLFW, GLM, glad
Dependencies should be placed in the External folder, in environment variables write the path to the directory, for example: 
For windows - D:/External as External;
For linux - $HOME/External as External;

**Open git-bash and type:**  
```console
git clone https://github.com/Eugene3535/Renderer.git 
mkdir build && cmake -S Renderer -B build
cmake --build build --config Release
./Renderer
```

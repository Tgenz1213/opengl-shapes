# cs330
OpenGL project to create complex shapes with primitive shapes

# About
This is my final project for CS330. It features an API that creates five different primitive shapes. The Source files show how to implement the API and one way of creating different complex shapes.

# Patterns Used
The MVC pattern is used to implement the Shape, View, and Controller classes. Shape is the base class for the primitive shapes that can be created. The View class is responsible for the GLFW window, displaying the shapes on the screen, and creating the shader programs. Controller is reponsible for handling user input. 

The Factory pattern is used to implement the Shape and ShapeFactory classes. The ShapeFactory class is used to initialize Shape objects in a consistent manner.

# How To Use This Code
To see the final product I made, navigate to "cs330\x64\Debug" and run "5-1.exe."

To build the project and modify the files, open the solution in Visual Studios. The necessary libraries and dependencies are already included in the project. You may have to install GLAD for your particular machine by checking your OpenGL version and then going to https://glad.dav1d.de/ to get the correct files and overwriting the directory in "cs330\includes\GLAD"

# Known issues
Cylinder's side vertex coordinate indices are not created correctly, which results in odd looking sides. However, the shape is there and it reflects light like the other shapes.

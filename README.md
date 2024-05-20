# OpenGL Shapes
OpenGL project to create complex shapes with primitive shapes

# About
This is my final project for CS330. It features an API that creates five different primitive shapes. The Source files show how to implement the API and one way of creating different complex shapes.

# Patterns Used
The MVC pattern is used to implement the Shape, View, and Controller classes. Shape is the base class for the primitive shapes that can be created. The View class is responsible for the GLFW window, displaying the shapes on the screen, and creating the shader programs. Controller is reponsible for handling user input. 

The Factory pattern is used to implement the Shape and ShapeFactory classes. The ShapeFactory class is used to initialize Shape objects in a consistent manner.

# How To Use This Code
To see the final product I made, navigate to ".\x64\Debug" and run "5-1.exe."

To build the project and modify the files, open the solution in Visual Studios. The necessary libraries and dependencies are already included in the project. You may have to install GLAD for your particular machine by checking your OpenGL version and then going to https://glad.dav1d.de/ to get the correct files and overwriting the directory in "cs330\includes\GLAD"

# Known issues
Cylinder's side vertex coordinate indices are not created correctly, which results in odd looking sides. However, the shape is there and it reflects light like the other shapes.

# School Project Prompt

## What new design skills has your work on the project helped you to craft?
I learned about new design patterns during this project. I also learned about the graphics pipeline and some linear algebra and calculus. The most important skill I've learned from this is the ability to recognize when to use design patterns and how to successfully use them.

## What design process did you follow for your project work?
The previously mentioned design patterns I learned are the MVC and factory patterns. These helped me organize and reuse my code to make additions later on. It also helped me understand how the different OpenGL libraries and shaders work. 

## How could tactics from your design approach be applied in future work?
I will start by approaching all my future work to see what patterns I can recognize to make my code reuseable and easy to read. The example code we were given at first was not very easy to follow, so now I understand the importance of making the code modular and simple. 

## What new development strategies did you use while working on your 3D scene?
I learned the importance of a good linter and formatter. This work was done in Visual Studio, so I used SonarLint and CodeMaid to expedite cleaning up my code.

## How did iteration factor into your development?
Iterative changes were incredibly important to my project. Before I adopted the MVC pattern, my code mainly produced a black screen. It was incredibly frustrating. After I got my code working, I would make smaller changes at a time to make sure everything still worked.

## How has your approach to developing code evolved throughout the milestones, which led you to the project’s completion?
I could keep my code all in one file like our example code. It started out that way, and quickly became messy and unorganized. I had to spend a lot of extra time figuring out how to neatly organize my code with the different design patterns in ways that made sense. Thanks to the new patterns I learned, the last two weeks were mainly about me focusing on adding the new shaders and primitive shapes, rather than fighting the black screen over and over. Adding the complex shape structs was extremely easy because the primitives were already set up.

## How do computational graphics and visualizations give you new knowledge and skills that can be applied in your future educational pathway?
I am not sure exactly what my next classes will require of me. My next class after this one is linear algebra, so learning about vectors, matrices, transformations, normals, and partial derives in this class will have primed me somewhat for that.

## How do computational graphics and visualizations give you new knowledge and skills that can be applied in your future professional pathway?
The design patterns and using a low-level API will demonstrate my ability to future employers. Right now, I am not completely set on what area of software engineering I want to get into. If it winds up having anything to do with graphics rendering, I'll have the basic skills to get started.

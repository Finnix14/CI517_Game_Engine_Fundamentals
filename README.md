# SDL2 Game Engine

This project is a **custom 2D game engine built using SDL2**, developed as part of the **CI517 – Game Engine Fundamentals** module at the University of Brighton.

Unlike the companion framework project built on XCube2D, this engine was implemented **directly on top of SDL2**, requiring the core engine systems to be designed and programmed from scratch.

The project demonstrates how a basic 2D game engine can be constructed using low level libraries and object oriented C++ design.

---

## Technologies

- C++
- SDL2
- Object Oriented Programming
- Custom Game Loop Architecture

---

## Engine Features

### Core Engine Loop

Implements the traditional game engine structure:

- initialization
- update loop
- rendering pipeline
- shutdown handling

The engine processes input, updates game objects, and renders each frame.

---

### Rendering System

Handles drawing textures and sprites to the screen using SDL.

Responsibilities include:

- window creation
- renderer initialization
- sprite rendering
- frame updates

---

### Input System

Processes player input from keyboard and other devices.

Responsibilities include:

- capturing SDL input events
- translating input into gameplay actions

---

### Game Object Management

Implements a structured approach to managing objects within the game world.

Responsibilities include:

- object creation
- update logic
- rendering control

---

### Audio System

Supports playback of sound effects and music using SDL audio libraries.

---

### Resource Management

Handles loading and managing assets such as:

- textures
- audio files
- game resources

---

## Learning Objectives

This project was created to develop understanding of:

- low level game engine architecture
- SDL rendering pipelines
- input handling and event systems
- structuring engine systems in C++
- implementing a real time game loop

---

## Academic Context

Module: **CI517 – Game Engine Fundamentals**  
University: **University of Brighton**

The focus of the assignment was to design and implement core engine systems rather than build a complete game.

---

## Author

**Finley Nye**  
Game Development Student – University of Brighton  

GitHub  
https://github.com/Finnix14

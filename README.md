# CS246 A5 Project

## About The Project
A simple GUI Chess game for CS246 A5 project

# Chess Game

## Overview

This project is a chess game developed in C++ as the final assignment (A5) for the CS246 course. The game allows two players and/or chess AI to compete against each other with all the standard rules and mechanics of chess implemented.

## Authors

- [Andrew Gera](https://github.com/Angeraa "Andrew Gera")
- [Grey Gimple](https://github.com/greygimple "Grey Gimple")
- [Youssef Guindi](https://github.com/YoussefWindy "Youssef Windy")

## Features

- Play chess
- Play more chess
- Cry when beaten by our chess AI

## Getting Started

### Prerequisites

To compile and run the game, you will need:

- [g++](https://gcc.gnu.org/ "Woah")
- [make](https://www.make.com/en "Make")

### Building the Project

Clone the repository:
```sh
git clone https://github.com/YoussefWindy/chess-1245.git
cd chess-1245
make
./bin/chess
```

# Development Notes
**Anything That Needs To Be Noted/Remembered Goes Here**

## To Do (in order)
- Finish making the AI
- Stress test EVERYTHING (this thing is gunna need so much therapy after im done stressing it out)
- DD2: uml-final.pdf
- DD2: design.pdf

## Working On

### Andrew
- XWindow

### Grey
- build stress test suite
    - small suites for specific features
    - larger suites for stress testing

### Youssef
- main.cc user interface in text-based display and input from cin
- AI class

## Completed Tasks
- Makefile now does stuff as some [files](https://www.youtube.com/watch?v=dQw4w9WgXcQ "files ;)") have been added, yippie!
- Utilities
- All the pieces! 🥳
- Plan of attack (DD1)
- UML Diagram (DD1)
- Checks and checkmate
- replaying game

## Rules
- Anytime we want to work on files, we first checkout to our respective branch
- Once we're done and want to commit, push to repo *while still in that branch*
- To do this, e.g. Andrew will do `git push origin Andrew` while in the Andrew branch
- After pushing to repo, we can merge the branch with the main branch
- To update the new files on our end, we pull and then merge main branch with our respective working branch

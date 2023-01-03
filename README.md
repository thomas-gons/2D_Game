[![OS](https://img.shields.io/badge/os-linux-blue.svg)](https://shields.io/)

# 2D_Game

Project in C using the Ncurses library to make a top-down 2D game (OS Linux only).

<br>

# Table of Contents
 
- [Game rules](#game-rules)
- [Requirements](#requirements)
- [Installation](#installation)
- [Compilation](#compilation)
- [Execution](#execution)
- [Features](#features)
- [Authors](#authors)

<br>

## GAME RULES

The player 🎗 has to come back to his house ⚑. On his trip, he will have to pick up stamina bonuses ✦ in order to have enough stamina to reach his house.

Avoid the obstacles X or you will suffer by losing your stamina.

Also an enemy ☠ is chasing the player. Destroy his house ⛫ to be safe.

Move using  Z Q S D or the 4 Arrow keys.

Press SPACE to use a bonus you have picked up.

Press R to rewind your move.

<br>

## REQUIREMENTS

Make sure you **have installed** `gcc`(>= 12.2.0) and `make` on Linux:
* For Ubuntu Linux:
```
    sudo apt install build-essential
```
* For Manjaro Linux:
```
    sudo pacman -S base-devel
```
**Install** the ncurses library:
* For Ubuntu Linux: 
```sh
    sudo apt install libncurses6 libncursesw6
```
* For Manjaro Linux:
```sh
    sudo pacman -S ncurses
```
<br>

## INSTALLATION

**Clone** this repository:
```sh
    git clone https://github.com/ThomasGons/2D_Game.git
```
**Move** to the cloned directory :
```sh
    cd 2D_Game
```
<br>

## COMPILATION

In the cloned directory, **run**:
```sh
    make
```
<br>

## EXECUTION

In the cloned directory, **run**:
```sh
    make run
```
<br>

## FEATURES

**Clean** the project directory :
```sh
    make clean
```
**Display** info about files in project directory:
```sh
    make info
```
**Display** help message:
```sh
    make help
```
<br>

## AUTHORS

CAUSSE Raphael, COTOT Lucas, GONS Thomas, MUNOZ Melvyn, PEREIRA Matheo.

Developped on Ubuntu Linux and Manjaro Linux, Visual Studio Code, 11/2022.

# SDL2-CPP-test
Created and implemented by Eduardo FIlipe Figueiredo Bagarrão, including the assets for a job application.

This project was implemented with CMake in order to have portability to build executables either for Linux or Windows.

The application has 2 builds. 1 for Windows 64 bits and the other one for Unix 64 bits(it has to be run on terminal). The Unity was developed and made in Ubuntu.

## About the game

The projects consists in managing a resource pool of balls that are moving either vertically or horizontally at different speeds. The pool as an initial size that can be expanded until a certain limit in some cases, but when the resources are not needed any more and are returned to the pool, if the pool is currently above it's default size, the size will be restored.

You start the game with 3 credits to play, and every time you play the game you lose a credit.

If you don't have any credits, you need to insert a new one to play or a message saying that you need to insert a credit will appear for some time.

You can not only insert a credit but you can also remove all the credits that you have.

You can see on the top left corner of the game your current credits, if you're playing or not, and if you're playing you'll also see how many active GameObjects are being rendered in the screen. 

## Instructions(Keys)

#### Credits

- **1** - Insert credit.
- **2** - Remove all credits.

#### Game State

- **z** - Start game.
- **x** - End Game.

#### Gameplay

- **up arrow** - Add a GameObject to the screen.
- **down arrow** - Remove a GameObject from the screen.

## My experience

Doing this project for the application increased my motivation even more! Working with C++ feels exciting and challenging!

## What could be improved

I feel I have a lot to improve, I would like to implement this but in a multi-threaded paradigm(Java is my background). Hope I can have oportunity to do it soon.

I also would like to add a simple music loop that I've made in FL Studio, but I didn't also had time to implement it on game. You can watch it on the resources folder of the project!

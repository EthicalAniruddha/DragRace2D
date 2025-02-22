# Drag-Race - EthicalAniruddha

This is a simple 2D drag racing game built using the Raylib library.

## Screenshot

![Screenshot of Drag-Race](https://github.com/EthicalAniruddha/DragRace2D/blob/main/race.png)

## Features

* **Car Physics:** Car goes broom.
* **Camera Following:** Camera which stalks the car.
* **Drifting:** Help me to add it.
* **Basic Controls:** Go speed!, go slow!.
* **Tile-based Background:** Repeatable soil tile background which is stolen.
* **FPS, Speed, Rotation, and Slip Angle Display:** Screen debug information.

## Dependencies

* Raylib: A simple and easy-to-use library for game development.

## Building and Running

1.  **Install Raylib:** Follow the installation instructions for your operating system from the official Raylib website (https://www.raylib.com/).
2.  **Compile the Code and run:**
    * Using GCC:
        ```bash
        bash build.bash
        ```
3.  **Assets:** Ensure that the `craftpix-889156-free-racing-game-kit/PNG/Car_1_Main_Positions/Car_1_01.png` and `craftpix-889156-free-racing-game-kit/PNG/Background_Tiles/Soil_Tile.png` files are in the same directory as the executable, or adjust the file paths within the code.

## Controls

* **W/Up Arrow:** Go broom
* **S/Down Arrow:** Go broom opposite
* **A/Left Arrow:** Turns Left
* **D/Right Arrow:** Turns Right

## Code Structure

* `main.cpp`: Contains the main game loop and logic.
* **Defines:** Constants for screen dimensions, car properties, and other game parameters.
* **Car Struct:** Represents the car with its position, speed, rotation, and texture.
* **Camera2D:** Raylib camera for smooth car following.
* **Texture Loading:** Loads car and soil textures from image files.
* **Game Loop:** Handles input, physics, rendering, and camera movement.

## Assets

* The car and soil textures are from the "Free Racing Game Kit" by Craftpix. Stolen.

## Future Improvements

* Add collision detection.
* Add Multiplayer.
* Improve the graphics and sound.
* Add better drift physics.
* Add menu.

## License

[The Unlicense](https://choosealicense.com/licenses/unlicense/)

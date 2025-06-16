# Display PBM library in C

## About the Project
This project is an C-Interface for [PBM P4](https://en.wikipedia.org/wiki/Netpbm#File_formats) pictures.

With the pbmIO directory, the image can be loaded and saved from the filesystem.
With the SDL2 library, it can be displayed in a window.

the pbmGraphic library can be used to write graphic elements to an image or write text.
The library is variable how to handle the image.
It gives 4 methods how to store the binary image: 
- PBM_DATA_HORIZONTAL_MSB (Used for PBM P4 format)
- PBM_DATA_HORIZONTAL_LSB
- PBM_DATA_VERTICAL_MSB
- PBM_DATA_VERTICAL_LSB

The graphic library can handle all these variants.
From this reason, it can also be used for LCD displays as image creator for lightweight embedded systems.

The fonts for the graphic library are based on bitmap for all characters.

### Build with
- C Standard libraries
- pbmIO display based on [SDL2](https://www.libsdl.org/)

## Getting Started
The library has the base header file [lib/pbm.h](lib/pbm.h).
This file includes all important structures and defines.

The graphic library header file ``pbmGraphics.h`` is located at [pbmGraphic/pbmGraphics.h](lib/pbmGraphic/pbmGraphics.h).
There are all prototypes for writing and drawing graphics to the image.
The font handler structure is in the same directory.

### Installation
- Graphics:
Add the lib directory to your project.
You can clone the project with:
```
git clone https://github.com/steinerAdi/pbmLib.git
```
Or add it as an submodule to your project with:
```
git submodule add https://github.com/steinerAdi/pbmLib.git
```

- IO:
    - Install SDL2 

    ```
    sudo apt install libsdl2-dev
    ```

### Example
An example can be found in the directory [tests/linux](tests/linux/).
The example can be compiled and started with:
```
make run
```
A window will be displayed and can be controlled with the ``RIGHT`` and ``UP KEY``.
With the right key, the windows switches between three images:
1. Fonts examples:

    Display the several test fonts (e.g [6x8_horizontal_LSB_1](tests/linux/6x8_horizontal_LSB_1.h))
    With the ``UP KEY``, you can switch through all loaded fonts

1. Graphic demonstration

    The next image displays the possibility how to write text at several positions, drawing circles and lines.
    As before, with the ``UP KEY``, the size or the alignment will change as demonstration.

1. Loaded image

    The example image [sample.pbm](tests/linux/sample.pbm) will be loaded from the filesystem and display in the window.
    With the ``UP KEY`` the color can be inverted.

When closing the window, the current displayed view is stored in the created build directory with the name ``saved.pbm``.

## Usage
PBM P4 interaction library to load, change and save the image. 

## License
Distributed under the GPLV3 license.
See [LICENSE](LICENSE) for more information.

## Author

Adrian STEINER (adi.steiner@hotmail.ch)

## Acknowledgments

- Used Algorithm from [Bresenham](https://de.wikipedia.org/wiki/Bresenham-Algorithmus) for line and circle creation


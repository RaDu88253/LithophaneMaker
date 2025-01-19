
# LithophaneMaker: A 3D Printing project

This is a simple project I wanted to make for myself, after seeing the browser based lithophane generator on 3dp.rocks. I initially tried to make this project using OpenSCAD, but I ran into the limitations of the STL engine that OpenSCAD uses.


## Run Locally

Clone the project

```bash
  git clone https://github.com/RaDu88253/LithophaneMaker.git
```
The project does not have a CLI, so it must be run manually using CMake. You will also need to change the input and output file path and other parameters by hand, inside main.c. The input file must be a .bmp file with a BITMAPV5HEADER header. For this purpose, you can use https://cloudconvert.com/ to generate such .bmp files from other image formats. In order for the program to run properly, and not generate too big of a stl file, you should adjust the block_size variable inside main. Higher values decrease resolution, thus decreasing processing time and number of faces in the STL.

## License
This project is published under an [MIT](https://choosealicense.com/licenses/mit/) license.


## Features

- Generates a 3D Printable STL file from a .bmp file
- Gives the option of customizing the size, depth, thickness and resolution of the STL file.
- Cross-platform


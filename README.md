# Gaussian-Blur-openGL

This program aims to appy gaussian blur to images. It is implemented on Ubuntu by using OpenGL version 3.3.

There are 3 different implementation of gaussian blur.
    * Naive implementation: 
        - Runs in O(n^2) time. 
    * Two-pass implementation:
        - Uses the property of gaussian function. Apply same effect both vertical and horizontal at different passes. Therefore, it runs in O(2*n).
    * Two-pass with bilinear filtering:
        - In addition to two-pass property, uses hardware-implemented bilinear filtering. It decreases the number of pixel fetches.

Usage:
    First call the following 2 commands to compile.
    * make clean
    * make
    Then, call the program.
    * ./blur <name_of_the_texture_image> <type_of_implementation>
    * To run naive implementation type 1 for <type_of_implementation>.
    * To run two-pass implementation type 2 for <type_of_implementation>.
    * To run two-pass with bilinear filtering implementation type 3 for <type_of_implementation>.
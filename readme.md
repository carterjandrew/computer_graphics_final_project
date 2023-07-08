# CSCI4229 Final Project: Ray Marching Basics
## Demonstrates procedural object and scene creation through ray marching
## Uses ray marching for lighting, normal calculation, shadow casting, binary object operands, fog, and texure mapping
### System Requirements:
1. Windows, macOS, x-11 or wayland based display sever
2. Compatability with GLUT or free GLUT
### Display modes:
1. Smooth intersection test
2. Repitition/shadow casting test
3. Object subtraction test
### View modes:
1. Lighting/texutre mode
2. Normal modes
### Build instructions:
#### Intended for a command line with gcc, glut, and optionally glew
```
make clean
make
```
### Key bindings:
  PageUp      Zoom out by one unit
  PageDown    Zoom in by one unit
  m           Cycle through display modes
  n           Toggle between view modes
  p           Play or pause animation in scene
  f           Fun stuff, get rid of max render distance to see how the shader handles mapping textures and normals to points outside the bounds of the draw range

### Sources:
1. https://mercury.sexy/hg_sdf/
2. https://github.com/SebLague/Ray-Marching
3. https://www.youtube.com/watch?v=Cp5WWtMoeKg
4. https://timcoster.com/2020/02/13/raymarching-shader-pt3-smooth-blending-operators/


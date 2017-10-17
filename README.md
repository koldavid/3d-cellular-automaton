# 3d-cellular-automaton

![3D Cellular Automaton](https://github.com/koldavid/3d-cellular-automaton/blob/master/3DCellularAutomaton/3d_cellular_automaton.png)

A simple-to-use application to explore various three dimensional cellular automaton rules. OpenGL and CUDA accelerated.

You can create different rulesets just by defining the rule for survival, birth and number of states for each cell in the settings.ini file. 
For example:

**Rule "GliderHell"**
 * Survival = 4 8
 * Birth = 4
 * Number Of States = 5
 
The starting state can be set as well in the settings.ini file. Currently it only supports creating a centered, 
random-generated cube as the starting state, but you can set the size and the density of this initial cube.

## Controls
- mouse drag - rotate cube
- mouse wheel - zoom in/out
- Space - start/stop generation
- C - change colors*
- number keys(0..9) - change rule*
- keypad +/- - speed up/slow down generation

*can be set in the settings.ini file

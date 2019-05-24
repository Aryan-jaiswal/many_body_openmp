# N-body Simulation OpenMp

In physics and astronomy, an N-body simulation is a simulation of a dynamical system of particles, usually under the influence of physical forces, such as gravity. N-body simulations are widely used tools in astrophysics, from investigating the dynamics of few-body systems like the Earth-Moon-Sun system to understanding the evolution of the large-scale structure of the universe.
Here, we have achieved the same assuming n bodies are trapped inside a cuboid of given dimensions using [OpenMp](https://www.openmp.org/).

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

What things you need to install the software and how to install them

```
OpenMp
Python >= 2.7
Mayavi
Pygame
```

### Installing

A step by step series of examples that tell you how to get a development env running.
In the terminal execute the following commands.

#### For installing OpenMp

```

$sudo apt-get install libomp-dev

```
#### For installing mayavi
```

$sudo easy_install pip<br/>
$sudo pip install mayavi

```
#### For installing pygame
```

$sudo apt-get install python-pygame

```
Pygame is used for 2D visualisation whereas mayavi is used for 3D visualisation.

## Running the simulation

In order the simulate the environment, one needs to do the following:

### Generating Trajectory file

This generates trajectory file for the specified number of threads which will be visualised.

```

$g++ -fopenmp many_body.cpp -o generate <br/>
$./generate number_of_threads

```
Specify the number of threads you want the code to run on in place of "number_of_threads".


## Visualisation
 
 Run the following commands for visualisation after trajectory file is generated.
 
### For 2D Visualisation

```
$python2 visualiser.py 1
```

### For 3D Visualisation

```
$python2 visualiser.py 2
```


## Contributing

Please feel free to contribute by sending in PR and opening and handling issues.  

## Authors

See the list of [contributors](https://github.com/Aryan-jaiswal/many_body_openmp/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* This project was made as a part of the course ``High Performance Parallel Programming``.



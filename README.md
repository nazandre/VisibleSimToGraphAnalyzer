# VisibleSimToGraphAnalyzer

Convert a VisibleSim configuration file into a file for the GraphAnalyzer tool.

### Dependencies

VisibleSimToGraphAnalyzer requires TinyXML library.

In order to install TinyXML on Ubuntu:

```
$ sudo apt-get install libtinyxml-dev
```
### Compilation

To compile VisibleSimToGraphAnalyzer, go into the `build/` folder and enter `make`.

```
$ cd build/
$ make
```

### Usage

VisibleSimToGraphAnalyzer works as following:

```
Usage: VisibleSimToGraphAnalyzer -i <input xml image> -t <target lattice> [options]
Supported target lattices (<target lattice>): 
  s: Square
  h: Hexagonal
  sc: Simple-Cubic
  fcc: Face-Centered Cubic
Options:
-o <output aj file> : output file for GraphAnalyzer
-h                  : print this usage and exit
```

### Example

Below is a full example of how to compile and use VisibleSimToGraphAnalyzer. In this example, `configurations/example.xml` is converted into `build/example.aj` considering a Simple Cubic (SC) lattice. Blinky Blocks are arranged into a SC lattice.

```
$ cd build/
$ make
$./visibleSimToGraphAnalyzer -i ../configurations/example.xml -t sc
Input: ../configurations/example.xml
Output: example.aj
Target lattice: sc
Grid size: x=10,y=10,z=10)
../configurations/example.xml opened and read!
../configurations/example.xml exported in example.aj
```

Below is the expected content of `build/example.aj`:
```
14
1 2 2 12
2 3 5 4 3
3 3 10 9 6
4 1 9
5 1 10
6 1 7
7 0
8 1 9
9 0
10 1 11
11 0
12 1 13
13 1 14
14 0

```
# Rasterman

Rasterman is a small application to enlarge bitmaps by converting them into rasterized scalable vector graphics to be used as posters. The software is heavily inspired by [The Rasterbator](https://rasterbator.net/), a similar application created by Matias Ärje in 2004. Whereas Rasterbator's standalone version is implemented in C# and comes with a GUI Rasterman is programmed in C++ using the Qt 6.8.1 framework and is avaliable as a console application only. The reason behind the development of Rasterman is Qt. I wanted to learn the framework and was looking for something interesting to develop. I'm planning to give it a GUI too and to deploy it in Linux and in Windows. 

```
$ ./rasterman -h
Usage: ./rasterman [options] input
Converts a bitmap file into a rasterized, scalable vector graphic.

Options:
  -h, --help                 Displays help on commandline options.
  --help-all                 Displays help, including generic Qt options.
  -v, --version              Displays version information.
  -o, --output <output.svg>  Name of output file.
  -w, --width <768>          Output width in mm.
  -t, --height <1024>        Output height in mm.
  -m, --maxcirclesize <4>    Circle diameter in mm.
  -s, --scaling <1>          Scaling mode (0: Constant, 1: Linear, 2: Logarithmic, 3: Square root).
  -c, --colorcalc <average>  Color calculation method (average or median).
  -d, --dpi <300>            Dots per inch.
  -f, --coverage <1.0>       Corner coverage factor (0.0 to 2.0).
  -g, --grayscale            Convert colors to grayscale.
  -y, --gamma <0.5>          Adapt luminance.

Arguments:
  input                      Input file
```
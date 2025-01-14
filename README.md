# Rasterman

Rasterman is a small application to enlarge bitmaps by converting them into rasterized scalable vector graphics to be used as posters. The software is  inspired by [The Rasterbator](https://rasterbator.net/), an application created in 2004 by Matias Ärje. All kudos go to him. Whereas Rasterbator's standalone version is implemented in C# and comes with a small GUI, Rasterman is programmed in C++ and uses the Qt 6.8.1 framework. The reason why I developed of Rasterman is Qt. I wanted to learn the framework and was I looking for something interesting and easy to develop. Currently Renderman is available as console application only. But I'm planning to give it a GUI too and to run it in Linux and in Windows. The current version is Linux-only.

To compile it you'll need Qt 6.8.1 which can be downloaded at https://www.qt.io/download-qt-installer-oss.

The command line arguments and options are self-explaining:

```
$ ./rasterman -h
Usage: ./rasterman [options] input
Converts a bitmap file into a rasterized, scalable vector graphic.

Arguments:
  input                      Input file

Options:
  -h, --help                 Displays help on commandline options.
  --help-all                 Displays help, including generic Qt options.
  -v, --version              Displays version information.
  -o, --output <output.svg>  Name of output file
  -w, --width <768>          Output width in mm
  -t, --height <1024>        Output height in mm
  -m, --maxcirclesize <4>    Circle diameter in mm
  -s, --scaling <1>          Scaling mode (0: Constant, 1: Linear, 2: Logarithmic, 3: Square root)
  -c, --colorcalc <average>  Color calculation method (average or median)
  -d, --dpi <300>            Dots per inch
  -f, --coverage <1.0>       Corner coverage/overlapping factor (0.0 to 2.0)
  -g, --grayscale            Convert output to grayscale
  -b, --black                Use black circles only
  -y, --gamma <0.5>          Adapt brigthness

```
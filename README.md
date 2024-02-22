# Vizu
*Automatic generation of cool graphical video representations of audio using digital signal processing techniques.*

Developed using C++ and the SFML library.

## Example Run
Watch a demo!

[![Video](https://img.youtube.com/vi/6vh7CZ3OkDs/hqdefault.jpg)](https://www.youtube.com/watch?v=6vh7CZ3OkDs)
## How to Build
The project uses CMake as its build system.

For a single-configuration generator (typically the case on Linux and macOS):
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
For a multi-configuration generator (typically the case on Windows):
```
cmake -S . -B build
cmake --build build --config Release
```
The binary will be located in `build/bin/Release`. You can adjust the configuration file `config.json` to your liking and enjoy!


## Detailed Project Description
Vizu is a desktop program developed for educational purposes which, given an audio file, allows you to automatically generate a 2D animation which portrays a visual representation of the audio's onsets in a synchronized manner.
This is mainly done by implementing several digital signal processing techniques and algorithms completely from scratch.

The project is composed of two main parts - an audio onset detection algorithm, and a small 2D animation generator. The former, given primarly a desired animation FPS and an audio file, serves the purpose of detecting the frame indices which mark the very beginning of a single, arbitrary sound. The latter uses this output in order to pre-compute a 2D animation, which features a freely falling ball that collides with "strategically" placed rectangular platforms in these exact marked frames in a satisfying way. The animation is then being rendered to a window using the SFML library.

In the following section I will elaborate on each of these two main parts.

### Audio Onset Detection
Vizu, as stated above, is mainly powered by a self-implemented audio onset detection algorithm which incorporates several known digital signal processing techniques. The code dedicated to it can be found in `core/src/Sound.cpp`.

The algorithm is as follows:
- **Segment the given signal into separated, overlapping windows**. This essentially assigns each frame with its corresponding audio segment. The windowing process has three parameters: hop size, window length size, and window function.

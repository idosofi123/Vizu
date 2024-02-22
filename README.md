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
Vizu, as stated above, is mainly powered by a self-implemented audio onset detection algorithm which incorporates several known digital signal processing techniques. The code dedicated to it can be found at `core/src/Sound.cpp`.

The algorithm is as follows:
- **Segment the given signal into separated, overlapping windows.** This essentially assigns each frame with its corresponding audio segment. The windowing process has three parameters: hop size, window length size, and a window function. The hop size determines the distance in audio samples between consecutive windows. In order for the windows to align with the desired animation frames, it is calculated to be the numbers of audio samples which make up a single frame, based on the desired animation FPS and the given audio file's sampling rate. The window length chosen is 4 times the hop size, with which we end up with 75% overlap between windows. After some experimenting, it seemed to yield the best balance between the temporal resolution and frequency resolution trade-off. Finally, we apply the Hann function on each window to prevent from discontinuities at the edges of our window to "pollute" the frequency spectrum that will be generated later on.
- **Compute the frequency content of each window.** By applying the Fast Fourier Transform algorithm, we can obtain the frequency spectrum of each window. The frequency spectrum of each window essentially tells us which are the dominant frequencies in the current audio segment, by mapping a magnitude value to each frequency range. This allows us to account for frequency changes as well as amplitude changes in our audio signal as an indication of a potential onset.
- **Measure the spectral flux between consecutive windows.** Spectral flux is a measurment of how quickly the frequency content changes from one frame to another. There are plenty of calculation variations that serve different applications and needs. In Vizu, the spectral flux is currently measured as:
$$\sum_{i = 0}^{N - 1}\max (C[i] / P[i] - 1, 0)$$
Where C is the current window, P is the previous window, and N is the amount of frequency bins. This formula only considers frequency bins in which the magnitude has risen. Moreover, by relying on the division of magnitude values, rather than the absolute or squared difference (which is a common approach), we are focusing on relative growth which allows the detection process to be independent of the overall volume of the audio.
- **Apply a threshold.** Eventually, we set a threshold to the computed flux values in order to determine which of the windows feature an onset. Different threshold values may yield more satisfying results among different audio samples. A lower threshold value is suitable if you want to account for more subtle events, and a higher value will perform better when dealing with a simple, monophonic audio with clear and discrete sounds.

And that's about it! The algorithm can certainly be further optimized and improved, but it seems to provide pretty good results so far.

### 2D Animation Generation
Now that we have gathered the frame indicies in which the musical onsets occur, it is time to generate our animation that will complement the audio. Initially, we define our simulation constants: the ball's radius, the gravitational power, the horizontal speed, etc. For each onset frame, we extrapolate the ball's state within it based on its current state and the time passed since the last collision frame. We can then place a new platform in accordance to its current position and velocity. We also make sure the calculated platforms do not interfere with the ball's path in frames where they are not supposed to collide to prevent an unpleasing sight. The calculations as well as these small adjustments are all done using simple arithmetic progression formulas. After the simulation is constructed, it is ready to be rendered by SFML.

The code showcasing this process can be found at `core/src/SimulationMap.cpp`.

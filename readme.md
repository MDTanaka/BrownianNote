# Brownian Note

Model frequencies of a sound as brownian particles diffusing in frequency space.

Add a potential such that the steady-state distribution of is 1/f from 100Hz to the Nyquist frequency (Sampling rate / 2).

Without a note playing this generates pink noise. When a note is played a periodic harmonic potential moves the particles near freqencies corresponding to the harmonic series of the fundamental. When the note is let go, the particles diffuse back to pink noise.

Currently monophonic and will immediately generate loud pink noise as soon as it is loaded in.

> **_Are you ready for the next revolution in sound?_**
>
> **_Sliced bread ain't got nothing on this._**
>
> **_If your ears aren't ready I don't blame you._**
>
> **_Build and install at your own risk, you may never be the same._**

## How to build/install

Get the VST3 SDK at https://www.steinberg.net/developers/.

Set the environment variable `VST3SDK_SOURCE_DIR` to the path to the subfolder of the SDK named `vst3sdk`.

Standard Cmake build process:

```
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

(I get an error that one of the post-build tests fails, but it doesn't seem to affect anything).

Move `build\VST3\Release\BrownianNote.vst3` to wherever you put your VST3s (FL Studio on Windows wants them at `C:\Program Files\Common Files\VST3` and trying to scan any other folder won't recognize it).

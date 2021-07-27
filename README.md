# Ohm

Cheat for Counter-Strike: Global Offensive written in C++. Currently only usable under a Windows environment due to the use of PE headers to find interfaces and heavy use of the `Windows.h` library.

## Requirements

* Windows OS
* [Visual Studio 2019](https://visualstudio.microsoft.com/vs/)

## Building

1. Clone the repository to a folder of your choosing.

```bash
git clone https://github.com/ramadan8/Ohm.git && cd Ohm
```

2. Open `Ohm.sln` within Visual Studio.

3. Choose your build mode at the top and press `Ctrl-Shift-B` to build.

4. The resulting `Ohm.dll` file will be available within the respective build folder.

## Usage

From here, simply inject the DLL into the game process using your favourite injector and use as you wish.

By default `HOME` will unload the cheat and `INSERT` will open/close the menu.

## Notes

* Configuration files will be saved to your *My Documents* folder under the *Ohm* directory as base 64 encoded JSON.
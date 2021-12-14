# Tetris

<a id="index"></a>

This project is an exercise from this [Youtube tutorial][VideoTutorial] by [javidx9][YTChannel]. The following text is made up of notes from that tutorial and some personal notes.

## About this project

Tetrominos construction:

```
# Wide (w) = 4

    y/x  0    1    2    3           y/x  0    1    2    3
      ┌────┬────┬────┬────┐           ┌────┬────┬────┬────┐
    0 │  0 │  1 │  2 │  3 │         0 │ 12 │  8 │  4 │  0 │
      ├────┼────┼────┼────┤           ├────┼────┼────┼────┤
    1 │  4 │  5 │  6 │  7 │         1 │ 13 │  9 │  5 │  1 │
      ├────┼────┼────┼────┤           ├────┼────┼────┼────┤
    2 │  8 │  9 │ 10 │ 11 │         2 │ 14 │ 10 │  6 │  2 │
      ├────┼────┼────┼────┤           ├────┼────┼────┼────┤
    3 │ 12 │ 13 │ 14 │ 15 │         3 │ 15 │ 11 │  7 │  3 │
      └────┴────┴────┴────┘           └────┴────┴────┴────┘
      (Normal position )              (90º rotation)

    y/x  0    1    2    3           y/x  0    1    2    3
      ┌────┬────┬────┬────┐           ┌────┬────┬────┬────┐
    0 │ 15 │ 14 │ 13 │ 12 │         0 │  3 │  7 │ 11 │ 15 │
      ├────┼────┼────┼────┤           ├────┼────┼────┼────┤
    1 │ 11 │ 10 │  9 │  8 │         1 │  2 │  6 │ 10 │ 14 │
      ├────┼────┼────┼────┤           ├────┼────┼────┼────┤
    2 │  7 │  6 │  5 │  4 │         2 │  1 │  5 │  9 │ 13 │
      ├────┼────┼────┼────┤           ├────┼────┼────┼────┤
    3 │  3 │  2 │  1 │  0 │         3 │  0 │  4 │  8 │ 12 │
      └────┴────┴────┴────┘           └────┴────┴────┴────┘
      (180º rotation)                 (270º rotation)

# Ecuations to find each value:
#
# 0º:   i = (y * w) + x
# 90º:  i = 12 + y - (x * w)
# 180º: i = 15 - (y * w) - x
# 270º: i = 3 - y + (x * w)
```

Set console window size equal to 80 x 30.

## Concepts

Game loops are the most important part in the game engine, these are the things that sequence everything that's going on.
It will typically include some elements that will handle **timing**, **user input**, updating the **game logic** and then **drawing** it to the screen and it just keeps doing this until the game's initial the user exits.

## About C++

These are new knowledge for me in this project about C++ language.

```c++
// Recommended for Windows
wstring someWString;

// Place "L" before an literal for right conversion in VS
// Add literals to string or wstring
someWString.append(L"Something");

// Write in console in specific location and size
swprintf_s( /* Buffer location */, /* Size of buffer */, /* String */ );
```

```c++
#include <Windows.h>

// Close an open object handle
CloseHandle( hConsole );

// Run the pause program, press any key to continue
system( "pause" );
```

```c++
#include <thread>

// To access the current thread
// Blocks the execution of the current thread for at least the specified sleep_duration
this_thread::sleep_for( /* sleep_duration */ );
```

```c++
#include <vector>

// Return true is there no elements
someVector.empty();

// Set vector empty
someVector.clear();
```

[Arriba][Index]

<!-- TODO: Fix last piece before game over -->

[VideoTutorial]:    https://www.youtube.com/watch?v=8OK8_tHeCIA
[YTChannel]:        https://www.youtube.com/channel/UC-yuWVUplUJZvieEligKBkA
[Index]:            #index

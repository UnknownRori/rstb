# rstb

[stb](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt) style utility for my day to day projects that always makes me reimplement them over and over again.

## Features

|Library                        |Latest Version|Category|LoC|Description                         |
|-------------------------------|--------------|--------|---|------------------------------------|
|**[rstb_da.h](rstb_da.h)**     |1.1           |misc    |166|Simple to use dynamic array         |
|**[rstb_arena.h](rstb_da.h)**  |0.1           |misc    |337|Simple to use arena (bump) allocator|
|**[rstb_sv.h](rstb_sv.h)**     |0.1           |misc    |200|Simple string manipulation helper   |

## Usage

The idea of single-header file libraries is that to make easy to use and distribute since all the code contained in single file.

To use it you need to include header file and add macro that specific on that header (each file has it's own docs inside them).

```c
#define  RSTB_DA_IMPLEMENTATION
#include "rstb_da.h"
```

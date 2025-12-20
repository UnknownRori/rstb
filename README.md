# rstb

[stb](https://github.com/nothings/stb/blob/master/docs/stb_howto.txt) style utility for my day to day projects that always makes me reimplement them over and over again

## Features

|Library                    |Latest Version|Category|LoC|Description                |
|---------------------------|--------------|--------|---|---------------------------|
|**[rstb_da.h](rstb_da.h)** |1.1           |misc    |166|Simple to use dynamic array|

## Usage

The idea of single-header file libraries is that to make easy to use and distribute since all the code contained in single file

To use it you need to include header file and add macro that specific on that header (each file has it's own docs inside them).

```c
#define  RSTB_DA_IMPLEMENTATION
#include "rstb_da.h"
```

﻿#pragma once

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int63 = __int64;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

enum class MoveDir
{
    WA,
    WD,
    SA,
    SD,
    W,
    A,
    S,
    D
};

enum class ShootDir
{
    UpLeft,
    UpRight,
    DownLeft,
    DownRight,
    Up,
    Left,
    Down,
    Right
};
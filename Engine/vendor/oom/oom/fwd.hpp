#pragma once

namespace oom
{
    using length_t = int;

    template<length_t Size, typename T> struct vec;
    template<length_t Size, typename T> struct point;
    template<length_t Col, length_t Row, typename T> struct mat;

    #ifdef OOM_FORCE_EXPLICIT_CTOR
    #   define OOM_EXPLICIT explicit
    #else
    #   define OOM_EXPLICIT
    #endif
}
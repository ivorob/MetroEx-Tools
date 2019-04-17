#pragma once

#include "../MetroTypes.h"
#include <variant>

using ParamValue = std::variant<uint16_t, bool, int, uint32_t, CharString, float>;
enum ParamType {
    Type_u16, // "u16"
    Type_bool, // "bool"
    Type_bool8, // "bool8"
    Type_time, // "time" | "u32"
    Type_anim,
    Type_choose, // "choose" | "stringz"
    Type_color_u32, // "color, u32"
    Type_entity,
    Type_fp32, // "fp32"
    Type_sz, // "stringz"
    Type_part,
    Type_u8, // "u8"
    Type_u32, // "u32"
};
struct MetaProp {
    ParamType   type;
    const char* name;
};
struct MetaInfo {
    MyArray<MetaProp>    props;
    MyArray<const char*> input;
    MyArray<const char*> output;
};

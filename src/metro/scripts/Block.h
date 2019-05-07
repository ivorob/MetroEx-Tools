#pragma once

#include "metro/MetroTypes.h"
#include "BlockMeta.h"

class Config;

struct Block {
    using Params = MyArray<std::pair<const char*, ParamValue>>;

    Block(MetroClsid clsid, const char* name, const MetaInfo* meta);

    MetroClsid clsid;
    CharString name;
    uint16_t   posx;
    uint16_t   posy;
    Params     params;

    virtual void Read(Config& cfg);

    const MetaInfo* meta = nullptr;
    BytesArray      unknown;
};

#pragma once

#include "../MetroTypes.h"

class Config;

struct Block {
    uint32_t   clsid;
    CharString name;

    virtual void Read(Config& cfg);
};

struct UnknownBlock : Block {
    BytesArray unknown;

    virtual void Read(Config& cfg) override;
};

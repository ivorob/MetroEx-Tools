#pragma once

#include "../MetroTypes.h"
#include "Block.h"
#include <array>

class Config;
class MetroReflectionReader;

struct Script {
    uint16_t                         version;
    MyArray<Block>                   blocks;
    MyArray<std::array<uint16_t, 4>> links;

    void Read(Config& cfg);
    void Serialize(MetroReflectionReader& r);
};

struct ScriptRef {
    CharString     vs_name;
    bool           vs_debug;
    bool           vs_active;
    bool           disable_qsave;
    bool           save_on_nextlevel;
    CharString     vs_ref;
    bool           dyn_state_exists;
    MyArray<Block> exposed_blocks;

    void Read(Config& cfg);
    void Serialize(MetroReflectionReader& r);
};

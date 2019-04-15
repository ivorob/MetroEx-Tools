#pragma once

#include "MetroTypes.h"

struct uobject;
class Config;
class MetroConfigsDatabase;
class VFXReader;

struct PresetBlend {
    CharString name;
    bool       game_time;
    bool       removing;
    int32_t    start;
    int32_t    finish;

    void Read(Config& cfg);
};

struct Startup {
    uint32_t             game_time;
    float                dc_prev_speed;
    float                dc_target_speed;
    float                dc_curr_speed;
    int32_t              dc_time_0;
    int32_t              dc_time_1;
    CharString           main_preset_name;
    CharString           current;
    PresetBlend          preset;
    uint8_t              source_type;
    MyArray<PresetBlend> modifiers;
    uint64_t             _total_time;
    CharString           weather_preset;
    uint32_t             dc_start;
    uint32_t             dc_duration;
    CharString           foliage_set;
    float                foliage_fuzziness;
    vec3                 map_positional_min;
    vec3                 map_positional_max;
    bool                 map_positional_preview_aabb;
    CharString           map_menu_name;
    uint8_t              map_rotation;
    CharString           subst_tablet_model;
    CharString           next_level;
    CharString           back_music;
    uint16_t             migration_rules;

    void Read(Config& cfg);
};

struct Params {
    uint16_t version;

    void Read(Config& cfg);
};

class LevelSpawn {
public:
    LevelSpawn();
    ~LevelSpawn();

    bool LoadFromData(MemStream& stream, const VFXReader& fs, const MetroConfigsDatabase& configInfo);

    Startup           mStartup;
    Params            mParams;
    MyArray<uobject*> mSpawn;
};

#include "LevelSpawn.h"
#include "Config.h"
#include "MetroConfigDatabase.h"
#include "VFXReader.h"
#include "entities/Entity.h"
#include "entities/EntityFactory.h"
#include <functional>
#include <hashing.h>

static const uint32_t LEVEL_ID = MakeFourcc<'l', 'e', 'v', 'l'>();

void PresetBlend::Read(Config& cfg) {
    name = cfg.r_sz("name");
    game_time = cfg.r_bool("game_time");
    removing = cfg.r_bool("removing");
    start = cfg.r_s32("start");
    finish = cfg.r_s32("finish");
}

void Startup::Read(Config& cfg) {
    game_time = cfg.r_u32("game_time");
    dc_prev_speed = cfg.r_fp32("dc_prev_speed");
    dc_target_speed = cfg.r_fp32("dc_target_speed");
    dc_curr_speed = cfg.r_fp32("dc_curr_speed");
    dc_time_0 = cfg.r_s32("dc_time_0");
    dc_time_1 = cfg.r_s32("dc_time_1");
    main_preset_name = cfg.r_sz("main_preset_name");
    current = cfg.r_sz("current");
    preset.Read(cfg);
    source_type = cfg.r_u8("source_type");
    cfg.ReadArray("modifiers", [this](Config& cfg, uint32_t idx) {
        PresetBlend m;
        m.Read(cfg);
        modifiers.push_back(m);
    });
    _total_time = cfg.r_u64("_total_time");
    weather_preset = cfg.r_choose("weather_preset");
    dc_start = cfg.r_time("dc_start");
    dc_duration = cfg.r_time("dc_duration");
    foliage_set = cfg.r_choose("foliage_set");
    foliage_fuzziness = cfg.r_fp32("foliage_fuzziness");
    map_positional_min = cfg.r_vec3f("map_positional_min");
    map_positional_max = cfg.r_vec3f("map_positional_max");
    map_positional_preview_aabb = cfg.r_bool("map_positional_preview_aabb");
    map_menu_name = cfg.r_choose("map_menu_name");
    map_rotation = cfg.r_u8("map_rotation");
    subst_tablet_model = cfg.r_choose("subst_tablet_model");
    next_level = cfg.r_choose("next_level");
    back_music = cfg.r_choose("back_music");
    migration_rules = cfg.r_u16("migration_rules");
}

void Params::Read(Config& cfg) {
    version = cfg.r_u16("version");
}

void InitData::Read(Config& cfg) {
    cls = cfg.r_u32("class");
    static_data_key = cfg.r_u32("static_data_key");
    att_bone_id = cfg.r_choose("att_bone_id");
    id = cfg.r_u16("id");
    parent_id = cfg.r_u16("parent_id");
    att_offset = cfg.r_pose("att_offset");
    att_root = cfg.r_bool("att_root");
}

LevelSpawn::LevelSpawn() {
}

LevelSpawn::~LevelSpawn() {
}

bool LevelSpawn::LoadFromData(MemStream& stream, const VFXReader& fs, const MetroConfigsDatabase& configDb) {
    auto      fileIdx = fs.FindFile("content\\config.bin");
    MemStream config = fs.ExtractFile(fileIdx);
    if (!config)
        return false;

    mSpawn.clear();

    auto id = stream.ReadTyped<uint32_t>();
    if (id != LEVEL_ID)
        return false;

    Config cfg(stream.Substream(stream.Remains()));
    cfg.ReadSection("startup", [this](Config& cfg) {
        mStartup.Read(cfg);
    });
    cfg.ReadSection("entities_params", [this](Config& cfg) {
        mParams.Read(cfg);
    });
    cfg.ReadArray("entities", [this, configDb, &config](Config& cfg, uint32_t idx) {
        InitData data;
        data.Read(cfg);
        auto t = EntityFactory::Create(data);
        t->param = EntityFactory::GetStaticParams(t->initData, configDb, config);
        t->Read(cfg);
        mSpawn.push_back(t);
        assert(cfg.SectionRemains() == 0);
    });
    return true;
}

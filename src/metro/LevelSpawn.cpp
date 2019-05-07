#include "LevelSpawn.h"
#include "MetroBinArchive.h"
#include "MetroConfigDatabase.h"
#include "MetroReflection.h"
#include "VFXReader.h"
#include "entities/Entity.h"
#include "entities/EntityFactory.h"
#include <functional>


void PresetBlend::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, name);
    METRO_READ_MEMBER(r, game_time);
    METRO_READ_MEMBER(r, removing);
    METRO_READ_MEMBER(r, start);
    METRO_READ_MEMBER(r, finish);
}

void Startup::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, game_time);
    METRO_READ_MEMBER(r, dc_prev_speed);
    METRO_READ_MEMBER(r, dc_target_speed);
    METRO_READ_MEMBER(r, dc_curr_speed);
    METRO_READ_MEMBER(r, dc_time_0);
    METRO_READ_MEMBER(r, dc_time_1);
    METRO_READ_MEMBER(r, main_preset_name);
    METRO_READ_MEMBER(r, current);
    preset.Serialize(r);
    METRO_READ_MEMBER(r, source_type);
    METRO_READ_CHILD_STRUCT_ARRAY(r, modifiers);
    METRO_READ_MEMBER(r, _total_time);
    METRO_READ_MEMBER_CHOOSE(r, weather_preset);
    METRO_READ_MEMBER(r, dc_start);
    METRO_READ_MEMBER(r, dc_duration);
    METRO_READ_MEMBER_CHOOSE(r, foliage_set);
    METRO_READ_MEMBER(r, foliage_fuzziness);
    METRO_READ_MEMBER(r, map_positional_min);
    METRO_READ_MEMBER(r, map_positional_max);
    METRO_READ_MEMBER(r, map_positional_preview_aabb);
    METRO_READ_MEMBER_CHOOSE(r, map_menu_name);
    METRO_READ_MEMBER(r, map_rotation);
    METRO_READ_MEMBER_CHOOSE(r, subst_tablet_model);
    METRO_READ_MEMBER_CHOOSE(r, next_level);
    METRO_READ_MEMBER_CHOOSE(r, back_music);
    METRO_READ_MEMBER(r, migration_rules);
}

void Params::Serialize(MetroReflectionReader& r) {
    METRO_READ_MEMBER(r, version);
}

LevelSpawn::LevelSpawn() {
}

LevelSpawn::~LevelSpawn() {
    ClearEntities();
}

void Serialize(MetroReflectionReader& r, uobject*& obj) {
    InitData data;
    data.Serialize(r);
    obj = EntityFactory::Create(data);
    // obj->param = EntityFactory::GetStaticParams(obj->initData, config);
    obj->Serialize(r);
}

bool LevelSpawn::LoadFromData(MemStream& stream) {
    bool result = false;

    const uint32_t magic = stream.ReadTyped<uint32_t>();
    if (magic == MakeFourcc<'l', 'e', 'v', 'l'>()) {
        stream.SetCursor(0);

        ClearEntities();

        MetroBinArchive a("level.bin", stream, 4);
        auto r = a.ReflectionReader();
        METRO_READ_CHILD_STRUCT(r, startup);
        METRO_READ_CHILD_STRUCT(r, entities_params);
        METRO_READ_CHILD_STRUCT_ARRAY(r, entities);

        result = true;
    }

    return result;
}

void LevelSpawn::ClearEntities() {
    std::for_each(entities.begin(), entities.end(), [](uobject* obj) {
        delete obj;
    });
}

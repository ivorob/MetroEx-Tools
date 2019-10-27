#pragma once
#include "mycommon.h"
#include "mymath.h"
#include "MetroTypes.h"

class MetroReflectionReader;

struct MetroWeaponHandle {
    HashString  name;
    CharString  upgrades;
    StringArray upgradesArray;

    void Serialize(MetroReflectionReader& s);
};

struct MetroWeaponAttachmentDesc {
    HashString          name;
    uint32_t            type;
    uint32_t            upgrade_id;
    CharString          base_handle;
    // data section
    CharString          shared_weapon_params;
    uint8_t             priority;
    uint16_t            ui_tag_lib;
    MyArray<uint16_t>   tags;
    MyArray<uint16_t>   excl_tags;
    MyArray<uint16_t>   dep_tags;
    MyArray<uint16_t>   autoinst_tags;
    bool                is_installed;
    CharString          editor_name;
    CharString          attach_hud_loc;
    posemat             attach_offset;
    bool                attach_to_root;
    CharString          visual;
    CharString          anim_track_prefix;
    CharString          anim_track_suffix;

    void Serialize(MetroReflectionReader& s);
};

class MetroWeaponsDatabase {
    IMPL_SINGLETON(MetroWeaponsDatabase)

protected:
    MetroWeaponsDatabase();
    ~MetroWeaponsDatabase();

public:
    bool    Initialize();

private:
    void    ReadWeaponHandles(MemStream& stream);
    void    ReadWeaponAttaches(MemStream& stream);

private:
    MyArray<MetroWeaponHandle>          mWeaponHandles;
    MyArray<MetroWeaponAttachmentDesc>  mWeaponAttachments;
};


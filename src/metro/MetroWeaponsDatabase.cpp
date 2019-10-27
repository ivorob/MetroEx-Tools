#include "MetroWeaponsDatabase.h"
#include "MetroReflection.h"
#include "MetroTypes.h"
#include "MetroBinArrayArchive.h"
#include "MetroBinArchive.h"
#include "MetroFileSystem.h"



void MetroWeaponHandle::Serialize(MetroReflectionReader& s) {
    //#TODO_SK: fix this shit! Should detect based on the Editor flag!
    METRO_READ_MEMBER_CHOOSE(s, upgrades);

    upgradesArray = StrSplit(upgrades, ',');
}


void MetroWeaponAttachmentDesc::Serialize(MetroReflectionReader& s) {
    METRO_READ_MEMBER(s, type);
    METRO_READ_MEMBER(s, upgrade_id);
    METRO_READ_MEMBER(s, base_handle);
    // data section
    auto data = s.OpenSection("data");
    if (data.Good()) {
        METRO_READ_MEMBER_CHOOSE(data, shared_weapon_params);
        METRO_READ_MEMBER(data, priority);
        METRO_READ_MEMBER(data, ui_tag_lib);
        METRO_READ_ARRAY_16_MEMBER(data, tags);
        METRO_READ_ARRAY_16_MEMBER(data, excl_tags);
        METRO_READ_ARRAY_16_MEMBER(data, dep_tags);
        METRO_READ_ARRAY_16_MEMBER(data, autoinst_tags);
        METRO_READ_MEMBER(data, is_installed);
        METRO_READ_MEMBER(data, editor_name);
        METRO_READ_MEMBER(data, attach_hud_loc);
        METRO_READ_MEMBER(data, attach_offset);
        METRO_READ_MEMBER(data, attach_to_root);
        METRO_READ_MEMBER_CHOOSE(data, visual);
        METRO_READ_MEMBER(data, anim_track_prefix);
        METRO_READ_MEMBER(data, anim_track_suffix);

        s.CloseSection(data);
    }
}


MetroWeaponsDatabase::MetroWeaponsDatabase() {
}
MetroWeaponsDatabase::~MetroWeaponsDatabase() {
}

bool MetroWeaponsDatabase::Initialize() {
    bool result = false;

    const MetroFileSystem& mfs = MetroFileSystem::Get();

    MyHandle fileHandle = mfs.FindFile(R"(content\weaponry\weapons_handles_storage.bin)");
    if (fileHandle != kInvalidHandle) {
        MemStream stream = mfs.OpenFileStream(fileHandle);
        if (stream) {
            this->ReadWeaponHandles(stream);
        }
    }

    fileHandle = mfs.FindFile(R"(content\weaponry\attaches_handles_storage.bin)");
    if (fileHandle != kInvalidHandle) {
        MemStream stream = mfs.OpenFileStream(fileHandle);
        if (stream) {
            this->ReadWeaponAttaches(stream);
        }
    }

    if (!mWeaponHandles.empty() && !mWeaponAttachments.empty()) {
        result = true;
    }

    return result;
}

void MetroWeaponsDatabase::ReadWeaponHandles(MemStream& stream) {
    const size_t numEntries = stream.ReadTyped<uint32_t>();

    mWeaponHandles.resize(numEntries);
    for (size_t i = 0; i < numEntries; ++i) {
        const size_t idx = stream.ReadTyped<uint32_t>();
        const size_t size = stream.ReadTyped<uint32_t>();

        MemStream subStream = stream.Substream(size);

        MetroWeaponHandle& weaponHandle = mWeaponHandles[i];

        weaponHandle.name = subStream.ReadStringZ();
        const uint8_t flags = subStream.ReadTyped<uint8_t>();

        MetroReflectionReader reader(subStream, flags);
        reader >> weaponHandle;

        stream.SkipBytes(size);
    }
}

void MetroWeaponsDatabase::ReadWeaponAttaches(MemStream& stream) {
    const size_t numEntries = stream.ReadTyped<uint32_t>();

    mWeaponAttachments.resize(numEntries);
    for (size_t i = 0; i < numEntries; ++i) {
        const size_t idx = stream.ReadTyped<uint32_t>();
        const size_t size = stream.ReadTyped<uint32_t>();

        MemStream subStream = stream.Substream(size);

        MetroWeaponAttachmentDesc& attachment = mWeaponAttachments[i];

        attachment.name = subStream.ReadStringZ();
        const uint8_t flags = subStream.ReadTyped<uint8_t>();

        MetroReflectionReader reader(subStream, flags);
        reader >> attachment;

        stream.SkipBytes(size);
    }
}

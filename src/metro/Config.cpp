#include "Config.h"
#include <array>

Config::Config(MemStream& stream) {
    mFlags = static_cast<Flags>(stream.ReadTyped<uint8_t>());
    if (TestBit(mFlags, RefStrings)) {
        while (!stream.Ended()) {
            const size_t chunkId = stream.ReadTyped<uint32_t>();
            const size_t chunkSize = stream.ReadTyped<uint32_t>();

            switch (chunkId) {
            case 1:
                mData = stream.Substream(chunkSize);
                stream.SkipBytes(chunkSize);
                break;
            case 2: {
                const size_t numStrings = stream.ReadTyped<uint32_t>();
                mStrings.resize(numStrings);
                for (CharString& s : mStrings) {
                    s = stream.ReadStringZ();
                }
            } break;
            }
        }
    } else
        mData = stream.Substream(stream.Remains());
    if (TestBit(mFlags, MultiPart)) {
        const size_t chunkId = mData.ReadTyped<uint32_t>();
        const size_t chunkSize = mData.ReadTyped<uint32_t>();
        assert(chunkId == Hash_CalculateCRC32("arch_chunk_0"));
        mData = mData.Substream(chunkSize);
    }
}

size_t Config::Remains() const {
    return mData.Remains();
}

size_t Config::SectionRemains() const {
    return mSectionEnds.back() - mData.GetCursor();
}

bool Config::Ended() const {
    return mData.Ended();
}

uint8_t Config::r_u8(const char* name) {
    return mData.ReadTyped<uint8_t>();
}

uint8_t Config::r_bool8(const char* name) {
    return mData.ReadTyped<uint8_t>();
}

uint16_t Config::r_u16(const char* name) {
    return mData.ReadTyped<uint16_t>();
}

uint16_t Config::r_entity_link(const char* name) {
    return mData.ReadTyped<uint16_t>();
}

int32_t Config::r_s32(const char* name) {
    return mData.ReadTyped<int32_t>();
}

uint32_t Config::r_u32(const char* name) {
    return mData.ReadTyped<uint32_t>();
}

uint32_t Config::r_time(const char* name) {
    return r_u32(name);
}

uint32_t Config::r_color_u32(const char* name) {
    return mData.ReadTyped<uint32_t>();
}

uint64_t Config::r_u64(const char* name) {
    return mData.ReadTyped<uint64_t>();
}

uint64_t Config::r_flags64(const char* name) {
    return r_u64(name);
}

float Config::r_fp32(const char* name) {
    return mData.ReadTyped<float>();
}

float Config::r_fp32_q8(const char* name) {
    uint8_t b = mData.ReadTyped<uint8_t>();
    return 2 * b / 255.0f;
}

float Config::r_angle(const char* name) {
    return mData.ReadTyped<float>();
}

CharString Config::r_sz(const char* name) {
    if (TestBit(mFlags, RefStrings)) {
        uint32_t idx = mData.ReadTyped<uint32_t>();
        return mStrings[idx];
    } else
        return mData.ReadStringZ();
}

CharString Config::r_name(const char* name) {
    return r_sz(name);
}

CharString Config::r_choose(const char* name) {
    return r_sz(name);
}

CharString Config::r_animation_str(const char* name) {
    return r_sz(name);
}

CharString Config::r_part_str(const char* name) {
    return r_sz(name);
}

CharString Config::r_sound(const char* name) {
    return r_sz(name);
}

CharString Config::r_choose_array(const char* name) {
    return r_sz(name);
}

CharString Config::r_attp_str(const char* name) {
    return r_sz(name);
}

bool Config::r_bool(const char* name) {
    auto b = mData.ReadTyped<uint8_t>();
    assert(b == 0 || b == 1);
    return b != 0;
}

vec3 Config::r_vec3f(const char* name) {
    return mData.ReadTyped<vec3>();
}

vec3 Config::r_ang3f(const char* name) {
    return mData.ReadTyped<vec3>();
}

vec4 Config::r_color(const char* name) {
    return mData.ReadTyped<vec4>();
}

mat43T Config::r_pose(const char* name) {
    return mData.ReadTyped<mat43T>();
}

std::array<uint16_t, 4> Config::r_vec4s16(const char* name) {
    std::array<uint16_t, 4> result;
    mData.ReadToBuffer(result.data(), sizeof(result));
    return result;
}

std::array<int32_t, 4> Config::r_vec4i(const char* name) {
    std::array<int32_t, 4> result;
    mData.ReadToBuffer(result.data(), sizeof(result));
    return result;
}

BytesArray Config::r_bytes(size_t size) {
    BytesArray result(size);
    mData.ReadToBuffer(result.data(), result.size());
    return result;
}

void Config::OpenSection(uint32_t section_id) {
    const auto id = mData.ReadTyped<uint32_t>();
    assert(id == section_id);
    const auto size = mData.ReadTyped<uint32_t>();
    mSectionEnds.push_back(mData.GetCursor() + size);
}

void Config::OpenSection(const char* section) {
    const auto crc = Hash_CalculateCRC32(section);
    OpenSection(crc);
}

void Config::CloseSection() {
    size_t end = mSectionEnds.back();
    mSectionEnds.pop_back();
    size_t diff = end - mData.GetCursor();
    assert(diff == 0);
    mData.SetCursor(end);
}

ParamValue Config::Read(ParamType type, const char* name) {
    switch (type) {
    case Type_u16:
        return r_u16(name);
    case Type_bool:
        return r_bool(name);
    case Type_bool8:
        return r_bool8(name);
    case Type_time:
        return r_time(name);
    case Type_anim:
        return r_animation_str(name);
    case Type_choose:
        return r_choose(name);
    case Type_color_u32:
        return r_color_u32(name);
    case Type_entity:
        return r_entity_link(name);
    case Type_fp32:
        return r_fp32(name);
    case Type_sz:
        return r_sz(name);
    case Type_part:
        return r_part_str(name);
    case Type_u8:
        return r_u8(name);
    case Type_u32:
        return r_u32(name);
    default:
        assert(false);
        return ParamValue();
    }
}

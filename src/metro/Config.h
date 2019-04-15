#pragma once

#include <mycommon.h>
#include <mymath.h>

class Config {
public:
    explicit Config(MemStream& stream);

    template <typename F>
    void ReadSection(uint32_t section_id, F f) {
        OpenSection(section_id);
        f(*this);
        CloseSection();
    }
    template <typename F>
    void ReadSection(const char* section, F f) {
        OpenSection(section);
        f(*this);
        CloseSection();
    }

    template <typename F>
    void ReadSubconfig(const char* section, F f) {
        OpenSection(section);
        uint8_t subFlags = static_cast<Flags>(mData.ReadTyped<uint8_t>());
        assert(subFlags == mFlags);
        f(*this);
        CloseSection();
    }

    template <typename F>
    void ReadArray(const char* section, F f) {
        ReadArray(section, std::numeric_limits<uint32_t>::max(), f);
    }

    template <typename F>
    void ReadArray(const char* section, uint32_t count, F f) {
        if (section)
            OpenSection(section);
        if (count == std::numeric_limits<uint32_t>::max())
            count = r_u32("count");
        assert(count != 0xFFFFFFFF);
        for (uint32_t i = 0; i != count; i++) {
            OpenSection(0u);
            f(*this, i);
            CloseSection();
        }
        if (section)
            CloseSection();
    }

    size_t Remains() const;
    size_t SectionRemains() const;
    bool   Ended() const;

    uint8_t                 r_u8(const char* name);
    uint8_t                 r_bool8(const char* name);
    uint16_t                r_u16(const char* name);
    uint16_t                r_entity_link(const char* name);
    int32_t                 r_s32(const char* name);
    uint32_t                r_u32(const char* name);
    uint32_t                r_time(const char* name);
    uint64_t                r_u64(const char* name);
    uint64_t                r_flags64(const char* name);
    float                   r_fp32(const char* name);
    float                   r_fp32_q8(const char* name);
    float                   r_angle(const char* name);
    CharString              r_sz(const char* name);
    CharString              r_name(const char* name);
    CharString              r_choose(const char* name);
    CharString              r_animation_str(const char* name);
    CharString              r_part_str(const char* name);
    CharString              r_sound(const char* name);
    CharString              r_choose_array(const char* name);
    CharString              r_attp_str(const char* name);
    bool                    r_bool(const char* name);
    vec3                    r_vec3f(const char* name);
    vec3                    r_ang3f(const char* name);
    vec4                    r_color(const char* name);
    mat43T                  r_pose(const char* name);
    std::array<uint16_t, 4> r_vec4s16(const char* name);
    std::array<int32_t, 4>  r_vec4i(const char* name);

    BytesArray r_bytes(size_t size);

private:
    enum Flags : uint8_t {
        HasDebugInfo = 1,
        RefStrings = 4,
        MultiPart = 32,
    };
    Flags           mFlags;
    MemStream       mData;
    StringArray     mStrings;
    MyArray<size_t> mSectionEnds;

    void OpenSection(uint32_t section_id);
    void OpenSection(const char* section);
    void CloseSection();
};

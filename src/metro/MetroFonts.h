#pragma once
#include "MetroTypes.h"

class MetroReflectionStream;

class MetroFontDescriptor {
public:
    struct CharInfo {
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
        uint16_t xoffset;
        uint16_t yoffset;
        uint16_t xadvance;

        void Serialize(MetroReflectionStream& s);
    };

    void Serialize(MetroReflectionStream& s);

private:
    CharString          lang;
    CharString          name;
    CharString          texture;
    vec2                inv_scale;
    float               line_height;
    MyArray<CharInfo>   tcmap;
};

class MetroFontsDatabase {
protected:
    MetroFontsDatabase();
    ~MetroFontsDatabase();

public:
    MetroFontsDatabase(MetroFontsDatabase const&) = delete;
    void operator=(MetroFontsDatabase const&) = delete;

public:
    static MetroFontsDatabase& Get(MetroLanguage lng);

public:
    bool LoadFromData(MemStream& stream);

private:
    MyArray<MetroFontDescriptor>    descriptions;
};

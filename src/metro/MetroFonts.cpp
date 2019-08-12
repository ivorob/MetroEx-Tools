#include "MetroFonts.h"
#include "MetroBinArchive.h"
#include "MetroReflection.h"


void MetroFontDescriptor::CharInfo::Serialize(MetroReflectionReader& s) {
    METRO_READ_MEMBER(s, x);
    METRO_READ_MEMBER(s, y);
    METRO_READ_MEMBER(s, width);
    METRO_READ_MEMBER(s, height);
    METRO_READ_MEMBER(s, xoffset);
    METRO_READ_MEMBER(s, yoffset);
    METRO_READ_MEMBER(s, xadvance);
}

void MetroFontDescriptor::Serialize(MetroReflectionReader& s) {
    METRO_READ_MEMBER(s, lang);
    METRO_READ_MEMBER(s, name);
    METRO_READ_MEMBER(s, texture);
    METRO_READ_MEMBER(s, inv_scale);
    METRO_READ_MEMBER(s, line_height);
    METRO_READ_STRUCT_ARRAY_MEMBER(s, tcmap);
}


MetroFontsDatabase::MetroFontsDatabase() {
}
MetroFontsDatabase::~MetroFontsDatabase() {
}

MetroFontsDatabase& MetroFontsDatabase::Get(MetroLanguage lng) {
    static MetroFontsDatabase _singletons[scast<size_t>(MetroLanguage::Count)];
    return _singletons[scast<size_t>(lng)];
}

bool MetroFontsDatabase::LoadFromData(MemStream& stream) {
    MetroBinArchive bin("fonts_xx.bin", stream, MetroBinArchive::kHeaderDoAutoSearch);
    MetroReflectionReader reader = bin.ReflectionReader();

    METRO_READ_STRUCT_ARRAY_MEMBER(reader, descriptions);

    return !descriptions.empty();
}

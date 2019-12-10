#include "MetroFonts.h"
#include "MetroBinArchive.h"
#include "MetroReflection.h"


void MetroFontDescriptor::CharInfo::Serialize(MetroReflectionStream& s) {
    METRO_SERIALIZE_MEMBER(s, x);
    METRO_SERIALIZE_MEMBER(s, y);
    METRO_SERIALIZE_MEMBER(s, width);
    METRO_SERIALIZE_MEMBER(s, height);
    METRO_SERIALIZE_MEMBER(s, xoffset);
    METRO_SERIALIZE_MEMBER(s, yoffset);
    METRO_SERIALIZE_MEMBER(s, xadvance);
}

void MetroFontDescriptor::Serialize(MetroReflectionStream& s) {
    METRO_SERIALIZE_MEMBER(s, lang);
    METRO_SERIALIZE_MEMBER(s, name);
    METRO_SERIALIZE_MEMBER(s, texture);
    METRO_SERIALIZE_MEMBER(s, inv_scale);
    METRO_SERIALIZE_MEMBER(s, line_height);
    METRO_SERIALIZE_STRUCT_ARRAY_MEMBER(s, tcmap);
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
    StrongPtr<MetroReflectionStream> reader = bin.ReflectionReader();

    METRO_SERIALIZE_STRUCT_ARRAY_MEMBER(*reader, descriptions);

    return !descriptions.empty();
}

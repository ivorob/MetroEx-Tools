#pragma once
#include "mycommon.h"

class MetroLocalization {
public:
    MetroLocalization();
    ~MetroLocalization();

    bool                LoadFromData(MemStream stream);

    bool                SaveToExcel2003(const fs::path& path);

    size_t              GetNumStrings() const;
    const CharString&   GetKey(const size_t idx) const;
    const WideString&   GetValue(const size_t idx) const;

private:
    struct LocPair {
        CharString  key;
        WideString  value;
    };

private:
    MyArray<LocPair>    mStrings;
};

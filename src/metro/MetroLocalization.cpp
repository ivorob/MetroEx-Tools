#include "MetroLocalization.h"

enum LocalizationChunk : size_t {
    LC_CharsTable   = 0x00000001,
    LC_StringsTable = 0x00000002
};

MetroLocalization::MetroLocalization() {
}
MetroLocalization::~MetroLocalization() {
}

bool MetroLocalization::LoadFromData(MemStream stream) {
    MyArray<uint16_t> charsTable;

    while (!stream.Ended()) {
        const size_t chunkId = stream.ReadTyped<uint32_t>();
        const size_t chunkSize = stream.ReadTyped<uint32_t>();
        const size_t chunkEnd = stream.GetCursor() + chunkSize;

        switch (chunkId) {
            case LC_CharsTable: {
                charsTable.resize(chunkSize / 2);
                stream.ReadToBuffer(charsTable.data(), chunkSize);
            } break;

            case LC_StringsTable: {
                assert(!charsTable.empty());
                if (charsTable.empty()) {
                    break;
                }

                MemStream subStream = stream.Substream(chunkSize);
                while (!subStream.Ended()) {
                    LocPair p;
                    p.key = subStream.ReadStringZ();

                    CharString encodedValue = subStream.ReadStringZ();
                    p.value.reserve(encodedValue.length());

                    const uint8_t* codes = rcast<const uint8_t*>(encodedValue.data());
                    const size_t len = encodedValue.length();
                    for (size_t i = 0; i < len; ++i) {
                        const size_t charIdx = scast<size_t>(codes[i]);
                        p.value.push_back(scast<WideString::value_type>(charsTable[charIdx]));
                    }

                    mStrings.emplace_back(p);
                }
            } break;
        }

        stream.SetCursor(chunkEnd);
    }

    return !mStrings.empty();
}

size_t MetroLocalization::GetNumStrings() const {
    return mStrings.size();
}

const CharString& MetroLocalization::GetKey(const size_t idx) const {
    return mStrings[idx].key;
}

const WideString& MetroLocalization::GetValue(const size_t idx) const {
    return mStrings[idx].value;
}

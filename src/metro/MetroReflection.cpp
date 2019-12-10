#include "MetroReflection.h"

MetroReflectionStream::MetroReflectionStream()
    : mMode(MetroReflectionStream::Mode::IN)
    , mUserData(kInvalidValue)
    , mSTable(nullptr)
    , mFlags(MetroReflectionFlags::None) {

}
MetroReflectionStream::~MetroReflectionStream() {

}


void MetroReflectionStream::SerializeBool(bool& v) {
    if (this->IsIn()) {
        uint8_t u;
        this->SerializeU8(u);
        v = (u != 0);
    } else {
        uint8_t u = v ? 1 : 0;
        this->SerializeU8(u);
    }
}

void MetroReflectionStream::SerializeI8(int8_t& v) {
    this->SerializeRawBytes(&v, sizeof(v));
}

void MetroReflectionStream::SerializeU8(uint8_t& v) {
    this->SerializeRawBytes(&v, sizeof(v));
}

void MetroReflectionStream::SerializeI16(int16_t& v) {
    this->SerializeRawBytes(&v, sizeof(v));
}

void MetroReflectionStream::SerializeU16(uint16_t& v) {
    this->SerializeRawBytes(&v, sizeof(v));
}

void MetroReflectionStream::SerializeI32(int32_t& v) {
    this->SerializeRawBytes(&v, sizeof(v));
}

void MetroReflectionStream::SerializeU32(uint32_t& v) {
    this->SerializeRawBytes(&v, sizeof(v));
}

void MetroReflectionStream::SerializeI64(int64_t& v) {
    this->SerializeRawBytes(&v, sizeof(v));
}

void MetroReflectionStream::SerializeU64(uint64_t& v) {
    this->SerializeRawBytes(&v, sizeof(v));
}

void MetroReflectionStream::SerializeF32(float& v) {
    this->SerializeRawBytes(&v, sizeof(v));
}

void MetroReflectionStream::SerializeF64(double& v) {
    this->SerializeRawBytes(&v, sizeof(v));
}

void MetroReflectionStream::SerializeString(CharString& v) {
    if (this->HasStringsTable()) {
        if (this->IsIn()) {
            uint32_t ref;
            this->SerializeU32(ref);

            if (ref != kInvalidValue32 && mSTable) {
                v = mSTable->GetString(ref);
            }
        } else {
            //#TODO_SK: Implement me !!!!
            assert(false);
        }
    } else {
        if (this->IsIn()) {
            this->ReadStringZ(v);
        } else {
            this->WriteStringZ(v);
        }
    }
}

// Advanced
void MetroReflectionStream::SerializeFloats(float* fv, const size_t numFloats) {
    this->SerializeRawBytes(fv, numFloats * sizeof(float));
}


// Metro data reading details
bool MetroReflectionStream::SerializeEditorTag(const CharString& propName) {
    //static CharString sChooseStr("choose");

    if (this->HasDebugInfo()) {
        CharString name;
        this->ReadStringZ(name);

        assert(name == propName);
        if (name != propName) {
            return false;
        }

        CharString chooseStr;
        this->ReadStringZ(chooseStr);
        //#TODO_SK: different choose attributes could be
        //assert(chooseStr == sChooseStr);
        //if (chooseStr != sChooseStr) {
        //    return false;
        //}
    }
    return true;
}

bool MetroReflectionStream::SerializeTypeInfo(const CharString& propName, const CharString& typeAlias) {
    if (this->HasDebugInfo()) {
        CharString name;
        this->ReadStringZ(name);

        assert(name == propName);
        if (name != propName) {
            return false;
        }

        CharString type;
        this->ReadStringZ(type);

        assert(type == typeAlias);
        if (type != typeAlias) {
            return false;
        }
    }

    return true;
}

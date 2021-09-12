#pragma once
#include <sstream>

#include "mycommon.h"
#include "mymath.h"

enum class MetroReflectionFlags : uint8_t {
    None           = 0,
    HasDebugInfo   = 1,
    Editor         = 2,
    StringsTable   = 4,
    Plain          = 8,
    NoSections     = 16,
    MultiChunk     = 32,

    // defaults
    DefaultOutFlags = StringsTable
};


#define METRO_MAKE_TYPE_ALIAS_STRING_NAME(type, alias)  sMetroRegisteredType##type##Alias##alias##Str
#define METRO_MAKE_TYPE_ARRAY_ALIAS_STRING_NAME(type, alias)  sMetroRegisteredTypeArray##type##Alias##alias##Str

template <typename T>
const CharString& MetroTypeGetAlias() {
    static const CharString empty;
    assert(false);
    return empty;
}

template <typename T>
const CharString& MetroTypeArrayGetAlias() {
    static const CharString empty;
    assert(false);
    return empty;
}


#define METRO_REGISTER_TYPE_ALIAS(type, alias)                                                  \
template <> inline const CharString& MetroTypeGetAlias<type>() {                                \
    static const CharString METRO_MAKE_TYPE_ALIAS_STRING_NAME(type, alias) = STRINGIFY(alias);  \
    return METRO_MAKE_TYPE_ALIAS_STRING_NAME(type, alias);                                      \
}


#define METRO_REGISTER_TYPE_ARRAY_ALIAS(type, alias)                                                            \
template <> inline const CharString& MetroTypeArrayGetAlias<type>() {                                           \
    static const CharString METRO_MAKE_TYPE_ARRAY_ALIAS_STRING_NAME(type, alias) = STRINGIFY(alias) "_array";   \
    return METRO_MAKE_TYPE_ARRAY_ALIAS_STRING_NAME(type, alias);                                                \
}


#define METRO_REGISTER_INHERITED_TYPE_ALIAS(type, baseType, alias)                                                      \
template <> inline const CharString& MetroTypeGetAlias<type>() {                                                        \
    static const CharString METRO_MAKE_TYPE_ALIAS_STRING_NAME(type, alias) = STRINGIFY(alias) ", " STRINGIFY(baseType); \
    return METRO_MAKE_TYPE_ALIAS_STRING_NAME(type, alias);                                                              \
}



METRO_REGISTER_TYPE_ALIAS(bool, bool)
METRO_REGISTER_TYPE_ALIAS(uint8_t, u8)
METRO_REGISTER_TYPE_ALIAS(uint16_t, u16)
METRO_REGISTER_TYPE_ALIAS(uint32_t, u32)
METRO_REGISTER_TYPE_ALIAS(int8_t, s8)
METRO_REGISTER_TYPE_ALIAS(int16_t, s16)
METRO_REGISTER_TYPE_ALIAS(int32_t, s32)
METRO_REGISTER_TYPE_ALIAS(float, fp32)
METRO_REGISTER_TYPE_ALIAS(vec2, vec2f)
METRO_REGISTER_TYPE_ALIAS(vec3, vec3f)
METRO_REGISTER_TYPE_ALIAS(vec4, vec4f)
METRO_REGISTER_TYPE_ALIAS(quat, vec4f)
METRO_REGISTER_TYPE_ALIAS(CharString, stringz)
METRO_REGISTER_TYPE_ALIAS(Bool8, bool8)

METRO_REGISTER_INHERITED_TYPE_ALIAS(color4f, vec4f, color)
METRO_REGISTER_INHERITED_TYPE_ALIAS(posemat, matrix_43T, pose)
METRO_REGISTER_INHERITED_TYPE_ALIAS(posematrix, matrix, pose)   // why in the fuck ???
METRO_REGISTER_INHERITED_TYPE_ALIAS(anglef, fp32, angle);

METRO_REGISTER_TYPE_ARRAY_ALIAS(bool, bool)
METRO_REGISTER_TYPE_ARRAY_ALIAS(uint8_t, u8)
METRO_REGISTER_TYPE_ARRAY_ALIAS(uint16_t, u16)
METRO_REGISTER_TYPE_ARRAY_ALIAS(uint32_t, u32)
METRO_REGISTER_TYPE_ARRAY_ALIAS(float, fp32)


// Base class for all reflection streams
class MetroReflectionStream {
public:
    enum Mode : size_t {
        IN,     // reading streams
        OUT     // writing streams
    };

public:
    MetroReflectionStream();
    virtual ~MetroReflectionStream();

    // Mode query
    inline Mode GetMode() const {
        return mMode;
    }

    inline bool IsIn() const {
        return mMode == Mode::IN;
    }

    inline bool IsOut() const {
        return mMode == Mode::OUT;
    }

    // Flags query
    inline bool HasDebugInfo() const {
        return TestBit(mFlags, MetroReflectionFlags::HasDebugInfo);
    }

    inline bool HasStringsTable() const {
        return TestBit(mFlags, MetroReflectionFlags::StringsTable);
    }

    inline bool HasNoSections() const {
        return TestBit(mFlags, MetroReflectionFlags::NoSections);
    }

    // State query
    virtual bool Good() const { return true; }

    inline void SetSectionName(const CharString& sectionName) {
        mSectionName = sectionName;
    }

    inline const CharString& GetSectionName() const {
        return mSectionName;
    }

    inline void SetUserData(const size_t userData) {
        mUserData = userData;
    }

    inline size_t GetUserData() const {
        return mUserData;
    }

    void SetSTable(StringsTable* stable) {
        mSTable = stable;
    }

    // Binary streams will need this
    virtual size_t GetCursor() const { return 0; }
    virtual size_t GetRemains() const { return 0; }
    virtual void SkipBytes(const size_t numBytes) { }

    // Serialization
    virtual void SerializeRawBytes(void* ptr, const size_t length) = 0;
    virtual void SerializeBool(bool& v);
    virtual void SerializeI8(int8_t& v);
    virtual void SerializeU8(uint8_t& v);
    virtual void SerializeI16(int16_t& v);
    virtual void SerializeU16(uint16_t& v);
    virtual void SerializeI32(int32_t& v);
    virtual void SerializeU32(uint32_t& v);
    virtual void SerializeI64(int64_t& v);
    virtual void SerializeU64(uint64_t& v);
    virtual void SerializeF32(float& v);
    virtual void SerializeF64(double& v);
    virtual void SerializeString(CharString& v);
    // Advanced
    virtual void SerializeFloats(float* fv, const size_t numFloats);

    template <typename T>
    inline void operator >>(T& v) {
        *this >> v;
    }

    // Metro data serialization details
    virtual bool SerializeEditorTag(const CharString& propName);
    virtual bool SerializeTypeInfo(const CharString& propName, const CharString& typeAlias);
    virtual MetroReflectionStream* OpenSection(const CharString& sectionName, const bool nameUnknown = false) = 0;
    virtual void CloseSection(MetroReflectionStream* section) = 0;

    template <typename TElement, typename TSize>
    void SerializeArray(MyArray<TElement>& v) {
        TSize numElements = scast<TSize>(v.size());
        (*this) >> numElements;

        if (this->IsIn()) {
            v.resize(numElements);
        }

        for (TElement& e : v) {
            (*this) >> e;
        }
    }

    template <typename T>
    bool SerializeStruct(const CharString& memberName, T& v) {
        MetroReflectionStream* s = this->OpenSection(memberName);
        if (s) {
            (*s) >> v;
            this->CloseSection(s);
        }

        return s != nullptr;
    }

    template <typename T>
    void SerializeStructArray(const CharString& memberName, MyArray<T>& v) {
        this->SerializeTypeInfo(memberName, "array");

        MetroReflectionStream* s = this->OpenSection(memberName);
        if (s) {
            s->SerializeTypeInfo("count", MetroTypeGetAlias<uint32_t>());

            uint32_t arraySize = scast<uint32_t>(v.size());
            (*s) >> arraySize;
            if (arraySize > 0) {
                if (s->IsIn()) {
                    v.resize(arraySize);
                }

                int idx = 0;
                for (T& e : v) {
                    MetroReflectionStream* subS = nullptr;
                    if (s->IsIn()) {
                        subS = s->OpenSection(kEmptyString, true);
                    } else {
                        std::stringstream _subs_name;
                        _subs_name << "rec_" << std::setw(4) << std::setfill('0') << idx;
                        subS = s->OpenSection(_subs_name.str(), true);
                        ++idx;
                    }

                    if (subS) {
                        (*subS) >> e;
                        s->CloseSection(subS);
                    }
                }
            }

            this->CloseSection(s);
        }
    }

protected:
    virtual void ReadStringZ(CharString& s) = 0;
    virtual void WriteStringZ(CharString& s) = 0;

protected:
    Mode            mMode;
    size_t          mUserData;
    CharString      mSectionName;
    StringsTable*   mSTable;
    uint8_t         mFlags;
};

// Base types serialization support
#define IMPLEMENT_BASE_TYPE_SERIALIZE(type, suffix)             \
inline void operator >>(MetroReflectionStream& s, type& v) {    \
    s.Serialize##suffix (v);                                    \
}

IMPLEMENT_BASE_TYPE_SERIALIZE(bool, Bool)
IMPLEMENT_BASE_TYPE_SERIALIZE(int8_t, I8)
IMPLEMENT_BASE_TYPE_SERIALIZE(uint8_t, U8)
IMPLEMENT_BASE_TYPE_SERIALIZE(int16_t, I16)
IMPLEMENT_BASE_TYPE_SERIALIZE(uint16_t, U16)
IMPLEMENT_BASE_TYPE_SERIALIZE(int32_t, I32)
IMPLEMENT_BASE_TYPE_SERIALIZE(uint32_t, U32)
IMPLEMENT_BASE_TYPE_SERIALIZE(int64_t, I64)
IMPLEMENT_BASE_TYPE_SERIALIZE(uint64_t, U64)
IMPLEMENT_BASE_TYPE_SERIALIZE(float, F32)
IMPLEMENT_BASE_TYPE_SERIALIZE(double, F64)
IMPLEMENT_BASE_TYPE_SERIALIZE(CharString, String)

#undef IMPLEMENT_BASE_TYPE_SERIALIZE

// Additional Metro types serialization
inline void operator >>(MetroReflectionStream& s, Bool8& v) {
    s >> v.val8;
}

inline void operator >>(MetroReflectionStream& s, vec2& v) {
    s.SerializeFloats(&v.x, 2);
}

inline void operator >>(MetroReflectionStream& s, vec3& v) {
    s.SerializeFloats(&v.x, 3);
}

inline void operator >>(MetroReflectionStream& s, vec4& v) {
    s.SerializeFloats(&v.x, 4);
}

inline void operator >>(MetroReflectionStream& s, quat& v) {
    s.SerializeFloats(&v.x, 4);
}

inline void operator >>(MetroReflectionStream& s, color4f& v) {
    s.SerializeFloats(&v.r, 4);
}

inline void operator >>(MetroReflectionStream& s, posemat& v) {
    s.SerializeFloats(rcast<float*>(&v), sizeof(v) / sizeof(float));
}

inline void operator >>(MetroReflectionStream& s, posematrix& v) {
    s.SerializeFloats(rcast<float*>(&v), sizeof(v) / sizeof(float));
}

inline void operator >>(MetroReflectionStream& s, anglef& v) {
    s >> v.x;
}

// Binary serialization
#include "MetroReflectionBinary.inl"
// Json serialization
#include "MetroReflectionJson.inl"




template <typename T>
struct ArrayElementTypeGetter {
    typedef typename T::value_type elem_type;
};


#define METRO_SERIALIZE_MEMBER_NO_VERIFY(s, memberName)  s >> memberName;

#define METRO_SERIALIZE_MEMBER(s, memberName)                                                   \
    (s).SerializeTypeInfo(STRINGIFY(memberName), MetroTypeGetAlias<decltype(memberName)>());    \
    (s) >> memberName;

#define METRO_SERIALIZE_STRUCT_MEMBER(s, memberName) (s).SerializeStruct(STRINGIFY(memberName), memberName)

#define METRO_SERIALIZE_ARRAY_MEMBER(s, memberName)                                                                                     \
    (s).SerializeTypeInfo(STRINGIFY(memberName), MetroTypeArrayGetAlias<ArrayElementTypeGetter<decltype(memberName)>::elem_type>());    \
    (s).SerializeArray<ArrayElementTypeGetter<decltype(memberName)>::elem_type, uint32_t>(memberName);

#define METRO_SERIALIZE_ARRAY_16_MEMBER(s, memberName)                                                                                  \
    (s).SerializeTypeInfo(STRINGIFY(memberName), MetroTypeArrayGetAlias<ArrayElementTypeGetter<decltype(memberName)>::elem_type>());    \
    (s).SerializeArray<ArrayElementTypeGetter<decltype(memberName)>::elem_type, uint16_t>(memberName);

#define METRO_SERIALIZE_STRUCT_ARRAY_MEMBER(s, memberName) (s).SerializeStructArray(STRINGIFY(memberName), memberName)

#define METRO_SERIALIZE_MEMBER_CHOOSE(s, memberName)                                            \
    (s).SerializeEditorTag(STRINGIFY(memberName));                                              \
    (s).SerializeTypeInfo(STRINGIFY(memberName), MetroTypeGetAlias<decltype(memberName)>());    \
    (s) >> memberName;

#define METRO_SERIALIZE_MEMBER_ANIMSTR   METRO_SERIALIZE_MEMBER_CHOOSE

#define METRO_SERIALIZE_MEMBER_STRARRAY_CHOOSE(s, memberName)                       \
    (s).SerializeEditorTag(STRINGIFY(memberName));                                  \
    (s).SerializeTypeInfo(STRINGIFY(memberName), MetroTypeGetAlias<CharString>());  \
    (s) >> memberName;


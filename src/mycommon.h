#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <filesystem>
#include <memory>
#include <numeric>
#include <algorithm>
#include <cassert>

#define STRINGIFY_UTIL_(s) #s
#define STRINGIFY(s) STRINGIFY_UTIL_(s)

namespace fs = std::filesystem;

template <typename T>
using MyArray = std::vector<T>;
template <typename K, typename T>
using MyDict = std::unordered_map<K, T>;
template <typename T>
using MyDeque = std::deque<T>;
using CharString = std::string;
using WideString = std::wstring;
using StringArray = MyArray<CharString>;
using BytesArray = MyArray<uint8_t>;

template <typename T>
using StrongPtr = std::unique_ptr<T>;

using MyHandle = size_t;

using flags8 = uint8_t;

static const uint32_t   kInvalidValue32 = ~0u;
static const size_t     kInvalidValue = ~0;
static const MyHandle   kInvalidHandle = ~0;
static const CharString kEmptyString;
static const char       kPathSeparator = '\\';


#define rcast reinterpret_cast
#define scast static_cast

#ifdef __GNUC__
#define PACKED_STRUCT_BEGIN
#define PACKED_STRUCT_END __attribute__((__packed__))
#else
#define PACKED_STRUCT_BEGIN __pragma(pack(push, 1))
#define PACKED_STRUCT_END __pragma(pack(pop))
#endif

// hashing
uint32_t Hash_CalculateCRC32(const uint8_t* data, const size_t dataLength);
uint32_t Hash_CalculateCRC32(const CharString& str);
uint32_t Hash_CalculateXX(const uint8_t* data, const size_t dataLength);
uint32_t Hash_CalculateXX(const CharString& str);


struct TypedString {
    enum {
        TS_DEFAULT_STRING   = 0,
        TS_OBJECT_CLSID     = 1,
        TS_SCRIPT_CLSID     = 2,
        TS_LOCATOR_ID       = 3,
        TS_SDATA_KEY        = 4,
        TS_TEXTURE_SET      = 5,
    };

    TypedString() : type(TS_DEFAULT_STRING), crc32(0u) {}
    TypedString(const TypedString& other) : type(other.type), crc32(other.crc32), str(other.str) {}
    TypedString(const CharString& other, const uint32_t _type) { *this = other; this->type = _type; }

    inline void operator =(const TypedString& other) { type = other.type; crc32 = other.crc32; str = other.str; }
    inline void operator =(const CharString& other) { type = TS_DEFAULT_STRING; crc32 = Hash_CalculateCRC32(other); str = other; }

    inline bool operator ==(const TypedString& other) const { return crc32 == other.crc32; }
    inline bool operator !=(const TypedString& other) const { return crc32 != other.crc32; }

    inline bool operator <(const TypedString& other) const { return crc32 < other.crc32; }
    inline bool operator >(const TypedString& other) const { return crc32 > other.crc32; }

    inline bool Valid() const { return crc32 != 0u; }

    uint32_t    type;
    uint32_t    crc32;
    CharString  str;
};
static const TypedString kEmptyTypedString;

struct HashString {
    HashString() : hash(0u) {}
    HashString(const HashString& other) : hash(other.hash), str(other.str) {}
    HashString(const CharString& other) { *this = other; }

    inline void operator =(const HashString& other) { hash = other.hash; str = other.str; }
    inline void operator =(const CharString& other) { str = other; hash = Hash_CalculateXX(other); }

    inline bool operator ==(const HashString& other) const { return hash == other.hash; }
    inline bool operator !=(const HashString& other) const { return hash != other.hash; }

    inline bool operator <(const HashString& other) const { return hash < other.hash; }
    inline bool operator >(const HashString& other) const { return hash > other.hash; }

    inline bool Valid() const { return hash != 0u; }

    uint32_t    hash;
    CharString  str;
};
static const HashString kEmptyHashString;

namespace std {
    template <> struct hash<HashString> {
        size_t operator()(const HashString& s) const {
            return scast<size_t>(s.hash);
        }
    };
}


inline bool StrEndsWith(const CharString& str, const CharString& ending) {
    return str.size() >= ending.size() && str.compare(str.size() - ending.size(), ending.size(), ending) == 0;
}

inline bool StrContains(const CharString& str, const CharString& value) {
    return str.size() >= value.size() && str.find(value) != CharString::npos;
}

// for string delimiter
inline StringArray StrSplit(const CharString& s, const char delimiter) {
    StringArray result;

    size_t pos_start = 0, pos_end;
    CharString token;

    while ((pos_end = s.find(delimiter, pos_start)) != CharString::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + 1;
        result.emplace_back(token);
    }

    result.push_back(s.substr(pos_start));
    return result;
}


struct StringsTable {
    const char* GetString(const size_t idx) const {
        return this->strings[idx];
    }

    uint32_t AddString(const HashString& str) {
        uint32_t result = kInvalidValue32;
        for (size_t i = 0, end = stringsAdded.size(); i < end; ++i) {
            const HashString& hs = this->stringsAdded[i];
            if (hs == str) {
                result = scast<uint32_t>(i);
                break;
            }
        }

        if (result == kInvalidValue32) {
            result = scast<uint32_t>(stringsAdded.size());
            stringsAdded.push_back(str);
        }

        return result;
    }

    MyArray<char>        data;
    MyArray<const char*> strings;       // holds strings when reading
    MyArray<HashString>  stringsAdded;  // holds strings when writing
};


class MemStream {
    using OwnedPtrType = std::shared_ptr<uint8_t>;

public:
    MemStream()
        : data(nullptr)
        , length(0)
        , cursor(0) {
    }

    MemStream(const void* _data, const size_t _size, const bool _ownMem = false)
        : data(rcast<const uint8_t*>(_data))
        , length(_size)
        , cursor(0)
    {
        //#NOTE_SK: dirty hack to own pointer
        if (_ownMem) {
            ownedPtr = OwnedPtrType(const_cast<uint8_t*>(data), free);
        }
    }
    MemStream(const MemStream& other)
        : data(other.data)
        , length(other.length)
        , cursor(other.cursor)
        , ownedPtr(other.ownedPtr) {
    }
    MemStream(MemStream&& other)
        : data(other.data)
        , length(other.length)
        , cursor(other.cursor)
        , ownedPtr(std::move(other.ownedPtr)) {
    }
    ~MemStream() {
    }

    inline MemStream& operator =(const MemStream& other) {
        this->data = other.data;
        this->length = other.length;
        this->cursor = other.cursor;
        this->ownedPtr = other.ownedPtr;
        return *this;
    }

    inline MemStream& operator =(MemStream&& other) {
        this->data = other.data;
        this->length = other.length;
        this->cursor = other.cursor;
        this->ownedPtr = std::move(other.ownedPtr);
        return *this;
    }

    inline void SetWindow(const size_t wndOffset, const size_t wndLength) {
        this->cursor = wndOffset;
        this->length = wndOffset + wndLength;
    }

    inline operator bool() const {
        return this->Good();
    }

    inline bool Good() const {
        return (this->data != nullptr && this->length > 0);
    }

    inline bool Ended() const {
        return this->cursor == this->length;
    }

    inline size_t Length() const {
        return this->length;
    }

    inline size_t Remains() const {
        return this->length - this->cursor;
    }

    inline const uint8_t* Data() const {
        return this->data;
    }

    void ReadToBuffer(void* buffer, const size_t bytesToRead) {
        if (this->cursor + bytesToRead <= this->length) {
            std::memcpy(buffer, this->data + this->cursor, bytesToRead);
            this->cursor += bytesToRead;
        }
    }

    void SkipBytes(const size_t bytesToSkip) {
        if (this->cursor + bytesToSkip <= this->length) {
            this->cursor += bytesToSkip;
        } else {
            this->cursor = this->length;
        }
    }

    template <typename T>
    T ReadTyped() {
        T result = T(0);
        if (this->cursor + sizeof(T) <= this->length) {
            result = *rcast<const T*>(this->data + this->cursor);
            this->cursor += sizeof(T);
        }
        return result;
    }

    template <typename T>
    void ReadStruct(T& s) {
        this->ReadToBuffer(&s, sizeof(T));
    }

    CharString ReadStringZ() {
        CharString result;

        size_t i = this->cursor;
        const char* ptr = rcast<const char*>(this->data);
        for (; i < this->length; ++i) {
            if (!ptr[i]) {
                break;
            }
        }

        result.assign(ptr + this->cursor, i - this->cursor);
        this->cursor = i + 1;

        return std::move(result);
    }

    inline size_t GetCursor() const {
        return this->cursor;
    }

    void SetCursor(const size_t pos) {
        this->cursor = std::min<size_t>(pos, this->length);
    }

    inline const uint8_t* GetDataAtCursor() const {
        return this->data + this->cursor;
    }

    MemStream Substream(const size_t subStreamLength) const {
        const size_t allowedLength = ((this->cursor + subStreamLength) > this->Length()) ? (this->Length() - this->cursor) : subStreamLength;
        return MemStream(this->GetDataAtCursor(), allowedLength);
    }

    MemStream Substream(const size_t subStreamOffset, const size_t subStreamLength) const {
        const size_t allowedOffset = (subStreamOffset > this->Length()) ? this->Length() : subStreamOffset;
        const size_t allowedLength = ((allowedOffset + subStreamLength) > this->Length()) ? (this->Length() - allowedOffset) : subStreamLength;
        return MemStream(this->data + allowedOffset, allowedLength);
    }

    MemStream Clone() const {
        if (this->ownedPtr) {
            return *this;
        } else {
            void* dataCopy = malloc(this->Length());
            memcpy(dataCopy, this->data, this->Length());
            return MemStream(dataCopy, this->Length(), true);
        }
    }

private:
    const uint8_t*  data;
    size_t          length;
    size_t          cursor;
    OwnedPtrType    ownedPtr;
};


class MemWriteStream {
public:
    MemWriteStream(const size_t startupSize = 4096) { mBuffer.reserve(startupSize); }
    ~MemWriteStream() {}

    void Swap(MemWriteStream& other) {
        mBuffer.swap(other.mBuffer);
    }

    void SwapBuffer(BytesArray& buffer) {
        mBuffer.swap(buffer);
    }

    void Write(const void* data, const size_t length) {
        const size_t cursor = this->GetWrittenBytesCount();
        mBuffer.resize(mBuffer.size() + length);
        memcpy(mBuffer.data() + cursor, data, length);
    }

    void WriteDupByte(const uint8_t value, const size_t numBytes) {
        const size_t cursor = this->GetWrittenBytesCount();
        mBuffer.resize(mBuffer.size() + numBytes);
        memset(mBuffer.data() + cursor, scast<int>(value), numBytes);
    }

    template <typename T>
    void Write(const T& v) {
        this->Write(&v, sizeof(v));
    }

    size_t GetWrittenBytesCount() const {
        return mBuffer.size();
    }

    void* Data() {
        return mBuffer.data();
    }

private:
    BytesArray  mBuffer;
};


template <typename T>
constexpr T SetBit(const T& v, const T& bit) {
    return v | bit;
}
template <typename T>
constexpr T RemoveBit(const T& v, const T& bit) {
    return v & ~bit;
}
template <typename T, typename U>
constexpr bool TestBit(const T& v, const U& bit) {
    return 0 != (v & static_cast<typename std::underlying_type<U>::type>(bit));
}

inline uint32_t CountBitsU32(uint32_t x) {
    x = (x & 0x55555555) + ((x >>  1) & 0x55555555);
    x = (x & 0x33333333) + ((x >>  2) & 0x33333333);
    x = (x & 0x0F0F0F0F) + ((x >>  4) & 0x0F0F0F0F);
    x = (x & 0x00FF00FF) + ((x >>  8) & 0x00FF00FF);
    x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);
    return x;
}

template <char a, char b, char c, char d>
constexpr uint32_t MakeFourcc() {
    const uint32_t result = scast<uint32_t>(a) | (scast<uint32_t>(b) << 8) | (scast<uint32_t>(c) << 16) | (scast<uint32_t>(d) << 24);
    return result;
}

PACKED_STRUCT_BEGIN
struct Bitset256 {
    uint32_t dwords[8];

    inline size_t CountOnes() const {
        size_t result = 0;
        for (uint32_t x : dwords) {
            result += CountBitsU32(x);
        }
        return result;
    }

    inline bool IsPresent(const size_t idx) const {
        const size_t i = idx >> 5;
        assert(i <= 7);
        const uint32_t mask = 1 << (idx & 0x1F);
        return (dwords[i] & mask) == mask;
    }
} PACKED_STRUCT_END;

PACKED_STRUCT_BEGIN
struct Bool8 {
    union {
        uint8_t val8;
        struct {
            bool    b0 : 1;
            bool    b1 : 1;
            bool    b2 : 1;
            bool    b3 : 1;
            bool    b4 : 1;
            bool    b5 : 1;
            bool    b6 : 1;
            bool    b7 : 1;
        };
    };
} PACKED_STRUCT_END;


#ifndef MySafeRelease
#define MySafeRelease(ptr)  \
    if (ptr) {              \
        (ptr)->Release();   \
        (ptr) = nullptr;    \
    }
#endif

#ifndef MySafeDelete
#define MySafeDelete(ptr)   \
    if (ptr) {              \
        delete (ptr);       \
        (ptr) = nullptr;    \
    }
#endif


#define IMPL_SINGLETON(T)           \
public:                             \
T(T const&) = delete;               \
void operator=(T const&) = delete;  \
static T& Get() {                   \
    static T _instance;             \
    return _instance;               \
}


#include "log.h"

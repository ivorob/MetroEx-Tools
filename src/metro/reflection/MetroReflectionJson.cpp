#include "MetroReflection.h"
#include "jansson.h"

void MetroReflectionJsonWriteStream::SerializeBool(bool& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), v ? json_true() : json_false());
}

void MetroReflectionJsonWriteStream::SerializeI8(int8_t& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), json_integer(v));
}

void MetroReflectionJsonWriteStream::SerializeU8(uint8_t& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), json_integer(v));
}

void MetroReflectionJsonWriteStream::SerializeI16(int16_t& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), json_integer(v));
}

void MetroReflectionJsonWriteStream::SerializeU16(uint16_t& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), json_integer(v));
}

void MetroReflectionJsonWriteStream::SerializeI32(int32_t& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), json_integer(v));
}

void MetroReflectionJsonWriteStream::SerializeU32(uint32_t& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), json_integer(v));
}

void MetroReflectionJsonWriteStream::SerializeI64(int64_t& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), json_integer(v));
}

void MetroReflectionJsonWriteStream::SerializeU64(uint64_t& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), json_integer(v));
}

void MetroReflectionJsonWriteStream::SerializeF32(float& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), json_real(v));
}

void MetroReflectionJsonWriteStream::SerializeF64(double& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), json_real(v));
}

void MetroReflectionJsonWriteStream::SerializeString(CharString& v) {
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), json_string_nocheck(v.c_str()));
}

// Advanced
void MetroReflectionJsonWriteStream::SerializeFloats(float* fv, const size_t numFloats) {
    json_t* a = json_array();
    for (size_t i = 0; i < numFloats; ++i) {
        json_array_append_new(a, json_real(fv[i]));
    }
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), a);
}

// Need to override this as we just need the name, not a type
bool MetroReflectionJsonWriteStream::SerializeTypeInfo(const CharString& propName, const CharString&) {
    mCurrentMemberName = propName;
    return true;
}

MetroReflectionStream* MetroReflectionJsonWriteStream::OpenSection(const CharString& sectionName, const bool nameUnknown) {
    mCurrentMemberName = sectionName;
    json_t* o = json_object();
    json_object_set_new_nocheck(mCurrentObj, mCurrentMemberName.c_str(), o);

    mTopObjectsStack.push_front(mCurrentObj);
    mCurrentObj = o;

    return this;
}

void MetroReflectionJsonWriteStream::CloseSection(MetroReflectionStream* section) {
    assert(this == section);
    if (this == section) {
        mCurrentObj = mTopObjectsStack.front();
        mTopObjectsStack.pop_front();
    }
}

// Json specific
CharString MetroReflectionJsonWriteStream::WriteToString() const {
    CharString result;

    char* str = json_dumps(mJson, JSON_INDENT(2));
    if (str) {
        result = str;
        free(str);
    }

    return std::move(result);
}

void MetroReflectionJsonWriteStream::Initialize() {
    mJson = json_object();
    mCurrentObj = mJson;
}

void MetroReflectionJsonWriteStream::Shutdown() {
    json_decref(mJson);
    mJson = nullptr;
    mCurrentObj = nullptr;

    mTopObjectsStack.clear();
}



// READ

// Specialize these
void MetroReflectionJsonReadStream::SerializeBool(bool& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_boolean(j));
    v = json_is_true(j);
    this->Iterate();
}

void MetroReflectionJsonReadStream::SerializeI8(int8_t& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_integer(j));
    v = scast<int8_t>(json_integer_value(j));
    this->Iterate();
}

void MetroReflectionJsonReadStream::SerializeU8(uint8_t& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_integer(j));
    v = scast<uint8_t>(json_integer_value(j));
    this->Iterate();
}

void MetroReflectionJsonReadStream::SerializeI16(int16_t& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_integer(j));
    v = scast<int16_t>(json_integer_value(j));
    this->Iterate();
}

void MetroReflectionJsonReadStream::SerializeU16(uint16_t& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_integer(j));
    v = scast<uint16_t>(json_integer_value(j));
    this->Iterate();
}

void MetroReflectionJsonReadStream::SerializeI32(int32_t& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_integer(j));
    v = scast<int32_t>(json_integer_value(j));
    this->Iterate();
}

void MetroReflectionJsonReadStream::SerializeU32(uint32_t& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_integer(j));
    v = scast<uint32_t>(json_integer_value(j));
    this->Iterate();
}

void MetroReflectionJsonReadStream::SerializeI64(int64_t& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_integer(j));
    v = scast<int64_t>(json_integer_value(j));
    this->Iterate();
}

void MetroReflectionJsonReadStream::SerializeU64(uint64_t& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_integer(j));
    v = scast<uint64_t>(json_integer_value(j));
    this->Iterate();
}

void MetroReflectionJsonReadStream::SerializeF32(float& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_real(j));
    v = scast<float>(json_real_value(j));
    this->Iterate();
}

void MetroReflectionJsonReadStream::SerializeF64(double& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_real(j));
    v = scast<double>(json_real_value(j));
    this->Iterate();
}

void MetroReflectionJsonReadStream::SerializeString(CharString& v) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_string(j));
    v = json_string_value(j);
    this->Iterate();
}

// Advanced
void MetroReflectionJsonReadStream::SerializeFloats(float* fv, const size_t numFloats) {
    json_t* j = json_object_iter_value(mCurrentIter);
    assert(json_is_array(j) && json_array_size(j) == numFloats);
    if (json_is_array(j) && json_array_size(j) == numFloats) {
        size_t idx;
        json_t* elem;
        json_array_foreach(j, idx, elem) {
            fv[idx] = scast<float>(json_real_value(elem));
        }
    }
    this->Iterate();
}

// Need to override this as we just need the name, not a type
bool MetroReflectionJsonReadStream::SerializeTypeInfo(const CharString& propName, const CharString&) {
    return mCurrentMemberName == propName;
}

MetroReflectionStream* MetroReflectionJsonReadStream::OpenSection(const CharString& sectionName, const bool nameUnknown) {
    MetroReflectionStream* result = nullptr;

    if (nameUnknown || mCurrentMemberName == sectionName) {
        json_t* j = json_object_iter_value(mCurrentIter);
        if (json_is_object(j)) {
            mTopObjectsStack.push_front({ mCurrentObj, mCurrentIter });
            mCurrentIter = json_object_iter(j);
            mCurrentObj = j;
            mCurrentMemberName = json_object_iter_key(mCurrentIter);
            result = this;
        }
    }

    return result;
}

void MetroReflectionJsonReadStream::CloseSection(MetroReflectionStream* section) {
    if (section == this) {
        ObjIter obji = mTopObjectsStack.front();
        mCurrentObj = obji.j;
        mCurrentIter = obji.i;
        mTopObjectsStack.pop_front();
        this->Iterate();
    }
}

void MetroReflectionJsonReadStream::Initialize(const CharString& jsonStr) {
    json_error_t jsonErr = {};
    mJson = json_loads(jsonStr.c_str(), JSON_DISABLE_EOF_CHECK, &jsonErr);

    if (mJson) {
        mCurrentObj = mJson;
        mCurrentIter = json_object_iter(mJson);
        mCurrentMemberName = json_object_iter_key(mCurrentIter);
    }
}

void MetroReflectionJsonReadStream::Shutdown() {
    json_decref(mJson);
    mJson = nullptr;
    mCurrentObj = nullptr;
    mCurrentIter = nullptr;
}

void MetroReflectionJsonReadStream::Iterate() {
    if (mCurrentIter) {
        void* next = json_object_iter_next(mCurrentObj, mCurrentIter);
        mCurrentIter = next;
        if (next) {
            mCurrentMemberName = json_object_iter_key(next);
        }
    }
}

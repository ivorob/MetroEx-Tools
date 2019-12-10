#include "MetroSkeleton.h"
#include "MetroBinArchive.h"
#include "MetroReflection.h"

enum SkeletonChunks : size_t {
    SC_SelfData             = 0x00000001,
    SC_StringsDictionary    = 0x00000002,
};

struct ReduxBoneBodyPartHelper {
    uint16_t bp;

    void Serialize(MetroReflectionStream& reader) {
        METRO_SERIALIZE_MEMBER(reader, bp);
    }
};


void ParentMapped::Serialize(MetroReflectionStream& reader) {
    METRO_SERIALIZE_MEMBER(reader, parent_bone);
    METRO_SERIALIZE_MEMBER(reader, self_bone);
    METRO_SERIALIZE_MEMBER(reader, q);
    METRO_SERIALIZE_MEMBER(reader, t);
    METRO_SERIALIZE_MEMBER(reader, s);
}

void MetroBone::Serialize(MetroReflectionStream& reader) {
    METRO_SERIALIZE_MEMBER(reader, name);
    METRO_SERIALIZE_MEMBER(reader, parent);
    METRO_SERIALIZE_MEMBER(reader, q);
    METRO_SERIALIZE_MEMBER(reader, t);

    const size_t skeletonVersion = reader.GetUserData();
    if (skeletonVersion > 18) {
        METRO_SERIALIZE_MEMBER(reader, bp);
        METRO_SERIALIZE_MEMBER(reader, bpf);
    } else {
        //#NOTE_SK: using a hack to read old (Redux) bones
        ReduxBoneBodyPartHelper helper;
        reader >> helper;

        this->bp = scast<uint8_t>(helper.bp & 0xff);
    }
}


MetroSkeleton::MetroSkeleton() {

}
MetroSkeleton::~MetroSkeleton() {

}

bool MetroSkeleton::LoadFromData(MemStream& stream) {
    bool result = false;

    MetroBinArchive bin(kEmptyString, stream, MetroBinArchive::kHeaderNotExist);
    StrongPtr<MetroReflectionStream> reader = bin.ReflectionReader();
    if (reader) {
        this->DeserializeSelf(*reader);
        result = !this->bones.empty();
    }

    return result;
}

size_t MetroSkeleton::GetNumBones() const {
    return this->bones.size();
}

const quat& MetroSkeleton::GetBoneRotation(const size_t idx) const {
    return this->bones[idx].q;
}

const vec3& MetroSkeleton::GetBonePosition(const size_t idx) const {
    return this->bones[idx].t;
}

mat4 MetroSkeleton::GetBoneTransform(const size_t idx) const {
    mat4 result = MatFromQuat(this->bones[idx].q);
    result[3] = vec4(this->bones[idx].t, 1.0f);

    return result;
}

mat4 MetroSkeleton::GetBoneFullTransform(const size_t idx) const {
    const size_t parentIdx = this->GetBoneParentIdx(idx);
    if (parentIdx == MetroBone::InvalidIdx) {
        return this->GetBoneTransform(idx);
    } else {
        return this->GetBoneFullTransform(parentIdx) * this->GetBoneTransform(idx);
    }
}

const size_t MetroSkeleton::GetBoneParentIdx(const size_t idx) const {
    size_t result = MetroBone::InvalidIdx;

    const CharString& parentName = this->bones[idx].parent;
    for (size_t i = 0; i < this->bones.size(); ++i) {
        if (this->bones[i].name == parentName) {
            result = i;
            break;
        }
    }

    return result;
}

const CharString& MetroSkeleton::GetBoneName(const size_t idx) const {
    return this->bones[idx].name;
}

const CharString& MetroSkeleton::GetMotionsStr() const {
    return this->motions;
}


void MetroSkeleton::DeserializeSelf(MetroReflectionStream& reader) {
    MetroReflectionStream* skeletonReader = reader.OpenSection("skeleton");
    if (skeletonReader) {
        METRO_SERIALIZE_MEMBER(*skeletonReader, ver);
        METRO_SERIALIZE_MEMBER(*skeletonReader, crc);

        skeletonReader->SetUserData(this->ver);

        if (this->ver < 15) {
            METRO_SERIALIZE_MEMBER(*skeletonReader, facefx);
        } else {
            METRO_SERIALIZE_MEMBER(*skeletonReader, pfnn); // if version > 16
        }
        if (this->ver > 20) {
            METRO_SERIALIZE_MEMBER(*skeletonReader, has_as); // if version > 20
        }
        METRO_SERIALIZE_MEMBER(*skeletonReader, motions);
        if (this->ver > 12) {
            METRO_SERIALIZE_MEMBER(*skeletonReader, source_info); // if version > 12
            if (this->ver > 13) {
                METRO_SERIALIZE_MEMBER(*skeletonReader, parent_skeleton); // if version > 13
                METRO_SERIALIZE_STRUCT_ARRAY_MEMBER(*skeletonReader, parent_bone_maps); // if version > 13
            }
        }
        METRO_SERIALIZE_STRUCT_ARRAY_MEMBER(*skeletonReader, bones);

        reader.CloseSection(skeletonReader);
    }

    //#NOTE_SK: fix-up bones transforms by swizzling them back
    for (auto& b : bones) {
        b.q = MetroSwizzle(b.q);
        b.t = MetroSwizzle(b.t);
    }

    //#TODO_SK:
    // locators
    // aux_bones
    // procedural
}

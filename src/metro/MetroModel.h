#pragma once
#include "MetroTypes.h"

class MetroSkeleton;
class MetroMotion;

class MetroModel {
public:
    MetroModel();
    ~MetroModel();

    bool                    LoadFromData(MemStream& stream, const size_t fileIdx);
    bool                    SaveAsOBJ(const fs::path& filePath);
    bool                    SaveAsFBX(const fs::path& filePath, const bool withAnims);

    bool                    IsAnimated() const;
    const AABBox&           GetBBox() const;
    const vec4&             GetBSphere() const;
    size_t                  GetNumMeshes() const;
    const MetroMesh*        GetMesh(const size_t idx) const;

    const CharString&       GetSkeletonPath() const;
    const MetroSkeleton*    GetSkeleton() const;
    size_t                  GetNumMotions() const;
    const MetroMotion*      GetMotion(const size_t idx) const;

    const CharString&       GetComment() const;

private:
    void                    ReadSubChunks(MemStream& stream);
    void                    LoadLinkedMeshes(const StringArray& links);
    void                    LoadMotions();

private:
    AABBox                  mBBox;
    vec4                    mBSphere;
    MyArray<MetroMesh*>     mMeshes;
    CharString              mSkeletonPath;
    MetroSkeleton*          mSkeleton;
    MyArray<MetroMotion*>   mMotions;
    CharString              mComment;

    // these are temp pointers, invalid after loading
    MetroMesh*              mCurrentMesh;
    size_t                  mThisFileIdx;
};

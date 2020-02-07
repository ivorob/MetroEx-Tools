class MetroReflectionJsonWriteStream : public MetroReflectionStream {
public:
    MetroReflectionJsonWriteStream()
        : MetroReflectionStream()
        , mJson(nullptr)
        , mCurrentObj(nullptr)
    {
        mMode = MetroReflectionStream::Mode::OUT;

        Initialize();
    }

    virtual ~MetroReflectionJsonWriteStream() {
        Shutdown();
    }

    virtual bool Good() const override {
        return mCurrentObj != nullptr;
    }

    virtual size_t GetCursor() const override {
        return 0;
    }

    virtual size_t GetRemains() const override {
        return 0;
    }

    virtual void SkipBytes(const size_t) override {
    }

    // Serialization
    virtual void SerializeRawBytes(void*, const size_t) override {
    }

    // Specialize these
    virtual void SerializeBool(bool& v) override;
    virtual void SerializeI8(int8_t& v) override;
    virtual void SerializeU8(uint8_t& v) override;
    virtual void SerializeI16(int16_t& v) override;
    virtual void SerializeU16(uint16_t& v) override;
    virtual void SerializeI32(int32_t& v) override;
    virtual void SerializeU32(uint32_t& v) override;
    virtual void SerializeI64(int64_t& v) override;
    virtual void SerializeU64(uint64_t& v) override;
    virtual void SerializeF32(float& v) override;
    virtual void SerializeF64(double& v) override;
    virtual void SerializeString(CharString& v) override;
    // Advanced
    virtual void SerializeFloats(float* fv, const size_t numFloats) override;

    // Need to override this as we just need the name, not a type
    virtual bool SerializeTypeInfo(const CharString& propName, const CharString& typeAlias) override;
    virtual MetroReflectionStream* OpenSection(const CharString& sectionName, const bool nameUnknown = false) override;
    virtual void CloseSection(MetroReflectionStream* section) override;

    // Json specific
    CharString WriteToString() const;

protected:
    virtual void ReadStringZ(CharString&) override {

    }
    virtual void WriteStringZ(CharString&) override {

    }

private:
    void    Initialize();
    void    Shutdown();

private:
    struct json_t*          mJson;
    struct json_t*          mCurrentObj;
    MyDeque<struct json_t*> mTopObjectsStack;
    CharString              mCurrentMemberName;
};


// READ

class MetroReflectionJsonReadStream : public MetroReflectionStream {
public:
    MetroReflectionJsonReadStream(const CharString& jsonStr)
        : MetroReflectionStream()
        , mJson(nullptr)
        , mCurrentObj(nullptr)
        , mCurrentIter(nullptr) {
        mMode = MetroReflectionStream::Mode::IN;

        Initialize(jsonStr);
    }

    virtual ~MetroReflectionJsonReadStream() {
        Shutdown();
    }

    virtual bool Good() const override {
        return mCurrentIter != nullptr;
    }

    virtual size_t GetCursor() const override {
        return 0;
    }

    virtual size_t GetRemains() const override {
        return 0;
    }

    virtual void SkipBytes(const size_t) override {
    }

    // Serialization
    virtual void SerializeRawBytes(void*, const size_t) override {
    }

    // Specialize these
    virtual void SerializeBool(bool& v) override;
    virtual void SerializeI8(int8_t& v) override;
    virtual void SerializeU8(uint8_t& v) override;
    virtual void SerializeI16(int16_t& v) override;
    virtual void SerializeU16(uint16_t& v) override;
    virtual void SerializeI32(int32_t& v) override;
    virtual void SerializeU32(uint32_t& v) override;
    virtual void SerializeI64(int64_t& v) override;
    virtual void SerializeU64(uint64_t& v) override;
    virtual void SerializeF32(float& v) override;
    virtual void SerializeF64(double& v) override;
    virtual void SerializeString(CharString& v) override;
    // Advanced
    virtual void SerializeFloats(float* fv, const size_t numFloats) override;

    // Need to override this as we just need the name, not a type
    virtual bool SerializeTypeInfo(const CharString& propName, const CharString& typeAlias) override;
    virtual MetroReflectionStream* OpenSection(const CharString& sectionName, const bool nameUnknown = false) override;
    virtual void CloseSection(MetroReflectionStream* section) override;

protected:
    virtual void ReadStringZ(CharString&) override {

    }
    virtual void WriteStringZ(CharString&) override {

    }

private:
    void    Initialize(const CharString& jsonStr);
    void    Shutdown();
    void    Iterate();

private:
    struct ObjIter {
        struct json_t*  j;
        void*           i;
    };

    struct json_t*      mJson;
    struct json_t*      mCurrentObj;
    void*               mCurrentIter;
    MyDeque<ObjIter>    mTopObjectsStack;
    CharString          mCurrentMemberName;
};


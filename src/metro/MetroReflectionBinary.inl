class MetroReflectionBinaryReadStream : public MetroReflectionStream {
public:
    MetroReflectionBinaryReadStream()
        : MetroReflectionStream()
    {
        mMode = MetroReflectionStream::Mode::IN;
    }

    MetroReflectionBinaryReadStream(const MemStream& s, const uint8_t flags = MetroReflectionFlags::None)
        : MetroReflectionStream()
        , mStream(s)
    {
        mMode = MetroReflectionStream::Mode::IN;
        mFlags = flags;
    }

    virtual ~MetroReflectionBinaryReadStream() {
    }

    virtual bool Good() const override {
        return mStream.Good();
    }

    virtual size_t GetCursor() const override {
        return mStream.GetCursor();
    }

    virtual size_t GetRemains() const override {
        return mStream.Remains();
    }

    virtual void SkipBytes(const size_t numBytes) override {
        mStream.SkipBytes(numBytes);
    }

    // Serialization
    virtual void SerializeRawBytes(void* ptr, const size_t length) override {
        mStream.ReadToBuffer(ptr, length);
    }

    // Metro data serialization details
    virtual MetroReflectionStream* OpenSection(const CharString& sectionName, const bool nameUnknown = false) override {
        if (this->HasNoSections()) {
            return this;
        } else {
            const uint32_t crc = sectionName.empty() ? 0u : Hash_CalculateCRC32(sectionName);

            const uint32_t sectionNameCrc = *rcast<const uint32_t*>(mStream.GetDataAtCursor());
            if (nameUnknown || sectionNameCrc == crc) {
                mStream.SkipBytes(sizeof(uint32_t));

                uint32_t sectionSize = mStream.ReadTyped<uint32_t>();
                uint8_t flags = mStream.ReadTyped<uint8_t>();

                const size_t dataSize = sectionSize - 1;
                MetroReflectionBinaryReadStream* result = new MetroReflectionBinaryReadStream(mStream.Substream(dataSize), flags);
                result->SetSTable(mSTable);
                result->SetUserData(mUserData);

                if (result->HasDebugInfo()) {
                    CharString name;
                    result->ReadStringZ(name);
                    if (!nameUnknown) {
                        assert(sectionName == name);
                    }
                }

                return result;
            } else {
                return nullptr;
            }
        }
    }

    virtual void CloseSection(MetroReflectionStream* section) override {
        if (section) {
            if (section == this) {  // this->HasNoSections()
                //#NOTE_SK: since it's our shadow copy, we just do nothing
            } else {
                MetroReflectionBinaryReadStream* binR = scast<MetroReflectionBinaryReadStream*>(section);
                mStream.SkipBytes(binR->mStream.Length());
                delete section;
            }
        }
    }

protected:
    virtual void ReadStringZ(CharString& s) override {
        s = mStream.ReadStringZ();
    }

    virtual void WriteStringZ(CharString&) override {
        // nothing here
    }

private:
    MemStream   mStream;
};


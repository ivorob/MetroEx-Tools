#include "MetroBinArchive.h"
#include "reflection/MetroReflection.h"

MetroBinArchive::MetroBinArchive(const CharString& name, const MemStream& stream, const size_t headerSize) : mOutputStream(nullptr) {
    // Get raw memory stream
    mFileName = name;
    mFileStream = stream;

    // Search for header
    mHeaderSize = 0;

    if (headerSize != kHeaderNotExist) {
        if (headerSize == kHeaderDoAutoSearch) {
            // Try to autosearch first chunk location, to get flags and header size
            // TODO: tricky and unreliable method, may fail
            size_t firstChunkPos = 0;

            // Try to search for .bin flags
            size_t _maxOffsetToSeek = 0x10;

            for (size_t offs = 0; offs <= _maxOffsetToSeek; offs++) {
                mFileStream.SetCursor(offs);
                uint32_t value = mFileStream.ReadTyped<uint32_t>();
                if (value == 1) { // looking for chunkIdx == 1
                    firstChunkPos = offs;
                    break;
                }
            }

            // Restore cursor position
            mFileStream.SetCursor(0);

            // Set header size
            mHeaderSize = firstChunkPos - 0x1 /*bin flags*/;
        } else {
            mHeaderSize = headerSize;
        }
    }

    // Read .bin flags
    mFileStream.SetCursor(GetOffsetBinFlags());
    mBinFlags = mFileStream.ReadTyped<uint8_t>();

    // Read chunks
    if (TestBit(mBinFlags, MetroReflectionFlags::StringsTable)) {
        while (!mFileStream.Ended()) {
            const size_t chunkId = mFileStream.ReadTyped<uint32_t>();
            const size_t chunkSize = mFileStream.ReadTyped<uint32_t>();
            mChunks.push_back({
                chunkId,
                mFileStream.GetCursor(),
                chunkSize
            });

            mFileStream.SkipBytes(chunkSize);
        }

        // read strings table chunk
        if (this->GetNumChunks() == 2) {
            this->ReadStringsTable();
        }
    }

    // Restore cursor position
    mFileStream.SetCursor(0);
}

//#NOTE_SK: this constructor creates a new Metro bin container from scratch
//          using MemWriteStream as the output stream
MetroBinArchive::MetroBinArchive(MemWriteStream& outStream) {
    mBinFlags = static_cast<uint8_t>(MetroReflectionFlags::DefaultOutFlags);
    outStream.Write(mBinFlags);

    if (TestBit(mBinFlags, MetroReflectionFlags::StringsTable)) {
        outStream.Write<uint32_t>(1);   // data chunk
        mOutputDataChunkSizeOffset = outStream.GetWrittenBytesCount();
        outStream.Write<uint32_t>(0);   // chunk size stub
    } else {
        mOutputDataChunkSizeOffset = kInvalidValue;
    }

    mOutputStream = &outStream;
}

StrongPtr<MetroReflectionStream> MetroBinArchive::ReflectionReader() const {
    StrongPtr<MetroReflectionStream> result;

    if (this->HasChunks()) {
        const ChunkData& chunk = this->GetFirstChunk();

        result.reset(new MetroReflectionBinaryReadStream(mFileStream.Substream(chunk.offset, chunk.size), mBinFlags));
    } else {
        result.reset(new MetroReflectionBinaryReadStream(mFileStream.Substream(1, mFileStream.Length() - 1), mBinFlags));
    }

    if (!mSTable.data.empty()) {
        result->SetSTable(const_cast<StringsTable*>(&mSTable)); // :(
    }

    return result;
}

void MetroBinArchive::Finalize() {
    if (TestBit(mBinFlags, MetroReflectionFlags::StringsTable)) {
        if (mOutputDataChunkSizeOffset != kInvalidValue) {
            // go back to the data chunk size offset and fill it
            const size_t chunkSize = mOutputStream->GetWrittenBytesCount() - mOutputDataChunkSizeOffset - sizeof(uint32_t);
            uint8_t* data = rcast<uint8_t*>(mOutputStream->Data());
            *rcast<uint32_t*>(data + mOutputDataChunkSizeOffset) = scast<uint32_t>(chunkSize);
        }

        this->WriteStringsTable();
    }
}



void MetroBinArchive::ReadStringsTable() {
    const ChunkData& stableChunk = this->GetLastChunk();

    MemStream stream = mFileStream.Substream(stableChunk.offset, stableChunk.size);
    const size_t numStrings = stream.ReadTyped<uint32_t>();

    const size_t dataSize = stream.Remains();
    mSTable.data.resize(dataSize);
    stream.ReadToBuffer(mSTable.data.data(), dataSize);
    mSTable.strings.resize(numStrings);

    const char* s = mSTable.data.data();
    for (size_t i = 0; i < numStrings; ++i) {
        mSTable.strings[i] = s;
        while (*s++);
    }
}

void MetroBinArchive::WriteStringsTable() {
    if (mOutputStream) {
        mOutputStream->Write<uint32_t>(2);  // chunk id
        const size_t chunkSizeOffset = mOutputStream->GetWrittenBytesCount();
        mOutputStream->Write<uint32_t>(0);  // chunk size will be written later
        mOutputStream->Write(scast<uint32_t>(mSTable.stringsAdded.size())); // strings count

        for (const HashString& hs : mSTable.stringsAdded) {
            if (hs.str.empty()) {
                mOutputStream->WriteDupByte(0, 1);  // empty string, just terminating zero written
            } else {
                mOutputStream->Write(hs.str.data(), hs.str.length() + 1);
            }
        }

        // go back to the chunk size offset and fill it
        const size_t chunkSize = mOutputStream->GetWrittenBytesCount() - chunkSizeOffset - sizeof(uint32_t);
        uint8_t* data = rcast<uint8_t*>(mOutputStream->Data());
        *rcast<uint32_t*>(data + chunkSizeOffset) = scast<uint32_t>(chunkSize);
    }
}

#include "CreateArchiveDlgImpl.h"
#include "metro/MetroCompression.h"
#include "metro/VFXReader.h"

#include <fstream>

#include "ui/ChooseFolderDlg.h"
#include "ui/UIHelpers.h"

namespace MetroEX {

CreateArchiveDlgImpl::CreateArchiveDlgImpl() {
}
CreateArchiveDlgImpl::~CreateArchiveDlgImpl() {
}

// text fields
void CreateArchiveDlgImpl::OnSourceFolderChanged() {
    this->CheckAndUnlockCreateBtn();
}

void CreateArchiveDlgImpl::OnTargetPathChanged() {
    this->CheckAndUnlockCreateBtn();
}

void CreateArchiveDlgImpl::OnBlobFilenameChanged() {
    this->CheckAndUnlockCreateBtn();
}

// buttons
void CreateArchiveDlgImpl::OnChooseSrcFolderClick() {
    fs::path folderPath = ChooseFolderDialog::ChooseFolder(L"Choose content foder...", this->Handle.ToPointer());
    if (!folderPath.empty()) {
        std::error_code err;
        fs::directory_entry sourceFolder(folderPath, err);
        if (sourceFolder.exists() && sourceFolder.is_directory() && folderPath.stem() == L"content") {
            this->txtSourceFolder->Text = PathToString(folderPath);
        } else {
            MetroEX::ShowErrorMessageBox(this, L"Please choose directory named \"content\" !");
        }
    }
}

void CreateArchiveDlgImpl::OnChooseTargetClick() {
    if (this->radioCreateNewArchive->Checked) {
        System::Windows::Forms::SaveFileDialog sfd;
        sfd.Title = L"Save vfx archive...";
        sfd.Filter = L"Archive description (*.vfx)|*.vfx";
        sfd.FileName = L"patch_00.vfx";
        sfd.FilterIndex = 0;
        sfd.RestoreDirectory = true;
        sfd.OverwritePrompt = true;
        if (sfd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            this->txtTargetPath->Text = sfd.FileName;
        }
    } else {
        System::Windows::Forms::OpenFileDialog ofd;
        ofd.Title = L"Choose vfx archive to modify...";
        ofd.Filter = L"Archive description (*.vfx)|*.vfx";
        ofd.FilterIndex = 0;
        ofd.CheckFileExists = true;
        ofd.RestoreDirectory = true;
        if (ofd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            this->txtTargetPath->Text = ofd.FileName;
        }
    }
}

void CreateArchiveDlgImpl::OnCreateArchiveClick() {
    fs::path vfxPath = StringToPath(this->txtTargetPath->Text);
    fs::path vfsName = vfxPath.stem().native() + L".vfs0";
    if (this->radioModifyArchive->Checked) {
        vfsName = StringToPath(this->txtBlobFileName->Text);
    }
    fs::path vfsPath = vfxPath.parent_path() / vfsName;

    fs::path sourcePath = StringToPath(this->txtSourceFolder->Text);

    MyArray<DirEntry> allFiles;
    this->CollectFilesAndFolders(sourcePath, allFiles);
    bool success = this->WriteFilesBlob(vfsPath, allFiles);

    if (success) {
        if (this->radioCreateNewArchive->Checked) {
            success = this->WriteArchiveDescriptionFile(vfxPath, vfsName.u8string(), allFiles);
        } else {
            success = this->ModifyArchiveDescriptionFile(vfxPath, vfsName.u8string(), allFiles);
        }
    }

    if (!success) {
        MetroEX::ShowErrorMessageBox(this, L"Operation failed :(");
    } else {
        MetroEX::ShowInfoMessageBox(this, L"Operation succeeded :)");
    }
}

void CreateArchiveDlgImpl::OnRadioButtonsCheckedChanged() {
    if (this->radioCreateNewArchive->Checked) {
        this->txtBlobFileName->Enabled = false;
        this->groupBox2->Enabled = true;
    } else {
        this->txtBlobFileName->Enabled = true;
        this->groupBox2->Enabled = false;
    }
}



void CreateArchiveDlgImpl::CheckAndUnlockCreateBtn() {
    std::error_code err;
    fs::directory_entry sourceFolder(StringToPath(this->txtSourceFolder->Text), err);
    const bool sourceValid = sourceFolder.exists();

    fs::path targetPath = StringToPath(this->txtTargetPath->Text);

    bool targetValid = false;
    if (this->radioCreateNewArchive->Checked) {
        targetValid = !targetPath.empty() && targetPath.has_extension() && (targetPath.extension() == L".vfx");
    } else {
        fs::directory_entry targetArchive(targetPath, err);
        targetValid = targetArchive.exists() && targetPath.extension() == L".vfx";
        targetValid = targetValid && (this->txtBlobFileName->Text->Length > 5);
    }

    this->btnCreateArchive->Enabled = sourceValid && targetValid;
}

#pragma region Helper functions

MyArray<fs::directory_entry> CollectDirectoryContent(const fs::path dirPath) {
    MyArray<fs::directory_entry> result;

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        result.push_back(entry);
    }

    return std::move(result);
}

MyArray<DirEntry> CollectDirectoryRecursive(DirEntry& entry, size_t& nextIdx) {
    MyArray<DirEntry> result;

    MyArray<fs::directory_entry> content = CollectDirectoryContent(entry.path);

    entry.file.numFiles = content.size();
    entry.file.firstFile = nextIdx;

    // we first collect all files in this directory
    for (const auto& e : content) {
        if (e.is_regular_file()) {
            DirEntry fileEntry;
            fileEntry.path = e.path();
            fileEntry.file = {};
            fileEntry.file.idx = nextIdx++;
            fileEntry.file.name = fileEntry.path.filename().u8string();

            result.emplace_back(fileEntry);
        }
    }

    size_t startFolder = result.size();
    size_t numFolders = 0;

    // then we collect all subfolders
    for (const auto& e : content) {
        if (e.is_directory()) {
            DirEntry dirEntry;
            dirEntry.path = e.path();
            dirEntry.file = {};
            dirEntry.file.flags = MetroFile::Flag_Folder | MetroFile::Flag_Unknown4;
            dirEntry.file.idx = nextIdx++;
            dirEntry.file.name = dirEntry.path.filename().u8string();

            result.emplace_back(dirEntry);
            ++numFolders;
        }
    }

    for (size_t i = 0; i < numFolders; ++i) {
        MyArray<DirEntry> collected = CollectDirectoryRecursive(result[startFolder + i], nextIdx);
        result.insert(result.end(), collected.begin(), collected.end());
    }

    return std::move(result);
}

void CopyFileContent(std::ifstream& src, std::ofstream& dst) {
#if 0
    std::copy(std::istreambuf_iterator<char>(src), {}, std::ostreambuf_iterator<char>(dst));
#else
    const size_t kBufferSize = 64 * 1024;
    char buffer[kBufferSize];

    while (!src.eof()) {
        src.read(buffer, kBufferSize);
        const size_t bytesRead = src.gcount();
        dst.write(buffer, bytesRead);
    }
#endif
}

void WriteStringZ(std::ofstream& stream, const CharString& str) {
    stream.write(str.data(), str.length() + 1);
}

void WriteStringXored(std::ofstream& stream, const CharString& str) {
    static const char sEmpty[3] = { 1, 0, 0 };
    if (str.empty()) {
        stream.write(sEmpty, sizeof(sEmpty));
    } else {
        BytesArray temp(str.length() + 1);
        memcpy(temp.data(), str.data(), temp.size());

        const uint8_t xorMask = scast<uint8_t>(rand() % 235) + 15;
        for (size_t i = 0, end = temp.size() - 1; i < end; ++i) {
            temp[i] ^= xorMask;
        }

        const uint16_t header = (scast<uint16_t>(xorMask) << 8) | scast<uint16_t>(temp.size() & 0xFF);
        stream.write(rcast<const char*>(&header), sizeof(header));
        stream.write(rcast<const char*>(temp.data()), temp.size());
    }
}

void WriteU16(std::ofstream& stream, const uint16_t value) {
    stream.write(rcast<const char*>(&value), sizeof(value));
}

void WriteU32(std::ofstream& stream, const uint32_t value) {
    stream.write(rcast<const char*>(&value), sizeof(value));
}

#pragma endregion

void CreateArchiveDlgImpl::CollectFilesAndFolders(const fs::path& path, MyArray<DirEntry>& result) {
    DirEntry content;
    content.path = path;
    content.file = {};
    content.file.flags = MetroFile::Flag_Folder;
    content.file.idx = 0;
    content.file.name = "content";

    size_t nextIdx = 1;

    result = CollectDirectoryRecursive(content, nextIdx);
    if (!result.empty()) {
        result.insert(result.begin(), content);
    }
}

bool CreateArchiveDlgImpl::WriteFilesBlob(const fs::path& path, MyArray<DirEntry>& files) {
    bool result = false;

    std::ofstream file(path, std::ofstream::binary);
    if (file.good()) {
        size_t vfsOffset = 0;
        for (auto& e : files) {
            if (e.file.IsFile()) {
                e.file.offset = vfsOffset;

                std::ifstream srcFile(e.path, std::ofstream::binary | std::ofstream::ate);
                if (srcFile.good()) {
                    const size_t fileSize = srcFile.tellg();
                    srcFile.seekg(0, std::ios::beg);

                    //#NOTE_SK: no compression atm
                    //#TODO_SK: add compression based on filters
                    CopyFileContent(srcFile, file);

                    e.file.sizeCompressed = fileSize;
                    e.file.sizeUncompressed = fileSize;

                    vfsOffset += fileSize;
                }
            }
        }

        file.flush();
        file.close();

        result = true;
    }

    return result;
}

bool CreateArchiveDlgImpl::WriteArchiveDescriptionFile(const fs::path& path, const CharString& vfsName, const MyArray<DirEntry>& files) {
    bool result = false;

    const bool isRedux = this->radioArchiveTypeRedux->Checked;
    const size_t vfxVersion = isRedux ? VFXReader::kVFXVersion2033Redux : VFXReader::kVFXVersionExodus;

    const CharString contentVersion = "492798"; // "491177"
    const MetroGuid guidRedux = { 0x3CC39580, 0x3AD4, 0x4007, 0xEDBF, { 0xC1, 0xB5, 0xE4, 0x4D, 0x20, 0xCB } };
    const MetroGuid guidExodus = { 0x9FE25B12, 0xF276, 0x40F4, 0xEAB8, { 0x0F, 0xE1, 0xA4, 0xC6, 0x9E, 0x7A } };
    size_t numFiles = files.size();

    std::ofstream vfxFile(path, std::ofstream::binary);
    if (vfxFile.good()) {
        // write header
        WriteU32(vfxFile, scast<uint32_t>(vfxVersion));
        WriteU32(vfxFile, scast<uint32_t>(MetroCompression::Type_LZ4));
        if (vfxVersion >= VFXReader::kVFXVersionExodus) {
            WriteStringZ(vfxFile, contentVersion);
            vfxFile.write(rcast<const char*>(&guidExodus), sizeof(guidExodus));
        } else {
            vfxFile.write(rcast<const char*>(&guidRedux), sizeof(guidRedux));
        }
        WriteU32(vfxFile, 1);                           // num packages
        WriteU32(vfxFile, scast<uint32_t>(numFiles));   // num files
        WriteU32(vfxFile, 0);                           // duplicates

        // write package
        WriteStringZ(vfxFile, vfsName);
        WriteU32(vfxFile, 0);                           // num levels
        WriteU32(vfxFile, 2);                           // chunk

        size_t vfsOffset = 0;
        for (auto& e : files) {
            const MetroFile& mf = e.file;

            WriteU16(vfxFile, scast<uint16_t>(mf.flags));
            if (mf.IsFile()) {
                WriteU16(vfxFile, scast<uint16_t>(mf.pakIdx));
                WriteU32(vfxFile, scast<uint32_t>(mf.offset));
                WriteU32(vfxFile, scast<uint32_t>(mf.sizeUncompressed));
                WriteU32(vfxFile, scast<uint32_t>(mf.sizeCompressed));
            } else {
                WriteU16(vfxFile, scast<uint16_t>(mf.numFiles));
                WriteU32(vfxFile, scast<uint32_t>(mf.firstFile));
            }
            WriteStringXored(vfxFile, mf.name);
        }
        WriteU32(vfxFile, 0);
        WriteU32(vfxFile, 0);

        vfxFile.flush();
        vfxFile.close();

        result = true;
    }

    return result;
}

struct FileFinderByName {
    CharString nameToFind;

    bool operator()(const MetroFile& f) {
        return f.name == nameToFind;
    }
};

bool CreateArchiveDlgImpl::ModifyArchiveDescriptionFile(const fs::path& path, const CharString& vfsName, const MyArray<DirEntry>& files) {
    bool result = false;

    LogPrint(LogLevel::Info, "Trying to modify " + path.u8string());

    VFXReader vfx;
    if (vfx.LoadFromFile(path)) {
        auto vfxPaks = vfx.GetAllPacks();
        auto vfxFiles = vfx.GetAllFiles();

        const size_t newPakIdx = vfxPaks.size();

        size_t numReplacedFiles = 0;
        for (const DirEntry& e : files) {
            if (e.file.IsFile()) {
                FileFinderByName finder = { e.file.name };
                auto it = std::find_if(vfxFiles.begin(), vfxFiles.end(), finder);
                if (it != vfxFiles.end()) {
                    const size_t idx = std::distance(vfxFiles.begin(), it);

                    MetroFile mf = vfx.GetFile(idx);
                    mf.pakIdx = newPakIdx;
                    mf.offset = e.file.offset;
                    mf.sizeCompressed = e.file.sizeCompressed;
                    mf.sizeUncompressed = e.file.sizeUncompressed;
                    vfx.ReplaceFileInfo(idx, mf);

                    ++numReplacedFiles;

                    LogPrintF(LogLevel::Info, "Modifying file %s at offset %zu", mf.name.c_str(), mf.offset);
                }
            }
        }

        if (numReplacedFiles) {
            const size_t nextChunk = vfxPaks.back().chunk + 1;

            LogPrintF(LogLevel::Info, "Adding new package %s with chunk = %zu", vfsName.c_str(), nextChunk);

            Package newPak;
            newPak.name = vfsName;
            newPak.chunk = nextChunk;
            vfx.AddPackage(newPak);

            if (vfx.SaveToFile(path)) {
                LogPrintF(LogLevel::Info, "Replaced %zu files", numReplacedFiles);
                LogPrint(LogLevel::Info, "Modification completed succesfully");

                result = true;
            } else {
                LogPrint(LogLevel::Error, "Failed to overwrite " + path.u8string());
            }
        } else {
            LogPrint(LogLevel::Error, "Couldn't find any file (none matched), modification failed");
        }
    } else {
        LogPrint(LogLevel::Error, "Failed to open vfx file");
    }

    return result;
}

} // namespace MetroEX

#include "mycommon.h"
#include <fstream>

#include "DlgCreateArchive.h"
#include "ui/ChooseFolderDlg.h"
#include "ui/UIHelpers.h"

namespace MetroEX {

    void DlgCreateArchive::txtSourceFolder_TextChanged(System::Object^, System::EventArgs^) {
        this->CheckAndUnlockCreateBtn();
    }

    void DlgCreateArchive::txtTargetPath_TextChanged(System::Object^, System::EventArgs^) {
        this->CheckAndUnlockCreateBtn();
    }

    void DlgCreateArchive::btnChooseSrcFolder_Click(System::Object^ sender, System::EventArgs^ e) {
        fs::path folderPath = ChooseFolderDialog::ChooseFolder("Choose content foder...", this->Handle.ToPointer());
        if (!folderPath.empty()) {
            std::error_code err;
            fs::directory_entry sourceFolder(folderPath, err);
            if (sourceFolder.exists() && sourceFolder.is_directory() && folderPath.stem() == L"content") {
                this->txtSourceFolder->Text = PathToString(folderPath);
            } else {
                System::Windows::Forms::MessageBoxButtons buttons = System::Windows::Forms::MessageBoxButtons::OK;
                System::Windows::Forms::MessageBoxIcon mbicon = System::Windows::Forms::MessageBoxIcon::Error;
                System::Windows::Forms::MessageBox::Show(L"Please choose directory named \"content\"", this->Text, buttons, mbicon);
            }
        }
    }

    void DlgCreateArchive::btnChooseTarget_Click(System::Object^ sender, System::EventArgs^ e) {
        if (this->radioCreateNewArchive->Checked) {
            SaveFileDialog sfd;
            sfd.Title = L"Save vfx archive...";
            sfd.Filter = L"Archive description (*.vfx)|*.vfx";
            sfd.FilterIndex = 0;
            sfd.RestoreDirectory = true;
            sfd.OverwritePrompt = true;
            if (sfd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
                this->txtTargetPath->Text = sfd.FileName;
            }
        } else {
            OpenFileDialog ofd;
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

    void DlgCreateArchive::btnCreateArchive_Click(System::Object^ sender, System::EventArgs^ e) {
        fs::path vfxPath = StringToPath(this->txtTargetPath->Text);
        fs::path vfsName = vfxPath.stem().native() + L".vfs0";
        if (this->radioExtendArchive->Checked) {
            vfsName = StringToPath(this->txtBlobFileName->Text);
        }
        fs::path vfsPath = vfxPath.parent_path() / vfsName;

        fs::path sourcePath = StringToPath(this->txtSourceFolder->Text);

        MyArray<DirEntry> allFiles;
        this->CollectFilesAndFolders(sourcePath, allFiles);
        this->WriteFilesBlob(vfsPath, allFiles);
    }

    void DlgCreateArchive::CheckAndUnlockCreateBtn() {
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

#pragma region Helper functios

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

#pragma endregion

    void DlgCreateArchive::CollectFilesAndFolders(const fs::path& path, MyArray<DirEntry>& result) {
        DirEntry content;
        content.path = path;
        content.file = {};
        content.file.flags = MetroFile::Flag_Folder;
        content.file.idx = 0;
        content.file.name = "content";

        size_t nextIdx = 1;

        result = CollectDirectoryRecursive(content, nextIdx);
    }

    bool DlgCreateArchive::WriteFilesBlob(const fs::path& path, MyArray<DirEntry>& files) {
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

                        std::copy(std::istreambuf_iterator<char>(srcFile), {}, std::ostreambuf_iterator<char>(file));

                        e.file.sizeCompressed = fileSize;
                        e.file.sizeUncompressed = fileSize;

                        vfsOffset += fileSize;
                    }
                }
            }

            result = true;
        }

        return result;
    }

} // namespace MetroEX


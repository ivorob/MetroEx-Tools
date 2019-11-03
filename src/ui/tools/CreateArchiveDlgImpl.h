#pragma once
#include "metro/MetroTypes.h"

namespace MetroEX {
    struct DirEntry {
        fs::path    path;
        MetroFile   file;
    };

    public ref class CreateArchiveDlgImpl : public MetroEXControls::CreateArchiveDlg {
    public:
        CreateArchiveDlgImpl();
        ~CreateArchiveDlgImpl();

    protected:
        // text fields
        virtual void OnSourceFolderChanged() override;
        virtual void OnTargetPathChanged() override;
        virtual void OnBlobFilenameChanged() override;
        // buttons
        virtual void OnChooseSrcFolderClick() override;
        virtual void OnChooseTargetClick() override;
        virtual void OnCreateArchiveClick() override;
        virtual void OnRadioButtonsCheckedChanged() override;

    private:
        void CheckAndUnlockCreateBtn();

        void CollectFilesAndFolders(const fs::path& path, MyArray<DirEntry>& result);
        bool WriteFilesBlob(const fs::path& path, MyArray<DirEntry>& files);
        bool WriteArchiveDescriptionFile(const fs::path& path, const CharString& vfsName, const MyArray<DirEntry>& files);
        bool ModifyArchiveDescriptionFile(const fs::path& path, const CharString& vfsName, const MyArray<DirEntry>& files);
    };

} // namespace MetroEX

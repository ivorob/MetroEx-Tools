#include "mycommon.h"
#include "metro/MetroTexture.h"

#include <queue>

#include "DlgConvertTextures.h"
#include "ui/ChooseFolderDlg.h"

#include "ui/UIHelpers.h"

namespace MetroEX {
    delegate void SafeCallDelegate();
    delegate void SafeCallDelegate1(System::Object^);

    void DlgConvertTextures::txtPath_TextChanged(System::Object^, System::EventArgs^) {
        std::error_code err;
        fs::directory_entry entry(StringToPath(this->txtPath->Text), err);
        this->btnConvert->Enabled = entry.exists() ? true : false;
    }

    void DlgConvertTextures::btnChooseFile_Click(System::Object^, System::EventArgs^) {
        OpenFileDialog ofd;
        ofd.Title = L"Open image file...";
        ofd.Filter = L"Image files (*.tga;*.png)|*.tga;*.png";
        ofd.FilterIndex = 0;
        ofd.RestoreDirectory = true;
        if (ofd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            this->txtPath->Text = ofd.FileName;
        }
    }

    void DlgConvertTextures::btnChooseFolder_Click(System::Object^, System::EventArgs^) {
        fs::path folderPath = ChooseFolderDialog::ChooseFolder("Choose textures foder...", this->Handle.ToPointer());
        if (!folderPath.empty()) {
            this->txtPath->Text = PathToString(folderPath);
        }
    }

    void DlgConvertTextures::btnConvert_Click(System::Object^ sender, System::EventArgs^ e) {
        if (mConversionInProgress) {
            return;
        }

        std::error_code err;
        fs::path path = StringToPath(this->txtPath->Text);
        fs::directory_entry entry(path, err);
        if (entry.exists()) {
            if (entry.is_regular_file()) {
                if (!this->ConvertOneFile(path)) {
                    System::Windows::Forms::MessageBox::Show(this, L"Failed to convert:\r\n" + PathToString(path), this->Text, MessageBoxButtons::OK, MessageBoxIcon::Error);
                } else {
                    System::Windows::Forms::MessageBox::Show(this, L"Successfully converted:\r\n" + PathToString(path), this->Text, MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
            } else {
                mIncludeSubfolders = this->chkWithSubfolders->Checked;
                mConversionThread = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &DlgConvertTextures::ConversionThreadFunc));
                mConversionThread->Start(PathToString(path));
            }
        }
    }

    void DlgConvertTextures::btnStop_Click(System::Object^, System::EventArgs^) {
        mStopRequested = true;
    }

    void DlgConvertTextures::DlgConvertTextures_FormClosing(System::Object^, System::Windows::Forms::FormClosingEventArgs^) {
        this->btnStop_Click(nullptr, nullptr);
    }


    // conversion
    bool DlgConvertTextures::ConvertOneFile(const fs::path& path) {
        bool result = false;

        this->Invoke(gcnew SafeCallDelegate1(this, &DlgConvertTextures::LogAction), L"Converting " + PathToString(path));

        MetroTexture texture;
        if (texture.LoadFromFile(path)) {
            fs::path resultPath = path.parent_path() / path.stem();
            if (texture.SaveAsMetroTexture(resultPath)) {
                result = true;
            } else {
                this->Invoke(gcnew SafeCallDelegate1(this, &DlgConvertTextures::LogAction), L"Failed to convert file!");
            }
        } else {
            this->Invoke(gcnew SafeCallDelegate1(this, &DlgConvertTextures::LogAction), L"Failed to load file!");
        }

        if (result) {
            this->Invoke(gcnew SafeCallDelegate1(this, &DlgConvertTextures::LogAction), L"Succeeded");
        }

        return result;
    }

    void DlgConvertTextures::ConvertFolder(const fs::path& path) {
        std::error_code err;

        std::queue<fs::directory_entry> folders;
        folders.push(fs::directory_entry(path, err));

        // collect all textures
        std::vector<fs::directory_entry> files;
        while (!folders.empty()) {
            const fs::directory_entry& folder = folders.front();

            if (folder.is_directory()) {
                for (const fs::directory_entry& subEntry : fs::directory_iterator(folder.path())) {
                    if (subEntry.is_regular_file()) {
                        const std::wstring extension = subEntry.path().extension();
                        if (extension == L".tga" || extension == L".png") {
                            files.push_back(subEntry);
                        }
                    } else if (mIncludeSubfolders && subEntry.is_directory()) {
                        folders.push(subEntry);
                    }
                }
            }

            folders.pop();
        }

        // convert all textures
        const size_t totalTextures = files.size();

        this->Invoke(gcnew SafeCallDelegate1(this, &DlgConvertTextures::InitProgressBar), scast<int>(totalTextures));

        for (const fs::directory_entry& file : files) {
            this->ConvertOneFile(file.path());

            this->Invoke(gcnew SafeCallDelegate(this, &DlgConvertTextures::StepProgressBar));

            if (mStopRequested) {
                break;
            }
        }
    }

    void DlgConvertTextures::ConversionThreadFunc(Object^ pathStr) {
        mStopRequested = false;
        mConversionInProgress = true;

        // block UI
        this->Invoke(gcnew SafeCallDelegate(this, &DlgConvertTextures::DisableUI));

        fs::path path = StringToPath(pathStr->ToString());
        this->ConvertFolder(path);

        // unblock UI
        this->Invoke(gcnew SafeCallDelegate(this, &DlgConvertTextures::EnableUI));

        mConversionInProgress = false;
    }

    // logging
    void DlgConvertTextures::LogAction(System::Object^ str) {
        this->txtLog->AppendText(safe_cast<String^>(str) + L"\r\n");
    }

    // threading stuff
    void DlgConvertTextures::DisableUI() {
        this->txtPath->Enabled = false;
        this->chkWithSubfolders->Enabled = false;
        this->btnChooseFile->Enabled = false;
        this->btnChooseFolder->Enabled = false;
        this->btnConvert->Enabled = false;
        this->btnStop->Enabled = true;
    }

    void DlgConvertTextures::EnableUI() {
        this->txtPath->Enabled = true;
        this->chkWithSubfolders->Enabled = true;
        this->btnChooseFile->Enabled = true;
        this->btnChooseFolder->Enabled = true;
        this->btnConvert->Enabled = true;
        this->btnStop->Enabled = false;
    }

    void DlgConvertTextures::InitProgressBar(System::Object^ obj) {
        int numFiles = safe_cast<int>(obj);

        this->prbProgress->Minimum = 0;
        this->prbProgress->Maximum = numFiles;
        this->prbProgress->Step = 1;
        this->prbProgress->Value = 0;
    }

    void DlgConvertTextures::StepProgressBar() {
        this->prbProgress->PerformStep();
    }

} // namespace MetroEX

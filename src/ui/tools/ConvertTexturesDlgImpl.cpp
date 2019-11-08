#include "ConvertTexturesDlgImpl.h"
#include "ui/ChooseFolderDlg.h"
#include "ui/UIHelpers.h"

#include "metro/MetroTexture.h"

#include <queue>


namespace MetroEX {
delegate void SafeCallDelegate();
delegate void SafeCallDelegate1(System::Object^);


ConvertTexturesDlgImpl::ConvertTexturesDlgImpl()
    : mConversionInProgress(false)
    , mStopRequested(false)
    , mConversionThread(nullptr)
    , mIncludeSubfolders(true)
    , mFormatExodus(false)
    , mCrunched(false)
{
}
ConvertTexturesDlgImpl::~ConvertTexturesDlgImpl() {
}

void ConvertTexturesDlgImpl::OnChooseFolderClicked() {
    fs::path folderPath = ChooseFolderDialog::ChooseFolder(L"Choose textures foder...", this->Handle.ToPointer());
    if (!folderPath.empty()) {
        this->txtPath->Text = PathToString(folderPath);
    }
}

void ConvertTexturesDlgImpl::OnConvertClicked() {
    if (mConversionInProgress) {
        return;
    }

    mFormatExodus = this->radioFormatExodus->Checked;
    mCrunched = mFormatExodus ? false : this->chkCrunched->Checked;

    std::error_code err;
    fs::path path = StringToPath(this->txtPath->Text);
    fs::directory_entry entry(path, err);
    if (entry.exists()) {
        if (entry.is_regular_file()) {
            if (!this->ConvertOneFile(path)) {
                ShowErrorMessageBox(this, L"Failed to convert:\r\n" + PathToString(path));
            } else {
                ShowInfoMessageBox(this, L"Successfully converted:\r\n" + PathToString(path));
            }
        } else {
            mIncludeSubfolders = this->chkWithSubfolders->Checked;
            mConversionThread = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &ConvertTexturesDlgImpl::ConversionThreadFunc));
            mConversionThread->Start(PathToString(path));
        }
    }
}

void ConvertTexturesDlgImpl::OnStopClicked() {
    mStopRequested = true;
}

// conversion
bool ConvertTexturesDlgImpl::ConvertOneFile(const fs::path& path) {
    bool result = false;

    this->Invoke(gcnew SafeCallDelegate1(this, &ConvertTexturesDlgImpl::LogAction), L"Converting " + PathToString(path));

    MetroTexture texture;
    if (texture.LoadFromFile(path)) {
        fs::path resultPath = path.parent_path() / path.stem();
        const MetroTexture::PixelFormat format = mFormatExodus ? MetroTexture::PixelFormat::BC7 : MetroTexture::PixelFormat::BC3;
        if (texture.SaveAsMetroTexture(resultPath, format, mCrunched)) {
            result = true;
        } else {
            this->Invoke(gcnew SafeCallDelegate1(this, &ConvertTexturesDlgImpl::LogAction), L"Failed to convert file!");
        }
    } else {
        this->Invoke(gcnew SafeCallDelegate1(this, &ConvertTexturesDlgImpl::LogAction), L"Failed to load file!");
    }

    if (result) {
        this->Invoke(gcnew SafeCallDelegate1(this, &ConvertTexturesDlgImpl::LogAction), L"Succeeded");
    }

    return result;
}

void ConvertTexturesDlgImpl::ConvertFolder(const fs::path& path) {
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

    this->Invoke(gcnew SafeCallDelegate1(this, &ConvertTexturesDlgImpl::InitProgressBar), scast<int>(totalTextures));

    for (const fs::directory_entry& file : files) {
        this->ConvertOneFile(file.path());

        this->Invoke(gcnew SafeCallDelegate(this, &ConvertTexturesDlgImpl::StepProgressBar));

        if (mStopRequested) {
            break;
        }
    }
}

void ConvertTexturesDlgImpl::ConversionThreadFunc(System::Object^ pathStr) {
    mStopRequested = false;
    mConversionInProgress = true;

    // block UI
    this->Invoke(gcnew SafeCallDelegate(this, &ConvertTexturesDlgImpl::DisableUI));

    fs::path path = StringToPath(pathStr->ToString());
    this->ConvertFolder(path);

    // unblock UI
    this->Invoke(gcnew SafeCallDelegate(this, &ConvertTexturesDlgImpl::EnableUI));

    mConversionInProgress = false;
}

// logging
void ConvertTexturesDlgImpl::LogAction(System::Object^ str) {
    this->txtLog->AppendText(safe_cast<System::String^>(str) + L"\r\n");
}

// threading stuff
void ConvertTexturesDlgImpl::DisableUI() {
    this->txtPath->Enabled = false;
    this->chkWithSubfolders->Enabled = false;
    this->btnChooseFile->Enabled = false;
    this->btnChooseFolder->Enabled = false;
    this->btnConvert->Enabled = false;
    this->btnStop->Enabled = true;
    this->groupBox1->Enabled = false;
}

void ConvertTexturesDlgImpl::EnableUI() {
    this->txtPath->Enabled = true;
    this->chkWithSubfolders->Enabled = true;
    this->btnChooseFile->Enabled = true;
    this->btnChooseFolder->Enabled = true;
    this->btnConvert->Enabled = true;
    this->btnStop->Enabled = false;
    this->groupBox1->Enabled = true;
}

void ConvertTexturesDlgImpl::InitProgressBar(System::Object^ obj) {
    int numFiles = safe_cast<int>(obj);

    this->prbProgress->Minimum = 0;
    this->prbProgress->Maximum = numFiles;
    this->prbProgress->Step = 1;
    this->prbProgress->Value = 0;
}

void ConvertTexturesDlgImpl::StepProgressBar() {
    this->prbProgress->PerformStep();
}

}

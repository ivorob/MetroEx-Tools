#include "metro/MetroDatabases.h"
#include "metro/MetroTexture.h"
#include "metro/MetroModel.h"
#include "metro/MetroSound.h"
#include "metro/MetroSkeleton.h"
#include "metro/MetroMotion.h"
#include "metro/MetroLocalization.h"

#include "mex_settings.h"

#include <fstream>

#include "ChooseFolderDlg.h"

#include "MainWindowImpl.h"

#include "TexturesDatabaseViewer.h"
#include "NodeSorter.h"
#include "SettingsDlgImpl.h"

#include "ui/tools/ConvertTexturesDlgImpl.h"
#include "ui/tools/CreateArchiveDlgImpl.h"
#include "ui/tools/FontsEditorImpl.h"

#include "UIHelpers.h"

namespace MetroEX {

enum class eNodeEventType : size_t {
    Default,
    Open,
    Close
};

static const int    kImageIdxFolderClosed = 0;
static const int    kImageIdxFolderOpen = 1;
static const int    kImageIdxFile = 2;
static const int    kImageIdxBinUnkn = 3;
static const int    kImageIdxBinArchive = 4;
static const int    kImageIdxBinEditable = 5;
static const int    kImageIdxTexture = 6;
static const int    kImageIdxMotion = 7;
static const int    kImageIdxSound = 8;
static const int    kImageIdxModel = 9;
static const int    kImageIdxLocalization = 10;

static const size_t kFileHandleMask = size_t(~0) >> 1;
static const size_t kFolderSortedFlag = size_t(1) << ((sizeof(size_t) * 8) - 1);

ref struct FileTagData {
    FileType    fileType;       // type of file
    MyHandle    fileHandle;     // file system file handle
    size_t      subFileIdx;     // index inside .bin database

    FileTagData(const FileType _fileType, MyHandle _file, const size_t _subFileIdx)
        : fileType(_fileType)
        , fileHandle(_file)
        , subFileIdx(_subFileIdx) {
    }
};

static FileType DetectFileType(const MyHandle file) {
    FileType result = FileType::Unknown;

    String^ name = ToNetString(MetroFileSystem::Get().GetName(file));

    if (name->EndsWith(L".dds") ||
        name->EndsWith(L".512") ||
        name->EndsWith(L".1024") ||
        name->EndsWith(L".2048")) {
        result = FileType::Texture;
    } else if (name->EndsWith(L".bin")) {
        result = FileType::Bin;
    } else if (name->EndsWith(L".model")) {
        result = FileType::Model;
    } else if (name->EndsWith(L".m2")) {
        result = FileType::Motion;
    } else if (name->EndsWith(L".vba")) {
        result = FileType::Sound;
    } else if (name->EndsWith(L"lightmaps")) {
        result = FileType::Level;
    } else if (name->EndsWith(L".lng")) {
        result = FileType::Localization;
    }

    return result;
}

static void UpdateNodeIcon(TreeNode^ Node, eNodeEventType eventType = eNodeEventType::Default) {
    FileTagData^ fileData = safe_cast<FileTagData^>(Node->Tag);
    FileType fileType = fileData->fileType;

    Node->ImageIndex = kImageIdxFile;
    Node->SelectedImageIndex = kImageIdxFile;

    switch (fileType) {
    case FileType::Unknown: {
    } break;

    case FileType::Folder:
    case FileType::FolderBin: {
        if (eventType == eNodeEventType::Open) {
            Node->ImageIndex = kImageIdxFolderOpen;
            Node->SelectedImageIndex = kImageIdxFolderOpen;
        } else {
            Node->ImageIndex = kImageIdxFolderClosed;
            Node->SelectedImageIndex = kImageIdxFolderClosed;
        }
    } break;

    case FileType::Bin: {
        Node->ImageIndex = kImageIdxBinUnkn;
        Node->SelectedImageIndex = kImageIdxBinUnkn;
    } break;

    case FileType::BinArchive: {
        Node->ImageIndex = kImageIdxBinArchive;
        Node->SelectedImageIndex = kImageIdxBinArchive;
    } break;

    case FileType::BinEditable: {
        Node->ImageIndex = kImageIdxBinEditable;
        Node->SelectedImageIndex = kImageIdxBinEditable;
    } break;

    case FileType::Model: {
        Node->ImageIndex = kImageIdxModel;
        Node->SelectedImageIndex = kImageIdxModel;
    } break;

    case FileType::Texture: {
        Node->ImageIndex = kImageIdxTexture;
        Node->SelectedImageIndex = kImageIdxTexture;
    } break;

    case FileType::Sound: {
        Node->ImageIndex = kImageIdxSound;
        Node->SelectedImageIndex = kImageIdxSound;
    } break;

    case FileType::Localization: {
        Node->ImageIndex = kImageIdxLocalization;
        Node->SelectedImageIndex = kImageIdxLocalization;
    } break;
    }
}



MainWindowImpl::MainWindowImpl() {
    mImagePanel = nullptr;
    mRenderPanel = nullptr;
    mSoundPanel = nullptr;
    mLocalizationPanel = nullptr;
    mDlgModelInfo = nullptr;

    mExtractionCtx = new FileExtractionCtx;
    mExtractionProgressDlg = nullptr;

    mOriginalRootNode = nullptr;
}

MainWindowImpl::~MainWindowImpl() {
    MySafeDelete(mExtractionCtx);
}

void MainWindowImpl::ResetTreeView() {
    if (this->filterableTreeView->TreeView == nullptr ||
        this->filterableTreeView->TreeView->Nodes->Count == 0 ||
        this->filterableTreeView->TreeView->Nodes[0] == this->mOriginalRootNode) {
        return;
    }

    this->filterableTreeView->TreeView->BeginUpdate();
    this->filterableTreeView->TreeView->Nodes->Clear();
    this->filterableTreeView->TreeView->Nodes->Add(this->mOriginalRootNode);
    this->filterableTreeView->TreeView->EndUpdate();

    this->filterableTreeView->FilterTextBox->Text = String::Empty;
}

bool MainWindowImpl::FindAndSelect(String^ text, array<String^>^ extensions) {
    return this->filterableTreeView->FindAndSelect(text, extensions);
}



void MainWindowImpl::OnFormLoad() {
//#ifdef _DEBUG
//        //#NOTE_SK: for debugging purposes we might want to extract raw files
//        this->ctxMenuExportModel->Items->Add(this->extractFileToolStripMenuItem);
//        this->ctxMenuExportModel->Size.Height += this->extractFileToolStripMenuItem->Size.Height;
//        this->ctxMenuExportTexture->Items->Add(this->extractFileToolStripMenuItem);
//        this->ctxMenuExportTexture->Size.Height += this->extractFileToolStripMenuItem->Size.Height;
//        this->ctxMenuExportSound->Items->Add(this->extractFileToolStripMenuItem);
//        this->ctxMenuExportSound->Size.Height += this->extractFileToolStripMenuItem->Size.Height;
//#endif

    mImagePanel = gcnew MetroEXControls::ImagePanel();
    this->pnlViewers->Controls->Add(mImagePanel);
    mImagePanel->Dock = System::Windows::Forms::DockStyle::Fill;
    mImagePanel->Location = System::Drawing::Point(0, 0);
    mImagePanel->Name = L"mImagePanel";
    mImagePanel->Size = System::Drawing::Size(528, 386);
    mImagePanel->AutoScroll = true;


    mSoundPanel = gcnew SoundPanel();
    this->pnlViewers->Controls->Add(mSoundPanel);
    mSoundPanel->Dock = System::Windows::Forms::DockStyle::Fill;
    mSoundPanel->Location = System::Drawing::Point(0, 0);
    mSoundPanel->Name = L"mSoundPanel";
    mSoundPanel->Size = System::Drawing::Size(528, 386);


    mLocalizationPanel = gcnew LocalizationPanel();
    this->pnlViewers->Controls->Add(mLocalizationPanel);
    mLocalizationPanel->Dock = System::Windows::Forms::DockStyle::Fill;
    mLocalizationPanel->Location = System::Drawing::Point(0, 0);
    mLocalizationPanel->Name = L"mLocalizationPanel";
    mLocalizationPanel->Size = System::Drawing::Size(528, 386);


    mRenderPanel = gcnew RenderPanel();
    this->pnlViewers->Controls->Add(mRenderPanel);
    mRenderPanel->Dock = System::Windows::Forms::DockStyle::Fill;
    mRenderPanel->Location = System::Drawing::Point(0, 0);
    mRenderPanel->Name = L"mRenderPanel";
    mRenderPanel->Size = System::Drawing::Size(528, 386);

    if (!mRenderPanel->InitGraphics()) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to initialize DirectX 11 graphics!\n3D viewer will be unavailable.");
    }

    // Create info panels
    mImageInfoPanel = gcnew MetroEXControls::ImageInfoPanel();
    this->pnlMetaProps->Controls->Add(this->mImageInfoPanel);
    mImageInfoPanel->Dock = System::Windows::Forms::DockStyle::None;
    mImageInfoPanel->Location = System::Drawing::Point(0, 0);
    mImageInfoPanel->Name = L"mImageInfoPanel";
    mImageInfoPanel->Size = System::Drawing::Size(481, 72);

    mModelInfoPanel = gcnew MetroEXControls::ModelInfoPanel();
    this->pnlMetaProps->Controls->Add(this->mModelInfoPanel);
    mModelInfoPanel->Dock = System::Windows::Forms::DockStyle::None;
    mModelInfoPanel->Location = System::Drawing::Point(0, 0);
    mModelInfoPanel->Name = L"mModelInfoPanel";
    mModelInfoPanel->Size = System::Drawing::Size(481, 72);
    mModelInfoPanel->OnMotionsListSelectionChanged += gcnew MetroEXControls::ModelInfoPanel::OnListSelectionChanged(this, &MainWindowImpl::lstMdlPropMotions_SelectedIndexChanged);
    mModelInfoPanel->OnPlayButtonClicked += gcnew MetroEXControls::ModelInfoPanel::OnButtonClicked(this, &MainWindowImpl::btnMdlPropPlayStopAnim_Click);
    mModelInfoPanel->OnInfoButtonClicked += gcnew MetroEXControls::ModelInfoPanel::OnButtonClicked(this, &MainWindowImpl::btnModelInfo_Click);
    mModelInfoPanel->OnMotionExportButtonClicked += gcnew MetroEXControls::ModelInfoPanel::OnButtonClicked(this, &MainWindowImpl::btnModelExportMotion_Click);
    mModelInfoPanel->OnLodsListSelectionChanged += gcnew MetroEXControls::ModelInfoPanel::OnListSelectionChanged(this, &MainWindowImpl::lstLods_SelectedIndexChanged);
    ////

    this->SwitchViewPanel(PanelType::Texture);
    this->SwitchInfoPanel(PanelType::Sound);
}

// toolbar
void MainWindowImpl::OnOpenGameFolderClicked() {
    fs::path folderPath = ChooseFolderDialog::ChooseFolder(L"Choose game directory...", this->Handle.ToPointer());
    if (!folderPath.empty()) {
        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::WaitCursor;

        if (MetroFileSystem::Get().InitFromGameFolder(folderPath)) {
            LoadDatabasesFromFile(folderPath);
            this->UpdateFilesList();
        }

        this->toolBtnTexturesDatabase->Enabled = true;
        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::Arrow;
    }
}

void MainWindowImpl::OnOpenSingleVFXClicked() {
    OpenFileDialog ofd;
    ofd.Title = L"Open Metro Exodus vfx file...";
    ofd.Filter = L"VFX files (*.vfx)|*.vfx";
    ofd.FilterIndex = 0;
    ofd.RestoreDirectory = true;
    if (ofd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::WaitCursor;

        fs::path vfxPath = StringToPath(ofd.FileName);
        fs::path vfxFolder = vfxPath.parent_path();
        if (MetroFileSystem::Get().InitFromSingleVFX(vfxPath)) {
            LoadDatabasesFromFile(vfxFolder);
            this->UpdateFilesList();
        }

        this->toolBtnTexturesDatabase->Enabled = true;
        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::Arrow;
    }
}

void MainWindowImpl::OnImgEnableAlphaClick() {
    if (mImagePanel) {
        this->toolBtnImgEnableAlpha->Checked = !this->toolBtnImgEnableAlpha->Checked;
        mImagePanel->TransparencyEnabled = toolBtnImgEnableAlpha->Checked;
    }
}

void MainWindowImpl::OnMdlShowWireframeClick() {
    if (mRenderPanel) {
        this->toolBtnMdlShowWireframe->Checked = !this->toolBtnMdlShowWireframe->Checked;
        mRenderPanel->SetShowWireframe(this->toolBtnMdlShowWireframe->Checked);
    }
}

void MainWindowImpl::OnBtnMdlShowCollisionClick() {
    if (mRenderPanel) {
        this->toolBtnMdlShowCollision->Checked = !this->toolBtnMdlShowCollision->Checked;
        mRenderPanel->SetShowCollision(this->toolBtnMdlShowCollision->Checked);
    }
}

void MainWindowImpl::OnBtnMdlResetCameraClick() {
    if (mRenderPanel) {
        mRenderPanel->ResetCamera(true);
    }
}

// files tree
void MainWindowImpl::OnFilesTreeNodeMouseClick(System::Windows::Forms::TreeNodeMouseClickEventArgs^ e) {
    if (e->Button == System::Windows::Forms::MouseButtons::Right) {
        FileTagData^ fileData = safe_cast<FileTagData^>(e->Node->Tag);
        const bool isSubFile = fileData->subFileIdx != kInvalidValue;

        MyHandle file = fileData->fileHandle & kFileHandleMask;

        const FileType fileType = isSubFile ? fileData->fileType : DetectFileType(file);

        *mExtractionCtx = {};
        mExtractionCtx->file = file;
        mExtractionCtx->type = fileType;
        mExtractionCtx->customOffset = kInvalidValue;
        mExtractionCtx->customLength = kInvalidValue;
        mExtractionCtx->customFileName = "";

        const bool isFolder = MetroFileSystem::Get().IsFolder(file);
        if (isFolder) {
            this->ctxMenuExportFolder->Show(this->filterableTreeView->TreeView, e->X, e->Y);
        } else {
            switch (fileType) {
                case FileType::Texture: {
                    //#NOTE_SK: if this is an albedo texture - enable whole set extraction option
                    const bool isAlbedo = MetroTexturesDatabase::Get().IsAlbedo(file);
                    this->saveSurfaceSetToolStripMenuItem->Enabled = isAlbedo;

                    this->ctxMenuExportTexture->Show(this->filterableTreeView->TreeView, e->X, e->Y);
                } break;

                case FileType::Model: {
                    this->ctxMenuExportModel->Show(this->filterableTreeView->TreeView, e->X, e->Y);
                } break;

                case FileType::Sound: {
                    this->ctxMenuExportSound->Show(this->filterableTreeView->TreeView, e->X, e->Y);
                } break;

                case FileType::Localization: {
                    this->ctxMenuExportLocalization->Show(this->filterableTreeView->TreeView, e->X, e->Y);
                } break;

                case FileType::Bin: {
                    if (isSubFile) {
                        const MetroConfigsDatabase::ConfigInfo& ci = MetroConfigsDatabase::Get().GetFileByIdx(fileData->subFileIdx);

                        mExtractionCtx->customOffset = ci.offset;
                        mExtractionCtx->customLength = ci.length;
                        mExtractionCtx->customFileName = NetToCharStr(e->Node->Text);
                        this->ctxMenuExportBin->Show(this->filterableTreeView->TreeView, e->X, e->Y);
                    } else {
                        //this->ctxMenuExportRaw->Show(this->filterableTreeView->TreeView, e->X, e->Y);
                        this->saveModifiedConfigBinToolStripMenuItem->Enabled = MetroConfigsDatabase::Get().IsDirty();
                        this->ctxMenuExportConfigBin->Show(this->filterableTreeView->TreeView, e->X, e->Y);
                    }
                } break;

                case FileType::FolderBin: {
                } break;

                default: {
                    this->ctxMenuExportRaw->Show(this->filterableTreeView->TreeView, e->X, e->Y);
                } break;
            }
        }
    }
}

void MainWindowImpl::OnFilesTreeAfterCollapse(System::Windows::Forms::TreeNode^ node) {
    UpdateNodeIcon(node, eNodeEventType::Close);
}

void MainWindowImpl::OnFilesTreeAfterExpand(System::Windows::Forms::TreeNode^ node) {
    if (node != nullptr) {
        UpdateNodeIcon(node, eNodeEventType::Open);

        FileTagData^ fileData = safe_cast<FileTagData^>(node->Tag);
        const bool isSubTreeSorted = (fileData->fileHandle & kFolderSortedFlag) != 0;

        if (!isSubTreeSorted) {
            MyHandle file = fileData->fileHandle & kFileHandleMask;

            if (node->Nodes->Count > 1) {
                System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::WaitCursor;

                //#NOTE_SK: somehow, BeginUpdate/BeginUpdate makes it even slower, so commented out for the moment
                //this->filterableTreeView->TreeView->BeginUpdate();
                this->filterableTreeView->TreeView->SuspendLayout();
                array<TreeNode^>^ nodes = gcnew array<TreeNode^>(node->Nodes->Count);
                node->Nodes->CopyTo(nodes, 0);
                NodeSorter^ sorter = gcnew NodeSorter();
                System::Array::Sort(nodes, sorter);
                node->Nodes->Clear();
                node->Nodes->AddRange(nodes);
                delete sorter;
                delete nodes;
                //this->filterableTreeView->TreeView->EndUpdate();
                this->filterableTreeView->TreeView->ResumeLayout(false);

                System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::Arrow;
            }

            if (!this->filterableTreeView->IsFiltering) {
                fileData->fileHandle = kFolderSortedFlag | file;
            }
        }
    }
}

void MainWindowImpl::OnFilesTreeAfterSelect(System::Windows::Forms::TreeNode^ node) {
    if (node != nullptr) {
        FileTagData^ fileData = safe_cast<FileTagData^>(node->Tag);
        MyHandle file = fileData->fileHandle & kFileHandleMask;
        const bool isSubFile = fileData->subFileIdx != kInvalidValue;

        const MetroFileSystem& mfs = MetroFileSystem::Get();
        if (!mfs.Empty()) {
            if (isSubFile) {
                const MetroConfigsDatabase::ConfigInfo& ci = MetroConfigsDatabase::Get().GetFileByIdx(fileData->subFileIdx);

                this->statusLabel1->Text = L"config.bin";
                this->statusLabel2->Text = fileData->subFileIdx.ToString();
                this->statusLabel3->Text = ci.offset.ToString();
                this->statusLabel4->Text = ci.length.ToString();
            } else {
                const bool isFolder = mfs.IsFolder(file);
                if (isFolder) {
                    this->statusLabel1->Text = String::Empty;
                    this->statusLabel2->Text = String::Empty;
                    this->statusLabel3->Text = String::Empty;
                    this->statusLabel4->Text = String::Empty;
                } else {
                    this->statusLabel1->Text = L"0";//mf.pakIdx.ToString();
                    this->statusLabel2->Text = L"0";//mf.offset.ToString();
                    this->statusLabel3->Text = mfs.GetCompressedSize(file).ToString();
                    this->statusLabel4->Text = mfs.GetUncompressedSize(file).ToString();

                    this->DetectFileAndShow(file);
                }
            }
        }
    }
}

// tools
void MainWindowImpl::OnTexturesDatabaseClick() {
    if (MetroTexturesDatabase::Get().Good()) {
        TexturesDatabaseViewer wnd(this, this->imageListMain);
        wnd.Icon = this->Icon;
        wnd.ShowDialog(this);
    }
}

void MainWindowImpl::OnTexturesConverterClick() {
    ConvertTexturesDlgImpl dlg;
    dlg.Icon = this->Icon;
    dlg.ShowDialog(this);
}

void MainWindowImpl::OnLocalizationConversionClick() {
    OpenFileDialog ofd;
    ofd.Title = L"Choose localization file to convert";
    ofd.Filter = L"Excel 2003 XML files (*.xml)|*.xml";
    ofd.FilterIndex = 0;
    ofd.CheckFileExists = true;
    ofd.RestoreDirectory = true;
    if (ofd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
        fs::path srcPath = StringToPath(ofd.FileName);

        MetroLocalization loc;
        if (loc.LoadFromExcel2003(srcPath)) {
            String^ fileName = PathToString(srcPath.stem().native() + L".lng");

            SaveFileDialog sfd;
            sfd.Title = L"Where to save Metro localization...";
            sfd.Filter = L"Metro localization (*.lng)|*.lng";
            sfd.FileName = fileName;
            sfd.RestoreDirectory = true;
            sfd.OverwritePrompt = true;
            if (sfd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
                if (loc.Save(StringToPath(sfd.FileName))) {
                    MetroEX::ShowInfoMessageBox(this, L"Conversion succeeded!");
                } else {
                    MetroEX::ShowErrorMessageBox(this, L"Failed to save " + sfd.FileName);
                }
            }
        } else {
            MetroEX::ShowErrorMessageBox(this, L"Failed to open " + ofd.FileName);
        }
    }
}

void MainWindowImpl::OnArchiveToolClick() {
    CreateArchiveDlgImpl dlg;
    dlg.Icon = this->Icon;
    dlg.ShowDialog(this);
}

void MainWindowImpl::OnFontsEditorClick() {
    FontsEditorImpl dlg;
    dlg.Icon = this->Icon;
    dlg.ShowDialog(this);
}

// settings
void MainWindowImpl::OnSettingsClick() {
    SettingsDlgImpl dlg;
    dlg.Icon = this->Icon;
    dlg.ShowDialog(this);
}

// extraction
//  raw
void MainWindowImpl::OnExtractRAWFileClicked() {
    if (!this->ExtractFile(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract file!");
    }
}

//  textures
void MainWindowImpl::OnExtractTextureDDSClicked() {
    mExtractionCtx->txSaveAsDds = true;
    mExtractionCtx->txUseBC3 = false;

    if (!this->ExtractTexture(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract texture!");
    }
}

void MainWindowImpl::OnExtractTextureLegacyDDSClicked() {
    mExtractionCtx->txSaveAsDds = true;
    mExtractionCtx->txUseBC3 = true;

    if (!this->ExtractTexture(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract texture!");
    }
}

void MainWindowImpl::OnExtractTextureTGAClicked() {
    mExtractionCtx->txSaveAsTga = true;

    if (!this->ExtractTexture(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract texture!");
    }
}

void MainWindowImpl::OnExtractTexturePNGClicked() {
    mExtractionCtx->txSaveAsPng = true;

    if (!this->ExtractTexture(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract texture!");
    }
}

void MainWindowImpl::OnExtractSurfaceSetClicked() {
    MetroSurfaceDescription surface = MetroTexturesDatabase::Get().GetSurfaceSet(mExtractionCtx->file);

    this->EnsureExtractionOptions();
    mExtractionCtx->batch = false;
    mExtractionCtx->raw = false;

    this->ExtractSurfaceSet(*mExtractionCtx, surface, fs::path());
}

//  models
void MainWindowImpl::OnExtractModelOBJClicked() {
    this->EnsureExtractionOptions();
    mExtractionCtx->mdlSaveAsObj = true;
    mExtractionCtx->mdlSaveAsFbx = false;

    if (!this->ExtractModel(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract model!");
    }
}

void MainWindowImpl::OnExtractModelFBXClicked() {
    this->EnsureExtractionOptions();
    mExtractionCtx->mdlSaveAsObj = false;
    mExtractionCtx->mdlSaveAsFbx = true;

    if (!this->ExtractModel(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract model!");
    }
}

//  sound
void MainWindowImpl::OnExtractSoundOGGClicked() {
    mExtractionCtx->sndSaveAsOgg = true;

    if (!this->ExtractSound(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract sound!");
    }
}

void MainWindowImpl::OnExtractSoundWAVClicked() {
    mExtractionCtx->sndSaveAsWav = true;

    if (!this->ExtractSound(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract sound!");
    }
}

//  localization
void MainWindowImpl::OnExtractLocalizationExcelClicked() {
    if (!this->ExtractLocalization(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract localization!");
    }
}

//  bin
void MainWindowImpl::OnExtractBinRootFileClicked() {
    mExtractionCtx->customOffset = kInvalidValue;
    mExtractionCtx->customLength = kInvalidValue;
    mExtractionCtx->customFileName = "";

    this->OnExtractRAWFileClicked();
}

void MainWindowImpl::OnExtractBinInnerFileClicked() {
    if (!this->ExtractFile(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract bin file chunk!");
    }
}

void MainWindowImpl::OnExtractConfigBinClicked() {
    mExtractionCtx->customOffset = kInvalidValue;
    mExtractionCtx->customLength = kInvalidValue;
    mExtractionCtx->customFileName = "";

    this->OnExtractRAWFileClicked();
}

void MainWindowImpl::OnExtractModifiedConfigBinClicked() {
    const MemStream& stream = MetroConfigsDatabase::Get().GetDataStream();
    if (stream.Good()) {
        fs::path resultPath;

        SaveFileDialog sfd;
        sfd.Title = L"Save Configs database...";
        sfd.Filter = L"Bin file (*.bin)|*.bin";
        sfd.FileName = "config.bin";
        sfd.RestoreDirectory = true;
        sfd.OverwritePrompt = true;

        if (sfd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            resultPath = StringToPath(sfd.FileName);
        }

        if (!resultPath.empty()) {
            bool result = false;

            std::ofstream file(resultPath, std::ofstream::binary);
            if (file.good()) {
                file.write(rcast<const char*>(stream.Data()), stream.Length());
                file.flush();
                file.close(); 

                result = true;
            }

            if (!result) {
                MetroEX::ShowErrorMessageBox(this, "Failed to save Configs Database!");
            } else {
                MetroEX::ShowInfoMessageBox(this, "Configs Database was successfully saved!");
            }
        }
    }
}

//  folder
void MainWindowImpl::OnExtractFolderClicked(bool withConversion) {
    fs::path folderPath = ChooseFolderDialog::ChooseFolder(L"Choose output directory...", this->Handle.ToPointer());
    if (!folderPath.empty()) {
        mExtractionCtx->batch = true;
        mExtractionCtx->raw = !withConversion;
        mExtractionCtx->numFilesTotal = MetroFileSystem::Get().CountFilesInFolder(mExtractionCtx->file);
        mExtractionCtx->progress = 0;

        pin_ptr<IProgressDialog*> ipdPtr(&mExtractionProgressDlg);
        HRESULT hr = ::CoCreateInstance(CLSID_ProgressDialog, NULL, CLSCTX_INPROC_SERVER, __uuidof(IProgressDialog), (void**)ipdPtr);
        if (SUCCEEDED(hr)) {
            mExtractionProgressDlg->SetTitle(L"Extracting files...");
            mExtractionProgressDlg->SetLine(0, L"Please wait while your files are being extracted...", FALSE, nullptr);
            mExtractionProgressDlg->StartProgressDialog(rcast<HWND>(this->Handle.ToPointer()), nullptr,
                PROGDLG_NORMAL | PROGDLG_MODAL | PROGDLG_AUTOTIME | PROGDLG_NOMINIMIZE,
                nullptr);
        }

        mExtractionThread = gcnew System::Threading::Thread(gcnew System::Threading::ParameterizedThreadStart(this, &MainWindowImpl::ExtractionProcessFunc));
        mExtractionThread->Start(PathToString(folderPath));
    }
}

void MainWindowImpl::UpdateFilesList() {
    this->filterableTreeView->TreeView->BeginUpdate();
    this->filterableTreeView->TreeView->Nodes->Clear();

    MetroFileSystem& mfs = MetroFileSystem::Get();
    if (!mfs.Empty()) {
        this->filterableTreeView->FilterTextBox->Text = String::Empty;

        // Get idx of config.bin
        const MyHandle configBinFile = mfs.FindFile("content\\config.bin");

        String^ rootName = "FileSystem";
        if (mfs.IsSingleVFX()) {
            rootName = ToNetString(mfs.GetVFXName(0));
        }
        TreeNode^ rootNode = this->filterableTreeView->TreeView->Nodes->Add(rootName);
        size_t rootIdx = 0;

        mOriginalRootNode = rootNode;

        rootNode->Tag = gcnew FileTagData(FileType::Folder, rootIdx, kInvalidValue);
        UpdateNodeIcon(rootNode);

        const MyHandle rootDir = mfs.GetRootFolder();
        for (MyHandle child = mfs.GetFirstChild(rootDir); child != kInvalidHandle; child = mfs.GetNextChild(child)) {
            if (mfs.IsFolder(child)) {
                this->AddFoldersRecursive(child, rootNode, configBinFile);
            } else {
                const FileType fileType = DetectFileType(child);
                TreeNode^ fileNode = rootNode->Nodes->Add(ToNetString(mfs.GetName(child)));
                fileNode->Tag = gcnew FileTagData(fileType, child, kInvalidValue);
                UpdateNodeIcon(fileNode);
            }
        }
    }

    this->filterableTreeView->TreeView->EndUpdate();
}

void MainWindowImpl::AddFoldersRecursive(MyHandle folder, TreeNode^ rootItem, const MyHandle configBinFile) {
    MetroFileSystem& mfs = MetroFileSystem::Get();

    // Add root folder
    TreeNode^ dirLeafNode = rootItem->Nodes->Add(ToNetString(mfs.GetName(folder)));

    dirLeafNode->Tag = gcnew FileTagData(FileType::Folder, folder, kInvalidValue);
    UpdateNodeIcon(dirLeafNode);

    // Add files and folders inside
    for (auto child = mfs.GetFirstChild(folder); child != kInvalidHandle; child = mfs.GetNextChild(child)) {
        if (mfs.IsFolder(child)) {
            // Add folder to list
            this->AddFoldersRecursive(child, dirLeafNode, configBinFile);
        } else {
            // Add file to list
            if (child == configBinFile) {
                // config.bin
                this->AddBinaryArchive(child, dirLeafNode);
            } else {
                //====> any other file
                const FileType fileType = DetectFileType(child);
                TreeNode^ fileNode = dirLeafNode->Nodes->Add(ToNetString(mfs.GetName(child)));
                fileNode->Tag = gcnew FileTagData(fileType, child, kInvalidValue);
                UpdateNodeIcon(fileNode);
            }
        }
    }
}

void MainWindowImpl::AddBinaryArchive(MyHandle file, TreeNode^ rootItem) {
    MetroFileSystem& mfs = MetroFileSystem::Get();

    TreeNode^ fileNode = rootItem->Nodes->Add(ToNetString(mfs.GetName(file)));
    fileNode->Tag = gcnew FileTagData(FileType::BinArchive, file, kInvalidValue);
    UpdateNodeIcon(fileNode);

    for (size_t idx = 0, numFiles = MetroConfigsDatabase::Get().GetNumFiles(); idx < numFiles; ++idx) {
        const MetroConfigsDatabase::ConfigInfo& ci = MetroConfigsDatabase::Get().GetFileByIdx(idx);

        const bool isNameDecrypted = !ci.nameStr.empty();

        String^ fileName = (isNameDecrypted ?
                ToNetString(ci.nameStr) :
                String::Format("unknCRC32_0x{0:X}.bin", ci.nameCRC)
            );

        TreeNode^ lastNode = fileNode; // folder to add file
        if (isNameDecrypted) {
            array<String^>^ pathArray = fileName->Split('\\');
            fileName = pathArray[pathArray->Length - 1];

            // Add all sub-folders
            String^ curPath = pathArray[0];
            for (int i = 0; i < (pathArray->Length - 1); ++i) {
                array<TreeNode^>^ folderNodes = lastNode->Nodes->Find(curPath, false);
                if (folderNodes->Length == 0) {
                    // Create new folder node
                    String^ folderName = pathArray[i];

                    lastNode = lastNode->Nodes->Add(folderName);
                    lastNode->Tag = gcnew FileTagData(FileType::FolderBin, file, 0);
                    lastNode->Name = curPath; // for Find()
                    UpdateNodeIcon(lastNode);
                } else {
                    // Use existing node folder
                    lastNode = folderNodes[0];
                }

                curPath += "\\" + pathArray[i + 1];
            }
        }

        // Add binary file
        TreeNode^ chunkNode = lastNode->Nodes->Add(fileName);
        chunkNode->Tag = gcnew FileTagData(FileType::Bin, file, idx);
        UpdateNodeIcon(chunkNode);
    }
}

void MainWindowImpl::DetectFileAndShow(MyHandle file) {
    const bool isFolder = MetroFileSystem::Get().IsFolder(file);
    if (!isFolder) {
        const FileType fileType = DetectFileType(file);

        switch (fileType) {
            case FileType::Texture: {
                this->ShowTexture(file);
            } break;

            case FileType::Model: {
                this->ShowModel(file);
            } break;

            case FileType::Sound: {
                this->ShowSound(file);
            } break;

            //case FileType::Level: {
            //    this->ShowLevel(file);
            //} break;

            case FileType::Localization: {
                this->ShowLocalization(file);
            } break;
        }
    }
}

Bitmap^ TextureToBitmap(const MetroTexture& texture) {
    BytesArray pixels;
    texture.GetBGRA(pixels);

    const int w = scast<int>(texture.GetWidth());
    const int h = scast<int>(texture.GetHeight());

    Bitmap^ bmp = gcnew Bitmap(w, h, Imaging::PixelFormat::Format32bppArgb);

    Drawing::Rectangle rc(0, 0, w, h);
    Imaging::BitmapData^ bmpData = bmp->LockBits(rc, Imaging::ImageLockMode::WriteOnly, bmp->PixelFormat);
    memcpy(bmpData->Scan0.ToPointer(), pixels.data(), pixels.size());
    bmp->UnlockBits(bmpData);

    return bmp;
}

void MainWindowImpl::ShowTexture(MyHandle file) {
    MemStream stream = MetroFileSystem::Get().OpenFileStream(file);
    if (stream) {
        MetroTexture texture;
        if (texture.LoadFromData(stream, file)) {
            if (texture.IsCubemap()) {
                this->SwitchViewPanel(PanelType::Model);
                mRenderPanel->SetCubemap(&texture);
            } else {
                this->SwitchViewPanel(PanelType::Texture);
                mImagePanel->Image = TextureToBitmap(texture);
            }

            this->SwitchInfoPanel(PanelType::Texture);

            mImageInfoPanel->ImgPropCompressionText = texture.IsCubemap() ? L"BC6H" : L"BC7";;
            mImageInfoPanel->ImgPropWidthText = texture.GetWidth().ToString();
            mImageInfoPanel->ImgPropHeightText = texture.GetHeight().ToString();
            mImageInfoPanel->ImgPropsMipsText = texture.GetNumMips().ToString();
        }
    }
}

void MainWindowImpl::ShowModel(MyHandle file) {
    this->SwitchViewPanel(PanelType::Model);
    this->SwitchInfoPanel(PanelType::Model);

    MemStream stream = MetroFileSystem::Get().OpenFileStream(file);
    if (stream) {
        MetroModel* mdl = new MetroModel();
        if (mdl->LoadFromData(stream, file)) {
            mRenderPanel->SetModel(nullptr);

            mModelInfoPanel->ClearLodsList();
            mModelInfoPanel->AddLodIdToList(0);
            if (mdl->GetLodModel(0) != nullptr) {
                mModelInfoPanel->AddLodIdToList(1);
                if (mdl->GetLodModel(1) != nullptr) {
                    mModelInfoPanel->AddLodIdToList(2);
                }
            }
            mModelInfoPanel->SelectLod(0);

            mRenderPanel->SetModel(mdl);

            mModelInfoPanel->ClearMotionsList();
            if (mdl->IsAnimated()) {
                const size_t numMotions = mdl->GetNumMotions();
                for (size_t i = 0; i < numMotions; ++i) {
                    const CharString& motionName = mdl->GetMotionName(i);
                    mModelInfoPanel->AddMotionToList(ToNetString(motionName));
                }

                mModelInfoPanel->MdlPropTypeText = L"Animated";
                mModelInfoPanel->MdlPropJointsText = mdl->GetSkeleton()->GetNumBones().ToString();
                mModelInfoPanel->MdlPropNumAnimsText = numMotions.ToString();
            } else {
                mModelInfoPanel->MdlPropTypeText = L"Static";
                mModelInfoPanel->MdlPropJointsText = L"0";
                mModelInfoPanel->MdlPropNumAnimsText = L"0";
            }

            size_t numVertices = 0, numTriangles = 0;
            const size_t numMeshes = mdl->GetNumMeshes();
            for (size_t i = 0; i < numMeshes; ++i) {
                const MetroMesh* mesh = mdl->GetMesh(i);
                numVertices += mesh->vertices.size();
                numTriangles += mesh->faces.size();
            }

            mModelInfoPanel->MdlPropVerticesText = numVertices.ToString();
            mModelInfoPanel->MdlPropTrianglesText = numTriangles.ToString();

            mModelInfoPanel->MdlPropPlayStopAnimBtnText = L"Play";

            if (mDlgModelInfo) {
                mDlgModelInfo->SetModel(mdl);
            }
        } else {
            MySafeDelete(mdl);
        }
    }
}

void MainWindowImpl::ShowSound(MyHandle file) {
    this->SwitchViewPanel(PanelType::Sound);
    this->SwitchInfoPanel(PanelType::Sound);

    MemStream stream = MetroFileSystem::Get().OpenFileStream(file);
    if (stream) {
        MetroSound* snd = new MetroSound();
        if (snd->LoadFromData(stream)) {
            mSoundPanel->SetSound(snd);
        } else {
            MySafeDelete(snd);
        }
    }
}

void MainWindowImpl::ShowLocalization(MyHandle file) {
    this->SwitchViewPanel(PanelType::Localization);
    this->SwitchInfoPanel(PanelType::Localization);

    MemStream stream = MetroFileSystem::Get().OpenFileStream(file);
    if (stream) {
        MetroLocalization loc;
        if (loc.LoadFromData(stream)) {
            mLocalizationPanel->SetLocalizationTable(&loc);
        }
    }
}

void MainWindowImpl::SwitchViewPanel(PanelType t) {
    switch (t) {
        case PanelType::Texture: {
            mRenderPanel->Hide();
            mSoundPanel->Hide();
            mLocalizationPanel->Hide();
            mImagePanel->Show();
        } break;

        case PanelType::Model: {
            mImagePanel->Hide();
            mSoundPanel->Hide();
            mLocalizationPanel->Hide();
            mRenderPanel->Show();
        } break;

        case PanelType::Sound: {
            mImagePanel->Hide();
            mRenderPanel->Hide();
            mLocalizationPanel->Hide();
            mSoundPanel->Show();
        } break;

        case PanelType::Localization: {
            mImagePanel->Hide();
            mRenderPanel->Hide();
            mSoundPanel->Hide();
            mLocalizationPanel->Show();
        } break;
    }
}

void MainWindowImpl::SwitchInfoPanel(PanelType t) {
    switch (t) {
        case PanelType::Texture: {
            mModelInfoPanel->Dock = System::Windows::Forms::DockStyle::None;
            mModelInfoPanel->Hide();

            mImageInfoPanel->Location = System::Drawing::Point(0, 0);
            mImageInfoPanel->Dock = System::Windows::Forms::DockStyle::Fill;
            mImageInfoPanel->Show();
        } break;

        case PanelType::Model: {
            mImageInfoPanel->Dock = System::Windows::Forms::DockStyle::None;
            mImageInfoPanel->Hide();

            mModelInfoPanel->Location = System::Drawing::Point(0, 0);
            mModelInfoPanel->Dock = System::Windows::Forms::DockStyle::Fill;
            mModelInfoPanel->Show();
        } break;

        case PanelType::Sound:
        case PanelType::Localization: {
            mModelInfoPanel->Dock = System::Windows::Forms::DockStyle::None;
            mModelInfoPanel->Hide();

            mImageInfoPanel->Dock = System::Windows::Forms::DockStyle::None;
            mImageInfoPanel->Hide();
        } break;
    }
}


// extraction
bool MainWindowImpl::EnsureExtractionOptions() {
    bool result = true;

    MEXSettings& s = MEXSettings::Get();

    if (s.extraction.askEveryTime) {
        SettingsDlgImpl dlg;
        dlg.Icon = this->Icon;
        auto dlgResult = dlg.ShowDialog(this);
        if (dlgResult == System::Windows::Forms::DialogResult::Cancel) {
            result = false;
        }
    }

    // models
    mExtractionCtx->mdlSaveAsObj = (s.extraction.modelFormat == MEXSettings::Extraction::MdlFormat::Obj);
    mExtractionCtx->mdlSaveAsFbx = (s.extraction.modelFormat == MEXSettings::Extraction::MdlFormat::Fbx);
    mExtractionCtx->mdlSaveWithAnims = s.extraction.modelSaveWithAnims;
    mExtractionCtx->mdlAnimsSeparate = s.extraction.modelAnimsSeparate;
    mExtractionCtx->mdlSaveWithTextures = s.extraction.modelSaveWithTextures;
    mExtractionCtx->mdlExcludeCollision = s.extraction.modelExcludeCollision;
    mExtractionCtx->mdlSaveLods = s.extraction.modelSaveLods;
    // textures
    mExtractionCtx->txSaveAsDds = (s.extraction.textureFormat == MEXSettings::Extraction::TexFormat::Dds || s.extraction.textureFormat == MEXSettings::Extraction::TexFormat::LegacyDds);
    mExtractionCtx->txUseBC3 = (s.extraction.textureFormat == MEXSettings::Extraction::TexFormat::LegacyDds);
    mExtractionCtx->txSaveAsTga = (s.extraction.textureFormat == MEXSettings::Extraction::TexFormat::Tga);
    mExtractionCtx->txSaveAsPng = (s.extraction.textureFormat == MEXSettings::Extraction::TexFormat::Png);
    // sounds
    mExtractionCtx->sndSaveAsOgg = (s.extraction.soundFormat == MEXSettings::Extraction::SndFormat::Ogg);
    mExtractionCtx->sndSaveAsWav = (s.extraction.soundFormat == MEXSettings::Extraction::SndFormat::Wav);

    return result;
}

CharString MainWindowImpl::MakeFileOutputName(MyHandle file, const FileExtractionCtx& ctx) {
    CharString name = MetroFileSystem::Get().GetName(file);

    switch (ctx.type) {
    case FileType::Texture: {
        const CharString::size_type dotPos = name.find_last_of('.');
        const size_t replaceLen = name.size() - dotPos;

        if (ctx.txSaveAsDds) {
            name = name.replace(dotPos, replaceLen, ".dds");
        } else if (ctx.txSaveAsTga) {
            name = name.replace(dotPos, replaceLen, ".tga");
        } else if (ctx.txSaveAsPng) {
            name = name.replace(dotPos, replaceLen, ".png");
        }
    } break;

    case FileType::Model: {
        const CharString::size_type dotPos = name.find_last_of('.');
        const size_t replaceLen = name.size() - dotPos;

        if (ctx.mdlSaveAsObj) {
            name = name.replace(dotPos, replaceLen, ".obj");
        } else {
            name = name.replace(dotPos, replaceLen, ".fbx");
        }
    } break;

    case FileType::Sound: {
        if (ctx.sndSaveAsOgg) {
            name[name.size() - 3] = 'o';
            name[name.size() - 2] = 'g';
            name[name.size() - 1] = 'g';
        } else {
            name[name.size() - 3] = 'w';
            name[name.size() - 2] = 'a';
            name[name.size() - 1] = 'v';
        }
    } break;

    case FileType::Localization: {
        name[name.size() - 3] = 'x';
        name[name.size() - 2] = 'm';
        name[name.size() - 1] = 'l';
    } break;
    }

    return name;
}

void MainWindowImpl::TextureSaveHelper(const fs::path& folderPath, const FileExtractionCtx& ctx, const CharString& name) {
    CharString textureName = CharString("content\\textures\\") + name;

    const MetroFileSystem& mfs = MetroFileSystem::Get();

    CharString textureNameSrc = textureName + ".2048";
    MyHandle textureHandle = mfs.FindFile(textureNameSrc);
    if (textureHandle == kInvalidHandle) {
        textureNameSrc = textureName + ".1024";
        textureHandle = mfs.FindFile(textureNameSrc);
    }
    if (textureHandle == kInvalidHandle) {
        textureNameSrc = textureName + ".512";
        textureHandle = mfs.FindFile(textureNameSrc);
    }

    if (textureHandle == kInvalidHandle) {
        // last try - Redux .bin
        textureNameSrc = textureName + ".bin";
        textureHandle = mfs.FindFile(textureNameSrc);
    }

    if (textureHandle == kInvalidHandle) {
        FileExtractionCtx tmpCtx = ctx;
        tmpCtx.type = FileType::Texture;
        tmpCtx.file = textureHandle;
        tmpCtx.txSaveAsDds = false;
        tmpCtx.txSaveAsTga = true;
        tmpCtx.txSaveAsPng = false;

        CharString outName = this->MakeFileOutputName(textureHandle, tmpCtx);
        this->ExtractTexture(tmpCtx, folderPath / outName);
    }
}

bool MainWindowImpl::ExtractFile(const FileExtractionCtx& ctx, const fs::path& outPath) {
    bool result = false;

    const CharString& fileName = MetroFileSystem::Get().GetName(ctx.file);
    String^ name = ctx.customFileName.empty() ?
        ToNetString(fileName) :
        ToNetString(ctx.customFileName);

    fs::path resultPath = outPath;
    if (resultPath.empty()) {
        SaveFileDialog sfd;
        sfd.Title = L"Save file...";
        sfd.Filter = L"All files (*.*)|*.*";
        sfd.FileName = name;
        sfd.RestoreDirectory = true;
        sfd.OverwritePrompt = true;

        if (sfd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            resultPath = StringToPath(sfd.FileName);
        } else {
            return true;
        }
    }

    if (!resultPath.empty()) {
        MemStream stream = MetroFileSystem::Get().OpenFileStream(ctx.file);
        if (stream) {
            std::ofstream file(resultPath, std::ofstream::binary);
            if (file.good()) {
                const bool hasCustomLength = ctx.customLength != kInvalidValue;
                const bool hasCustomOffset = ctx.customOffset != kInvalidValue;

                size_t lengthToWrite = hasCustomLength ? ctx.customLength : stream.Remains();

                if (hasCustomOffset) {
                    stream.SetCursor(ctx.customOffset);

                    if (hasCustomLength == false) {
                        lengthToWrite = stream.Length() - ctx.customOffset;
                    }
                }

                file.write(rcast<const char*>(stream.GetDataAtCursor()), lengthToWrite);
                file.flush();

                result = true;
            }
        }
    }

    return result;
}

bool MainWindowImpl::ExtractTexture(const FileExtractionCtx& ctx, const fs::path& outPath) {
    bool result = false;

    fs::path resultPath = outPath;
    if (resultPath.empty()) {
        String^ title;
        String^ filter;
        if (ctx.txSaveAsDds) {
            title = L"Save DDS texture...";
            filter = L"DirectDraw Surface (*.dds)|*.dds";
        } else if (ctx.txSaveAsTga) {
            title = L"Save TGA texture...";
            filter = L"Targa images (*.tga)|*.tga";
        } else {
            title = L"Save PNG texture...";
            filter = L"PNG images (*.png)|*.png";
        }

        CharString nameWithExt = this->MakeFileOutputName(ctx.file, ctx);

        SaveFileDialog sfd;
        sfd.Title = title;
        sfd.Filter = filter;
        sfd.FileName = ToNetString(nameWithExt);
        sfd.RestoreDirectory = true;
        sfd.OverwritePrompt = true;

        if (sfd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            resultPath = StringToPath(sfd.FileName);
        } else {
            return true;
        }
    }

    if (!resultPath.empty()) {
        MemStream stream = MetroFileSystem::Get().OpenFileStream(ctx.file);
        if (stream) {
            MetroTexture texture;
            if (texture.LoadFromData(stream, ctx.file)) {
                if (ctx.txSaveAsDds) {
                    if (ctx.txUseBC3) {
                        result = texture.SaveAsLegacyDDS(resultPath);
                    } else {
                        result = texture.SaveAsDDS(resultPath);
                    }
                } else if (ctx.txSaveAsTga) {
                    result = texture.SaveAsTGA(resultPath);
                } else {
                    result = texture.SaveAsPNG(resultPath);
                }
            }
        }
    }

    return result;
}

bool MainWindowImpl::ExtractSurfaceSet(const FileExtractionCtx& ctx, const MetroSurfaceDescription& surface, const fs::path& outFolder) {
    bool result = false;

    fs::path folderPath = outFolder;
    if (folderPath.empty()) {
        folderPath = ChooseFolderDialog::ChooseFolder(L"Choose output directory...", this->Handle.ToPointer());
    }

    if (!folderPath.empty()) {
        const MetroFileSystem& mfs = MetroFileSystem::Get();

        FileExtractionCtx setCtx = ctx;
        setCtx.type = FileType::Texture;

#define EXTRACT_SURFACE_TEXTURE(tex_name)                                                       \
            if (!surface.tex_name##Path.empty()) {                                              \
                const MyHandle file = mfs.FindFile(surface.tex_name##Path);                     \
                if (file != kInvalidHandle) {                                                   \
                    setCtx.file = file;                                                         \
                    CharString nameWithExt = this->MakeFileOutputName(file, setCtx);            \
                    result = this->ExtractTexture(setCtx, folderPath / nameWithExt) && result;  \
                }                                                                               \
            }

        EXTRACT_SURFACE_TEXTURE(albedo);
        EXTRACT_SURFACE_TEXTURE(bump);
        EXTRACT_SURFACE_TEXTURE(normalmap);
        EXTRACT_SURFACE_TEXTURE(detail);

#undef EXTRACT_SURFACE_TEXTURE
    }

    return result;
}

bool MainWindowImpl::ExtractModel(const FileExtractionCtx& ctx, const fs::path& outPath) {
    bool result = false;

    fs::path resultPath = outPath;
    if (resultPath.empty()) {
        String^ title;
        String^ filter;
        if (ctx.mdlSaveAsObj) {
            title = L"Save OBJ model...";
            filter = L"OBJ model (*.obj)|*.obj";
        } else {
            title = L"Save FBX model...";
            filter = L"FBX model (*.fbx)|*.fbx";
        }

        CharString nameWithExt = this->MakeFileOutputName(ctx.file, ctx);

        SaveFileDialog sfd;
        sfd.Title = title;
        sfd.Filter = filter;
        sfd.FileName = ToNetString(nameWithExt);
        sfd.RestoreDirectory = true;
        sfd.OverwritePrompt = true;

        if (sfd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            resultPath = StringToPath(sfd.FileName);
        } else {
            return true;
        }
    }

    const MEXSettings& settings = MEXSettings::Get();

    if (!resultPath.empty()) {
        MemStream& stream = MetroFileSystem::Get().OpenFileStream(ctx.file);
        if (stream) {
            MetroModel mdl;
            if (mdl.LoadFromData(stream, ctx.file)) {
                if (ctx.mdlSaveAsObj) {
                    mdl.SaveAsOBJ(resultPath, ctx.mdlExcludeCollision);
                    if (ctx.mdlSaveLods) {
                        WideString resultLodPath = resultPath.parent_path() / resultPath.stem();
                        MetroModel* lod1 = mdl.GetLodModel(0);
                        MetroModel* lod2 = mdl.GetLodModel(1);

                        if (lod1) {
                            lod1->SaveAsOBJ(resultLodPath + L"_lod1.obj", ctx.mdlExcludeCollision);
                        }
                        if (lod2) {
                            lod2->SaveAsOBJ(resultLodPath + L"_lod2.obj", ctx.mdlExcludeCollision);
                        }
                    }
                } else {
                    size_t fbxOptions = MetroModel::FBX_Export_Mesh | MetroModel::FBX_Export_Skeleton;
                    if (ctx.mdlExcludeCollision) {
                        fbxOptions |= MetroModel::FBX_Export_ExcludeCollision;
                    }
                    if (settings.extraction.modelSaveWithAnims && !settings.extraction.modelAnimsSeparate) {
                        fbxOptions |= MetroModel::FBX_Export_Animation;
                    }

                    mdl.SaveAsFBX(resultPath, fbxOptions);

                    if (ctx.mdlSaveLods) {
                        WideString resultLodPath = resultPath.parent_path() / resultPath.stem();
                        MetroModel* lod1 = mdl.GetLodModel(0);
                        MetroModel* lod2 = mdl.GetLodModel(1);

                        if (lod1) {
                            lod1->SaveAsFBX(resultLodPath + L"_lod1.fbx", ctx.mdlExcludeCollision);
                        }
                        if (lod2) {
                            lod2->SaveAsFBX(resultLodPath + L"_lod2.fbx", ctx.mdlExcludeCollision);
                        }
                    }

                    if (settings.extraction.modelSaveWithAnims && settings.extraction.modelAnimsSeparate) {
                        fbxOptions = MetroModel::FBX_Export_Skeleton | MetroModel::FBX_Export_Animation;

                        fs::path modelBasePath = resultPath.parent_path() / resultPath.stem();
                        for (size_t motionIdx = 0; motionIdx != mdl.GetNumMotions(); ++motionIdx) {
                            const MetroMotion* motion = mdl.GetMotion(motionIdx);
                            fs::path animPath = modelBasePath.native() + fs::path("@" + motion->GetName()).native() + L".fbx";
                            mdl.SaveAsFBX(animPath, fbxOptions, motionIdx);
                        }
                    }
                }

                if (!ctx.batch && ctx.mdlSaveWithTextures) {
                    fs::path folderPath = resultPath.parent_path();
                    for (size_t i = 0; i < mdl.GetNumMeshes(); ++i) {
                        const MetroMesh* mesh = mdl.GetMesh(i);
                        if (!mesh->materials.empty()) {
                            const CharString& textureName = mesh->materials.front();

                            if (settings.extraction.modelSaveSurfaceSet) {
                                MetroSurfaceDescription surface = MetroTexturesDatabase::Get().GetSurfaceSet(textureName);
                                this->ExtractSurfaceSet(ctx, surface, folderPath);
                            } else {
                                const CharString& sourceName = MetroTexturesDatabase::Get().GetSourceName(textureName);
                                this->TextureSaveHelper(folderPath, ctx, sourceName);
                            }
                        }
                    }
                }

                result = true;
            }
        }
    }

    return result;
}

bool MainWindowImpl::ExtractMotion(const FileExtractionCtx& ctx, const fs::path& outPath) {
    bool result = false;

    MetroModel* model = mRenderPanel->GetModel();
    const int motionIdx = mModelInfoPanel->SelectedMotionIdx;

    if (!model || motionIdx < 0) {
        return false;
    }

    fs::path resultPath = outPath;
    if (resultPath.empty()) {
        String^ name = ToNetString(model->GetMotionName(scast<size_t>(motionIdx)));

        SaveFileDialog sfd;
        sfd.Title = L"Save FBX animation...";
        sfd.Filter = L"FBX animation (*.fbx)|*.fbx";
        sfd.FileName = name + L".fbx";
        sfd.RestoreDirectory = true;
        sfd.OverwritePrompt = true;

        if (sfd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            resultPath = StringToPath(sfd.FileName);
        } else {
            return true;
        }
    }

    if (!resultPath.empty()) {
        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::WaitCursor;
        const size_t fbxOptions = MetroModel::FBX_Export_Skeleton | MetroModel::FBX_Export_Animation;

        result = model->SaveAsFBX(resultPath, fbxOptions, scast<size_t>(motionIdx));

        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::Arrow;
    }

    return result;
}

bool MainWindowImpl::ExtractSound(const FileExtractionCtx& ctx, const fs::path& outPath) {
    bool result = false;

    fs::path resultPath = outPath;
    if (resultPath.empty()) {
        String^ title;
        String^ filter;
        if (ctx.sndSaveAsOgg) {
            title = L"Save Ogg sound...";
            filter = L"Ogg Vorbis (*.ogg)|*.ogg";
        } else {
            title = L"Save WAV sound...";
            filter = L"Wave sounds (*.wav)|*.wav";
        }

        CharString nameWithExt = this->MakeFileOutputName(ctx.file, ctx);

        SaveFileDialog sfd;
        sfd.Title = title;
        sfd.Filter = filter;
        sfd.FileName = ToNetString(nameWithExt);
        sfd.RestoreDirectory = true;
        sfd.OverwritePrompt = true;

        if (sfd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            resultPath = StringToPath(sfd.FileName);
        } else {
            return true;
        }
    }

    if (!resultPath.empty()) {
        MemStream stream = MetroFileSystem::Get().OpenFileStream(ctx.file);
        if (stream) {
            MetroSound sound;
            if (sound.LoadFromData(stream)) {
                if (ctx.sndSaveAsOgg) {
                    result = sound.SaveAsOGG(resultPath);
                } else {
                    result = sound.SaveAsWAV(resultPath);
                }
            }
        }
    }

    return result;
}

bool MainWindowImpl::ExtractLocalization(const FileExtractionCtx& ctx, const fs::path& outPath) {
    bool result = false;

    fs::path resultPath = outPath;
    if (resultPath.empty()) {
        CharString nameWithExt = this->MakeFileOutputName(ctx.file, ctx);

        SaveFileDialog sfd;
        sfd.Title = L"Save Excel 2003 XML...";
        sfd.Filter = L"Excel 2003 XML (*.xml)|*.xml";
        sfd.FileName = ToNetString(nameWithExt);
        sfd.RestoreDirectory = true;
        sfd.OverwritePrompt = true;

        if (sfd.ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
            resultPath = StringToPath(sfd.FileName);
        } else {
            return true;
        }
    }

    if (!resultPath.empty()) {
        MemStream stream = MetroFileSystem::Get().OpenFileStream(ctx.file);
        if (stream) {
            MetroLocalization loc;
            if (loc.LoadFromData(stream)) {
                result = loc.SaveToExcel2003(resultPath);
            }
        }
    }

    return result;
}

bool MainWindowImpl::ExtractFolderComplete(const FileExtractionCtx& ctx, const fs::path& outPath) {
    bool result = false;

    const MetroFileSystem& mfs = MetroFileSystem::Get();
    const CharString& folderName = mfs.GetName(ctx.file);

    fs::path curPath = outPath / folderName;
    fs::create_directories(curPath);

    FileExtractionCtx tmpCtx = ctx;
    for (MyHandle child = mfs.GetFirstChild(ctx.file); child != kInvalidHandle; child = mfs.GetNextChild(child)) {
        tmpCtx.file = child;
        tmpCtx.type = DetectFileType(child);

        const bool isFolder = mfs.IsFolder(child);
        if (isFolder) {
            this->ExtractFolderComplete(tmpCtx, curPath);
        } else {
            if (ctx.raw) {
                const CharString& childName = mfs.GetName(child);
                fs::path filePath = curPath / childName;
                this->ExtractFile(tmpCtx, filePath);
            } else {
                fs::path filePath = curPath / this->MakeFileOutputName(child, tmpCtx);
                switch (tmpCtx.type) {
                case FileType::Texture: {
                    this->ExtractTexture(tmpCtx, filePath);
                } break;

                case FileType::Model: {
                    this->ExtractModel(tmpCtx, filePath);
                } break;

                case FileType::Sound: {
                    this->ExtractSound(tmpCtx, filePath);
                } break;

                case FileType::Localization: {
                    this->ExtractLocalization(tmpCtx, filePath);
                } break;

                default: {
                    this->ExtractFile(tmpCtx, filePath);
                } break;
                }
            }

            mExtractionCtx->progress++;
            if (mExtractionProgressDlg) {
                mExtractionProgressDlg->SetProgress64(mExtractionCtx->progress, mExtractionCtx->numFilesTotal);
            }
        }
    }

    result = true;

    return result;
}

void MainWindowImpl::ExtractionProcessFunc(Object^ folderPath) {
    this->ExtractFolderComplete(*mExtractionCtx, StringToPath(folderPath->ToString()));

    if (mExtractionProgressDlg) {
        mExtractionProgressDlg->StopProgressDialog();
        MySafeRelease(mExtractionProgressDlg);
    }
}


// property panels
// model props
void MainWindowImpl::lstMdlPropMotions_SelectedIndexChanged(int selection) {
    if (selection >= 0) {
        mRenderPanel->SwitchMotion(scast<size_t>(selection));
    }
}

void MainWindowImpl::lstLods_SelectedIndexChanged(int selection) {
    if (selection >= 0 && selection <= scast<int>(MetroModel::kMetroModelMaxLods)) {
        mRenderPanel->SetLod(scast<size_t>(selection));
    }
}

void MainWindowImpl::btnMdlPropPlayStopAnim_Click(System::Object^ sender) {
    mRenderPanel->PlayAnim(!mRenderPanel->IsPlayingAnim());
    mModelInfoPanel->MdlPropPlayStopAnimBtnText = mRenderPanel->IsPlayingAnim() ? L"Stop" : L"Play";
}

void MainWindowImpl::btnModelInfo_Click(System::Object^ sender) {
    if (!mDlgModelInfo) {
        mDlgModelInfo = gcnew MetroEX::DlgModelInfo();
        mDlgModelInfo->Closed += gcnew System::EventHandler(this, &MetroEX::MainWindowImpl::OnDlgModelInfo_Closed);

        mDlgModelInfo->Icon = this->Icon;
        mDlgModelInfo->SetModel(mRenderPanel->GetModel());
        mDlgModelInfo->Show();
    }
}

void MainWindowImpl::btnModelExportMotion_Click(System::Object^ sender) {
    if (!this->ExtractMotion(*mExtractionCtx, fs::path())) {
        MetroEX::ShowErrorMessageBox(this, L"Failed to extract motion!");
    }
}

void MainWindowImpl::OnDlgModelInfo_Closed(System::Object^ sender, System::EventArgs^ e) {
    MySafeDelete(mDlgModelInfo);
}


} // namespace MetroEX

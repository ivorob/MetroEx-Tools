#pragma once
#include "mycommon.h"
#include "metro/MetroTypes.h"
#include "metro/MetroFileSystem.h"

#include <shlobj_core.h>

#include "RenderPanel.h"
#include "SoundPanel.h"
#include "LocalizationPanel.h"
#include "DlgModelInfo.h"

namespace MetroEX {
    enum class FileType : size_t {
        Unknown,
        Folder,
        FolderBin,
        Bin,
        BinArchive,
        BinEditable,
        Texture,
        Model,
        Motion,
        Level,
        Sound,
        Localization
    };

    enum class PanelType {
        Texture,
        Model,
        Sound,
        Localization
    };

    struct FileExtractionCtx {
        MyHandle    file;
        FileType    type;

        size_t      customOffset;
        size_t      customLength;
        CharString  customFileName;

        // models
        bool        mdlSaveAsObj;
        bool        mdlSaveAsFbx;
        bool        mdlSaveWithAnims;
        bool        mdlAnimsSeparate;
        bool        mdlSaveWithTextures;
        bool        mdlExcludeCollision;
        bool        mdlSaveLods;
        // textures
        bool        txUseBC3;
        bool        txSaveAsDds;
        bool        txSaveAsTga;
        bool        txSaveAsPng;
        // sounds
        bool        sndSaveAsOgg;
        bool        sndSaveAsWav;

        // batch
        bool        batch;
        bool        raw;
        size_t      numFilesTotal;
        size_t      progress;
    };

    public ref class MainWindowImpl : public MetroEXControls::MainWindow {
    public:
        MainWindowImpl();
        ~MainWindowImpl();

        void ResetTreeView();
        bool FindAndSelect(String^ text, array<String^>^ extensions);

    protected:
        virtual void OnFormLoad() override;
        // toolbar
        virtual void OnOpenGameFolderClicked() override;
        virtual void OnOpenSingleVFXClicked() override;
        virtual void OnImgEnableAlphaClick() override;
        virtual void OnMdlShowWireframeClick() override;
        virtual void OnBtnMdlShowCollisionClick() override;
        virtual void OnBtnMdlResetCameraClick() override;
        // files tree
        virtual void OnFilesTreeNodeMouseClick(System::Windows::Forms::TreeNodeMouseClickEventArgs^ e) override;
        virtual void OnFilesTreeAfterCollapse(System::Windows::Forms::TreeNode^ node) override;
        virtual void OnFilesTreeAfterExpand(System::Windows::Forms::TreeNode^ node) override;
        virtual void OnFilesTreeAfterSelect(System::Windows::Forms::TreeNode^ node) override;
        // tools
        virtual void OnTexturesDatabaseClick() override;
        virtual void OnTexturesConverterClick() override;
        virtual void OnLocalizationConversionClick() override;
        virtual void OnArchiveToolClick() override;
        virtual void OnFontsEditorClick() override;
        // settings
        virtual void OnSettingsClick() override;
        // extraction
        //  raw
        virtual void OnExtractRAWFileClicked() override;
        //  textures
        virtual void OnExtractTextureDDSClicked() override;
        virtual void OnExtractTextureLegacyDDSClicked() override;
        virtual void OnExtractTextureTGAClicked() override;
        virtual void OnExtractTexturePNGClicked() override;
        virtual void OnExtractSurfaceSetClicked() override;
        //  models
        virtual void OnExtractModelOBJClicked() override;
        virtual void OnExtractModelFBXClicked() override;
        //  sound
        virtual void OnExtractSoundOGGClicked() override;
        virtual void OnExtractSoundWAVClicked() override;
        //  localization
        virtual void OnExtractLocalizationExcelClicked() override;
        //  bin
        virtual void OnExtractBinRootFileClicked() override;
        virtual void OnExtractBinInnerFileClicked() override;
        virtual void OnExtractConfigBinClicked() override;
        virtual void OnExtractModifiedConfigBinClicked() override;
        //  folder
        virtual void OnExtractFolderClicked(bool withConversion) override;

    private:
        void UpdateFilesList();
        void AddFoldersRecursive(MyHandle folder, TreeNode^ rootItem, const MyHandle configBinFile);
        void AddBinaryArchive(MyHandle file, TreeNode^ rootItem);
        void DetectFileAndShow(MyHandle file);
        void ShowTexture(MyHandle file);
        void ShowModel(MyHandle file);
        void ShowSound(MyHandle file);
        void ShowLocalization(MyHandle file);
        void SwitchViewPanel(PanelType t);
        void SwitchInfoPanel(PanelType t);

        // extraction
        bool EnsureExtractionOptions();
        CharString MakeFileOutputName(MyHandle file, const FileExtractionCtx& ctx);
        void TextureSaveHelper(const fs::path& folderPath, const FileExtractionCtx& ctx, const CharString& name);
        bool ExtractFile(const FileExtractionCtx& ctx, const fs::path& outPath);
        bool ExtractTexture(const FileExtractionCtx& ctx, const fs::path& outPath);
        bool ExtractSurfaceSet(const FileExtractionCtx& ctx, const MetroSurfaceDescription& surface, const fs::path& outFolder);
        bool ExtractModel(const FileExtractionCtx& ctx, const fs::path& outPath);
        bool ExtractMotion(const FileExtractionCtx& ctx, const fs::path& outPath);
        bool ExtractSound(const FileExtractionCtx& ctx, const fs::path& outPath);
        bool ExtractLocalization(const FileExtractionCtx& ctx, const fs::path& outPath);
        bool ExtractFolderComplete(const FileExtractionCtx& ctx, const fs::path& outPath);
        void ExtractionProcessFunc(Object^ folderPath);

        // property panels
        // model props
        void lstMdlPropMotions_SelectedIndexChanged(int selection);
        void lstLods_SelectedIndexChanged(int selection);
        void btnMdlPropPlayStopAnim_Click(System::Object^ sender);
        void btnModelInfo_Click(System::Object^ sender);
        void btnModelExportMotion_Click(System::Object^ sender);
        void OnDlgModelInfo_Closed(System::Object^ sender, System::EventArgs^ e);

    private:
        MetroEXControls::ImagePanel^        mImagePanel;
        MetroEX::RenderPanel^               mRenderPanel;
        MetroEX::SoundPanel^                mSoundPanel;
        MetroEX::LocalizationPanel^         mLocalizationPanel;
        // Info panels
        MetroEXControls::ImageInfoPanel^    mImageInfoPanel;
        MetroEXControls::ModelInfoPanel^    mModelInfoPanel;
        //
        MetroEX::DlgModelInfo^              mDlgModelInfo;

        //
        FileExtractionCtx*                  mExtractionCtx;
        System::Threading::Thread^          mExtractionThread;
        IProgressDialog*                    mExtractionProgressDlg;

        TreeNode^                           mOriginalRootNode;
    };

}

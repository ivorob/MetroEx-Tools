#pragma once
#include "mycommon.h"
#include "metro/MetroTypes.h"

#include <shlobj_core.h>

#include "RenderPanel.h"
#include "ImagePanel.h"
#include "SoundPanel.h"
#include "LocalizationPanel.h"
#include "DlgModelInfo.h"

class MetroConfigsDatabase;

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
        size_t      fileIdx;
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

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;


    public ref class MainForm : public ComponentFactory::Krypton::Toolkit::KryptonForm {
    public:
        MainForm() {
            mImagePanel = nullptr;
            mRenderPanel = nullptr;
            mSoundPanel = nullptr;
            mLocalizationPanel = nullptr;
            mDlgModelInfo = nullptr;

            mExtractionCtx = new FileExtractionCtx;
            mExtractionProgressDlg = nullptr;

            mOriginalRootNode = nullptr;

            InitializeComponent();

            this->filterableTreeView->TreeView->ImageList = this->imageListMain;
            this->filterableTreeView->TreeView->NodeMouseClick += (gcnew TreeNodeMouseClickEventHandler(this, &MainForm::filterableTreeView_NodeMouseClick));
            this->filterableTreeView->TreeView->AfterCollapse += (gcnew TreeViewEventHandler(this, &MainForm::filterableTreeView_AfterCollapse));
            this->filterableTreeView->TreeView->AfterExpand += (gcnew TreeViewEventHandler(this, &MainForm::filterableTreeView_AfterExpand));
            this->filterableTreeView->TreeView->AfterSelect += (gcnew TreeViewEventHandler(this, &MainForm::filterableTreeView_AfterSelect));
        }

    protected:
        ~MainForm() {
            if (components) {
                delete components;
            }
        }

    private:
        MetroEX::ImagePanel^                mImagePanel;
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

        MetroConfigsDatabase*               mConfigsDatabase;

    protected:

    private: System::Windows::Forms::StatusStrip^  statusStrip1;
    private: System::Windows::Forms::SplitContainer^  splitContainer1;

    private: System::Windows::Forms::ImageList^  imageListMain;
    private: System::Windows::Forms::ToolStripStatusLabel^  statusLabel1;
    private: System::Windows::Forms::ToolStripStatusLabel^  statusLabel2;
    private: System::Windows::Forms::ToolStripStatusLabel^  statusLabel3;
    private: System::Windows::Forms::ToolStripStatusLabel^  statusLabel4;
    private: System::Windows::Forms::ContextMenuStrip^  ctxMenuExportTexture;
    private: System::Windows::Forms::ContextMenuStrip^  ctxMenuExportModel;
    private: System::Windows::Forms::ContextMenuStrip^  ctxMenuExportSound;
    private: System::Windows::Forms::ContextMenuStrip^  ctxMenuExportRaw;
    private: System::Windows::Forms::ContextMenuStrip^  ctxMenuExportFolder;
    private: System::Windows::Forms::ToolStripMenuItem^  saveAsDDSToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  saveAsLegacyDDSToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  saveAsTGAToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  saveAsPNGToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  extractFileToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  saveAsOBJToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  saveAsFBXToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  saveAsOGGToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  saveAsWAVToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  extractFolderToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  extractFolderWithConversionToolStripMenuItem;

    private: System::Windows::Forms::ToolStrip^  toolStrip1;
    private: System::Windows::Forms::ToolStripButton^  toolBtnFileOpen;
    private: System::Windows::Forms::ToolStripButton^  toolBtnAbout;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
    private: System::Windows::Forms::ToolStripButton^  toolBtnImgEnableAlpha;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;

    private: System::Windows::Forms::ContextMenuStrip^  ctxMenuExportBin;
    private: System::Windows::Forms::ToolStripMenuItem^  extractBinRootToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  extractBinChunkToolStripMenuItem;
    private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel2;
    private: System::Windows::Forms::Panel^  pnlViewers;
    private: System::Windows::Forms::Panel^  pnlMetaProps;






    private: System::Windows::Forms::ToolStripButton^  toolBtnTexturesDatabase;
    private: System::Windows::Forms::ContextMenuStrip^  ctxMenuExportLocalization;
    private: System::Windows::Forms::ToolStripMenuItem^  saveAsExcel2003XMLToolStripMenuItem;
private: MetroEXControls::FilterableTreeView^  filterableTreeView;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator3;
    private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator4;
    private: System::Windows::Forms::ToolStripSplitButton^  toolStripSplitButton1;
    private: System::Windows::Forms::ToolStripMenuItem^  texturesConverterToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  archiveToolToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  localizationConversionToolStripMenuItem;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator5;
private: System::Windows::Forms::ToolStripButton^  toolBtnSettings;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator6;
private: System::Windows::Forms::ToolStripButton^  toolBtnMdlShowWireframe;
private: System::Windows::Forms::ToolStripButton^  toolBtnMdlShowCollision;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator7;
private: System::Windows::Forms::ToolStripButton^  toolBtnMdlResetCamera;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbon^  ribbon;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonTab^  fileTab;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroup^  archiveGroup;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple^  kryptonRibbonGroupTriple1;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  ribbonBtnOpenFile;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroup^  toolsGroup;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple^  kryptonRibbonGroupTriple2;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  ribbonBtnTexturesDatabaseViewer;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupSeparator^  kryptonRibbonGroupSeparator1;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple^  kryptonRibbonGroupTriple4;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  ribbonBtnTexturesConverter;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  ribbonBtnLocalizationConversion;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupSeparator^  kryptonRibbonGroupSeparator2;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple^  kryptonRibbonGroupTriple5;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  ribbonBtnArchiveTool;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple^  kryptonRibbonGroupTriple3;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  kryptonRibbonGroupButton1;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  kryptonRibbonGroupButton2;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  kryptonRibbonGroupButton3;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  kryptonRibbonGroupButton7;
private: ComponentFactory::Krypton::Toolkit::KryptonPanel^  kryptonPanel1;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonTab^  viewTab;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroup^  kryptonRibbonGroup1;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple^  kryptonRibbonGroupTriple6;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  ribbonBtnEnableAlpha;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupSeparator^  kryptonRibbonGroupSeparator3;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple^  kryptonRibbonGroupTriple7;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  ribbonBtnShowWireframe;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  ribbonBtnShowCollision;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupSeparator^  kryptonRibbonGroupSeparator4;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple^  kryptonRibbonGroupTriple8;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  ribbonBtnResetCamera;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonTab^  miscTab;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroup^  kryptonRibbonGroup2;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple^  kryptonRibbonGroupTriple9;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  ribbonBtnSettings;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton^  ribbonBtnAbout;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonQATButton^  QATBtnSettings;
private: ComponentFactory::Krypton::Ribbon::KryptonRibbonQATButton^  QATBtnAbout;
    private: System::ComponentModel::IContainer^  components;





    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>



#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
            this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
            this->statusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->statusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->statusLabel3 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->statusLabel4 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
            this->filterableTreeView = (gcnew MetroEXControls::FilterableTreeView());
            this->tableLayoutPanel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
            this->pnlViewers = (gcnew System::Windows::Forms::Panel());
            this->pnlMetaProps = (gcnew System::Windows::Forms::Panel());
            this->imageListMain = (gcnew System::Windows::Forms::ImageList(this->components));
            this->ctxMenuExportTexture = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->saveAsDDSToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveAsLegacyDDSToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveAsTGAToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveAsPNGToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ctxMenuExportModel = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->saveAsOBJToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveAsFBXToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ctxMenuExportSound = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->saveAsOGGToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveAsWAVToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ctxMenuExportRaw = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->extractFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ctxMenuExportFolder = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->extractFolderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->extractFolderWithConversionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
            this->toolBtnFileOpen = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->toolBtnImgEnableAlpha = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSeparator7 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->toolBtnMdlShowWireframe = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolBtnMdlShowCollision = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolBtnMdlResetCamera = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSeparator3 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->toolBtnTexturesDatabase = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSeparator4 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->toolStripSplitButton1 = (gcnew System::Windows::Forms::ToolStripSplitButton());
            this->texturesConverterToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->localizationConversionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator5 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->archiveToolToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->toolBtnSettings = (gcnew System::Windows::Forms::ToolStripButton());
            this->toolStripSeparator6 = (gcnew System::Windows::Forms::ToolStripSeparator());
            this->toolBtnAbout = (gcnew System::Windows::Forms::ToolStripButton());
            this->ctxMenuExportBin = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->extractBinRootToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->extractBinChunkToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ctxMenuExportLocalization = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->saveAsExcel2003XMLToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ribbon = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbon());
            this->fileTab = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonTab());
            this->archiveGroup = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroup());
            this->kryptonRibbonGroupTriple1 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple());
            this->ribbonBtnOpenFile = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->toolsGroup = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroup());
            this->kryptonRibbonGroupTriple2 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple());
            this->ribbonBtnTexturesDatabaseViewer = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->kryptonRibbonGroupSeparator1 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupSeparator());
            this->kryptonRibbonGroupTriple3 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple());
            this->kryptonRibbonGroupButton1 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->kryptonRibbonGroupButton2 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->kryptonRibbonGroupButton3 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->kryptonRibbonGroupTriple4 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple());
            this->ribbonBtnTexturesConverter = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->ribbonBtnLocalizationConversion = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->kryptonRibbonGroupButton7 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->kryptonRibbonGroupSeparator2 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupSeparator());
            this->kryptonRibbonGroupTriple5 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple());
            this->ribbonBtnArchiveTool = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->kryptonPanel1 = (gcnew ComponentFactory::Krypton::Toolkit::KryptonPanel());
            this->viewTab = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonTab());
            this->kryptonRibbonGroup1 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroup());
            this->kryptonRibbonGroupTriple6 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple());
            this->ribbonBtnEnableAlpha = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->kryptonRibbonGroupSeparator3 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupSeparator());
            this->kryptonRibbonGroupTriple7 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple());
            this->ribbonBtnShowWireframe = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->ribbonBtnShowCollision = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->kryptonRibbonGroupSeparator4 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupSeparator());
            this->kryptonRibbonGroupTriple8 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple());
            this->ribbonBtnResetCamera = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->miscTab = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonTab());
            this->kryptonRibbonGroup2 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroup());
            this->kryptonRibbonGroupTriple9 = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupTriple());
            this->ribbonBtnSettings = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->ribbonBtnAbout = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupButton());
            this->QATBtnSettings = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonQATButton());
            this->QATBtnAbout = (gcnew ComponentFactory::Krypton::Ribbon::KryptonRibbonQATButton());
            this->statusStrip1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
            this->splitContainer1->Panel1->SuspendLayout();
            this->splitContainer1->Panel2->SuspendLayout();
            this->splitContainer1->SuspendLayout();
            this->tableLayoutPanel2->SuspendLayout();
            this->ctxMenuExportTexture->SuspendLayout();
            this->ctxMenuExportModel->SuspendLayout();
            this->ctxMenuExportSound->SuspendLayout();
            this->ctxMenuExportRaw->SuspendLayout();
            this->ctxMenuExportFolder->SuspendLayout();
            this->toolStrip1->SuspendLayout();
            this->ctxMenuExportBin->SuspendLayout();
            this->ctxMenuExportLocalization->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ribbon))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->kryptonPanel1))->BeginInit();
            this->kryptonPanel1->SuspendLayout();
            this->SuspendLayout();
            // 
            // statusStrip1
            // 
            this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
                this->statusLabel1, this->statusLabel2,
                    this->statusLabel3, this->statusLabel4
            });
            this->statusStrip1->Location = System::Drawing::Point(0, 697);
            this->statusStrip1->Name = L"statusStrip1";
            this->statusStrip1->Size = System::Drawing::Size(1159, 22);
            this->statusStrip1->TabIndex = 1;
            this->statusStrip1->Text = L"statusStrip1";
            // 
            // statusLabel1
            // 
            this->statusLabel1->Name = L"statusLabel1";
            this->statusLabel1->Size = System::Drawing::Size(286, 17);
            this->statusLabel1->Spring = true;
            this->statusLabel1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            // 
            // statusLabel2
            // 
            this->statusLabel2->Name = L"statusLabel2";
            this->statusLabel2->Size = System::Drawing::Size(286, 17);
            this->statusLabel2->Spring = true;
            this->statusLabel2->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            // 
            // statusLabel3
            // 
            this->statusLabel3->Name = L"statusLabel3";
            this->statusLabel3->Size = System::Drawing::Size(286, 17);
            this->statusLabel3->Spring = true;
            this->statusLabel3->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            // 
            // statusLabel4
            // 
            this->statusLabel4->BorderStyle = System::Windows::Forms::Border3DStyle::Raised;
            this->statusLabel4->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->statusLabel4->Name = L"statusLabel4";
            this->statusLabel4->Size = System::Drawing::Size(286, 17);
            this->statusLabel4->Spring = true;
            this->statusLabel4->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
            // 
            // splitContainer1
            // 
            this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer1->Location = System::Drawing::Point(0, 0);
            this->splitContainer1->Margin = System::Windows::Forms::Padding(0);
            this->splitContainer1->Name = L"splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this->splitContainer1->Panel1->Controls->Add(this->filterableTreeView);
            this->splitContainer1->Panel1->Controls->Add(this->toolStrip1);
            // 
            // splitContainer1.Panel2
            // 
            this->splitContainer1->Panel2->Controls->Add(this->tableLayoutPanel2);
            this->splitContainer1->Size = System::Drawing::Size(1159, 554);
            this->splitContainer1->SplitterDistance = 301;
            this->splitContainer1->TabIndex = 2;
            // 
            // filterableTreeView
            // 
            this->filterableTreeView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->filterableTreeView->FilterPlaceholder = L"Search here...";
            this->filterableTreeView->FilterTimeout = 1000;
            this->filterableTreeView->Location = System::Drawing::Point(0, 0);
            this->filterableTreeView->Margin = System::Windows::Forms::Padding(0);
            this->filterableTreeView->Name = L"filterableTreeView";
            this->filterableTreeView->Size = System::Drawing::Size(301, 554);
            this->filterableTreeView->TabIndex = 0;
            // 
            // tableLayoutPanel2
            // 
            this->tableLayoutPanel2->ColumnCount = 1;
            this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
                100)));
            this->tableLayoutPanel2->Controls->Add(this->pnlViewers, 0, 0);
            this->tableLayoutPanel2->Controls->Add(this->pnlMetaProps, 0, 1);
            this->tableLayoutPanel2->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tableLayoutPanel2->Location = System::Drawing::Point(0, 0);
            this->tableLayoutPanel2->Margin = System::Windows::Forms::Padding(0);
            this->tableLayoutPanel2->Name = L"tableLayoutPanel2";
            this->tableLayoutPanel2->RowCount = 2;
            this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
            this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 80)));
            this->tableLayoutPanel2->Size = System::Drawing::Size(854, 554);
            this->tableLayoutPanel2->TabIndex = 0;
            // 
            // pnlViewers
            // 
            this->pnlViewers->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pnlViewers->Location = System::Drawing::Point(0, 0);
            this->pnlViewers->Margin = System::Windows::Forms::Padding(0);
            this->pnlViewers->Name = L"pnlViewers";
            this->pnlViewers->Size = System::Drawing::Size(854, 474);
            this->pnlViewers->TabIndex = 0;
            // 
            // pnlMetaProps
            // 
            this->pnlMetaProps->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->pnlMetaProps->Dock = System::Windows::Forms::DockStyle::Fill;
            this->pnlMetaProps->Location = System::Drawing::Point(0, 474);
            this->pnlMetaProps->Margin = System::Windows::Forms::Padding(0);
            this->pnlMetaProps->Name = L"pnlMetaProps";
            this->pnlMetaProps->Size = System::Drawing::Size(854, 80);
            this->pnlMetaProps->TabIndex = 1;
            // 
            // imageListMain
            // 
            this->imageListMain->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^>(resources->GetObject(L"imageListMain.ImageStream")));
            this->imageListMain->TransparentColor = System::Drawing::Color::Transparent;
            this->imageListMain->Images->SetKeyName(0, L"folder_closed.png");
            this->imageListMain->Images->SetKeyName(1, L"folder_opened.png");
            this->imageListMain->Images->SetKeyName(2, L"file_any.png");
            this->imageListMain->Images->SetKeyName(3, L"bin_unkn.png");
            this->imageListMain->Images->SetKeyName(4, L"bin_archive.png");
            this->imageListMain->Images->SetKeyName(5, L"bin_editable.png");
            this->imageListMain->Images->SetKeyName(6, L"image.png");
            this->imageListMain->Images->SetKeyName(7, L"anim.png");
            this->imageListMain->Images->SetKeyName(8, L"sound.png");
            this->imageListMain->Images->SetKeyName(9, L"model.png");
            this->imageListMain->Images->SetKeyName(10, L"localization.png");
            // 
            // ctxMenuExportTexture
            // 
            this->ctxMenuExportTexture->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
                this->saveAsDDSToolStripMenuItem,
                    this->saveAsLegacyDDSToolStripMenuItem, this->saveAsTGAToolStripMenuItem, this->saveAsPNGToolStripMenuItem
            });
            this->ctxMenuExportTexture->Name = L"ctxMenuExportTexture";
            this->ctxMenuExportTexture->Size = System::Drawing::Size(184, 92);
            // 
            // saveAsDDSToolStripMenuItem
            // 
            this->saveAsDDSToolStripMenuItem->Name = L"saveAsDDSToolStripMenuItem";
            this->saveAsDDSToolStripMenuItem->Size = System::Drawing::Size(183, 22);
            this->saveAsDDSToolStripMenuItem->Text = L"Save as DDS...";
            this->saveAsDDSToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveAsDDSToolStripMenuItem_Click);
            // 
            // saveAsLegacyDDSToolStripMenuItem
            // 
            this->saveAsLegacyDDSToolStripMenuItem->Name = L"saveAsLegacyDDSToolStripMenuItem";
            this->saveAsLegacyDDSToolStripMenuItem->Size = System::Drawing::Size(183, 22);
            this->saveAsLegacyDDSToolStripMenuItem->Text = L"Save as legacy DDS...";
            this->saveAsLegacyDDSToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveAsLegacyDDSToolStripMenuItem_Click);
            // 
            // saveAsTGAToolStripMenuItem
            // 
            this->saveAsTGAToolStripMenuItem->Name = L"saveAsTGAToolStripMenuItem";
            this->saveAsTGAToolStripMenuItem->Size = System::Drawing::Size(183, 22);
            this->saveAsTGAToolStripMenuItem->Text = L"Save as TGA...";
            this->saveAsTGAToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveAsTGAToolStripMenuItem_Click);
            // 
            // saveAsPNGToolStripMenuItem
            // 
            this->saveAsPNGToolStripMenuItem->Name = L"saveAsPNGToolStripMenuItem";
            this->saveAsPNGToolStripMenuItem->Size = System::Drawing::Size(183, 22);
            this->saveAsPNGToolStripMenuItem->Text = L"Save as PNG...";
            this->saveAsPNGToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveAsPNGToolStripMenuItem_Click);
            // 
            // ctxMenuExportModel
            // 
            this->ctxMenuExportModel->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                this->saveAsOBJToolStripMenuItem,
                    this->saveAsFBXToolStripMenuItem
            });
            this->ctxMenuExportModel->Name = L"ctxMenuExportModel";
            this->ctxMenuExportModel->Size = System::Drawing::Size(145, 48);
            // 
            // saveAsOBJToolStripMenuItem
            // 
            this->saveAsOBJToolStripMenuItem->Name = L"saveAsOBJToolStripMenuItem";
            this->saveAsOBJToolStripMenuItem->Size = System::Drawing::Size(144, 22);
            this->saveAsOBJToolStripMenuItem->Text = L"Save as OBJ...";
            this->saveAsOBJToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveAsOBJToolStripMenuItem_Click);
            // 
            // saveAsFBXToolStripMenuItem
            // 
            this->saveAsFBXToolStripMenuItem->Name = L"saveAsFBXToolStripMenuItem";
            this->saveAsFBXToolStripMenuItem->Size = System::Drawing::Size(144, 22);
            this->saveAsFBXToolStripMenuItem->Text = L"Save as FBX...";
            this->saveAsFBXToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveAsFBXToolStripMenuItem_Click);
            // 
            // ctxMenuExportSound
            // 
            this->ctxMenuExportSound->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                this->saveAsOGGToolStripMenuItem,
                    this->saveAsWAVToolStripMenuItem
            });
            this->ctxMenuExportSound->Name = L"ctxMenuExportSound";
            this->ctxMenuExportSound->Size = System::Drawing::Size(150, 48);
            // 
            // saveAsOGGToolStripMenuItem
            // 
            this->saveAsOGGToolStripMenuItem->Name = L"saveAsOGGToolStripMenuItem";
            this->saveAsOGGToolStripMenuItem->Size = System::Drawing::Size(149, 22);
            this->saveAsOGGToolStripMenuItem->Text = L"Save as OGG...";
            this->saveAsOGGToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveAsOGGToolStripMenuItem_Click);
            // 
            // saveAsWAVToolStripMenuItem
            // 
            this->saveAsWAVToolStripMenuItem->Name = L"saveAsWAVToolStripMenuItem";
            this->saveAsWAVToolStripMenuItem->Size = System::Drawing::Size(149, 22);
            this->saveAsWAVToolStripMenuItem->Text = L"Save as WAV...";
            this->saveAsWAVToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveAsWAVToolStripMenuItem_Click);
            // 
            // ctxMenuExportRaw
            // 
            this->ctxMenuExportRaw->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->extractFileToolStripMenuItem });
            this->ctxMenuExportRaw->Name = L"ctxMenuExportRaw";
            this->ctxMenuExportRaw->Size = System::Drawing::Size(138, 26);
            // 
            // extractFileToolStripMenuItem
            // 
            this->extractFileToolStripMenuItem->Name = L"extractFileToolStripMenuItem";
            this->extractFileToolStripMenuItem->Size = System::Drawing::Size(137, 22);
            this->extractFileToolStripMenuItem->Text = L"Extract file...";
            this->extractFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::extractFileToolStripMenuItem_Click);
            // 
            // ctxMenuExportFolder
            // 
            this->ctxMenuExportFolder->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                this->extractFolderToolStripMenuItem,
                    this->extractFolderWithConversionToolStripMenuItem
            });
            this->ctxMenuExportFolder->Name = L"ctxMenuExportFolder";
            this->ctxMenuExportFolder->Size = System::Drawing::Size(240, 48);
            // 
            // extractFolderToolStripMenuItem
            // 
            this->extractFolderToolStripMenuItem->Name = L"extractFolderToolStripMenuItem";
            this->extractFolderToolStripMenuItem->Size = System::Drawing::Size(239, 22);
            this->extractFolderToolStripMenuItem->Text = L"Extract folder...";
            this->extractFolderToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::extractFolderToolStripMenuItem_Click);
            // 
            // extractFolderWithConversionToolStripMenuItem
            // 
            this->extractFolderWithConversionToolStripMenuItem->Name = L"extractFolderWithConversionToolStripMenuItem";
            this->extractFolderWithConversionToolStripMenuItem->Size = System::Drawing::Size(239, 22);
            this->extractFolderWithConversionToolStripMenuItem->Text = L"Extract folder with conversion...";
            this->extractFolderWithConversionToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::extractFolderWithConversionToolStripMenuItem_Click);
            // 
            // toolStrip1
            // 
            this->toolStrip1->Dock = System::Windows::Forms::DockStyle::None;
            this->toolStrip1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
            this->toolStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
            this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(15) {
                this->toolBtnFileOpen,
                    this->toolStripSeparator1, this->toolBtnImgEnableAlpha, this->toolStripSeparator7, this->toolBtnMdlShowWireframe, this->toolBtnMdlShowCollision,
                    this->toolBtnMdlResetCamera, this->toolStripSeparator3, this->toolBtnTexturesDatabase, this->toolStripSeparator4, this->toolStripSplitButton1,
                    this->toolStripSeparator2, this->toolBtnSettings, this->toolStripSeparator6, this->toolBtnAbout
            });
            this->toolStrip1->Location = System::Drawing::Point(50, 697);
            this->toolStrip1->Name = L"toolStrip1";
            this->toolStrip1->Size = System::Drawing::Size(255, 25);
            this->toolStrip1->TabIndex = 5;
            this->toolStrip1->Text = L"toolStrip1";
            // 
            // toolBtnFileOpen
            // 
            this->toolBtnFileOpen->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->toolBtnFileOpen->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolBtnFileOpen.Image")));
            this->toolBtnFileOpen->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolBtnFileOpen->Name = L"toolBtnFileOpen";
            this->toolBtnFileOpen->Size = System::Drawing::Size(23, 22);
            this->toolBtnFileOpen->ToolTipText = L"Open Metro Exodus archive...";
            this->toolBtnFileOpen->Click += gcnew System::EventHandler(this, &MainForm::toolBtnFileOpen_Click);
            // 
            // toolStripSeparator1
            // 
            this->toolStripSeparator1->Name = L"toolStripSeparator1";
            this->toolStripSeparator1->Size = System::Drawing::Size(6, 25);
            // 
            // toolBtnImgEnableAlpha
            // 
            this->toolBtnImgEnableAlpha->Checked = true;
            this->toolBtnImgEnableAlpha->CheckState = System::Windows::Forms::CheckState::Checked;
            this->toolBtnImgEnableAlpha->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->toolBtnImgEnableAlpha->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline))));
            this->toolBtnImgEnableAlpha->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolBtnImgEnableAlpha.Image")));
            this->toolBtnImgEnableAlpha->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolBtnImgEnableAlpha->Name = L"toolBtnImgEnableAlpha";
            this->toolBtnImgEnableAlpha->Size = System::Drawing::Size(23, 22);
            this->toolBtnImgEnableAlpha->Text = L"A";
            this->toolBtnImgEnableAlpha->ToolTipText = L"Enable alpha";
            this->toolBtnImgEnableAlpha->Click += gcnew System::EventHandler(this, &MainForm::toolBtnImgEnableAlpha_Click);
            // 
            // toolStripSeparator7
            // 
            this->toolStripSeparator7->Name = L"toolStripSeparator7";
            this->toolStripSeparator7->Size = System::Drawing::Size(6, 25);
            // 
            // toolBtnMdlShowWireframe
            // 
            this->toolBtnMdlShowWireframe->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->toolBtnMdlShowWireframe->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline))));
            this->toolBtnMdlShowWireframe->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolBtnMdlShowWireframe.Image")));
            this->toolBtnMdlShowWireframe->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolBtnMdlShowWireframe->Name = L"toolBtnMdlShowWireframe";
            this->toolBtnMdlShowWireframe->Size = System::Drawing::Size(23, 22);
            this->toolBtnMdlShowWireframe->Text = L"W";
            this->toolBtnMdlShowWireframe->ToolTipText = L"Show wireframe";
            this->toolBtnMdlShowWireframe->Click += gcnew System::EventHandler(this, &MainForm::toolBtnMdlShowWireframe_Click);
            // 
            // toolBtnMdlShowCollision
            // 
            this->toolBtnMdlShowCollision->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
            this->toolBtnMdlShowCollision->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, static_cast<System::Drawing::FontStyle>((System::Drawing::FontStyle::Bold | System::Drawing::FontStyle::Underline))));
            this->toolBtnMdlShowCollision->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolBtnMdlShowCollision.Image")));
            this->toolBtnMdlShowCollision->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolBtnMdlShowCollision->Name = L"toolBtnMdlShowCollision";
            this->toolBtnMdlShowCollision->Size = System::Drawing::Size(23, 22);
            this->toolBtnMdlShowCollision->Text = L"C";
            this->toolBtnMdlShowCollision->ToolTipText = L"Show collision (if any)";
            this->toolBtnMdlShowCollision->Click += gcnew System::EventHandler(this, &MainForm::toolBtnMdlShowCollision_Click);
            // 
            // toolBtnMdlResetCamera
            // 
            this->toolBtnMdlResetCamera->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->toolBtnMdlResetCamera->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolBtnMdlResetCamera.Image")));
            this->toolBtnMdlResetCamera->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolBtnMdlResetCamera->Name = L"toolBtnMdlResetCamera";
            this->toolBtnMdlResetCamera->Size = System::Drawing::Size(23, 22);
            this->toolBtnMdlResetCamera->Text = L"R";
            this->toolBtnMdlResetCamera->ToolTipText = L"Reset camera";
            this->toolBtnMdlResetCamera->Click += gcnew System::EventHandler(this, &MainForm::toolBtnMdlResetCamera_Click);
            // 
            // toolStripSeparator3
            // 
            this->toolStripSeparator3->Name = L"toolStripSeparator3";
            this->toolStripSeparator3->Size = System::Drawing::Size(6, 25);
            // 
            // toolBtnTexturesDatabase
            // 
            this->toolBtnTexturesDatabase->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->toolBtnTexturesDatabase->Enabled = false;
            this->toolBtnTexturesDatabase->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolBtnTexturesDatabase.Image")));
            this->toolBtnTexturesDatabase->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolBtnTexturesDatabase->Name = L"toolBtnTexturesDatabase";
            this->toolBtnTexturesDatabase->Size = System::Drawing::Size(23, 22);
            this->toolBtnTexturesDatabase->ToolTipText = L"Textures Database Viewer";
            this->toolBtnTexturesDatabase->Click += gcnew System::EventHandler(this, &MainForm::toolBtnTexturesDatabase_Click);
            // 
            // toolStripSeparator4
            // 
            this->toolStripSeparator4->Name = L"toolStripSeparator4";
            this->toolStripSeparator4->Size = System::Drawing::Size(6, 25);
            // 
            // toolStripSplitButton1
            // 
            this->toolStripSplitButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->toolStripSplitButton1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
                this->texturesConverterToolStripMenuItem,
                    this->localizationConversionToolStripMenuItem, this->toolStripSeparator5, this->archiveToolToolStripMenuItem
            });
            this->toolStripSplitButton1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolStripSplitButton1.Image")));
            this->toolStripSplitButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolStripSplitButton1->Name = L"toolStripSplitButton1";
            this->toolStripSplitButton1->Size = System::Drawing::Size(32, 22);
            this->toolStripSplitButton1->Text = L"Tools";
            // 
            // texturesConverterToolStripMenuItem
            // 
            this->texturesConverterToolStripMenuItem->Name = L"texturesConverterToolStripMenuItem";
            this->texturesConverterToolStripMenuItem->Size = System::Drawing::Size(207, 22);
            this->texturesConverterToolStripMenuItem->Text = L"Textures converter...";
            this->texturesConverterToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::texturesConverterToolStripMenuItem_Click);
            // 
            // localizationConversionToolStripMenuItem
            // 
            this->localizationConversionToolStripMenuItem->Name = L"localizationConversionToolStripMenuItem";
            this->localizationConversionToolStripMenuItem->Size = System::Drawing::Size(207, 22);
            this->localizationConversionToolStripMenuItem->Text = L"Localization conversion...";
            this->localizationConversionToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::localizationConversionToolStripMenuItem_Click);
            // 
            // toolStripSeparator5
            // 
            this->toolStripSeparator5->Name = L"toolStripSeparator5";
            this->toolStripSeparator5->Size = System::Drawing::Size(204, 6);
            // 
            // archiveToolToolStripMenuItem
            // 
            this->archiveToolToolStripMenuItem->Name = L"archiveToolToolStripMenuItem";
            this->archiveToolToolStripMenuItem->Size = System::Drawing::Size(207, 22);
            this->archiveToolToolStripMenuItem->Text = L"Archive tool...";
            this->archiveToolToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::archiveToolToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this->toolStripSeparator2->Name = L"toolStripSeparator2";
            this->toolStripSeparator2->Size = System::Drawing::Size(6, 25);
            // 
            // toolBtnSettings
            // 
            this->toolBtnSettings->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->toolBtnSettings->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolBtnSettings.Image")));
            this->toolBtnSettings->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolBtnSettings->Name = L"toolBtnSettings";
            this->toolBtnSettings->Size = System::Drawing::Size(23, 22);
            this->toolBtnSettings->Text = L"Settings";
            this->toolBtnSettings->Click += gcnew System::EventHandler(this, &MainForm::toolBtnSettings_Click);
            // 
            // toolStripSeparator6
            // 
            this->toolStripSeparator6->Name = L"toolStripSeparator6";
            this->toolStripSeparator6->Size = System::Drawing::Size(6, 25);
            // 
            // toolBtnAbout
            // 
            this->toolBtnAbout->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
            this->toolBtnAbout->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"toolBtnAbout.Image")));
            this->toolBtnAbout->ImageTransparentColor = System::Drawing::Color::Magenta;
            this->toolBtnAbout->Name = L"toolBtnAbout";
            this->toolBtnAbout->Size = System::Drawing::Size(23, 22);
            this->toolBtnAbout->ToolTipText = L"About";
            this->toolBtnAbout->Click += gcnew System::EventHandler(this, &MainForm::toolBtnAbout_Click);
            // 
            // ctxMenuExportBin
            // 
            this->ctxMenuExportBin->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
                this->extractBinRootToolStripMenuItem,
                    this->extractBinChunkToolStripMenuItem
            });
            this->ctxMenuExportBin->Name = L"ctxMenuExportBin";
            this->ctxMenuExportBin->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
            this->ctxMenuExportBin->Size = System::Drawing::Size(163, 48);
            // 
            // extractBinRootToolStripMenuItem
            // 
            this->extractBinRootToolStripMenuItem->Name = L"extractBinRootToolStripMenuItem";
            this->extractBinRootToolStripMenuItem->Size = System::Drawing::Size(162, 22);
            this->extractBinRootToolStripMenuItem->Text = L"Extract root file...";
            this->extractBinRootToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::extractBinRootToolStripMenuItem_Click);
            // 
            // extractBinChunkToolStripMenuItem
            // 
            this->extractBinChunkToolStripMenuItem->Name = L"extractBinChunkToolStripMenuItem";
            this->extractBinChunkToolStripMenuItem->Size = System::Drawing::Size(162, 22);
            this->extractBinChunkToolStripMenuItem->Text = L"Extract this file...";
            this->extractBinChunkToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::extractBinChunkToolStripMenuItem_Click);
            // 
            // ctxMenuExportLocalization
            // 
            this->ctxMenuExportLocalization->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->saveAsExcel2003XMLToolStripMenuItem });
            this->ctxMenuExportLocalization->Name = L"ctxMenuExportLocalization";
            this->ctxMenuExportLocalization->Size = System::Drawing::Size(196, 26);
            // 
            // saveAsExcel2003XMLToolStripMenuItem
            // 
            this->saveAsExcel2003XMLToolStripMenuItem->Name = L"saveAsExcel2003XMLToolStripMenuItem";
            this->saveAsExcel2003XMLToolStripMenuItem->Size = System::Drawing::Size(195, 22);
            this->saveAsExcel2003XMLToolStripMenuItem->Text = L"Save as Excel 2003 XML";
            this->saveAsExcel2003XMLToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::saveAsExcel2003XMLToolStripMenuItem_Click);
            // 
            // ribbon
            // 
            this->ribbon->InDesignHelperMode = true;
            this->ribbon->Name = L"ribbon";
            this->ribbon->QATButtons->AddRange(gcnew cli::array< System::ComponentModel::Component^  >(2) { this->QATBtnSettings, this->QATBtnAbout });
            this->ribbon->QATUserChange = false;
            this->ribbon->RibbonAppButton->AppButtonVisible = false;
            this->ribbon->RibbonTabs->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonTab^  >(3) {
                this->fileTab,
                    this->viewTab, this->miscTab
            });
            this->ribbon->SelectedContext = nullptr;
            this->ribbon->SelectedTab = this->fileTab;
            this->ribbon->Size = System::Drawing::Size(1159, 143);
            this->ribbon->TabIndex = 7;
            // 
            // fileTab
            // 
            this->fileTab->Groups->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroup^  >(2) {
                this->archiveGroup,
                    this->toolsGroup
            });
            this->fileTab->Text = L"File";
            // 
            // archiveGroup
            // 
            this->archiveGroup->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupContainer^  >(1) { this->kryptonRibbonGroupTriple1 });
            this->archiveGroup->TextLine1 = L"Archive";
            // 
            // kryptonRibbonGroupTriple1
            // 
            this->kryptonRibbonGroupTriple1->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupItem^  >(1) { this->ribbonBtnOpenFile });
            // 
            // ribbonBtnOpenFile
            // 
            this->ribbonBtnOpenFile->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnOpenFile.ImageLarge")));
            this->ribbonBtnOpenFile->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnOpenFile.ImageSmall")));
            this->ribbonBtnOpenFile->TextLine1 = L"Open";
            this->ribbonBtnOpenFile->TextLine2 = L"VFX";
            this->ribbonBtnOpenFile->Click += gcnew System::EventHandler(this, &MainForm::toolBtnFileOpen_Click);
            // 
            // toolsGroup
            // 
            this->toolsGroup->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupContainer^  >(5) {
                this->kryptonRibbonGroupTriple2,
                    this->kryptonRibbonGroupSeparator1, this->kryptonRibbonGroupTriple4, this->kryptonRibbonGroupSeparator2, this->kryptonRibbonGroupTriple5
            });
            this->toolsGroup->TextLine1 = L"Tools";
            // 
            // kryptonRibbonGroupTriple2
            // 
            this->kryptonRibbonGroupTriple2->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupItem^  >(1) { this->ribbonBtnTexturesDatabaseViewer });
            // 
            // ribbonBtnTexturesDatabaseViewer
            // 
            this->ribbonBtnTexturesDatabaseViewer->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnTexturesDatabaseViewer.ImageLarge")));
            this->ribbonBtnTexturesDatabaseViewer->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnTexturesDatabaseViewer.ImageSmall")));
            this->ribbonBtnTexturesDatabaseViewer->TextLine1 = L"Textures";
            this->ribbonBtnTexturesDatabaseViewer->TextLine2 = L"Database";
            this->ribbonBtnTexturesDatabaseViewer->Click += gcnew System::EventHandler(this, &MainForm::toolBtnTexturesDatabase_Click);
            // 
            // kryptonRibbonGroupTriple3
            // 
            this->kryptonRibbonGroupTriple3->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupItem^  >(3) {
                this->kryptonRibbonGroupButton1,
                    this->kryptonRibbonGroupButton2, this->kryptonRibbonGroupButton3
            });
            // 
            // kryptonRibbonGroupButton1
            // 
            this->kryptonRibbonGroupButton1->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"kryptonRibbonGroupButton1.ImageLarge")));
            this->kryptonRibbonGroupButton1->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"kryptonRibbonGroupButton1.ImageSmall")));
            this->kryptonRibbonGroupButton1->TextLine1 = L"Textures";
            this->kryptonRibbonGroupButton1->TextLine2 = L"Database";
            // 
            // kryptonRibbonGroupButton2
            // 
            this->kryptonRibbonGroupButton2->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"kryptonRibbonGroupButton2.ImageLarge")));
            this->kryptonRibbonGroupButton2->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"kryptonRibbonGroupButton2.ImageSmall")));
            this->kryptonRibbonGroupButton2->TextLine1 = L"Textures";
            this->kryptonRibbonGroupButton2->TextLine2 = L"Converter";
            // 
            // kryptonRibbonGroupButton3
            // 
            this->kryptonRibbonGroupButton3->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"kryptonRibbonGroupButton3.ImageLarge")));
            this->kryptonRibbonGroupButton3->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"kryptonRibbonGroupButton3.ImageSmall")));
            this->kryptonRibbonGroupButton3->TextLine1 = L"Localization";
            this->kryptonRibbonGroupButton3->TextLine2 = L"Conversion";
            // 
            // kryptonRibbonGroupTriple4
            // 
            this->kryptonRibbonGroupTriple4->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupItem^  >(2) {
                this->ribbonBtnTexturesConverter,
                    this->ribbonBtnLocalizationConversion
            });
            // 
            // ribbonBtnTexturesConverter
            // 
            this->ribbonBtnTexturesConverter->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnTexturesConverter.ImageLarge")));
            this->ribbonBtnTexturesConverter->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnTexturesConverter.ImageSmall")));
            this->ribbonBtnTexturesConverter->TextLine1 = L"Textures";
            this->ribbonBtnTexturesConverter->TextLine2 = L"Converter";
            this->ribbonBtnTexturesConverter->Click += gcnew System::EventHandler(this, &MainForm::texturesConverterToolStripMenuItem_Click);
            // 
            // ribbonBtnLocalizationConversion
            // 
            this->ribbonBtnLocalizationConversion->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnLocalizationConversion.ImageLarge")));
            this->ribbonBtnLocalizationConversion->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnLocalizationConversion.ImageSmall")));
            this->ribbonBtnLocalizationConversion->TextLine1 = L"Localization";
            this->ribbonBtnLocalizationConversion->TextLine2 = L"Conversion";
            this->ribbonBtnLocalizationConversion->Click += gcnew System::EventHandler(this, &MainForm::localizationConversionToolStripMenuItem_Click);
            // 
            // kryptonRibbonGroupButton7
            // 
            this->kryptonRibbonGroupButton7->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"kryptonRibbonGroupButton7.ImageLarge")));
            this->kryptonRibbonGroupButton7->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"kryptonRibbonGroupButton7.ImageSmall")));
            this->kryptonRibbonGroupButton7->TextLine1 = L"Textures";
            this->kryptonRibbonGroupButton7->TextLine2 = L"Converter";
            // 
            // kryptonRibbonGroupTriple5
            // 
            this->kryptonRibbonGroupTriple5->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupItem^  >(1) { this->ribbonBtnArchiveTool });
            // 
            // ribbonBtnArchiveTool
            // 
            this->ribbonBtnArchiveTool->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnArchiveTool.ImageLarge")));
            this->ribbonBtnArchiveTool->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnArchiveTool.ImageSmall")));
            this->ribbonBtnArchiveTool->TextLine1 = L"Archive";
            this->ribbonBtnArchiveTool->TextLine2 = L"Tool";
            this->ribbonBtnArchiveTool->Click += gcnew System::EventHandler(this, &MainForm::archiveToolToolStripMenuItem_Click);
            // 
            // kryptonPanel1
            // 
            this->kryptonPanel1->Controls->Add(this->splitContainer1);
            this->kryptonPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->kryptonPanel1->Location = System::Drawing::Point(0, 143);
            this->kryptonPanel1->Name = L"kryptonPanel1";
            this->kryptonPanel1->Size = System::Drawing::Size(1159, 554);
            this->kryptonPanel1->TabIndex = 8;
            // 
            // viewTab
            // 
            this->viewTab->Groups->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroup^  >(1) { this->kryptonRibbonGroup1 });
            this->viewTab->Text = L"View";
            // 
            // kryptonRibbonGroup1
            // 
            this->kryptonRibbonGroup1->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupContainer^  >(5) {
                this->kryptonRibbonGroupTriple6,
                    this->kryptonRibbonGroupSeparator3, this->kryptonRibbonGroupTriple7, this->kryptonRibbonGroupSeparator4, this->kryptonRibbonGroupTriple8
            });
            this->kryptonRibbonGroup1->TextLine1 = L"View";
            // 
            // kryptonRibbonGroupTriple6
            // 
            this->kryptonRibbonGroupTriple6->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupItem^  >(1) { this->ribbonBtnEnableAlpha });
            // 
            // ribbonBtnEnableAlpha
            // 
            this->ribbonBtnEnableAlpha->ButtonType = ComponentFactory::Krypton::Ribbon::GroupButtonType::Check;
            this->ribbonBtnEnableAlpha->Checked = true;
            this->ribbonBtnEnableAlpha->TextLine1 = L"Enable";
            this->ribbonBtnEnableAlpha->TextLine2 = L"Alpha";
            this->ribbonBtnEnableAlpha->Click += gcnew System::EventHandler(this, &MainForm::toolBtnImgEnableAlpha_Click);
            // 
            // kryptonRibbonGroupTriple7
            // 
            this->kryptonRibbonGroupTriple7->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupItem^  >(2) {
                this->ribbonBtnShowWireframe,
                    this->ribbonBtnShowCollision
            });
            // 
            // ribbonBtnShowWireframe
            // 
            this->ribbonBtnShowWireframe->TextLine1 = L"Show";
            this->ribbonBtnShowWireframe->TextLine2 = L"Wireframe";
            this->ribbonBtnShowWireframe->Click += gcnew System::EventHandler(this, &MainForm::toolBtnMdlShowWireframe_Click);
            // 
            // ribbonBtnShowCollision
            // 
            this->ribbonBtnShowCollision->TextLine1 = L"Show";
            this->ribbonBtnShowCollision->TextLine2 = L"Collision";
            this->ribbonBtnShowCollision->Click += gcnew System::EventHandler(this, &MainForm::toolBtnMdlShowCollision_Click);
            // 
            // kryptonRibbonGroupTriple8
            // 
            this->kryptonRibbonGroupTriple8->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupItem^  >(1) { this->ribbonBtnResetCamera });
            // 
            // ribbonBtnResetCamera
            // 
            this->ribbonBtnResetCamera->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnResetCamera.ImageLarge")));
            this->ribbonBtnResetCamera->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnResetCamera.ImageSmall")));
            this->ribbonBtnResetCamera->TextLine1 = L"Reset";
            this->ribbonBtnResetCamera->TextLine2 = L"Camera";
            this->ribbonBtnResetCamera->Click += gcnew System::EventHandler(this, &MainForm::toolBtnMdlResetCamera_Click);
            // 
            // miscTab
            // 
            this->miscTab->Groups->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroup^  >(1) { this->kryptonRibbonGroup2 });
            this->miscTab->Text = L"Misc";
            // 
            // kryptonRibbonGroup2
            // 
            this->kryptonRibbonGroup2->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupContainer^  >(1) { this->kryptonRibbonGroupTriple9 });
            this->kryptonRibbonGroup2->TextLine1 = L" ";
            // 
            // kryptonRibbonGroupTriple9
            // 
            this->kryptonRibbonGroupTriple9->Items->AddRange(gcnew cli::array< ComponentFactory::Krypton::Ribbon::KryptonRibbonGroupItem^  >(2) {
                this->ribbonBtnSettings,
                    this->ribbonBtnAbout
            });
            // 
            // ribbonBtnSettings
            // 
            this->ribbonBtnSettings->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnSettings.ImageLarge")));
            this->ribbonBtnSettings->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnSettings.ImageSmall")));
            this->ribbonBtnSettings->TextLine1 = L"Settings";
            this->ribbonBtnSettings->Click += gcnew System::EventHandler(this, &MainForm::toolBtnSettings_Click);
            // 
            // ribbonBtnAbout
            // 
            this->ribbonBtnAbout->ImageLarge = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnAbout.ImageLarge")));
            this->ribbonBtnAbout->ImageSmall = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ribbonBtnAbout.ImageSmall")));
            this->ribbonBtnAbout->TextLine1 = L"About";
            this->ribbonBtnAbout->Click += gcnew System::EventHandler(this, &MainForm::toolBtnAbout_Click);
            // 
            // QATBtnSettings
            // 
            this->QATBtnSettings->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"QATBtnSettings.Image")));
            this->QATBtnSettings->Click += gcnew System::EventHandler(this, &MainForm::toolBtnSettings_Click);
            // 
            // QATBtnAbout
            // 
            this->QATBtnAbout->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"QATBtnAbout.Image")));
            this->QATBtnAbout->Click += gcnew System::EventHandler(this, &MainForm::toolBtnAbout_Click);
            // 
            // MainForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1159, 719);
            this->Controls->Add(this->kryptonPanel1);
            this->Controls->Add(this->ribbon);
            this->Controls->Add(this->statusStrip1);
            this->Name = L"MainForm";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"MetroEX - \"Metro Exodus\" files explorer";
            this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
            this->statusStrip1->ResumeLayout(false);
            this->statusStrip1->PerformLayout();
            this->splitContainer1->Panel1->ResumeLayout(false);
            this->splitContainer1->Panel1->PerformLayout();
            this->splitContainer1->Panel2->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
            this->splitContainer1->ResumeLayout(false);
            this->tableLayoutPanel2->ResumeLayout(false);
            this->ctxMenuExportTexture->ResumeLayout(false);
            this->ctxMenuExportModel->ResumeLayout(false);
            this->ctxMenuExportSound->ResumeLayout(false);
            this->ctxMenuExportRaw->ResumeLayout(false);
            this->ctxMenuExportFolder->ResumeLayout(false);
            this->toolStrip1->ResumeLayout(false);
            this->toolStrip1->PerformLayout();
            this->ctxMenuExportBin->ResumeLayout(false);
            this->ctxMenuExportLocalization->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->ribbon))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->kryptonPanel1))->EndInit();
            this->kryptonPanel1->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        void MainForm_Load(System::Object^ sender, System::EventArgs^ e);
        // toolstrip buttons
        void toolBtnFileOpen_Click(System::Object^ sender, System::EventArgs^ e);
        void toolBtnAbout_Click(System::Object^ sender, System::EventArgs^ e);
        void toolBtnImgEnableAlpha_Click(System::Object^  sender, System::EventArgs^ e);
        void toolBtnMdlShowWireframe_Click(System::Object^ sender, System::EventArgs^ e);
        void toolBtnMdlShowCollision_Click(System::Object^ sender, System::EventArgs^ e);
        void toolBtnMdlResetCamera_Click(System::Object^ sender, System::EventArgs^ e);
        // treeview
        void filterableTreeView_AfterSelect(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e);
        void filterableTreeView_AfterCollapse(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e);
        void filterableTreeView_AfterExpand(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e);
        void filterableTreeView_NodeMouseClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e);
        // context menu
        void extractFileToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void saveAsDDSToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void saveAsLegacyDDSToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void saveAsTGAToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void saveAsPNGToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void saveAsOBJToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void saveAsFBXToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void saveAsOGGToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void saveAsWAVToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void saveAsExcel2003XMLToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void extractBinRootToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void extractBinChunkToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);

        void extractFolderToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void extractFolderWithConversionToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);

    private:
        void UpdateFilesList();
        void AddFoldersRecursive(const MetroFile& dir, const size_t folderIdx, TreeNode^ rootItem, const size_t configBinIdx);
        void AddBinaryArchive(const MetroFile& mf, const size_t fileIdx, TreeNode^ rootItem);
        void DetectFileAndShow(const size_t fileIdx);
        void ShowTexture(const size_t fileIdx);
        void ShowModel(const size_t fileIdx);
        void ShowSound(const size_t fileIdx);
        void ShowLocalization(const size_t fileIdx);
        void SwitchViewPanel(PanelType t);
        void SwitchInfoPanel(PanelType t);

        // extraction
        bool EnsureExtractionOptions();
        CharString MakeFileOutputName(const MetroFile& mf, const FileExtractionCtx& ctx);
        void TextureSaveHelper(const fs::path& folderPath, const FileExtractionCtx& ctx, const CharString& name);
        bool ExtractFile(const FileExtractionCtx& ctx, const fs::path& outPath);
        bool ExtractTexture(const FileExtractionCtx& ctx, const fs::path& outPath);
        bool ExtractModel(const FileExtractionCtx& ctx, const fs::path& outPath);
        bool ExtractSound(const FileExtractionCtx& ctx, const fs::path& outPath);
        bool ExtractLocalization(const FileExtractionCtx& ctx, const fs::path& outPath);
        bool ExtractFolderComplete(const FileExtractionCtx& ctx, const fs::path& outPath);
        void ExtractionProcessFunc(Object^ folderPath);

        // property panels
        // model props
        void lstMdlPropMotions_SelectedIndexChanged(int selection);
        void btnMdlPropPlayStopAnim_Click(System::Object^ sender);
        void btnModelInfo_Click(System::Object^ sender);
        void OnDlgModelInfo_Closed(System::Object^ sender, System::EventArgs^ e);

        // patch creation
        void archiveToolToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void texturesConverterToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
        void localizationConversionToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);

        // textures database
        void toolBtnTexturesDatabase_Click(System::Object^ sender, System::EventArgs^ e);

        // settings
        void toolBtnSettings_Click(System::Object^ sender, System::EventArgs^ e);

    public:
        void ResetTreeView();
        bool FindAndSelect(String^ text, array<String^>^ extensions);
    };
}

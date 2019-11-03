using System;
using System.Windows.Forms;

namespace MetroEXControls {
    public abstract partial class MainWindow : Form {
        #region Abstract methods
        protected abstract void OnFormLoad();
        // toolbar
        protected abstract void OnOpenGameFolderClicked();
        protected abstract void OnOpenSingleVFXClicked();
        protected abstract void OnImgEnableAlphaClick();
        protected abstract void OnMdlShowWireframeClick();
        protected abstract void OnBtnMdlShowCollisionClick();
        protected abstract void OnBtnMdlResetCameraClick();
        // files tree
        protected abstract void OnFilesTreeNodeMouseClick(TreeNodeMouseClickEventArgs e);
        protected abstract void OnFilesTreeAfterCollapse(TreeNode node);
        protected abstract void OnFilesTreeAfterExpand(TreeNode node);
        protected abstract void OnFilesTreeAfterSelect(TreeNode node);
        // tools
        protected abstract void OnTexturesDatabaseClick();
        protected abstract void OnTexturesConverterClick();
        protected abstract void OnLocalizationConversionClick();
        protected abstract void OnArchiveToolClick();
        // settings
        protected abstract void OnSettingsClick();
        // extraction
        //  raw
        protected abstract void OnExtractRAWFileClicked();
        //  textures
        protected abstract void OnExtractTextureDDSClicked();
        protected abstract void OnExtractTextureLegacyDDSClicked();
        protected abstract void OnExtractTextureTGAClicked();
        protected abstract void OnExtractTexturePNGClicked();
        protected abstract void OnExtractSurfaceSetClicked();
        //  models
        protected abstract void OnExtractModelOBJClicked();
        protected abstract void OnExtractModelFBXClicked();
        //  sound
        protected abstract void OnExtractSoundOGGClicked();
        protected abstract void OnExtractSoundWAVClicked();
        //  localization
        protected abstract void OnExtractLocalizationExcelClicked();
        //  bin
        protected abstract void OnExtractBinRootFileClicked();
        protected abstract void OnExtractBinInnerFileClicked();
        //  folder
        protected abstract void OnExtractFolderClicked(bool withConversion);
        #endregion

        public MainWindow() {
            InitializeComponent();

            this.filterableTreeView.TreeView.ImageList = this.imageListMain;
            this.filterableTreeView.TreeView.NodeMouseClick += this.TreeView_NodeMouseClick;
            this.filterableTreeView.TreeView.AfterCollapse += this.TreeView_AfterCollapse;
            this.filterableTreeView.TreeView.AfterExpand += this.TreeView_AfterExpand;
            this.filterableTreeView.TreeView.AfterSelect += this.TreeView_AfterSelect;
        }

        private void MainWindow_Load(object sender, EventArgs e) {
            this.OnFormLoad();
        }

        private void toolBtnAbout_Click(object sender, EventArgs e) {
            AboutDlg dlg = new AboutDlg();
            dlg.Icon = this.Icon;
            dlg.Text = this.Text;
            dlg.ShowDialog(this);
        }

        private void toolBtnFileOpen_ButtonClick(object sender, EventArgs e) {
            this.OnOpenGameFolderClicked();
        }

        private void openGameFolderToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnOpenGameFolderClicked();
        }

        private void openSingleVFXArchiveToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnOpenSingleVFXClicked();
        }

        private void toolBtnImgEnableAlpha_Click(object sender, EventArgs e) {
            this.OnImgEnableAlphaClick();
        }

        private void toolBtnMdlShowWireframe_Click(object sender, EventArgs e) {
            this.OnMdlShowWireframeClick();
        }

        private void toolBtnMdlShowCollision_Click(object sender, EventArgs e) {
            this.OnBtnMdlShowCollisionClick();
        }

        private void toolBtnMdlResetCamera_Click(object sender, EventArgs e) {
            this.OnBtnMdlResetCameraClick();
        }

        private void TreeView_AfterSelect(object sender, TreeViewEventArgs e) {
            this.OnFilesTreeAfterSelect(e.Node);
        }

        private void TreeView_AfterExpand(object sender, TreeViewEventArgs e) {
            this.OnFilesTreeAfterExpand(e.Node);
        }

        private void TreeView_AfterCollapse(object sender, TreeViewEventArgs e) {
            this.OnFilesTreeAfterCollapse(e.Node);
        }

        private void TreeView_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e) {
            this.OnFilesTreeNodeMouseClick(e);
        }

        private void texturesConverterToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnTexturesConverterClick();
        }

        private void localizationConversionToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnLocalizationConversionClick();
        }

        private void archiveToolToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnArchiveToolClick();
        }

        private void toolBtnTexturesDatabase_Click(object sender, EventArgs e) {
            this.OnTexturesDatabaseClick();
        }

        private void toolBtnSettings_Click(object sender, EventArgs e) {
            this.OnSettingsClick();
        }

        private void extractFileToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractRAWFileClicked();
        }

        private void saveAsDDSToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractTextureDDSClicked();
        }

        private void saveAsLegacyDDSToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractTextureLegacyDDSClicked();
        }

        private void saveAsTGAToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractTextureTGAClicked();
        }

        private void saveAsPNGToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractTexturePNGClicked();
        }

        private void saveSurfaceSetToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractSurfaceSetClicked();
        }

        private void saveAsOBJToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractModelOBJClicked();
        }

        private void saveAsFBXToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractModelFBXClicked();
        }

        private void saveAsOGGToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractSoundOGGClicked();
        }

        private void saveAsWAVToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractSoundWAVClicked();
        }

        private void saveAsExcel2003XMLToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractLocalizationExcelClicked();
        }

        private void extractBinRootToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractBinRootFileClicked();
        }

        private void extractBinChunkToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractBinInnerFileClicked();
        }

        private void extractFolderToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractFolderClicked(false);
        }

        private void extractFolderWithConversionToolStripMenuItem_Click(object sender, EventArgs e) {
            this.OnExtractFolderClicked(true);
        }
    }
}

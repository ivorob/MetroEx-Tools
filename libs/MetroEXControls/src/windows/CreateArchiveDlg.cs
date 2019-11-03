using System;
using System.Windows.Forms;

namespace MetroEXControls {
    public abstract partial class CreateArchiveDlg : Form {
        #region Abstract methods
        // text fields
        protected abstract void OnSourceFolderChanged();
        protected abstract void OnTargetPathChanged();
        protected abstract void OnBlobFilenameChanged();
        // buttons
        protected abstract void OnChooseSrcFolderClick();
        protected abstract void OnChooseTargetClick();
        protected abstract void OnCreateArchiveClick();
        protected abstract void OnRadioButtonsCheckedChanged();
        #endregion

        public CreateArchiveDlg() {
            InitializeComponent();
        }

        private void txtSourceFolder_TextChanged(object sender, EventArgs e) {
            this.OnSourceFolderChanged();
        }

        private void txtTargetPath_TextChanged(object sender, EventArgs e) {
            this.OnTargetPathChanged();
        }

        private void txtBlobFileName_TextChanged(object sender, EventArgs e) {
            this.OnBlobFilenameChanged();
        }

        private void btnChooseSrcFolder_Click(object sender, EventArgs e) {
            this.OnChooseSrcFolderClick();
        }

        private void btnChooseTarget_Click(object sender, EventArgs e) {
            this.OnChooseTargetClick();
        }

        private void btnCreateArchive_Click(object sender, EventArgs e) {
            this.OnCreateArchiveClick();
        }

        private void radioButtons_CheckedChanged(object sender, EventArgs e) {
            this.OnRadioButtonsCheckedChanged();
        }
    }
}

using System;
using System.IO;
using System.Windows.Forms;

namespace MetroEXControls {
    public abstract partial class ConvertTexturesDlg : Form {
        #region Abstract methods
        protected abstract void OnChooseFolderClicked();
        protected abstract void OnConvertClicked();
        protected abstract void OnStopClicked();
        #endregion

        public ConvertTexturesDlg() {
            InitializeComponent();
        }

        private void txtPath_TextChanged(object sender, EventArgs e) {
            string path = this.txtPath.Text;
            bool exists = path.Length > 0;
            if (exists) {
                exists = File.Exists(path) ? true : Directory.Exists(path);
            }
            this.btnConvert.Enabled = exists;
        }

        private void btnChooseFile_Click(object sender, EventArgs e) {
            using (OpenFileDialog ofd = new OpenFileDialog()) {
                ofd.Title = @"Open image file...";
                ofd.Filter = @"Image files (*.tga;*.png)|*.tga;*.png";
                ofd.FilterIndex = 0;
                ofd.RestoreDirectory = true;
                if (ofd.ShowDialog(this) == DialogResult.OK) {
                    this.txtPath.Text = ofd.FileName;
                }
            }
        }

        private void btnChooseFolder_Click(object sender, EventArgs e) {
            //#NOTE_SK: this one we pass to the C++ side as it already has code to show modern folder browser
            this.OnChooseFolderClicked();
        }

        private void btnConvert_Click(object sender, EventArgs e) {
            this.OnConvertClicked();
        }

        private void btnStop_Click(object sender, EventArgs e) {
            this.OnStopClicked();
        }

        private void ConvertTexturesDlg_FormClosing(object sender, FormClosingEventArgs e) {
            this.OnStopClicked();
        }
    }
}

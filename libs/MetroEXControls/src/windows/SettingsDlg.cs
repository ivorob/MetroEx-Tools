using System;
using System.Windows.Forms;

namespace MetroEXControls {
    public abstract partial class SettingsDlg : Form {
        #region Abstract methods
        protected abstract void OnFormLoaded();
        protected abstract void OnOKButtonClicked();
        #endregion

        public SettingsDlg() {
            InitializeComponent();
        }

        private void SettingsDlg_Load(object sender, EventArgs e) {
            this.OnFormLoaded();
        }

        private void btnOK_Click(object sender, EventArgs e) {
            this.OnOKButtonClicked();
        }

        private void btnCancel_Click(object sender, EventArgs e) {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
    }
}

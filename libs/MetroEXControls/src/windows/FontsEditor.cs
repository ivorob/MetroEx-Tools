using System;
using System.Windows.Forms;

namespace MetroEXControls {
    public abstract partial class FontsEditor : Form {
        #region Abstract methods
        protected abstract void OnFormLoaded();
        protected abstract void OnLanguageSelected(int idx);
        protected abstract void OnFontSelected(int idx);
        protected abstract void OnCharValueChanged(object sender);
        protected abstract void OnLanguageExport();
        protected abstract void OnLanguageImport();
        protected abstract void OnLanguageSave();
        protected abstract void OnTestFont();
        #endregion

        public FontsEditor() {
            InitializeComponent();
        }

        private void FontsEditor_Load(object sender, EventArgs e) {
            this.OnFormLoaded();
        }

        private void cmbLanguages_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.cmbLanguages.SelectedIndex >= 0) {
                this.OnLanguageSelected(this.cmbLanguages.SelectedIndex);
            }
        }

        private void lstFonts_SelectedIndexChanged(object sender, EventArgs e) {
            if (this.lstFonts.SelectedIndex >= 0) {
                this.OnFontSelected(this.lstFonts.SelectedIndex);
            }
        }

        private void charNumValueChanged(object sender, EventArgs e) {
            this.OnCharValueChanged(sender);
        }

        private void btnExportLanguage_Click(object sender, EventArgs e) {
            this.OnLanguageExport();
        }

        private void btnImportLanguage_Click(object sender, EventArgs e) {
            this.OnLanguageImport();
        }

        private void btnSaveLanguage_Click(object sender, EventArgs e) {
            this.OnLanguageSave();
        }

        private void btnTest_Click(object sender, EventArgs e) {
            this.OnTestFont();
        }
    }
}

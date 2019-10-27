using System;
using System.Windows.Forms;

namespace MetroEXControls {
    public partial class AboutDlg : Form {
        public AboutDlg() {
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e) {
            this.Close();
        }

        private void AboutDlg_Load(object sender, EventArgs e) {
            byte[] byteArray = System.Text.Encoding.ASCII.GetBytes(MetroEXControls.Properties.Resources.credits);
            using (System.IO.MemoryStream stream = new System.IO.MemoryStream(byteArray)) {
                this.rtfCredits.LoadFile(stream, RichTextBoxStreamType.RichText);
            }
        }
    }
}

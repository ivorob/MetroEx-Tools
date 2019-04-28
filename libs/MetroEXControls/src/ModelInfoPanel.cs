using System;
using System.Windows.Forms;

namespace MetroEXControls {
    public partial class ModelInfoPanel : UserControl {
        public delegate void OnButtonClicked(Object sender);
        public delegate void OnListSelectionChanged(int selection);

        public OnButtonClicked OnPlayButtonClicked {
            get;
            set;
        }

        public OnButtonClicked OnInfoButtonClicked {
            get;
            set;
        }

        public OnListSelectionChanged OnMotionsListSelectionChanged {
            get;
            set;
        }

        public ModelInfoPanel() {
            InitializeComponent();

            this.OnPlayButtonClicked = null;
            this.OnInfoButtonClicked = null;
            this.OnMotionsListSelectionChanged = null;
        }

        public String MdlPropTypeText {
            set {
                this.lblMdlPropType.Text = value;
            }
        }

        public String MdlPropVerticesText {
            set {
                this.lblMdlPropVertices.Text = value;
            }
        }

        public String MdlPropTrianglesText {
            set {
                this.lblMdlPropTriangles.Text = value;
            }
        }

        public String MdlPropJointsText {
            set {
                this.lblMdlPropJoints.Text = value;
            }
        }

        public String MdlPropNumAnimsText {
            set {
                this.lblMdlPropNumAnims.Text = value;
            }
        }

        public String MdlPropPlayStopAnimBtnText {
            set {
                this.btnMdlPropPlayStopAnim.Text = value;
            }
        }

        public void ClearMotionsList() {
            this.lstMdlPropMotions.Items.Clear();
        }

        public void AddMotionToList(String name) {
            this.lstMdlPropMotions.Items.Add(name);
        }

        private void btnMdlPropPlayStopAnim_Click(object sender, EventArgs e) {
            this.OnPlayButtonClicked?.Invoke(sender);
        }

        private void btnModelInfo_Click(object sender, EventArgs e) {
            this.OnInfoButtonClicked?.Invoke(sender);
        }

        private void lstMdlPropMotions_SelectedIndexChanged(object sender, EventArgs e) {
            this.OnMotionsListSelectionChanged?.Invoke(this.lstMdlPropMotions.SelectedIndex);
        }
    }
}

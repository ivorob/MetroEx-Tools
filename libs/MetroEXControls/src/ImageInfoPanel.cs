using System;
using System.Windows.Forms;

namespace MetroEXControls {
    public partial class ImageInfoPanel : UserControl {
        public ImageInfoPanel() {
            InitializeComponent();
        }

        public String ImgPropCompressionText {
            set {
                this.lblImgPropCompression.Text = value;
            }
        }

        public String ImgPropWidthText {
            set {
                this.lblImgPropWidth.Text = value;
            }
        }

        public String ImgPropHeightText {
            set {
                this.lblImgPropHeight.Text = value;
            }
        }

        public String ImgPropsMipsText {
            set {
                this.lblImgPropMips.Text = value;
            }
        }
    }
}

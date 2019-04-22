namespace MetroEXControls
{
    partial class ImageInfoPanel
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.pnlImageProps = new System.Windows.Forms.Panel();
            this.lblImgPropMips = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.lblImgPropHeight = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.lblImgPropWidth = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.lblImgPropCompression = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.pnlImageProps.SuspendLayout();
            this.SuspendLayout();
            // 
            // pnlImageProps
            // 
            this.pnlImageProps.Controls.Add(this.lblImgPropMips);
            this.pnlImageProps.Controls.Add(this.label5);
            this.pnlImageProps.Controls.Add(this.lblImgPropHeight);
            this.pnlImageProps.Controls.Add(this.label4);
            this.pnlImageProps.Controls.Add(this.lblImgPropWidth);
            this.pnlImageProps.Controls.Add(this.label2);
            this.pnlImageProps.Controls.Add(this.lblImgPropCompression);
            this.pnlImageProps.Controls.Add(this.label1);
            this.pnlImageProps.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlImageProps.Location = new System.Drawing.Point(0, 0);
            this.pnlImageProps.Margin = new System.Windows.Forms.Padding(0);
            this.pnlImageProps.Name = "pnlImageProps";
            this.pnlImageProps.Size = new System.Drawing.Size(745, 150);
            this.pnlImageProps.TabIndex = 1;
            // 
            // lblImgPropMips
            // 
            this.lblImgPropMips.AutoSize = true;
            this.lblImgPropMips.Location = new System.Drawing.Point(81, 52);
            this.lblImgPropMips.Name = "lblImgPropMips";
            this.lblImgPropMips.Size = new System.Drawing.Size(19, 13);
            this.lblImgPropMips.TabIndex = 7;
            this.lblImgPropMips.Text = "10";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(4, 52);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(32, 13);
            this.label5.TabIndex = 6;
            this.label5.Text = "Mips:";
            // 
            // lblImgPropHeight
            // 
            this.lblImgPropHeight.AutoSize = true;
            this.lblImgPropHeight.Location = new System.Drawing.Point(81, 36);
            this.lblImgPropHeight.Name = "lblImgPropHeight";
            this.lblImgPropHeight.Size = new System.Drawing.Size(31, 13);
            this.lblImgPropHeight.TabIndex = 5;
            this.lblImgPropHeight.Text = "2048";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(4, 36);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "Height:";
            // 
            // lblImgPropWidth
            // 
            this.lblImgPropWidth.AutoSize = true;
            this.lblImgPropWidth.Location = new System.Drawing.Point(81, 20);
            this.lblImgPropWidth.Name = "lblImgPropWidth";
            this.lblImgPropWidth.Size = new System.Drawing.Size(31, 13);
            this.lblImgPropWidth.TabIndex = 3;
            this.lblImgPropWidth.Text = "2048";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(4, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Width:";
            // 
            // lblImgPropCompression
            // 
            this.lblImgPropCompression.AutoSize = true;
            this.lblImgPropCompression.Location = new System.Drawing.Point(81, 4);
            this.lblImgPropCompression.Name = "lblImgPropCompression";
            this.lblImgPropCompression.Size = new System.Drawing.Size(35, 13);
            this.lblImgPropCompression.TabIndex = 1;
            this.lblImgPropCompression.Text = "BC6H";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(4, 4);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(70, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Compression:";
            // 
            // ImageInfoPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pnlImageProps);
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Name = "ImageInfoPanel";
            this.Size = new System.Drawing.Size(745, 150);
            this.pnlImageProps.ResumeLayout(false);
            this.pnlImageProps.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel pnlImageProps;
        private System.Windows.Forms.Label lblImgPropMips;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label lblImgPropHeight;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label lblImgPropWidth;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lblImgPropCompression;
        private System.Windows.Forms.Label label1;
    }
}

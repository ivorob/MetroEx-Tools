namespace MetroEXControls
{
    partial class CreateArchiveDlg
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radioModifyArchive = new System.Windows.Forms.RadioButton();
            this.radioCreateNewArchive = new System.Windows.Forms.RadioButton();
            this.btnChooseSrcFolder = new System.Windows.Forms.Button();
            this.txtSourceFolder = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.prbProgress = new System.Windows.Forms.ProgressBar();
            this.label5 = new System.Windows.Forms.Label();
            this.btnCreateArchive = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.txtBlobFileName = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.btnChooseTarget = new System.Windows.Forms.Button();
            this.txtTargetPath = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.radioArchiveTypeRedux = new System.Windows.Forms.RadioButton();
            this.radioArchiveTypeExodus = new System.Windows.Forms.RadioButton();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radioModifyArchive);
            this.groupBox1.Controls.Add(this.radioCreateNewArchive);
            this.groupBox1.Location = new System.Drawing.Point(94, 52);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(156, 81);
            this.groupBox1.TabIndex = 16;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Operation type:";
            // 
            // radioModifyArchive
            // 
            this.radioModifyArchive.AutoSize = true;
            this.radioModifyArchive.Location = new System.Drawing.Point(7, 48);
            this.radioModifyArchive.Name = "radioModifyArchive";
            this.radioModifyArchive.Size = new System.Drawing.Size(132, 17);
            this.radioModifyArchive.TabIndex = 1;
            this.radioModifyArchive.Text = "Modify existing archive";
            this.radioModifyArchive.UseVisualStyleBackColor = true;
            // 
            // radioCreateNewArchive
            // 
            this.radioCreateNewArchive.AutoSize = true;
            this.radioCreateNewArchive.Checked = true;
            this.radioCreateNewArchive.Location = new System.Drawing.Point(7, 21);
            this.radioCreateNewArchive.Name = "radioCreateNewArchive";
            this.radioCreateNewArchive.Size = new System.Drawing.Size(117, 17);
            this.radioCreateNewArchive.TabIndex = 0;
            this.radioCreateNewArchive.TabStop = true;
            this.radioCreateNewArchive.Text = "Create new archive";
            this.radioCreateNewArchive.UseVisualStyleBackColor = true;
            this.radioCreateNewArchive.CheckedChanged += new System.EventHandler(this.radioButtons_CheckedChanged);
            // 
            // btnChooseSrcFolder
            // 
            this.btnChooseSrcFolder.Location = new System.Drawing.Point(550, 21);
            this.btnChooseSrcFolder.Name = "btnChooseSrcFolder";
            this.btnChooseSrcFolder.Size = new System.Drawing.Size(75, 22);
            this.btnChooseSrcFolder.TabIndex = 15;
            this.btnChooseSrcFolder.Text = "Choose...";
            this.btnChooseSrcFolder.UseVisualStyleBackColor = true;
            this.btnChooseSrcFolder.Click += new System.EventHandler(this.btnChooseSrcFolder_Click);
            // 
            // txtSourceFolder
            // 
            this.txtSourceFolder.Location = new System.Drawing.Point(11, 22);
            this.txtSourceFolder.Name = "txtSourceFolder";
            this.txtSourceFolder.Size = new System.Drawing.Size(533, 20);
            this.txtSourceFolder.TabIndex = 14;
            this.txtSourceFolder.TextChanged += new System.EventHandler(this.txtSourceFolder_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(73, 13);
            this.label1.TabIndex = 13;
            this.label1.Text = "Source folder:";
            // 
            // prbProgress
            // 
            this.prbProgress.Location = new System.Drawing.Point(11, 256);
            this.prbProgress.Name = "prbProgress";
            this.prbProgress.Size = new System.Drawing.Size(533, 23);
            this.prbProgress.TabIndex = 25;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(8, 240);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(51, 13);
            this.label5.TabIndex = 24;
            this.label5.Text = "Progress:";
            // 
            // btnCreateArchive
            // 
            this.btnCreateArchive.Enabled = false;
            this.btnCreateArchive.Location = new System.Drawing.Point(550, 256);
            this.btnCreateArchive.Name = "btnCreateArchive";
            this.btnCreateArchive.Size = new System.Drawing.Size(75, 23);
            this.btnCreateArchive.TabIndex = 23;
            this.btnCreateArchive.Text = "Create !";
            this.btnCreateArchive.UseVisualStyleBackColor = true;
            this.btnCreateArchive.Click += new System.EventHandler(this.btnCreateArchive_Click);
            // 
            // label4
            // 
            this.label4.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label4.Location = new System.Drawing.Point(12, 233);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(614, 2);
            this.label4.TabIndex = 22;
            // 
            // txtBlobFileName
            // 
            this.txtBlobFileName.Enabled = false;
            this.txtBlobFileName.Location = new System.Drawing.Point(11, 204);
            this.txtBlobFileName.Name = "txtBlobFileName";
            this.txtBlobFileName.Size = new System.Drawing.Size(533, 20);
            this.txtBlobFileName.TabIndex = 21;
            this.txtBlobFileName.TextChanged += new System.EventHandler(this.txtBlobFileName_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(8, 188);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(244, 13);
            this.label3.TabIndex = 20;
            this.label3.Text = "Blob file name (extend existing archive mode only):";
            // 
            // btnChooseTarget
            // 
            this.btnChooseTarget.Location = new System.Drawing.Point(550, 160);
            this.btnChooseTarget.Name = "btnChooseTarget";
            this.btnChooseTarget.Size = new System.Drawing.Size(75, 22);
            this.btnChooseTarget.TabIndex = 19;
            this.btnChooseTarget.Text = "Choose...";
            this.btnChooseTarget.UseVisualStyleBackColor = true;
            this.btnChooseTarget.Click += new System.EventHandler(this.btnChooseTarget_Click);
            // 
            // txtTargetPath
            // 
            this.txtTargetPath.Location = new System.Drawing.Point(11, 161);
            this.txtTargetPath.Name = "txtTargetPath";
            this.txtTargetPath.Size = new System.Drawing.Size(533, 20);
            this.txtTargetPath.TabIndex = 18;
            this.txtTargetPath.TextChanged += new System.EventHandler(this.txtTargetPath_TextChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 145);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(103, 13);
            this.label2.TabIndex = 17;
            this.label2.Text = "Target archive path:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.radioArchiveTypeRedux);
            this.groupBox2.Controls.Add(this.radioArchiveTypeExodus);
            this.groupBox2.Location = new System.Drawing.Point(288, 52);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(200, 81);
            this.groupBox2.TabIndex = 26;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Archive type:";
            // 
            // radioArchiveTypeRedux
            // 
            this.radioArchiveTypeRedux.AutoSize = true;
            this.radioArchiveTypeRedux.Location = new System.Drawing.Point(7, 48);
            this.radioArchiveTypeRedux.Name = "radioArchiveTypeRedux";
            this.radioArchiveTypeRedux.Size = new System.Drawing.Size(167, 17);
            this.radioArchiveTypeRedux.TabIndex = 1;
            this.radioArchiveTypeRedux.Text = "Metro 2033 / Last Ligh Redux";
            this.radioArchiveTypeRedux.UseVisualStyleBackColor = true;
            // 
            // radioArchiveTypeExodus
            // 
            this.radioArchiveTypeExodus.AutoSize = true;
            this.radioArchiveTypeExodus.Checked = true;
            this.radioArchiveTypeExodus.Location = new System.Drawing.Point(7, 21);
            this.radioArchiveTypeExodus.Name = "radioArchiveTypeExodus";
            this.radioArchiveTypeExodus.Size = new System.Drawing.Size(90, 17);
            this.radioArchiveTypeExodus.TabIndex = 0;
            this.radioArchiveTypeExodus.TabStop = true;
            this.radioArchiveTypeExodus.Text = "Metro Exodus";
            this.radioArchiveTypeExodus.UseVisualStyleBackColor = true;
            // 
            // CreateArchiveDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(639, 295);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnChooseSrcFolder);
            this.Controls.Add(this.txtSourceFolder);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.prbProgress);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.btnCreateArchive);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.txtBlobFileName);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.btnChooseTarget);
            this.Controls.Add(this.txtTargetPath);
            this.Controls.Add(this.label2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "CreateArchiveDlg";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Archive creation/modification tool";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        protected System.Windows.Forms.GroupBox groupBox1;
        protected System.Windows.Forms.RadioButton radioModifyArchive;
        protected System.Windows.Forms.RadioButton radioCreateNewArchive;
        protected System.Windows.Forms.Button btnChooseSrcFolder;
        protected System.Windows.Forms.TextBox txtSourceFolder;
        protected System.Windows.Forms.Label label1;
        protected System.Windows.Forms.ProgressBar prbProgress;
        protected System.Windows.Forms.Label label5;
        protected System.Windows.Forms.Button btnCreateArchive;
        protected System.Windows.Forms.Label label4;
        protected System.Windows.Forms.TextBox txtBlobFileName;
        protected System.Windows.Forms.Label label3;
        protected System.Windows.Forms.Button btnChooseTarget;
        protected System.Windows.Forms.TextBox txtTargetPath;
        protected System.Windows.Forms.Label label2;
        protected System.Windows.Forms.GroupBox groupBox2;
        protected System.Windows.Forms.RadioButton radioArchiveTypeRedux;
        protected System.Windows.Forms.RadioButton radioArchiveTypeExodus;
    }
}
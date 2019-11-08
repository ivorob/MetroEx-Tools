namespace MetroEXControls
{
    partial class ConvertTexturesDlg
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
            this.txtLog = new System.Windows.Forms.TextBox();
            this.btnStop = new System.Windows.Forms.Button();
            this.btnConvert = new System.Windows.Forms.Button();
            this.chkWithSubfolders = new System.Windows.Forms.CheckBox();
            this.prbProgress = new System.Windows.Forms.ProgressBar();
            this.label2 = new System.Windows.Forms.Label();
            this.btnChooseFile = new System.Windows.Forms.Button();
            this.btnChooseFolder = new System.Windows.Forms.Button();
            this.txtPath = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radioFormatRedux = new System.Windows.Forms.RadioButton();
            this.radioFormatExodus = new System.Windows.Forms.RadioButton();
            this.chkCrunched = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // txtLog
            // 
            this.txtLog.Location = new System.Drawing.Point(13, 264);
            this.txtLog.Multiline = true;
            this.txtLog.Name = "txtLog";
            this.txtLog.ReadOnly = true;
            this.txtLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtLog.Size = new System.Drawing.Size(636, 125);
            this.txtLog.TabIndex = 19;
            // 
            // btnStop
            // 
            this.btnStop.Enabled = false;
            this.btnStop.Location = new System.Drawing.Point(366, 233);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(122, 23);
            this.btnStop.TabIndex = 18;
            this.btnStop.Text = "Stop";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // btnConvert
            // 
            this.btnConvert.Enabled = false;
            this.btnConvert.Location = new System.Drawing.Point(528, 233);
            this.btnConvert.Name = "btnConvert";
            this.btnConvert.Size = new System.Drawing.Size(122, 23);
            this.btnConvert.TabIndex = 17;
            this.btnConvert.Text = "Convert !";
            this.btnConvert.UseVisualStyleBackColor = true;
            this.btnConvert.Click += new System.EventHandler(this.btnConvert_Click);
            // 
            // chkWithSubfolders
            // 
            this.chkWithSubfolders.AutoSize = true;
            this.chkWithSubfolders.Checked = true;
            this.chkWithSubfolders.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkWithSubfolders.Location = new System.Drawing.Point(13, 59);
            this.chkWithSubfolders.Name = "chkWithSubfolders";
            this.chkWithSubfolders.Size = new System.Drawing.Size(99, 17);
            this.chkWithSubfolders.TabIndex = 16;
            this.chkWithSubfolders.Text = "With subfolders";
            this.chkWithSubfolders.UseVisualStyleBackColor = true;
            // 
            // prbProgress
            // 
            this.prbProgress.Location = new System.Drawing.Point(13, 203);
            this.prbProgress.Name = "prbProgress";
            this.prbProgress.Size = new System.Drawing.Size(637, 23);
            this.prbProgress.TabIndex = 15;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(10, 187);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(51, 13);
            this.label2.TabIndex = 14;
            this.label2.Text = "Progress:";
            // 
            // btnChooseFile
            // 
            this.btnChooseFile.Location = new System.Drawing.Point(366, 55);
            this.btnChooseFile.Name = "btnChooseFile";
            this.btnChooseFile.Size = new System.Drawing.Size(122, 23);
            this.btnChooseFile.TabIndex = 13;
            this.btnChooseFile.Text = "Choose file...";
            this.btnChooseFile.UseVisualStyleBackColor = true;
            this.btnChooseFile.Click += new System.EventHandler(this.btnChooseFile_Click);
            // 
            // btnChooseFolder
            // 
            this.btnChooseFolder.Location = new System.Drawing.Point(528, 55);
            this.btnChooseFolder.Name = "btnChooseFolder";
            this.btnChooseFolder.Size = new System.Drawing.Size(122, 23);
            this.btnChooseFolder.TabIndex = 12;
            this.btnChooseFolder.Text = "Choose folder...";
            this.btnChooseFolder.UseVisualStyleBackColor = true;
            this.btnChooseFolder.Click += new System.EventHandler(this.btnChooseFolder_Click);
            // 
            // txtPath
            // 
            this.txtPath.Location = new System.Drawing.Point(13, 29);
            this.txtPath.Name = "txtPath";
            this.txtPath.Size = new System.Drawing.Size(637, 20);
            this.txtPath.TabIndex = 11;
            this.txtPath.TextChanged += new System.EventHandler(this.txtPath_TextChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(32, 13);
            this.label1.TabIndex = 10;
            this.label1.Text = "Path:";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.chkCrunched);
            this.groupBox1.Controls.Add(this.radioFormatExodus);
            this.groupBox1.Controls.Add(this.radioFormatRedux);
            this.groupBox1.Location = new System.Drawing.Point(232, 84);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(200, 98);
            this.groupBox1.TabIndex = 20;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Settings:";
            // 
            // radioFormatRedux
            // 
            this.radioFormatRedux.AutoSize = true;
            this.radioFormatRedux.Location = new System.Drawing.Point(7, 20);
            this.radioFormatRedux.Name = "radioFormatRedux";
            this.radioFormatRedux.Size = new System.Drawing.Size(86, 17);
            this.radioFormatRedux.TabIndex = 0;
            this.radioFormatRedux.Text = "Metro Redux";
            this.radioFormatRedux.UseVisualStyleBackColor = true;
            this.radioFormatRedux.CheckedChanged += new System.EventHandler(this.FormatChanged);
            // 
            // radioFormatExodus
            // 
            this.radioFormatExodus.AutoSize = true;
            this.radioFormatExodus.Checked = true;
            this.radioFormatExodus.Location = new System.Drawing.Point(7, 73);
            this.radioFormatExodus.Name = "radioFormatExodus";
            this.radioFormatExodus.Size = new System.Drawing.Size(90, 17);
            this.radioFormatExodus.TabIndex = 1;
            this.radioFormatExodus.TabStop = true;
            this.radioFormatExodus.Text = "Metro Exodus";
            this.radioFormatExodus.UseVisualStyleBackColor = true;
            this.radioFormatExodus.CheckedChanged += new System.EventHandler(this.FormatChanged);
            // 
            // chkCrunched
            // 
            this.chkCrunched.AutoSize = true;
            this.chkCrunched.Enabled = false;
            this.chkCrunched.Location = new System.Drawing.Point(7, 43);
            this.chkCrunched.Name = "chkCrunched";
            this.chkCrunched.Size = new System.Drawing.Size(72, 17);
            this.chkCrunched.TabIndex = 2;
            this.chkCrunched.Text = "Crunched";
            this.chkCrunched.UseVisualStyleBackColor = true;
            // 
            // ConvertTexturesDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(661, 397);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.txtLog);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.btnConvert);
            this.Controls.Add(this.chkWithSubfolders);
            this.Controls.Add(this.prbProgress);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.btnChooseFile);
            this.Controls.Add(this.btnChooseFolder);
            this.Controls.Add(this.txtPath);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ConvertTexturesDlg";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Textures converter";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ConvertTexturesDlg_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        protected System.Windows.Forms.TextBox txtLog;
        protected System.Windows.Forms.Button btnStop;
        protected System.Windows.Forms.Button btnConvert;
        protected System.Windows.Forms.CheckBox chkWithSubfolders;
        protected System.Windows.Forms.ProgressBar prbProgress;
        protected System.Windows.Forms.Label label2;
        protected System.Windows.Forms.Button btnChooseFile;
        protected System.Windows.Forms.Button btnChooseFolder;
        protected System.Windows.Forms.TextBox txtPath;
        protected System.Windows.Forms.Label label1;
        protected System.Windows.Forms.GroupBox groupBox1;
        protected System.Windows.Forms.CheckBox chkCrunched;
        protected System.Windows.Forms.RadioButton radioFormatExodus;
        protected System.Windows.Forms.RadioButton radioFormatRedux;
    }
}
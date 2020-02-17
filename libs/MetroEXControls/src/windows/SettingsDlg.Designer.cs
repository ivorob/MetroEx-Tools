namespace MetroEXControls
{
    partial class SettingsDlg
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
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.radioTexAsPNG = new System.Windows.Forms.RadioButton();
            this.radioTexAsTGA = new System.Windows.Forms.RadioButton();
            this.radioTexAsLegacyDDS = new System.Windows.Forms.RadioButton();
            this.radioTexAsDDS = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.chkMdlExportLods = new System.Windows.Forms.CheckBox();
            this.chkMdlSaveSurfaceTextures = new System.Windows.Forms.CheckBox();
            this.chkMdlExcludeCollision = new System.Windows.Forms.CheckBox();
            this.chkMdlSaveWithTextures = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.chkMdlExportAnimsSeparate = new System.Windows.Forms.CheckBox();
            this.chkMdlExportAnims = new System.Windows.Forms.CheckBox();
            this.radioMdlAsFBX = new System.Windows.Forms.RadioButton();
            this.radioMdlAsOBJ = new System.Windows.Forms.RadioButton();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.radioSndAsWAV = new System.Windows.Forms.RadioButton();
            this.radioSndAsOGG = new System.Windows.Forms.RadioButton();
            this.chkExportAskEveryTime = new System.Windows.Forms.CheckBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.tabPageExport = new System.Windows.Forms.TabPage();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.chkSearchSwitch = new System.Windows.Forms.CheckBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.panel1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.tabPageExport.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnOK
            // 
            this.btnOK.Dock = System.Windows.Forms.DockStyle.Right;
            this.btnOK.Location = new System.Drawing.Point(335, 0);
            this.btnOK.Margin = new System.Windows.Forms.Padding(3, 3, 5, 3);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 25);
            this.btnOK.TabIndex = 1;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Dock = System.Windows.Forms.DockStyle.Right;
            this.btnCancel.Location = new System.Drawing.Point(416, 0);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 25);
            this.btnCancel.TabIndex = 0;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.btnOK);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.btnCancel);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 418);
            this.panel1.Margin = new System.Windows.Forms.Padding(0, 0, 3, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(491, 25);
            this.panel1.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.Dock = System.Windows.Forms.DockStyle.Right;
            this.label1.Location = new System.Drawing.Point(410, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(6, 25);
            this.label1.TabIndex = 2;
            this.label1.Text = "label1";
            // 
            // radioTexAsPNG
            // 
            this.radioTexAsPNG.AutoSize = true;
            this.radioTexAsPNG.Location = new System.Drawing.Point(7, 92);
            this.radioTexAsPNG.Name = "radioTexAsPNG";
            this.radioTexAsPNG.Size = new System.Drawing.Size(95, 17);
            this.radioTexAsPNG.TabIndex = 3;
            this.radioTexAsPNG.Text = "Export as PNG";
            this.radioTexAsPNG.UseVisualStyleBackColor = true;
            // 
            // radioTexAsTGA
            // 
            this.radioTexAsTGA.AutoSize = true;
            this.radioTexAsTGA.Location = new System.Drawing.Point(7, 68);
            this.radioTexAsTGA.Name = "radioTexAsTGA";
            this.radioTexAsTGA.Size = new System.Drawing.Size(94, 17);
            this.radioTexAsTGA.TabIndex = 2;
            this.radioTexAsTGA.Text = "Export as TGA";
            this.radioTexAsTGA.UseVisualStyleBackColor = true;
            // 
            // radioTexAsLegacyDDS
            // 
            this.radioTexAsLegacyDDS.AutoSize = true;
            this.radioTexAsLegacyDDS.Location = new System.Drawing.Point(7, 44);
            this.radioTexAsLegacyDDS.Name = "radioTexAsLegacyDDS";
            this.radioTexAsLegacyDDS.Size = new System.Drawing.Size(129, 17);
            this.radioTexAsLegacyDDS.TabIndex = 1;
            this.radioTexAsLegacyDDS.Text = "Export as legacy DDS";
            this.radioTexAsLegacyDDS.UseVisualStyleBackColor = true;
            // 
            // radioTexAsDDS
            // 
            this.radioTexAsDDS.AutoSize = true;
            this.radioTexAsDDS.Checked = true;
            this.radioTexAsDDS.Location = new System.Drawing.Point(7, 20);
            this.radioTexAsDDS.Name = "radioTexAsDDS";
            this.radioTexAsDDS.Size = new System.Drawing.Size(95, 17);
            this.radioTexAsDDS.TabIndex = 0;
            this.radioTexAsDDS.TabStop = true;
            this.radioTexAsDDS.Text = "Export as DDS";
            this.radioTexAsDDS.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.radioTexAsPNG);
            this.groupBox1.Controls.Add(this.radioTexAsTGA);
            this.groupBox1.Controls.Add(this.radioTexAsLegacyDDS);
            this.groupBox1.Controls.Add(this.radioTexAsDDS);
            this.groupBox1.Location = new System.Drawing.Point(8, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(468, 117);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Textures options:";
            // 
            // chkMdlExportLods
            // 
            this.chkMdlExportLods.AutoSize = true;
            this.chkMdlExportLods.Location = new System.Drawing.Point(242, 91);
            this.chkMdlExportLods.Name = "chkMdlExportLods";
            this.chkMdlExportLods.Size = new System.Drawing.Size(78, 17);
            this.chkMdlExportLods.TabIndex = 8;
            this.chkMdlExportLods.Text = "Export lods";
            this.chkMdlExportLods.UseVisualStyleBackColor = true;
            // 
            // chkMdlSaveSurfaceTextures
            // 
            this.chkMdlSaveSurfaceTextures.AutoSize = true;
            this.chkMdlSaveSurfaceTextures.Location = new System.Drawing.Point(242, 68);
            this.chkMdlSaveSurfaceTextures.Name = "chkMdlSaveSurfaceTextures";
            this.chkMdlSaveSurfaceTextures.Size = new System.Drawing.Size(142, 17);
            this.chkMdlSaveSurfaceTextures.TabIndex = 7;
            this.chkMdlSaveSurfaceTextures.Text = "Save all surface textures";
            this.chkMdlSaveSurfaceTextures.UseVisualStyleBackColor = true;
            // 
            // chkMdlExcludeCollision
            // 
            this.chkMdlExcludeCollision.AutoSize = true;
            this.chkMdlExcludeCollision.Location = new System.Drawing.Point(242, 44);
            this.chkMdlExcludeCollision.Name = "chkMdlExcludeCollision";
            this.chkMdlExcludeCollision.Size = new System.Drawing.Size(150, 17);
            this.chkMdlExcludeCollision.TabIndex = 6;
            this.chkMdlExcludeCollision.Text = "Exclude collision geometry";
            this.chkMdlExcludeCollision.UseVisualStyleBackColor = true;
            // 
            // chkMdlSaveWithTextures
            // 
            this.chkMdlSaveWithTextures.AutoSize = true;
            this.chkMdlSaveWithTextures.Checked = true;
            this.chkMdlSaveWithTextures.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkMdlSaveWithTextures.Location = new System.Drawing.Point(242, 20);
            this.chkMdlSaveWithTextures.Name = "chkMdlSaveWithTextures";
            this.chkMdlSaveWithTextures.Size = new System.Drawing.Size(113, 17);
            this.chkMdlSaveWithTextures.TabIndex = 5;
            this.chkMdlSaveWithTextures.Text = "Save with textures";
            this.chkMdlSaveWithTextures.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label2.Location = new System.Drawing.Point(233, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(2, 106);
            this.label2.TabIndex = 4;
            // 
            // chkMdlExportAnimsSeparate
            // 
            this.chkMdlExportAnimsSeparate.AutoSize = true;
            this.chkMdlExportAnimsSeparate.Location = new System.Drawing.Point(7, 92);
            this.chkMdlExportAnimsSeparate.Name = "chkMdlExportAnimsSeparate";
            this.chkMdlExportAnimsSeparate.Size = new System.Drawing.Size(154, 17);
            this.chkMdlExportAnimsSeparate.TabIndex = 3;
            this.chkMdlExportAnimsSeparate.Text = "Animations to separate files";
            this.chkMdlExportAnimsSeparate.UseVisualStyleBackColor = true;
            // 
            // chkMdlExportAnims
            // 
            this.chkMdlExportAnims.AutoSize = true;
            this.chkMdlExportAnims.Checked = true;
            this.chkMdlExportAnims.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkMdlExportAnims.Location = new System.Drawing.Point(7, 68);
            this.chkMdlExportAnims.Name = "chkMdlExportAnims";
            this.chkMdlExportAnims.Size = new System.Drawing.Size(109, 17);
            this.chkMdlExportAnims.TabIndex = 2;
            this.chkMdlExportAnims.Text = "Export animations";
            this.chkMdlExportAnims.UseVisualStyleBackColor = true;
            // 
            // radioMdlAsFBX
            // 
            this.radioMdlAsFBX.AutoSize = true;
            this.radioMdlAsFBX.Location = new System.Drawing.Point(7, 44);
            this.radioMdlAsFBX.Name = "radioMdlAsFBX";
            this.radioMdlAsFBX.Size = new System.Drawing.Size(92, 17);
            this.radioMdlAsFBX.TabIndex = 1;
            this.radioMdlAsFBX.TabStop = true;
            this.radioMdlAsFBX.Text = "Export as FBX";
            this.radioMdlAsFBX.UseVisualStyleBackColor = true;
            // 
            // radioMdlAsOBJ
            // 
            this.radioMdlAsOBJ.AutoSize = true;
            this.radioMdlAsOBJ.Checked = true;
            this.radioMdlAsOBJ.Location = new System.Drawing.Point(7, 20);
            this.radioMdlAsOBJ.Name = "radioMdlAsOBJ";
            this.radioMdlAsOBJ.Size = new System.Drawing.Size(92, 17);
            this.radioMdlAsOBJ.TabIndex = 0;
            this.radioMdlAsOBJ.TabStop = true;
            this.radioMdlAsOBJ.Text = "Export as OBJ";
            this.radioMdlAsOBJ.UseVisualStyleBackColor = true;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.chkMdlExportLods);
            this.groupBox2.Controls.Add(this.chkMdlSaveSurfaceTextures);
            this.groupBox2.Controls.Add(this.chkMdlExcludeCollision);
            this.groupBox2.Controls.Add(this.chkMdlSaveWithTextures);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.chkMdlExportAnimsSeparate);
            this.groupBox2.Controls.Add(this.chkMdlExportAnims);
            this.groupBox2.Controls.Add(this.radioMdlAsFBX);
            this.groupBox2.Controls.Add(this.radioMdlAsOBJ);
            this.groupBox2.Location = new System.Drawing.Point(9, 130);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(467, 120);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Models options:";
            // 
            // radioSndAsWAV
            // 
            this.radioSndAsWAV.AutoSize = true;
            this.radioSndAsWAV.Location = new System.Drawing.Point(7, 44);
            this.radioSndAsWAV.Name = "radioSndAsWAV";
            this.radioSndAsWAV.Size = new System.Drawing.Size(97, 17);
            this.radioSndAsWAV.TabIndex = 1;
            this.radioSndAsWAV.TabStop = true;
            this.radioSndAsWAV.Text = "Export as WAV";
            this.radioSndAsWAV.UseVisualStyleBackColor = true;
            // 
            // radioSndAsOGG
            // 
            this.radioSndAsOGG.AutoSize = true;
            this.radioSndAsOGG.Checked = true;
            this.radioSndAsOGG.Location = new System.Drawing.Point(7, 20);
            this.radioSndAsOGG.Name = "radioSndAsOGG";
            this.radioSndAsOGG.Size = new System.Drawing.Size(96, 17);
            this.radioSndAsOGG.TabIndex = 0;
            this.radioSndAsOGG.TabStop = true;
            this.radioSndAsOGG.Text = "Export as OGG";
            this.radioSndAsOGG.UseVisualStyleBackColor = true;
            // 
            // chkExportAskEveryTime
            // 
            this.chkExportAskEveryTime.AutoSize = true;
            this.chkExportAskEveryTime.Location = new System.Drawing.Point(243, 19);
            this.chkExportAskEveryTime.Name = "chkExportAskEveryTime";
            this.chkExportAskEveryTime.Size = new System.Drawing.Size(168, 17);
            this.chkExportAskEveryTime.TabIndex = 6;
            this.chkExportAskEveryTime.Text = "Show this dialog before export";
            this.chkExportAskEveryTime.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.radioSndAsWAV);
            this.groupBox3.Controls.Add(this.radioSndAsOGG);
            this.groupBox3.Location = new System.Drawing.Point(8, 256);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(468, 71);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Sounds options:";
            // 
            // tabPageExport
            // 
            this.tabPageExport.Controls.Add(this.groupBox4);
            this.tabPageExport.Controls.Add(this.groupBox3);
            this.tabPageExport.Controls.Add(this.groupBox2);
            this.tabPageExport.Controls.Add(this.groupBox1);
            this.tabPageExport.Location = new System.Drawing.Point(4, 22);
            this.tabPageExport.Name = "tabPageExport";
            this.tabPageExport.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageExport.Size = new System.Drawing.Size(486, 392);
            this.tabPageExport.TabIndex = 0;
            this.tabPageExport.Text = "Export options";
            this.tabPageExport.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.chkSearchSwitch);
            this.groupBox4.Controls.Add(this.chkExportAskEveryTime);
            this.groupBox4.Location = new System.Drawing.Point(8, 333);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(468, 48);
            this.groupBox4.TabIndex = 7;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Search:";
            // 
            // chkSearchSwitch
            // 
            this.chkSearchSwitch.AutoSize = true;
            this.chkSearchSwitch.ForeColor = System.Drawing.Color.Olive;
            this.chkSearchSwitch.Location = new System.Drawing.Point(8, 19);
            this.chkSearchSwitch.Name = "chkSearchSwitch";
            this.chkSearchSwitch.Size = new System.Drawing.Size(174, 17);
            this.chkSearchSwitch.TabIndex = 7;
            this.chkSearchSwitch.Text = "Enable search with (enter) only.";
            this.chkSearchSwitch.UseVisualStyleBackColor = true;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPageExport);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(494, 418);
            this.tabControl1.TabIndex = 1;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.tabControl1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(494, 443);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // SettingsDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(494, 443);
            this.Controls.Add(this.tableLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SettingsDlg";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Settings";
            this.Load += new System.EventHandler(this.SettingsDlg_Load);
            this.panel1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.tabPageExport.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        protected System.Windows.Forms.Button btnOK;
        protected System.Windows.Forms.Button btnCancel;
        protected System.Windows.Forms.Panel panel1;
        protected System.Windows.Forms.Label label1;
        protected System.Windows.Forms.RadioButton radioTexAsPNG;
        protected System.Windows.Forms.RadioButton radioTexAsTGA;
        protected System.Windows.Forms.RadioButton radioTexAsLegacyDDS;
        protected System.Windows.Forms.RadioButton radioTexAsDDS;
        protected System.Windows.Forms.GroupBox groupBox1;
        protected System.Windows.Forms.CheckBox chkMdlExportLods;
        protected System.Windows.Forms.CheckBox chkMdlSaveSurfaceTextures;
        protected System.Windows.Forms.CheckBox chkMdlExcludeCollision;
        protected System.Windows.Forms.CheckBox chkMdlSaveWithTextures;
        protected System.Windows.Forms.Label label2;
        protected System.Windows.Forms.CheckBox chkMdlExportAnimsSeparate;
        protected System.Windows.Forms.CheckBox chkMdlExportAnims;
        protected System.Windows.Forms.RadioButton radioMdlAsFBX;
        protected System.Windows.Forms.RadioButton radioMdlAsOBJ;
        protected System.Windows.Forms.GroupBox groupBox2;
        protected System.Windows.Forms.RadioButton radioSndAsWAV;
        protected System.Windows.Forms.RadioButton radioSndAsOGG;
        protected System.Windows.Forms.CheckBox chkExportAskEveryTime;
        protected System.Windows.Forms.GroupBox groupBox3;
        protected System.Windows.Forms.TabPage tabPageExport;
        protected System.Windows.Forms.TabControl tabControl1;
        protected System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        protected System.Windows.Forms.GroupBox groupBox4;
        protected System.Windows.Forms.CheckBox chkSearchSwitch;
    }
}
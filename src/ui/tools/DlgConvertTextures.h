#pragma once

namespace MetroEX {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Threading;

    /// <summary>
    /// Summary for DlgConvertTextures
    /// </summary>
    public ref class DlgConvertTextures : public System::Windows::Forms::Form {
    public:
        DlgConvertTextures() {
            InitializeComponent();

            mConversionInProgress = false;
            mStopRequested = false;
            mConversionThread = nullptr;
            mIncludeSubfolders = true;
        }

    private:
        volatile bool   mConversionInProgress;
        volatile bool   mStopRequested;
        Thread^         mConversionThread;
        bool            mIncludeSubfolders;

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~DlgConvertTextures() {
            MySafeDelete(components);
            MySafeDelete(mConversionThread);
        }
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::TextBox^  txtPath;
    private: System::Windows::Forms::Button^  btnChooseFolder;
    private: System::Windows::Forms::Button^  btnChooseFile;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::ProgressBar^  prbProgress;

    private: System::Windows::Forms::CheckBox^  chkWithSubfolders;
    private: System::Windows::Forms::Button^  btnConvert;
    private: System::Windows::Forms::Button^  btnStop;
    private: System::Windows::Forms::TextBox^  txtLog;



    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->txtPath = (gcnew System::Windows::Forms::TextBox());
            this->btnChooseFolder = (gcnew System::Windows::Forms::Button());
            this->btnChooseFile = (gcnew System::Windows::Forms::Button());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->prbProgress = (gcnew System::Windows::Forms::ProgressBar());
            this->chkWithSubfolders = (gcnew System::Windows::Forms::CheckBox());
            this->btnConvert = (gcnew System::Windows::Forms::Button());
            this->btnStop = (gcnew System::Windows::Forms::Button());
            this->txtLog = (gcnew System::Windows::Forms::TextBox());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(9, 14);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(32, 13);
            this->label1->TabIndex = 0;
            this->label1->Text = L"Path:";
            // 
            // txtPath
            // 
            this->txtPath->Location = System::Drawing::Point(12, 30);
            this->txtPath->Name = L"txtPath";
            this->txtPath->Size = System::Drawing::Size(637, 20);
            this->txtPath->TabIndex = 1;
            this->txtPath->TextChanged += gcnew System::EventHandler(this, &DlgConvertTextures::txtPath_TextChanged);
            // 
            // btnChooseFolder
            // 
            this->btnChooseFolder->Location = System::Drawing::Point(527, 56);
            this->btnChooseFolder->Name = L"btnChooseFolder";
            this->btnChooseFolder->Size = System::Drawing::Size(122, 23);
            this->btnChooseFolder->TabIndex = 2;
            this->btnChooseFolder->Text = L"Choose folder...";
            this->btnChooseFolder->UseVisualStyleBackColor = true;
            this->btnChooseFolder->Click += gcnew System::EventHandler(this, &DlgConvertTextures::btnChooseFolder_Click);
            // 
            // btnChooseFile
            // 
            this->btnChooseFile->Location = System::Drawing::Point(365, 57);
            this->btnChooseFile->Name = L"btnChooseFile";
            this->btnChooseFile->Size = System::Drawing::Size(122, 23);
            this->btnChooseFile->TabIndex = 3;
            this->btnChooseFile->Text = L"Choose file...";
            this->btnChooseFile->UseVisualStyleBackColor = true;
            this->btnChooseFile->Click += gcnew System::EventHandler(this, &DlgConvertTextures::btnChooseFile_Click);
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(9, 84);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(51, 13);
            this->label2->TabIndex = 4;
            this->label2->Text = L"Progress:";
            // 
            // prbProgress
            // 
            this->prbProgress->Location = System::Drawing::Point(12, 100);
            this->prbProgress->Name = L"prbProgress";
            this->prbProgress->Size = System::Drawing::Size(637, 23);
            this->prbProgress->TabIndex = 5;
            // 
            // chkWithSubfolders
            // 
            this->chkWithSubfolders->AutoSize = true;
            this->chkWithSubfolders->Checked = true;
            this->chkWithSubfolders->CheckState = System::Windows::Forms::CheckState::Checked;
            this->chkWithSubfolders->Location = System::Drawing::Point(12, 60);
            this->chkWithSubfolders->Name = L"chkWithSubfolders";
            this->chkWithSubfolders->Size = System::Drawing::Size(99, 17);
            this->chkWithSubfolders->TabIndex = 6;
            this->chkWithSubfolders->Text = L"With subfolders";
            this->chkWithSubfolders->UseVisualStyleBackColor = true;
            // 
            // btnConvert
            // 
            this->btnConvert->Enabled = false;
            this->btnConvert->Location = System::Drawing::Point(527, 130);
            this->btnConvert->Name = L"btnConvert";
            this->btnConvert->Size = System::Drawing::Size(122, 23);
            this->btnConvert->TabIndex = 7;
            this->btnConvert->Text = L"Convert !";
            this->btnConvert->UseVisualStyleBackColor = true;
            this->btnConvert->Click += gcnew System::EventHandler(this, &DlgConvertTextures::btnConvert_Click);
            // 
            // btnStop
            // 
            this->btnStop->Enabled = false;
            this->btnStop->Location = System::Drawing::Point(365, 130);
            this->btnStop->Name = L"btnStop";
            this->btnStop->Size = System::Drawing::Size(122, 23);
            this->btnStop->TabIndex = 8;
            this->btnStop->Text = L"Stop";
            this->btnStop->UseVisualStyleBackColor = true;
            this->btnStop->Click += gcnew System::EventHandler(this, &DlgConvertTextures::btnStop_Click);
            // 
            // txtLog
            // 
            this->txtLog->Location = System::Drawing::Point(12, 160);
            this->txtLog->Multiline = true;
            this->txtLog->Name = L"txtLog";
            this->txtLog->ReadOnly = true;
            this->txtLog->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->txtLog->Size = System::Drawing::Size(636, 125);
            this->txtLog->TabIndex = 9;
            // 
            // DlgConvertTextures
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(661, 297);
            this->Controls->Add(this->txtLog);
            this->Controls->Add(this->btnStop);
            this->Controls->Add(this->btnConvert);
            this->Controls->Add(this->chkWithSubfolders);
            this->Controls->Add(this->prbProgress);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->btnChooseFile);
            this->Controls->Add(this->btnChooseFolder);
            this->Controls->Add(this->txtPath);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->Name = L"DlgConvertTextures";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
            this->Text = L"Textures converter";
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &DlgConvertTextures::DlgConvertTextures_FormClosing);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        void txtPath_TextChanged(System::Object^ sender, System::EventArgs^ e);
        void btnChooseFile_Click(System::Object^ sender, System::EventArgs^ e);
        void btnChooseFolder_Click(System::Object^ sender, System::EventArgs^ e);
        void btnConvert_Click(System::Object^ sender, System::EventArgs^ e);
        void btnStop_Click(System::Object^ sender, System::EventArgs^ e);
        void DlgConvertTextures_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);

        // conversion
        bool ConvertOneFile(const fs::path& path);
        void ConvertFolder(const fs::path& path);
        void ConversionThreadFunc(Object^ pathStr);

        // logging
        void LogAction(System::Object^ str);

        // threading stuff
        void DisableUI();
        void EnableUI();
        void InitProgressBar(System::Object^ obj);
        void StepProgressBar();
    };
}

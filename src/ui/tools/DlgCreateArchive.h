#pragma once
#include "metro/MetroTypes.h"

namespace MetroEX {
    struct DirEntry {
        fs::path    path;
        MetroFile   file;
    };

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Summary for DlgCreateArchive
    /// </summary>
    public ref class DlgCreateArchive : public System::Windows::Forms::Form {
    public:
        DlgCreateArchive() {
            InitializeComponent();
        }

    protected:
        ~DlgCreateArchive() {
            MySafeDelete(components);
        }

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::TextBox^  txtSourceFolder;
    protected:

    private: System::Windows::Forms::Button^  btnChooseSrcFolder;
    private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::RadioButton^  radioExtendArchive;
    private: System::Windows::Forms::RadioButton^  radioCreateNewArchive;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::TextBox^  txtTargetPath;

    private: System::Windows::Forms::Button^  btnChooseTarget;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::TextBox^  txtBlobFileName;

    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Button^  btnCreateArchive;
    private: System::Windows::Forms::Label^  label5;
    private: System::Windows::Forms::ProgressBar^  prbProgress;


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
            this->txtSourceFolder = (gcnew System::Windows::Forms::TextBox());
            this->btnChooseSrcFolder = (gcnew System::Windows::Forms::Button());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->radioExtendArchive = (gcnew System::Windows::Forms::RadioButton());
            this->radioCreateNewArchive = (gcnew System::Windows::Forms::RadioButton());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->txtTargetPath = (gcnew System::Windows::Forms::TextBox());
            this->btnChooseTarget = (gcnew System::Windows::Forms::Button());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->txtBlobFileName = (gcnew System::Windows::Forms::TextBox());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->btnCreateArchive = (gcnew System::Windows::Forms::Button());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->prbProgress = (gcnew System::Windows::Forms::ProgressBar());
            this->groupBox1->SuspendLayout();
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(9, 14);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(73, 13);
            this->label1->TabIndex = 0;
            this->label1->Text = L"Source folder:";
            // 
            // txtSourceFolder
            // 
            this->txtSourceFolder->Location = System::Drawing::Point(12, 30);
            this->txtSourceFolder->Name = L"txtSourceFolder";
            this->txtSourceFolder->Size = System::Drawing::Size(700, 20);
            this->txtSourceFolder->TabIndex = 1;
            this->txtSourceFolder->TextChanged += gcnew System::EventHandler(this, &DlgCreateArchive::txtSourceFolder_TextChanged);
            // 
            // btnChooseSrcFolder
            // 
            this->btnChooseSrcFolder->Location = System::Drawing::Point(718, 29);
            this->btnChooseSrcFolder->Name = L"btnChooseSrcFolder";
            this->btnChooseSrcFolder->Size = System::Drawing::Size(75, 22);
            this->btnChooseSrcFolder->TabIndex = 2;
            this->btnChooseSrcFolder->Text = L"Choose...";
            this->btnChooseSrcFolder->UseVisualStyleBackColor = true;
            this->btnChooseSrcFolder->Click += gcnew System::EventHandler(this, &DlgCreateArchive::btnChooseSrcFolder_Click);
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->radioExtendArchive);
            this->groupBox1->Controls->Add(this->radioCreateNewArchive);
            this->groupBox1->Location = System::Drawing::Point(13, 57);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(156, 81);
            this->groupBox1->TabIndex = 3;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"Operation type:";
            // 
            // radioExtendArchive
            // 
            this->radioExtendArchive->AutoSize = true;
            this->radioExtendArchive->Location = System::Drawing::Point(7, 48);
            this->radioExtendArchive->Name = L"radioExtendArchive";
            this->radioExtendArchive->Size = System::Drawing::Size(134, 17);
            this->radioExtendArchive->TabIndex = 1;
            this->radioExtendArchive->Text = L"Extend existing archive";
            this->radioExtendArchive->UseVisualStyleBackColor = true;
            // 
            // radioCreateNewArchive
            // 
            this->radioCreateNewArchive->AutoSize = true;
            this->radioCreateNewArchive->Checked = true;
            this->radioCreateNewArchive->Location = System::Drawing::Point(7, 21);
            this->radioCreateNewArchive->Name = L"radioCreateNewArchive";
            this->radioCreateNewArchive->Size = System::Drawing::Size(117, 17);
            this->radioCreateNewArchive->TabIndex = 0;
            this->radioCreateNewArchive->TabStop = true;
            this->radioCreateNewArchive->Text = L"Create new archive";
            this->radioCreateNewArchive->UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(176, 57);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(103, 13);
            this->label2->TabIndex = 4;
            this->label2->Text = L"Target archive path:";
            // 
            // txtTargetPath
            // 
            this->txtTargetPath->Location = System::Drawing::Point(179, 74);
            this->txtTargetPath->Name = L"txtTargetPath";
            this->txtTargetPath->Size = System::Drawing::Size(533, 20);
            this->txtTargetPath->TabIndex = 5;
            this->txtTargetPath->TextChanged += gcnew System::EventHandler(this, &DlgCreateArchive::txtTargetPath_TextChanged);
            // 
            // btnChooseTarget
            // 
            this->btnChooseTarget->Location = System::Drawing::Point(719, 73);
            this->btnChooseTarget->Name = L"btnChooseTarget";
            this->btnChooseTarget->Size = System::Drawing::Size(75, 22);
            this->btnChooseTarget->TabIndex = 6;
            this->btnChooseTarget->Text = L"Choose...";
            this->btnChooseTarget->UseVisualStyleBackColor = true;
            this->btnChooseTarget->Click += gcnew System::EventHandler(this, &DlgCreateArchive::btnChooseTarget_Click);
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(176, 101);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(244, 13);
            this->label3->TabIndex = 7;
            this->label3->Text = L"Blob file name (extend existing archive mode only):";
            // 
            // txtBlobFileName
            // 
            this->txtBlobFileName->Location = System::Drawing::Point(179, 117);
            this->txtBlobFileName->Name = L"txtBlobFileName";
            this->txtBlobFileName->ReadOnly = true;
            this->txtBlobFileName->Size = System::Drawing::Size(533, 20);
            this->txtBlobFileName->TabIndex = 8;
            // 
            // label4
            // 
            this->label4->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
            this->label4->Location = System::Drawing::Point(13, 145);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(780, 2);
            this->label4->TabIndex = 9;
            // 
            // btnCreateArchive
            // 
            this->btnCreateArchive->Enabled = false;
            this->btnCreateArchive->Location = System::Drawing::Point(719, 197);
            this->btnCreateArchive->Name = L"btnCreateArchive";
            this->btnCreateArchive->Size = System::Drawing::Size(75, 23);
            this->btnCreateArchive->TabIndex = 10;
            this->btnCreateArchive->Text = L"Create !";
            this->btnCreateArchive->UseVisualStyleBackColor = true;
            this->btnCreateArchive->Click += gcnew System::EventHandler(this, &DlgCreateArchive::btnCreateArchive_Click);
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(9, 152);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(51, 13);
            this->label5->TabIndex = 11;
            this->label5->Text = L"Progress:";
            // 
            // prbProgress
            // 
            this->prbProgress->Location = System::Drawing::Point(12, 168);
            this->prbProgress->Name = L"prbProgress";
            this->prbProgress->Size = System::Drawing::Size(781, 23);
            this->prbProgress->TabIndex = 12;
            // 
            // DlgCreateArchive
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(805, 233);
            this->Controls->Add(this->prbProgress);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->btnCreateArchive);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->txtBlobFileName);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->btnChooseTarget);
            this->Controls->Add(this->txtTargetPath);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->btnChooseSrcFolder);
            this->Controls->Add(this->txtSourceFolder);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->Name = L"DlgCreateArchive";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
            this->Text = L"Create patch archive";
            this->groupBox1->ResumeLayout(false);
            this->groupBox1->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        void txtSourceFolder_TextChanged(System::Object^ sender, System::EventArgs^ e);
        void txtTargetPath_TextChanged(System::Object^ sender, System::EventArgs^ e);
        void btnChooseSrcFolder_Click(System::Object^ sender, System::EventArgs^ e);
        void btnChooseTarget_Click(System::Object^ sender, System::EventArgs^ e);
        void btnCreateArchive_Click(System::Object^ sender, System::EventArgs^ e);

        void CheckAndUnlockCreateBtn();

        void CollectFilesAndFolders(const fs::path& path, MyArray<DirEntry>& result);
        bool WriteFilesBlob(const fs::path& path, MyArray<DirEntry>& files);
    };
}

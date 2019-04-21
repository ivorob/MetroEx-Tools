#pragma once

class MetroLocalization;

namespace MetroEX {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;


    public ref class LocalizationPanel : public System::Windows::Forms::UserControl {
    public:
        LocalizationPanel() {
            InitializeComponent();
        }

        void SetLocalizationTable(const MetroLocalization* loc);

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~LocalizationPanel() {
            if (components)
            {
                delete components;
            }
        }
    private: System::Windows::Forms::ListView^  lstStringsTable;
    protected:

    protected:
    private: System::Windows::Forms::ColumnHeader^  columnHeader1;
    private: System::Windows::Forms::ColumnHeader^  columnHeader2;



    protected:

    protected:

    protected:

    protected:



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
            this->lstStringsTable = (gcnew System::Windows::Forms::ListView());
            this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
            this->SuspendLayout();
            // 
            // lstStringsTable
            // 
            this->lstStringsTable->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {
                this->columnHeader1,
                    this->columnHeader2
            });
            this->lstStringsTable->Dock = System::Windows::Forms::DockStyle::Fill;
            this->lstStringsTable->FullRowSelect = true;
            this->lstStringsTable->Location = System::Drawing::Point(0, 0);
            this->lstStringsTable->Margin = System::Windows::Forms::Padding(0);
            this->lstStringsTable->Name = L"lstStringsTable";
            this->lstStringsTable->Size = System::Drawing::Size(852, 599);
            this->lstStringsTable->TabIndex = 0;
            this->lstStringsTable->UseCompatibleStateImageBehavior = false;
            this->lstStringsTable->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader1
            // 
            this->columnHeader1->Text = L"Key";
            this->columnHeader1->Width = 300;
            // 
            // columnHeader2
            // 
            this->columnHeader2->Text = L"Value";
            this->columnHeader2->Width = 500;
            // 
            // LocalizationPanel
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->Controls->Add(this->lstStringsTable);
            this->Name = L"LocalizationPanel";
            this->Size = System::Drawing::Size(852, 599);
            this->ResumeLayout(false);

        }
#pragma endregion
    };
}

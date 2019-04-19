#pragma once
#include <msclr/marshal_cppstd.h>
#include "mycommon.h"
#include "metro\MetroTexturesDatabase.h"
#include "MainForm.h"
#include "ui\TexturePropertiesViewer.h"
#include "ui\NodeSorter.h"

namespace MetroEX {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Summary for TexturesDatabaseViewer
    /// </summary>
    public ref class TexturesDatabaseViewer : public System::Windows::Forms::Form {
    public:
        TexturesDatabaseViewer(MainForm^ form, MetroTexturesDatabase* data, System::Windows::Forms::ImageList^ imageList);

    protected:
        ~TexturesDatabaseViewer();


    private: System::Windows::Forms::SplitContainer^  splitContainer1;
    private: System::Windows::Forms::PropertyGrid^  propertyGrid;
    private: MetroEXControls::FilterableTreeView^  filterableTreeView;





    private: System::ComponentModel::IContainer^  components;






    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>


#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
            this->filterableTreeView = (gcnew MetroEXControls::FilterableTreeView());
            this->propertyGrid = (gcnew System::Windows::Forms::PropertyGrid());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
            this->splitContainer1->Panel1->SuspendLayout();
            this->splitContainer1->Panel2->SuspendLayout();
            this->splitContainer1->SuspendLayout();
            this->SuspendLayout();
            // 
            // splitContainer1
            // 
            this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer1->Location = System::Drawing::Point(0, 0);
            this->splitContainer1->Name = L"splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this->splitContainer1->Panel1->Controls->Add(this->filterableTreeView);
            // 
            // splitContainer1.Panel2
            // 
            this->splitContainer1->Panel2->Controls->Add(this->propertyGrid);
            this->splitContainer1->Size = System::Drawing::Size(946, 728);
            this->splitContainer1->SplitterDistance = 315;
            this->splitContainer1->TabIndex = 1;
            // 
            // filterableTreeView
            // 
            this->filterableTreeView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->filterableTreeView->FilterTimeout = 1000;
            this->filterableTreeView->Location = System::Drawing::Point(0, 0);
            this->filterableTreeView->Name = L"filterableTreeView";
            this->filterableTreeView->Size = System::Drawing::Size(315, 728);
            this->filterableTreeView->TabIndex = 0;
            // 
            // propertyGrid
            // 
            this->propertyGrid->Dock = System::Windows::Forms::DockStyle::Fill;
            this->propertyGrid->Location = System::Drawing::Point(0, 0);
            this->propertyGrid->Name = L"propertyGrid";
            this->propertyGrid->Size = System::Drawing::Size(627, 728);
            this->propertyGrid->TabIndex = 0;
            // 
            // TexturesDatabaseViewer
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(946, 728);
            this->Controls->Add(this->splitContainer1);
            this->Name = L"TexturesDatabaseViewer";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
            this->Text = L"Textures Database Viewer";
            this->splitContainer1->Panel1->ResumeLayout(false);
            this->splitContainer1->Panel2->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
            this->splitContainer1->ResumeLayout(false);
            this->ResumeLayout(false);

        }

#pragma endregion

    private:
        void FillWithData();
        TreeNode^ mOriginalRootNode;
        MetroTexturesDatabase* mDataProvider;
        MainForm^ mMainForm;
        array<String^>^ mFileExtensions;
        TexturePropertiesViewer^ mPropertiesViewer;
        String^ GetRealPath(const size_t index);
        TreeNode^ FindNode(TreeNode^ parent, String^ text);
        void SortNodesRecursively(TreeNode^ parent, NodeSorter^ sorter);
        void filterableTreeView_NodeMouseClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e);
        void filterableTreeView_NodeMouseDoubleClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e);
        void filterableTreeView_AfterCollapse(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e);
        void filterableTreeView_AfterExpand(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e);
};
}

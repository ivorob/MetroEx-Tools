#include <msclr/marshal_cppstd.h>
#include "TexturesDatabaseViewer.h"
#include "ui\MainForm.h"
#include "ui\NodeSorter.h"

using namespace msclr::interop;

static const int kImageIdxFile = 6;
static const int kImageIdxFolderClosed = 0;
static const int kImageIdxFolderOpen = 1;

namespace MetroEX {

    void TexturesDatabaseViewer::FillWithData() {
        if (mDataProvider == nullptr) {
            return;
        }

        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::WaitCursor;

        this->filterableTreeView->TreeView->BeginUpdate();
        this->filterableTreeView->TreeView->Nodes->Clear();

        mOriginalRootNode = this->filterableTreeView->TreeView->Nodes->Add("content");
        mOriginalRootNode->ImageIndex = kImageIdxFolderClosed;
        mOriginalRootNode->SelectedImageIndex = kImageIdxFolderClosed;

        for (size_t i = 0, end = mDataProvider->GetNumTextures(); i < end; ++i) {
            const MetroTextureInfo& texInfo = mDataProvider->GetTextureInfo(i);
            String^ name = marshal_as<String^>(texInfo.name);
            array<String^>^ parts = name->Split('\\');

            TreeNode^ parentNode = mOriginalRootNode;
            for (int i = 0; i < parts->Length - 1; i++) {
                TreeNode^ foundNode = FindNode(parentNode, parts[i]);

                if (foundNode == nullptr) {
                    parentNode = parentNode->Nodes->Add(parts[i]);
                    parentNode->ImageIndex = kImageIdxFolderClosed;
                    parentNode->SelectedImageIndex = kImageIdxFolderClosed;
                } else {
                    parentNode = foundNode;
                }
            }

            TreeNode^ node = parentNode->Nodes->Add(parts[parts->Length - 1]);
            node->Tag = i;
            node->ImageIndex = kImageIdxFile;
            node->SelectedImageIndex = kImageIdxFile;
        }

        NodeSorter^ sorter = gcnew NodeSorter();
        this->SortNodesRecursively(mOriginalRootNode, sorter);

        mOriginalRootNode->Expand();
        this->filterableTreeView->TreeView->EndUpdate();

        this->filterableTreeView->TreeView->NodeMouseClick += (gcnew TreeNodeMouseClickEventHandler(this, &TexturesDatabaseViewer::filterableTreeView_NodeMouseClick));
        this->filterableTreeView->TreeView->NodeMouseDoubleClick += (gcnew TreeNodeMouseClickEventHandler(this, &TexturesDatabaseViewer::filterableTreeView_NodeMouseDoubleClick));
        this->filterableTreeView->TreeView->AfterCollapse += (gcnew TreeViewEventHandler(this, &TexturesDatabaseViewer::filterableTreeView_AfterCollapse));
        this->filterableTreeView->TreeView->AfterExpand += (gcnew TreeViewEventHandler(this, &TexturesDatabaseViewer::filterableTreeView_AfterExpand));

        System::Windows::Forms::Cursor::Current = System::Windows::Forms::Cursors::Arrow;
    }

    TreeNode^ TexturesDatabaseViewer::FindNode(TreeNode^ parent, String^ text) {
        String^ term = text->ToUpper();

        for (int i = 0; i < parent->Nodes->Count; i++) {
            if (parent->Nodes[i]->Text->ToUpper() == term) {
                return parent->Nodes[i];
            }
        }

        return nullptr;
    }

    void TexturesDatabaseViewer::SortNodesRecursively(TreeNode^ parent, NodeSorter^ sorter)
    {
        array<TreeNode^>^ nodes = gcnew array<TreeNode^>(parent->Nodes->Count);
        parent->Nodes->CopyTo(nodes, 0);
        System::Array::Sort(nodes, sorter);
        parent->Nodes->Clear();
        parent->Nodes->AddRange(nodes);

        delete nodes;

        for (int i = 0; i < parent->Nodes->Count; i++) {
            if (parent->Nodes[i]->Nodes->Count > 0) {
                this->SortNodesRecursively(parent->Nodes[i], sorter);
            }
        }
    }

    void TexturesDatabaseViewer::filterableTreeView_NodeMouseClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e) {
        if (e->Button != System::Windows::Forms::MouseButtons::Left || e->Node == nullptr || e->Node->Nodes->Count > 0) return;

        if (mPropertiesViewer == nullptr) {
            mPropertiesViewer = gcnew TexturePropertiesViewer();
        }

        const size_t index = safe_cast<size_t>(e->Node->Tag);
        const MetroTextureInfo& texInfo = mDataProvider->GetTextureInfo(index);
        String^ realPath = this->GetRealPath(index);

        mPropertiesViewer->SetTextureInfo(&texInfo);
        mPropertiesViewer->SetRealPath(realPath);
        this->propertyGrid->SelectedObject = mPropertiesViewer;
    }

    void TexturesDatabaseViewer::filterableTreeView_NodeMouseDoubleClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e) {
        if (e->Button != System::Windows::Forms::MouseButtons::Left || e->Node == nullptr || e->Node->Nodes->Count > 0) return;

        size_t index = safe_cast<size_t>(e->Node->Tag);
        String^ path = this->GetRealPath(index);

        mMainForm->ResetTreeView();
        if (!mMainForm->FindAndSelect("content\\textures\\" + path, mFileExtensions)) {
            mMainForm->ShowErrorMessage("Couldn't find texture!");
        }
    }

    String^ TexturesDatabaseViewer::GetRealPath(const size_t index) {
        const MetroTextureInfo& texInfo = mDataProvider->GetTextureInfo(index);
        const CharString& sourceName = mDataProvider->GetSourceName(texInfo.name);
        return marshal_as<String^>(sourceName);
    }

    void TexturesDatabaseViewer::filterableTreeView_AfterCollapse(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e) {
        e->Node->ImageIndex = kImageIdxFolderClosed;
        e->Node->SelectedImageIndex = kImageIdxFolderClosed;
    }

    void TexturesDatabaseViewer::filterableTreeView_AfterExpand(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e) {
        e->Node->ImageIndex = kImageIdxFolderOpen;
        e->Node->SelectedImageIndex = kImageIdxFolderOpen;
    }

}

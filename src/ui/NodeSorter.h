#pragma once

namespace MetroEX {
    ref class NodeSorter : public System::Collections::IComparer {
    public:
        virtual int Compare(Object^ x, Object^ y) {
            System::Windows::Forms::TreeNode^ left = safe_cast<System::Windows::Forms::TreeNode^>(x);
            System::Windows::Forms::TreeNode^ right = safe_cast<System::Windows::Forms::TreeNode^>(y);

            if (left->Nodes->Count) {
                return (right->Nodes->Count > 0) ? left->Text->CompareTo(right->Text) : -1;
            }
            else {
                return (right->Nodes->Count > 0) ? 1 : left->Text->CompareTo(right->Text);
            }
        }
    };
}
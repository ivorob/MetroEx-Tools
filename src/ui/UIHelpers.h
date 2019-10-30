#pragma once
#include "mycommon.h"
#include <msclr/marshal_cppstd.h>

namespace MetroEX {
    inline fs::path StringToPath(System::String^ s) {
        return msclr::interop::marshal_as<std::wstring>(s);
    }
    inline System::String^ PathToString(const fs::path& p) {
        return msclr::interop::marshal_as<System::String^>(p.native());
    }

    inline System::String^ ToNetString(const CharString& s) {
        return msclr::interop::marshal_as<System::String^>(s);
    }
    inline System::String^ ToNetString(const WideString& s) {
        return msclr::interop::marshal_as<System::String^>(s);
    }

    inline CharString NetToCharStr(System::String^ s) {
        return msclr::interop::marshal_as<CharString>(s);
    }
    inline WideString NetToWideStr(System::String^ s) {
        return msclr::interop::marshal_as<WideString>(s);
    }

    // Message boxes
    static void ShowErrorMessageBox(System::Windows::Forms::Control^ parent, System::String^ message) {
        System::Windows::Forms::MessageBoxButtons buttons = System::Windows::Forms::MessageBoxButtons::OK;
        System::Windows::Forms::MessageBoxIcon mbicon = System::Windows::Forms::MessageBoxIcon::Error;
        System::Windows::Forms::MessageBox::Show(parent, message, parent->Text, buttons, mbicon);
    }

    static void ShowInfoMessageBox(System::Windows::Forms::Control^ parent, System::String^ message) {
        System::Windows::Forms::MessageBoxButtons buttons = System::Windows::Forms::MessageBoxButtons::OK;
        System::Windows::Forms::MessageBoxIcon mbicon = System::Windows::Forms::MessageBoxIcon::Information;
        System::Windows::Forms::MessageBox::Show(parent, message, parent->Text, buttons, mbicon);
    }
}

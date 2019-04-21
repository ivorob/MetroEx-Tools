#pragma once
#include "mycommon.h"
#include <msclr/marshal_cppstd.h>

namespace MetroEX {
    static fs::path StringToPath(System::String^ s) {
        return msclr::interop::marshal_as<std::wstring>(s);
    }
    static System::String^ PathToString(const fs::path& p) {
        return msclr::interop::marshal_as<System::String^>(p.native());
    }

    static System::String^ ToNetString(const CharString& s) {
        return msclr::interop::marshal_as<System::String^>(s);
    }
    static System::String^ ToNetString(const WideString& s) {
        return msclr::interop::marshal_as<System::String^>(s);
    }

    static CharString NetToCharStr(System::String^ s) {
        return msclr::interop::marshal_as<CharString>(s);
    }
    static WideString NetToWideStr(System::String^ s) {
        return msclr::interop::marshal_as<WideString>(s);
    }
}

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
}

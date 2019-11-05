#pragma once
#include "mycommon.h"

// Vista-style dialog
struct ChooseFolderDialog {
    static fs::path ChooseFolder(const WideString& title, void* parentHwnd = nullptr);
};

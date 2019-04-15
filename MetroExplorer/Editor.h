#pragma once

#include <metro/MetroConfigDatabase.h>
#include <metro/VFXReader.h>

enum class FileType {
    Unknown,
    Texture,
    Model,
    Level,
    Sound,
    LevelSpawn,
};

class Editor {
public:
    void SetRoot(const std::string& root);
    void SetCurrentFile(int curFile);
    void SetMainWnd(HWND wnd);

    VFXReader            mFs;
    int                  mCurFile = -1;
    MetroConfigsDatabase mConfig;
    HWND                 mWnd = nullptr;
};

extern Editor gEditor;

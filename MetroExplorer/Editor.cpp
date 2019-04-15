#include "stdafx.h"
#include "Editor.h"

Editor gEditor;

void Editor::SetRoot(const std::string& root) {
    mFs.LoadFromFile(root);

    auto fileIdx = mFs.FindFile("content\\config.bin");
    if (fileIdx != MetroFile::InvalidFileIdx) {
        MemStream stream = mFs.ExtractFile(fileIdx);
        if (stream) {
            mConfig.LoadFromData(stream);
        }
    }
}

void Editor::SetCurrentFile(int curFile) {
    mCurFile = curFile;
}

void  Editor::SetMainWnd(HWND wnd)
{
    mWnd = wnd;
}

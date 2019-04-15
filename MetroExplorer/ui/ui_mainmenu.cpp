#include "stdafx.h"
#include "../Editor.h"
#include "ui.h"
#include <shobjidl.h>

fs::path ChooseFolder(const std::string& title, HWND parentHwnd) {
    fs::path result;

    IFileDialog* pfd = nullptr;
    HRESULT      hr = ::CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr)) {
        DWORD dwOptions = 0;
        hr = pfd->GetOptions(&dwOptions);
        if (SUCCEEDED(hr)) {
            pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
        }

        hr = pfd->Show(parentHwnd);
        if (SUCCEEDED(hr)) {
            IShellItem* psi = nullptr;
            hr = pfd->GetResult(&psi);
            if (SUCCEEDED(hr)) {
                LPWSTR path = nullptr;
                hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &path);
                if (SUCCEEDED(hr)) {
                    result = path;
                    ::CoTaskMemFree(path);
                }
                psi->Release();
            }
        }
        pfd->Release();
    }

    return result;
}

fs::path ChooseFile(const std::string& title, HWND parentHwnd, const std::string& fileTypes) {
    fs::path result;

    IFileDialog* pfd = nullptr;
    HRESULT      hr = ::CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr)) {
        DWORD dwOptions = 0;
        hr = pfd->GetOptions(&dwOptions);
        if (SUCCEEDED(hr)) {
            pfd->SetOptions(dwOptions);
        }
        COMDLG_FILTERSPEC filters[] = { {L"VFX files", L"*.vfx"}, {L"All files", L"*.*"} };
        pfd->SetFileTypes(sizeof(filters) / sizeof(filters[0]), filters);

        hr = pfd->Show(parentHwnd);
        if (SUCCEEDED(hr)) {
            IShellItem* psi = nullptr;
            hr = pfd->GetResult(&psi);
            if (SUCCEEDED(hr)) {
                LPWSTR path = nullptr;
                hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &path);
                if (SUCCEEDED(hr)) {
                    result = path;
                    ::CoTaskMemFree(path);
                }
                psi->Release();
            }
        }
        pfd->Release();
    }

    return result;
}

namespace {

struct MainMenu : public Tool {
    static Tool* create() {
        return new MainMenu();
    }

    MainMenu()
        : Tool(Tool::MainMenu) {
    }

    bool draw() override {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open Archive")) {
                    fs::path path = ChooseFile("Choose VFX file", gEditor.mWnd, "");
                    if (!path.empty()) {
                        gEditor.SetRoot(path.string());
                        showTool(Tool::FileSystem);
                    }
                }
                if (ImGui::MenuItem("Open Folder")) {
                    //auto root = ChooseFolder("Open Metro Exodus", nullptr);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                ImGui::MenuItem("About");
                ImGui::MenuItem("Demo", "", &show_demo_window);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code
        // to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        return true;
    }

    bool show_demo_window = false;
};

ToolRegistrator regMainMenu(Tool::MainMenu, &MainMenu::create);
}

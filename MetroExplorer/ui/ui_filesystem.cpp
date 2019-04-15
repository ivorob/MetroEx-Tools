#include "stdafx.h"
#include "../Editor.h"
#include "ui.h"
#include <metro/VFXReader.h>

namespace {

struct FileSystemTool : public Tool {
    static Tool* create() {
        return new FileSystemTool();
    }

    FileSystemTool()
        : Tool(Tool::FileSystem) {
    }

    bool draw() override {
        bool isOpen = true;
        if (ImGui::Begin("File System", &isOpen)) {
            m_nextSel = std::numeric_limits<size_t>::max();
            const MetroFile& rootDir = gEditor.mFs.GetRootFolder();
            const MetroFile& visualRoot
                = rootDir.name.empty() && rootDir.numFiles == 1 ? gEditor.mFs.GetFile(rootDir.firstFile) : rootDir;
            drawDir(visualRoot);
            if (m_nextSel != std::numeric_limits<size_t>::max()) {
                m_curSel = m_nextSel;
                if (gEditor.mFs.GetFile(m_curSel).IsFile()) {
                    gEditor.SetCurrentFile(m_curSel);
                    showTool(Tool::FileViewer);
                }
            }
        }
        ImGui::End();
        return isOpen;
    }

    void drawDir(const MetroFile& dir) {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        bool               b = ImGui::TreeNodeEx((void*)(intptr_t)dir.idx,
            node_flags | (dir.idx == m_curSel ? ImGuiTreeNodeFlags_Selected : 0), dir.name.c_str());
        if (ImGui::IsItemClicked())
            m_nextSel = dir.idx;
        if (!b)
            return;

        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        for (size_t idx = dir.firstFile; idx < dir.firstFile + dir.numFiles; ++idx) {
            const MetroFile& mf = gEditor.mFs.GetFile(idx);
            if (mf.IsFile()) {
                ImGui::TreeNodeEx((void*)(intptr_t)idx,
                    node_flags | (idx == m_curSel ? ImGuiTreeNodeFlags_Selected : 0), mf.name.c_str());
                if (ImGui::IsItemClicked())
                    m_nextSel = idx;
            } else {
                drawDir(mf);
            }
        }
        ImGui::TreePop();
    }

    size_t m_curSel = std::numeric_limits<size_t>::max();
    size_t m_nextSel = std::numeric_limits<size_t>::max();
};

ToolRegistrator regFileSystem(Tool::FileSystem, &FileSystemTool::create);

}

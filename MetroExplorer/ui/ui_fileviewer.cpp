#include "stdafx.h"
#include "../Editor.h"
#include "ui.h"
#include <metro/LevelSpawn.h>
#include <metro/VFXReader.h>
#include <metro/entities/Entity.h>

namespace {

enum class FileType {
    Unknown,
    Error,
    LevelSpawn,
};

struct Viewer {
    virtual ~Viewer() {
    }

    virtual void draw() = 0;
};

class LevelSpawnViewer : public Viewer {
public:
    LevelSpawnViewer(MemStream& data) {
        m_spawn.LoadFromData(data, gEditor.mFs, gEditor.mConfig);
    }
    ~LevelSpawnViewer() {
    }

    void draw() override {
        if (ImGui::CollapsingHeader("startup")) {
        }
        if (ImGui::CollapsingHeader("entities_params")) {
        }
        // if (ImGui::CollapsingHeader("entities")) {
        int columnWidth[4];
        ImGui::Columns(4, "mycolumns");
        columnWidth[0] = ImGui::GetColumnWidth();
        ImGui::Separator();
        ImGui::Text("ID");
        ImGui::NextColumn();
        columnWidth[1] = ImGui::GetColumnWidth();
        ImGui::Text("name");
        ImGui::NextColumn();
        columnWidth[2] = ImGui::GetColumnWidth();
        ImGui::Text("class");
        ImGui::NextColumn();
        columnWidth[3] = ImGui::GetColumnWidth();
        ImGui::Text("static_data");
        ImGui::NextColumn();
        ImGui::Columns(1);
        ImGui::Separator();
        if (ImGui::BeginChild("entities_list")) {
            ImGui::Columns(4, "mycolumns_data");
            for (size_t i = 0; i != std::size(columnWidth); i++)
                ImGui::SetColumnWidth(i, columnWidth[i]);
            ImGuiListClipper clipper(m_spawn.mSpawn.size(), ImGui::GetTextLineHeightWithSpacing());
            while (clipper.Step())
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                    auto el = m_spawn.mSpawn[i];
                    char idStr[10];
                    sprintf(idStr, "%u", el->initData.id);
                    if (ImGui::Selectable(idStr, selected == i, ImGuiSelectableFlags_SpanAllColumns))
                        selected = i;
                    ImGui::NextColumn();
                    ImGui::Text(el->name.c_str());
                    ImGui::NextColumn();
                    ImGui::Text(el->cls.c_str());
                    ImGui::NextColumn();
                    ImGui::Text(el->static_data.c_str());
                    ImGui::NextColumn();
                }
            ImGui::Columns(1);
            ImGui::Separator();
        }
        ImGui::EndChild();
        //}
    }

private:
    LevelSpawn m_spawn;
    int        selected = -1;
};

struct FileViewerTool : public Tool {
    static Tool* create() {
        return new FileViewerTool();
    }

    FileViewerTool()
        : Tool(Tool::FileViewer) {
    }

    bool draw() override {
        if (m_curFile != gEditor.mCurFile)
            load(gEditor.mCurFile);

        bool isOpen = true;
        if (ImGui::Begin(m_title.c_str(), &isOpen)) {
            if (m_view)
                m_view->draw();
        }
        ImGui::End();
        return isOpen;
    }

    void load(int curFile) {
        m_curFile = curFile;
        m_file = gEditor.mFs.GetFile(m_curFile);
        updateTitle();
        m_data = MemStream();
        m_type = getFileType(m_file);
        if (m_type != FileType::Unknown) {
            m_data = gEditor.mFs.ExtractFile(curFile);
            if (!m_data)
                m_type = FileType::Error;
        }
        processFile();
    }

    CharString GetFullName(const MetroFile* file, const VFXReader& fs) {
        const MetroFile* folder = fs.GetParentFolder(file->idx);
        if (folder) {
            CharString result = GetFullName(folder, fs);
            if (!result.empty())
                result += '\\';
            return result + file->name;
        } else
            return file->name;
    }

    void updateTitle() {
        m_title = GetFullName(&m_file, gEditor.mFs) + "###FileViewer";
    }

    FileType getFileType(const MetroFile& f) const {
        if (f.name == "level.bin")
            return FileType::LevelSpawn;

        return FileType::Unknown;
    }

    void processFile() {
        switch (m_type) {
        case FileType::LevelSpawn:
            m_view.reset(new LevelSpawnViewer(m_data));
            break;
        default:
            m_view.reset(nullptr);
        }
    }

    std::string             m_title = "###FileViewer";
    MetroFile               m_file;
    MemStream               m_data;
    FileType                m_type = FileType::Unknown;
    std::unique_ptr<Viewer> m_view;
    int                     m_curFile = -1;
};

ToolRegistrator regFileSystem(Tool::FileViewer, &FileViewerTool::create);

}

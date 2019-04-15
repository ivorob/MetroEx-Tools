#include "stdafx.h"
#include "ui.h"

namespace {
using Registry = std::map<Tool::Types, std::function<ToolCreator>>;
Registry& registry() {
    static Registry registry;
    return registry;
}

using EditorId = std::pair<Tool::Types, void*>;
std::map<EditorId, std::unique_ptr<Tool>> activeTools;
}

ToolRegistrator::ToolRegistrator(Tool::Types type, ToolCreator creator) {
    registry().emplace(type, creator);
}

bool isToolActive(Tool::Types type) {
    EditorId id(type, nullptr);
    return activeTools.find(id) != activeTools.end();
}

void switchTool(Tool::Types type) {
    EditorId id(type, nullptr);
    if (activeTools.find(id) == activeTools.end()) {
        activeTools[id] = std::unique_ptr<Tool>(registry()[type]());
    } else
        activeTools.erase(id);
}
void drawActiveTools() {
    for (auto i = activeTools.begin(), end = activeTools.end(); i != end; ++i) {
        bool show = i->second->draw();
        if (!show)
            activeTools.erase(i->first);
    }
}

void showTool(Tool::Types type) {
    EditorId id(type, nullptr);
    if (activeTools.find(id) == activeTools.end())
        activeTools[id] = std::unique_ptr<Tool>(registry()[type]());
}

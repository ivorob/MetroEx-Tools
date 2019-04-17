#include "stdafx.h"
#include "../Editor.h"
#include "ui.h"
#include <addons/imguinodegrapheditor/imguinodegrapheditor.h>
#include <sstream>

namespace {

enum MyNodeTypes {
    MNT_BLOCK_NODE = 0,

    MNT_COUNT
};
const char* MyNodeTypeNames[MNT_COUNT] = {
    "block",
};

ImU32 blockColor = IM_COL32(60, 60, 60, 255);
ImU32 actionColor = IM_COL32(171, 183, 50, 255);
ImU32 logicColor = IM_COL32(90, 57, 90, 255);
ImU32 triggerColor = IM_COL32(12, 8, 81, 255);

class BlockNode : public ImGui::Node {
public:
    static BlockNode* Create(const ImVec2& pos, size_t idx, const Block& block) {
        BlockNode* node = (BlockNode*)ImGui::MemAlloc(sizeof(BlockNode));
        new (node) BlockNode();
        std::string name = std::to_string(idx) + '@' + block.name;
        std::string input, output;
        if (block.meta) {
            for (const auto& el : block.meta->input) {
                if (!input.empty())
                    input += ';';
                input += el;
            }
            for (const auto& el : block.meta->output) {
                if (!output.empty())
                    output += ';';
                output += el;
            }
        } else {
            input = "0;1;2;3";
            output = "0;1;2;3";
        }

        node->init(name.c_str(), pos, input.c_str(), output.c_str(), MNT_BLOCK_NODE);
        if (block.name.find("actions/") != std::string::npos)
            node->overrideTitleBgColor = actionColor;
        else if (block.name.find("logic/") != std::string::npos)
            node->overrideTitleBgColor = logicColor;
        else if (block.name.find("triggers/") != std::string::npos || block.name.find("trigger/") != std::string::npos)
            node->overrideTitleBgColor = triggerColor;
        else
            node->overrideTitleBgColor = blockColor;
        return node;
    }
};

struct ScriptViewerTool : public Tool {
    static Tool* create() {
        return new ScriptViewerTool();
    }

    ScriptViewerTool()
        : Tool(Tool::ScriptViewer) {
    }

    bool draw() override {
        if (mObjName != gEditor.mObjName) {
            mObjName = gEditor.mObjName;
            mTitle = "Script for object [" + mObjName + "]###ScriptViewer";
            mBuf.clear();
            mGraph = ImGui::NodeGraphEditor();
        }
        bool isOpen = true;
        ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_Once);
        if (ImGui::Begin(mTitle.c_str(), &isOpen)) {
            ImGui::RadioButton("Text", &mView, Text);
            ImGui::SameLine();
            ImGui::RadioButton("Node", &mView, Node);
            switch (mView) {
            case Text:
                drawText();
                break;
            case Node:
                drawNode();
                break;
            }
        }
        ImGui::End();
        return isOpen;
    }

    void drawText() {
        const Script& script = gEditor.mScripts[0];

        if (mBuf.empty()) {
            std::stringstream buf;
            for (size_t i = 0; i != script.blocks.size(); i++) {
                const auto& block = script.blocks[i];
                buf << '[' << i << '@' << block.name << ']' << std::endl;
                for (const auto& prop : block.params) {
                    buf << "  " << prop.first << " = ";
                    std::visit(
                        [&buf](auto&& arg) {
                            buf << arg << std::endl;
                        },
                        prop.second);
                }
                for (const auto& link : script.links) {
                    if (link[0] == i) {
                        CharString  from = block.meta ? block.meta->output[link[1]] : std::to_string(link[1]);
                        const auto& toBlock = script.blocks[link[2]];
                        CharString  to = toBlock.meta ? toBlock.meta->input[link[3]] : std::to_string(link[3]);
                        buf << "  " << from << " -> " << link[2] << '@' << toBlock.name << '.' << to << std::endl;
                    }
                }
                buf << std::endl;
            }
            mBuf = buf.str();
        }
        ImGui::InputTextMultiline("ScriptText", mBuf.data(), mBuf.size(), ImVec2(-1, -1));
    }

    void drawNode() {
        const Script& script = gEditor.mScripts[0];
        if (mGraph.isInited()) {
            for (size_t i = 0; i != script.blocks.size(); i++) {
                const auto& block = script.blocks[i];
                mGraph.addNode(BlockNode::Create(ImVec2((int16_t)block.posx * 3, (int16_t)block.posy * 2), i, block));
            }
            for (const auto& link : script.links)
                mGraph.addLink(mGraph.getNode(link[0]), link[1], mGraph.getNode(link[2]), link[3]);
        }
        mGraph.render();
    }

    enum View : int {
        Text,
        Node,
    };
    int                    mView = Node;
    CharString             mBuf;
    ImGui::NodeGraphEditor mGraph;
    CharString             mObjName;
    CharString             mTitle;
};

}
ToolRegistrator regFileSystem(Tool::ScriptViewer, &ScriptViewerTool::create);

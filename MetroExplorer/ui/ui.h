#pragma once

struct Tool {
public:
    /// Виды инструментов
    enum Types {
        None,
        MainMenu, ///< Главное меню
        FileSystem, ///< Файловая система
        FileViewer, ///< Просмотрщик файлов
    };

    Types type() {
        return m_type;
    }

    virtual ~Tool() {
    }

    virtual bool draw() = 0;

protected:
    Tool(Types type)
        : m_type(type) {};

private:
    Types m_type = None;
};

using ToolCreator = Tool*();
struct ToolRegistrator {
    ToolRegistrator(Tool::Types type, ToolCreator creator);
};

bool isToolActive(Tool::Types type);
void switchTool(Tool::Types type);
void drawActiveTools();
void showTool(Tool::Types type);

#pragma once
#include "metro/MetroFileSystem.h"
#include "metro/MetroTypedStrings.h"
#include "metro/MetroTexturesDatabase.h"
#include "metro/MetroConfigDatabase.h"
#include "metro/MetroMaterialsDatabase.h"
#include "metro/MetroFonts.h"
#include "metro/MetroWeaponsDatabase.h"

static void LoadDatabasesFromFile(const fs::path& gameFolder) {
    size_t fileIdx = 0;

    MetroConfigsDatabase::Get().Reset();

    LogPrint(LogLevel::Info, "Loading typed_strings.bin from " + gameFolder.u8string());
    if (!MetroTypedStrings::Get().Initialize(gameFolder)) {
        LogPrint(LogLevel::Error, "Failed to load typed_strings.bin !");
    }

    const MetroFileSystem& mfs = MetroFileSystem::Get();

    // Load textures_handles_storage.bin
    fileIdx = mfs.FindFile(R"(content\textures_handles_storage.bin)");
    if (MetroFile::InvalidFileIdx != fileIdx) {
        MemStream stream = mfs.OpenFileStream(fileIdx);
        if (stream) {
            MetroTexturesDatabase::Get().LoadFromData(stream);
        }
    }

    fileIdx = mfs.FindFile(R"(content\scripts\texture_aliases.bin)");
    if (MetroFile::InvalidFileIdx != fileIdx) {
        MemStream stream = mfs.OpenFileStream(fileIdx);
        if (stream) {
            MetroTexturesDatabase::Get().LoadAliasesFromData(stream);
        }
    }

    // Load config.bin
    fileIdx = mfs.FindFile(R"(content\config.bin)");
    if (MetroFile::InvalidFileIdx != fileIdx) {
        MemStream stream = mfs.OpenFileStream(fileIdx);
        if (stream) {
            MetroConfigsDatabase::Get().LoadFromData(stream);
        }
    }

    // load fonts
    if (MetroConfigsDatabase::Get().GetNumFiles() > 0) {
        for (size_t i = scast<size_t>(MetroLanguage::First); i <= scast<size_t>(MetroLanguage::Last); ++i) {
            CharString binName = MetroFontsDatabase::MakeFontDBPath(scast<MetroLanguage>(i));
            MemStream stream = MetroConfigsDatabase::Get().GetFileStream(binName);
            if (stream) {
                MetroFontsDatabase::Get(scast<MetroLanguage>(i)).LoadFromData(stream);
            }
        }
    }

    //MetroWeaponsDatabase::Get().Initialize();

    // load materials database
    //if (cfgDb) {
    //    MemStream materialsStream = cfgDb->GetFileStream(R"(content\scripts\materials.bin)");
    //    if (materialsStream.Good()) {
    //        MetroMaterialsDatabase materialsDb;
    //        materialsDb.LoadFromData(materialsStream);
    //    }
    //}
}

#pragma once
#include "metro/VFXReader.h"
#include "metro/MetroTexturesDatabase.h"
#include "metro/MetroConfigDatabase.h"
#include "metro/MetroMaterialsDatabase.h"
#include "metro/MetroFonts.h"

static void LoadDatabasesFromFile(MetroConfigsDatabase*& cfgDb) {
    size_t fileIdx = 0;

    cfgDb = nullptr;

    // Load textures_handles_storage.bin
    fileIdx = VFXReader::Get().FindFile("content\\textures_handles_storage.bin");
    if (MetroFile::InvalidFileIdx != fileIdx) {
        MemStream stream = VFXReader::Get().ExtractFile(fileIdx);
        if (stream) {
            MetroTexturesDatabase::Get().LoadFromData(stream);
        }
    }

    fileIdx = VFXReader::Get().FindFile("content\\scripts\\texture_aliases.bin");
    if (MetroFile::InvalidFileIdx != fileIdx) {
        MemStream stream = VFXReader::Get().ExtractFile(fileIdx);
        if (stream) {
            MetroTexturesDatabase::Get().LoadAliasesFromData(stream);
        }
    }

    // Load config.bin
    fileIdx = VFXReader::Get().FindFile("content\\config.bin");
    if (MetroFile::InvalidFileIdx != fileIdx) {
        MemStream stream = VFXReader::Get().ExtractFile(fileIdx);
        if (stream) {
            cfgDb = new MetroConfigsDatabase();
            cfgDb->LoadFromData(stream);
        }
    }

    // load fonts
    if (cfgDb) {
        for (size_t i = scast<size_t>(MetroLanguage::First); i <= scast<size_t>(MetroLanguage::Last); ++i) {
            CharString binName = R"(content\scripts\fonts_)";
            binName += MetroLanguagesStr[i] + ".bin";
            MemStream stream = cfgDb->GetFileStream(binName);
            if (stream) {
                MetroFontsDatabase::Get(scast<MetroLanguage>(i)).LoadFromData(stream);
            }
        }
    }

    // load materials database
    //if (cfgDb) {
    //    MemStream materialsStream = cfgDb->GetFileStream(R"(content\scripts\materials.bin)");
    //    if (materialsStream.Good()) {
    //        MetroMaterialsDatabase materialsDb;
    //        materialsDb.LoadFromData(materialsStream);
    //    }
    //}
}

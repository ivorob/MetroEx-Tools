#pragma once
#include "metro/VFXReader.h"
#include "metro/MetroTexturesDatabase.h"
#include "metro/MetroConfigDatabase.h"
#include "metro/MetroMaterialsDatabase.h"

static void LoadDatabasesFromFile() {
    size_t fileIdx = 0;

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
            MetroConfigsDatabase::Get().LoadFromData(stream);
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

#include "MetroLocalization.h"
#include "pugixml.hpp"


static CharString WideToUtf8(const WideString& wstr) {
    CharString result;

    for (wchar_t wc : wstr) {
        if (wc < 0x80) {            // U+0000..U+007F
            result.push_back(scast<char>(wc));
        } else if (wc < 0x800) {    // U+0080..U+07FF
            result.push_back(scast<char>(0xC0 | (wc >> 6)));
            result.push_back(scast<char>(0x80 | (wc & 0x3F)));
        } else {                    // U+0800..U+FFFF
            result.push_back(scast<char>(0xE0 | (wc >> 12)));
            result.push_back(scast<char>(0x80 | ((wc >> 6) & 0x3F)));
            result.push_back(scast<char>(0x80 | (wc & 0x3F)));
        }
    }

    return result;
}

enum LocalizationChunk : size_t {
    LC_CharsTable   = 0x00000001,
    LC_StringsTable = 0x00000002
};

MetroLocalization::MetroLocalization() {
}
MetroLocalization::~MetroLocalization() {
}

bool MetroLocalization::LoadFromData(MemStream stream) {
    MyArray<uint16_t> charsTable;

    while (!stream.Ended()) {
        const size_t chunkId = stream.ReadTyped<uint32_t>();
        const size_t chunkSize = stream.ReadTyped<uint32_t>();
        const size_t chunkEnd = stream.GetCursor() + chunkSize;

        switch (chunkId) {
            case LC_CharsTable: {
                charsTable.resize(chunkSize / 2);
                stream.ReadToBuffer(charsTable.data(), chunkSize);
            } break;

            case LC_StringsTable: {
                assert(!charsTable.empty());
                if (charsTable.empty()) {
                    break;
                }

                MemStream subStream = stream.Substream(chunkSize);
                while (!subStream.Ended()) {
                    LocPair p;
                    p.key = subStream.ReadStringZ();

                    CharString encodedValue = subStream.ReadStringZ();
                    p.value.reserve(encodedValue.length());

                    const uint8_t* codes = rcast<const uint8_t*>(encodedValue.data());
                    const size_t len = encodedValue.length();
                    for (size_t i = 0; i < len; ++i) {
                        const size_t charIdx = scast<size_t>(codes[i]);
                        p.value.push_back(scast<WideString::value_type>(charsTable[charIdx]));
                    }

                    mStrings.emplace_back(p);
                }
            } break;
        }

        stream.SetCursor(chunkEnd);
    }

    return !mStrings.empty();
}

bool MetroLocalization::SaveToExcel2003(const fs::path& path) {
    pugi::xml_document doc;
    // add a custom declaration nodes
    // <?xml version="1.0" encoding="UTF-8"?>
    pugi::xml_node xmlDecl = doc.append_child(pugi::node_declaration);
    xmlDecl.set_name("xml");
    xmlDecl.append_attribute("version") = "1.0";
    xmlDecl.append_attribute("encoding") = "UTF-8";
    // <?mso-application progid="Excel.Sheet"?>
    pugi::xml_node excelDecl = doc.append_child(pugi::node_declaration);
    excelDecl.set_name("mso-application");
    excelDecl.append_attribute("progid") = "Excel.Sheet";

    pugi::xml_node workbook = doc.append_child("Workbook");
    workbook.append_attribute("xmlns") = "urn:schemas-microsoft-com:office:spreadsheet";
    workbook.append_attribute("xmlns:o") = "urn:schemas-microsoft-com:office:office";
    workbook.append_attribute("xmlns:x") = "urn:schemas-microsoft-com:office:excel";
    workbook.append_attribute("xmlns:ss") = "urn:schemas-microsoft-com:office:spreadsheet";
    workbook.append_attribute("xmlns:html") = R"(http://www.w3.org/TR/REC-html40)";

    pugi::xml_node worksheet = workbook.append_child("Worksheet");
    worksheet.append_attribute("ss:Name") = "Metro localization";

    pugi::xml_node table = worksheet.append_child("Table");

    pugi::xml_node col1 = table.append_child("Column");
    col1.append_attribute("ss:AutoFitWidth") = "0";
    col1.append_attribute("ss:Width") = "480";
    pugi::xml_node col2 = table.append_child("Column");
    col2.append_attribute("ss:AutoFitWidth") = "0";
    col2.append_attribute("ss:Width") = "650";

    for (const auto& p : mStrings) {
        pugi::xml_node row = table.append_child("Row");

        pugi::xml_node nodeKey = row.append_child("Cell").append_child("Data");
        nodeKey.append_attribute("ss:Type") = "String";
        nodeKey.text() = p.key.c_str();

        pugi::xml_node nodeValue = row.append_child("Cell").append_child("Data");
        nodeValue.append_attribute("ss:Type") = "String";
        nodeValue.text() = WideToUtf8(p.value).c_str();
    }

    return doc.save_file(path.native().c_str(), "", pugi::format_default, pugi::encoding_utf8);
}


size_t MetroLocalization::GetNumStrings() const {
    return mStrings.size();
}

const CharString& MetroLocalization::GetKey(const size_t idx) const {
    return mStrings[idx].key;
}

const WideString& MetroLocalization::GetValue(const size_t idx) const {
    return mStrings[idx].value;
}

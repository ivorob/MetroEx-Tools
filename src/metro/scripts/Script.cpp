#include "Script.h"
#include "../Config.h"
#include "../MetroReflection.h"
#include "Block.h"
#include "BlockFactory.h"

void Script::Serialize(MetroReflectionReader& r) {
    MyArray<uint8_t> unknown(r.GetStream().Remains());
    r.GetStream().ReadToBuffer(unknown.data(), unknown.size());
}

void Script::Read(Config& cfg) {
    cfg.ReadArray("groups", [this](Config& cfg, uint32_t idx) {
        assert(false);
    });
    cfg.ReadSection("blocks", [this](Config& cfg) {
        version = cfg.r_u16("version");
        uint32_t blockCount = cfg.r_u32("block_count");
        cfg.ReadArray(nullptr, blockCount, [this](Config& cfg, uint32_t idx) {
            uint32_t clsid = cfg.r_u32("clsid");
            Block    block = BlockFactory::Create(clsid);
            block.Read(cfg);
            blocks.push_back(block);
        });
    });
    uint32_t linkCount = cfg.r_u32("link_count");
    for (uint32_t j = 0; j != linkCount; j++) {
        std::array<uint16_t, 4> link = cfg.r_vec4s16(std::to_string(j).c_str());
        links.push_back(link);
    }
}

void ScriptRef::Serialize(MetroReflectionReader& r) {
    MyArray<uint8_t> unknown(r.GetStream().Remains());
    r.GetStream().ReadToBuffer(unknown.data(), unknown.size());
}

void ScriptRef::Read(Config& cfg) {
    vs_name = cfg.r_sz("vs_name");
    vs_debug = cfg.r_bool("vs_debug");
    vs_active = cfg.r_bool("vs_active");
    disable_qsave = cfg.r_bool("disable_qsave");
    save_on_nextlevel = cfg.r_bool("save_on_nextlevel");
    vs_ref = cfg.r_sz("vs_ref");
    dyn_state_exists = cfg.r_bool("vs_ref_dyn_state_exist");
    assert(dyn_state_exists == false);
    if (!vs_ref.empty()) {
        cfg.ReadArray("exposed_blocks", [this](Config& cfg, uint32_t idx) {
            uint16_t blkid = cfg.r_u16("blkid");
            uint32_t clsid = cfg.r_u32("clsid");
            Block    block = BlockFactory::Create(clsid);
            block.Read(cfg);
            exposed_blocks.push_back(block);
        });
    }
}

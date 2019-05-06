#include "Block.h"
#include "../Config.h"

Block::Block(uint32_t clsid, const char* name, const MetaInfo* meta)
    : clsid(clsid)
    , name(name)
    , meta(meta) {
}

void Block::Read(Config& cfg) {
    posx = cfg.r_u16("posx");
    posy = cfg.r_u16("posy");
    if (meta) {
        for (const auto& el : meta->props)
            params.emplace_back(el.name, cfg.Read(el.type, el.name));
    } else
        unknown = cfg.r_bytes(cfg.SectionRemains());
}

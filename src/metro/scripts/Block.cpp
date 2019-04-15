#include "Block.h"
#include "../Config.h"

void Block::Read(Config& cfg) {
}

void UnknownBlock::Read(Config& cfg) {
    unknown = cfg.r_bytes(cfg.SectionRemains());
}

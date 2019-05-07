#pragma once

#include "Entity.h"
#include <mycommon.h>

class MetroConfigsDatabase;

namespace EntityFactory {
uobject*               Create(const InitData& data);
uobject_static_params* GetStaticParams(const InitData& init, MemStream& config);
}

#pragma once
#include "threadref.h"

namespace rbx::classes {
	WeakThreadRef::Node node(uintptr_t address);
	uintptr_t lua_state(uintptr_t address);
}
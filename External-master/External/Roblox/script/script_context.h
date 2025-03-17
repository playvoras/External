#pragma once
#include <cstdint>

namespace rbx::classes {
	uint64_t max_capabilities(uintptr_t address);
	uint64_t set_max_capabilities(uintptr_t address, uint64_t value);
	bool is_modulesunlocked(uintptr_t address);
	bool set_modulesunlocked(uintptr_t address, bool value);

}
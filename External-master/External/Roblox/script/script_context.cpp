#include "../../offsets.h"
#include "script_context.h"
#include <Memory.h>


namespace rbx::classes {
	using namespace offsets;

	uint64_t max_capabilities(uintptr_t address) {
		uintptr_t capabilities_address = process->read_longlong(address + SCRIPTCONTEXT_CAPABILITIES);
		uintptr_t max_capabilities_address = process->read_longlong(capabilities_address);
		uint64_t max_capabilities_b = process->read_longlong(max_capabilities_address + 0x10);

		return max_capabilities_b;
	}

	uint64_t set_max_capabilities(uintptr_t address, uint64_t value) {
		uint64_t old_capabilities = max_capabilities(address);

		uintptr_t capabilities_address = process->read_longlong(address + SCRIPTCONTEXT_CAPABILITIES);
		uintptr_t max_capabilities_address = process->read_longlong(capabilities_address);

		process->write_longlong(max_capabilities_address + 0x10, &value);
		process->write_longlong(max_capabilities_address + 0x18, &value);

		return old_capabilities;
	}

	bool is_modulesunlocked(uintptr_t address) {
		return process->read_bool(address + SCRIPTCONTEXT_MODULESUNLOCKED);
	}

	bool set_modulesunlocked(uintptr_t address, bool value) {
		process->write_bool(
			address + SCRIPTCONTEXT_MODULESUNLOCKED, 
			&value
		);
	}
}
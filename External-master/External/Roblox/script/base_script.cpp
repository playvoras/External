#include "../../offsets.h"
#include "base_script.h"
#include "module_script.h"
#include <Memory.h>

namespace rbx::classes {
	using namespace offsets;

	uintptr_t lua_state(uintptr_t address) {
		return node(address).first().live_thread_ref().thread();
	}
}
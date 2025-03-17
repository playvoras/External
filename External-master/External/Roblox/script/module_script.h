#pragma once

#include "threadref.h"

namespace rbx::classes {
	class PerVMState {
	public:
		PerVMState(uintptr_t address = 0);

		uintptr_t address;

		int loading_state();
		WeakThreadRef::Node node();
		uintptr_t global_state();
	};

	bool is_unlocked(uintptr_t address);
	bool set_unlocked(uintptr_t address, bool value);
	PerVMState vm_state(uintptr_t address);
	WeakThreadRef::Node node(uintptr_t address);
}
#include "../offsets.h"
#include <Memory.h>
#include "module_script.h"
#include "base_script.h"

namespace rbx::classes {
	using namespace offsets;

	PerVMState::PerVMState(uintptr_t address) : address(address) {}

	int PerVMState::loading_state() {
		return process->read_int(
			address + PERVMSTATE_LOADINGSTATE
		);
	}

	WeakThreadRef::Node PerVMState::node() {
		uintptr_t node_address = process->read_longlong(
			address + PERVMSTATE_WEAKTHREADREFNODE
		);

		return WeakThreadRef::Node(node_address);
	}

	uintptr_t PerVMState::global_state() {
		uintptr_t global_state_address = process->read_longlong(
			address + PERVMSTATE_GLOBALSTATE
		);

		return global_state_address;
	}

	bool is_unlocked(uintptr_t address) {
		return process->read_bool(address + MODULESCRIPT_UNLOCKED);
	}

	bool set_unlocked(uintptr_t address, bool value) {
		bool was_unlocked = is_unlocked(address);
		process->write_bool(address + MODULESCRIPT_UNLOCKED, &value);
		return was_unlocked;
	}
	
	PerVMState vm_state(uintptr_t address) {
		uintptr_t state_map_address = process->read_longlong(address + MODULESCRIPT_VMSTATEMAP);
		uintptr_t vm_state_address = process->read_longlong(state_map_address);

		return PerVMState(vm_state_address);
	}

	WeakThreadRef::Node node(uintptr_t address) {
		return vm_state(address).node();
	}
}
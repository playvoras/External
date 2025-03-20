#include "../offsets.h"
#include <Memory.h>
#include "threadref.h"

namespace rbx::classes {
	using namespace offsets;

	LiveThreadRef::LiveThreadRef(uintptr_t address) : address(address) {}

	uintptr_t LiveThreadRef::thread() {
		uintptr_t thread_address = process->read_longlong(
			address + LIVETHREADREF_THREAD
		);

		return thread_address; // convert to lua state class
	}

	WeakThreadRef::WeakThreadRef(uintptr_t address) : address(address) {}

	WeakThreadRef::Node WeakThreadRef::node() {
		uintptr_t node_address = process->read_longlong(
			address + WEAKTHREADREF_NODE
		);

		return Node(node_address);
	}

	WeakThreadRef WeakThreadRef::previous() {
		uintptr_t previous_address = process->read_longlong(
			address + WEAKTHREADREF_PREVIOUS
		);

		return WeakThreadRef(previous_address);
	}

	WeakThreadRef WeakThreadRef::next() {
		uintptr_t next_address = process->read_longlong(
			address + WEAKTHREADREF_NEXT
		);

		return WeakThreadRef(next_address);
	}

	LiveThreadRef WeakThreadRef::live_thread_ref() {
		uintptr_t live_thread_ref_address = process->read_longlong(
			address + WEAKTHREADREF_LIVETHREADREF
		);

		return LiveThreadRef(live_thread_ref_address);
	}

	WeakThreadRef::Node::Node(uintptr_t address) : address(address) {}

	WeakThreadRef WeakThreadRef::Node::first() {
		uintptr_t first_address = process->read_longlong(
			address + WEAKTHREADREFNODE_FIRST
		);

		return WeakThreadRef(first_address);
	}

	void WeakThreadRef::Node::foreach_refs(std::function<void(WeakThreadRef)> callback) {
		for (WeakThreadRef ref = first(); (ref.address != NULL && ref.address < 0x1000); ref = ref.next()) {
			callback(ref);
		}
	}
}
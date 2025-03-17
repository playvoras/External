#pragma once

#include <cstdint>
#include <functional>

namespace rbx::classes {
	class LiveThreadRef {
	public:
		LiveThreadRef(uintptr_t address = 0);

		uintptr_t address;

		uintptr_t thread();
	};

	class WeakThreadRef {
	public:
		class Node {
		public:
			Node(uintptr_t address = 0);

			uintptr_t address;

			WeakThreadRef first();
			void foreach_refs(std::function<void(WeakThreadRef)> callback);
		};

		WeakThreadRef(uintptr_t address = 0);

		uintptr_t address;

		WeakThreadRef previous();
		WeakThreadRef next();
		Node node();
		LiveThreadRef live_thread_ref();
	};
}
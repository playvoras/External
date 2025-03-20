#include "instance.h"
#include "../Utils/Memory.h"
#include "../offsets.h"

auto tInstance::GetName() -> std::string
{
	auto Name = process->read_string(process->read_longlong(this->InstanceAddress + rbx::offsets::INSTANCE_NAME));
	if (Name.empty()) return "";

	return Name;
}

auto tInstance::GetRobloxClassName() -> std::string
{
	uintptr_t PointerClassDescriptor = process->read_longlong(this->InstanceAddress + rbx::offsets::INSTANCE_CLASSDESCRIPTOR);
	auto ClassName = process->read_string(process->read_longlong(PointerClassDescriptor + rbx::offsets::INSTANCE_SELF));
	if (ClassName.empty()) return "";

	return ClassName;
}

auto tInstance::Parent() -> uintptr_t
{
	auto ParentAddress = process->read_longlong(this->InstanceAddress + rbx::offsets::INSTANCE_PARENT);
	if (ParentAddress) return 0;

	return ParentAddress;
}

auto tInstance::GetKids() -> std::vector<tInstance*>
{
	std::vector<tInstance*> selfs;

	if (!this->InstanceAddress)
		return selfs;

	uintptr_t start = process->read_longlong(this->InstanceAddress + rbx::offsets::INSTANCE_CHILDREN);

	if (!start || start == 0)
		return selfs;

	uintptr_t end = process->read_longlong(start + 8);
	uintptr_t instances = process->read_longlong(start);

	for (int i = 0; i < 9000; i++) {
		if (instances == end) {
			break;
		}

		auto Child = Instance(process->read_longlong(instances));
		selfs.push_back(Child);

		instances += 0x10;
	}

	return selfs;
}

auto tInstance::FindFirstChild(std::string ChildName) -> tInstance*
{
	tInstance* self = nullptr;
	auto childs = Instance(this->InstanceAddress)->GetKids();



	for (auto child : childs) {
		auto name = child->GetName();
		if (name == ChildName) {
			self = child;
			return self;
		}
	}

	return self;
}

auto tInstance::WaitForChild(std::string ChildName, int timeout) -> tInstance*
{
	tInstance* self = nullptr;
	auto start = std::chrono::steady_clock::now();

	while (true)
	{
		auto childs = Instance(this->InstanceAddress)->GetKids();
		for (auto child : childs) {
			auto name = child->GetName();
			if (name == ChildName) {
				self = child;
			}
		}


		auto current = std::chrono::steady_clock::now();
		auto e = std::chrono::duration_cast<std::chrono::seconds>(current - start).count();

		if (e >= timeout) {
			return self;
		}


	}



	return self;
}


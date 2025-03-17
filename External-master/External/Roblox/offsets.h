#pragma once

#include <cstdint>

namespace rbx::offsets {
	constexpr uint32_t INSTANCE_SELF = 0x8;
	constexpr uint32_t INSTANCE_CLASSDESCRIPTOR = 0x18;
	constexpr uint32_t INSTANCE_ONDEMANDINSTANCE = 0x30;
	constexpr uint32_t INSTANCE_PARENT = 0x50;
	constexpr uint32_t INSTANCE_NAME = 0x70;
	constexpr uint32_t INSTANCE_CHILDREN = 0x78;
	constexpr uint32_t OBJECTVALUE_VALUEBASE = 0xD0;
	
	constexpr uint32_t MEMBERDESCRIPTOR_NAME = 0x8;

	constexpr uint32_t CLASSDESCRIPTOR_PROPERTIES = 0x28;
	constexpr uint32_t CLASSDESCRIPTOR_EVENTS = 0xD0;
	constexpr uint32_t CLASSDESCRIPTOR_METHODS = 0x178;
	
	constexpr uint32_t PROPERTYDESCRIPTOR_SCRIPTABLE = 0x40;
	constexpr uint32_t PROPERTYDESCRIPTOR_PUBLIC = 0x44;

	constexpr uint32_t ONDEMANDINSTANCE_ATTRIBUTESCOUNT = 0x120;
	constexpr uint32_t ONDEMANDINSTANCE_ATTRIBUTES = 0x130;

	constexpr uint32_t ATTRIBUTE_NAME = 0;
	constexpr uint32_t ATTRIBUTE_TTYPE = 0x20;
	constexpr uint32_t ATTRIBUTE_VALUE = 0x30;

	constexpr uint32_t EMBEDDEDCODE_BYTECODE = 0x10;
	constexpr uint32_t EMBEDDEDCODE_SIZE = 0x20;

	constexpr uint32_t BASESCRIPT_NODE = 0x1A0;
	constexpr uint32_t BASESCRIPT_EMBEDDEDCODE = 0x1C8;
	constexpr uint32_t MODULESCRIPT_EMBEDDEDCODE = 0x170;
	constexpr uint32_t MODULESCRIPT_UNLOCKED = 0x1B8;
	constexpr uint32_t MODULESCRIPT_VMSTATEMAP = 0x1C8;

	
	constexpr uint32_t SCRIPTCONTEXT_CAPABILITIES = 0x658;
	constexpr uint32_t SCRIPTCONTEXT_MODULESUNLOCKED = 0x690;
	constexpr uint32_t SCRIPTCONTEXT_GCJOB = SCRIPTCONTEXT_MODULESUNLOCKED - 0x188;

	constexpr uint32_t LIVETHREADREF_THREAD = 0x8;
	constexpr uint32_t WEAKTHREADREF_PREVIOUS = 0x10;
	constexpr uint32_t WEAKTHREADREF_NEXT = 0x18;
	constexpr uint32_t WEAKTHREADREF_LIVETHREADREF = 0x20;
	constexpr uint32_t WEAKTHREADREF_NODE = 0x28;
	constexpr uint32_t WEAKTHREADREFNODE_FIRST = 0x8;

	constexpr uint32_t PERVMSTATE_LOADINGSTATE = 0x20;
	constexpr uint32_t PERVMSTATE_WEAKTHREADREFNODE = 0x28;
	constexpr uint32_t PERVMSTATE_GLOBALSTATE = 0x30;
}
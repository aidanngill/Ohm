#pragma once

// https://github.com/ValveSoftware/source-sdk-2013/blob/0d8dceea4310fde5706b3ce1c70609d72a38efdf/mp/src/public/tier1/interface.h#L68-L83

typedef void* (*InstantiateInterfaceFn)();

class InterfaceReg {
public:
	InterfaceReg(InstantiateInterfaceFn fn, const char* pName);

public:
	InstantiateInterfaceFn	m_CreateFn;
	const char* m_pName;

	InterfaceReg* m_pNext;
	static InterfaceReg* s_pInterfaceRegs;
};
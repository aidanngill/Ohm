#pragma once

class IHandleEntity {
public:
	virtual ~IHandleEntity() {};
};

class IClientUnknown : public IHandleEntity {};
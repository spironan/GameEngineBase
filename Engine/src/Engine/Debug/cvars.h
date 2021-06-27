#pragma once

#include <cstdint> //uint32_t
#include "Utility/Hash.h"
#include <../vendor/glm/glm/glm.hpp>

//forward declaration
class CVarParameter;

enum class CVarFlags : uint32_t
{
	None = 0,
	Noedit = 1 << 1,
	EditReadOnly = 1 << 2,
	Advanced = 1 << 3,

	EditCheckbox = 1 << 8,
	EditFloatDrag = 1 << 9,
};



class CVarSystem
{
public:
	static CVarSystem *Get();

	virtual CVarParameter *GetCVar(engine::utility::StringHash hash) = 0;

	virtual double *GetFloatCVar(engine::utility::StringHash hash) = 0;
	virtual int32_t *GetIntCVar(engine::utility::StringHash hash) = 0;
	virtual const char *GetStringCVar(engine::utility::StringHash hash) = 0;
	virtual glm::vec3* GetVec3CVar(engine::utility::StringHash hash) = 0;

	virtual void SetFloatCVar(engine::utility::StringHash hash, double value) = 0; 
	virtual void SetIntCVar(engine::utility::StringHash hash, int32_t value) = 0;
	virtual void SetStringCVar(engine::utility::StringHash hash, const char *value) = 0;
	virtual void SetVec3CVar(engine::utility::StringHash hash, glm::vec3 value) = 0;

	// cvar creation
	virtual CVarParameter *CreateFloatCVar(const char *name, const char *description, double defaultValue, double currentValue) = 0;
	virtual CVarParameter *CreateIntCVar(const char *name, const char *description, int32_t defaultValue, int32_t currentValue) = 0;
	virtual CVarParameter *CreateStringCVar(const char *name, const char *description, const char *defaultValue, const char *currentValue) = 0;
	virtual CVarParameter *CreateVec3CVar(const char *name, const char *description, glm::vec3 defaultValue, glm::vec3 currentValue) = 0;

	virtual void DrawImguiEditor() = 0;
};


template <typename T>
struct AutoCVar
{
protected:
	int index;
	using CVarType = T;
};

struct AutoCVar_Float : AutoCVar<double>
{
	AutoCVar_Float(const char *name, const char *description, double defaultValue, CVarFlags flags = CVarFlags::None);
	double Get();
	float GetFloat();
	double* GetPtr();
	float* GetFloatPtr();
	void Set(double val);
};

struct AutoCVar_Int : AutoCVar<int32_t>
{
	AutoCVar_Int(const char* name, const char* description, int32_t defaultValue, CVarFlags flags = CVarFlags::None);
	int32_t Get();
	int32_t* GetPtr();
	void Set(int32_t val);
};

struct AutoCVar_String : AutoCVar<std::string>
{
	AutoCVar_String(const char* name, const char* description, const char* defaultValue, CVarFlags flags = CVarFlags::None);
	const char* Get();
	void Set(std::string&& val);
};

struct AutoCVar_Vec3 : AutoCVar<glm::vec3>
{
	AutoCVar_Vec3(const char* name, const char* description, glm::vec3 defaultValue, CVarFlags flags = CVarFlags::None);
	glm::vec3 Get();
	void Set(glm::vec3&& val);
};
#include "pch.h"

#include "cvars.h"
#include <string>
#include <unordered_map>
#include <algorithm> //sort
#include "imgui.h"
#include "imgui_stdlib.h" //imgui library for using std::string with input
#include "imgui_internal.h"

#include <shared_mutex>

// cvar types
enum class CVarType : char
{
	INT,
	FLOAT,
	STRING,
	VEC_3,
};

// "header" data for cvars
class CVarParameter
{
public:
	friend class CVarSystemImpl;

	int32_t arrayIndex{};

	CVarType type{};
	CVarFlags flags{};
	std::string name{};
	std::string description{};
};

//storage container for cvar data
template<typename T>
struct CVarStorage
{
	T initial{};
	T current{};
	CVarParameter* parameter{ nullptr };
};

// Array to hold all cvars
template<typename T>
struct CVarArray
{
	CVarStorage<T>* cvars{ nullptr };
	int32_t lastCVar{ 0 };

	// allocate the array
	CVarArray(size_t size)
	{
		cvars = new CVarStorage<T>[size]();
	}
	// de-allocate the array
	~CVarArray()
	{
		delete[] cvars;
	}

	//get the data of the cvar
	CVarStorage<T>* GetCurrentStorate(int32_t index)
	{
		return &cvars[index];
	}

	// get pointer to the current data
	T* GetCurrentPtr(int32_t index)
	{
		return &cvars[index].current;
	}

	// get copy of current data
	T GetCurrent(int32_t index)
	{
		return cvars[index].current;
	}

	// set data of current
	void SetCurrent(const T& val, int32_t index)
	{
		cvars[index].current = val;
	}

	//add new cvar to the system. returns id of the cvar
	int Add(const T& value, CVarParameter* param)
	{
		int index = lastCVar;

		cvars[index].current = value;
		cvars[index].initial = value;
		cvars[index].parameter = param;

		param->arrayIndex = index;
		lastCVar++;
		return index;
	}

	// add new a cvar with a default value
	int Add(const T& initialValue, const T& currentValue, CVarParameter* param)
	{
		int index = lastCVar;

		cvars[index].current = currentValue;
		cvars[index].initial = initialValue;
		cvars[index].parameter = param;

		param->arrayIndex = index;
		lastCVar++;

		return index;
	}

};

// implementation of a cvar system (singleton)
class CVarSystemImpl : public CVarSystem
{
public:
	CVarParameter* GetCVar(engine::utility::StringHash hash)  override final;

	double* GetFloatCVar(engine::utility::StringHash hash) override final;
	int32_t* GetIntCVar(engine::utility::StringHash hash) override final;
	const char* GetStringCVar(engine::utility::StringHash hash) override final;
	glm::vec3* GetVec3CVar(engine::utility::StringHash hash) override final;

	void SetFloatCVar(engine::utility::StringHash hash, double value) override final;
	void SetIntCVar(engine::utility::StringHash hash, int32_t value) override final;
	void SetStringCVar(engine::utility::StringHash hash, const char* value) override final;
	void SetVec3CVar(engine::utility::StringHash hash, glm::vec3 value) override final;

	CVarParameter* CreateFloatCVar(const char* name, const char* description, double defaultValue, double currentValue) override final;
	CVarParameter* CreateIntCVar(const char* name, const char* description, int32_t defaultValue, int32_t currentValue) override final;
	CVarParameter* CreateStringCVar(const char* name, const char* description, const char* defaultValue, const char* currentValue) override final;
	CVarParameter* CreateVec3CVar(const char* name, const char* description, glm::vec3 defaultValue, glm::vec3 currentValue) override final;


	//initialize storage containers
	constexpr static int MAX_INT_CVARS = 1000;
	CVarArray<int32_t> intCVars2{ MAX_INT_CVARS };

	constexpr static int MAX_FLOAT_CVARS = 1000;
	CVarArray<double> floatCVars{ MAX_FLOAT_CVARS };

	constexpr static int MAX_STRING_CVARS = 200;
	CVarArray<std::string> stringCVars{ MAX_STRING_CVARS };

	constexpr static int MAX_VEC3_CVARS = 1000;
	CVarArray<glm::vec3> vec3CVars{ MAX_VEC3_CVARS };

	//using tempaltes with specializations to get the cvar arrays for each type
	//if you try to use a type that doesnt have a specialization it will trigger a linked error
	template <typename T>
	CVarArray<T>* GetCVarArray(); // Intentionally not defined

	template<>
	CVarArray<int32_t>* GetCVarArray() { return &intCVars2; }

	template<>
	CVarArray<double>* GetCVarArray() { return &floatCVars; }

	template<>
	CVarArray<std::string>* GetCVarArray() { return &stringCVars; }

	template<>
	CVarArray<glm::vec3>* GetCVarArray() { return &vec3CVars; }



	template <typename T>
	T* GetCVarCurrent(uint32_t namehash)
	{
		CVarParameter* par = GetCVar(namehash);
		if (!par)
		{
			return nullptr;
		}
		else
		{
			return GetCVarArray<T>()->GetCurrentPtr(par->arrayIndex);
		}
	}

	template <typename T>
	void SetCVarCurrent(uint32_t namehash, const T& value)
	{
		CVarParameter* cvar = GetCVar(namehash);
		if (cvar)
		{
			GetCVarArray<T>()->SetCurrent(value, cvar->arrayIndex);
		}
	}

	static CVarSystemImpl* Get()
	{
		return static_cast<CVarSystemImpl*>(CVarSystem::Get());
	}

	void DrawImguiEditor() override final;

	void EditParameter(CVarParameter* p, float textWidth);

private:	
	mutable std::shared_mutex mutex_;

	CVarParameter* InitCVar(const char* name, const char* description);

	std::unordered_map<uint32_t, CVarParameter> savedCVars;

	std::vector<CVarParameter*> cachedEditParameters;
};


double* CVarSystemImpl::GetFloatCVar(engine::utility::StringHash hash)
{
	return GetCVarCurrent<double>(hash);
}

int32_t* CVarSystemImpl::GetIntCVar(engine::utility::StringHash hash)
{
	return GetCVarCurrent<int32_t>(hash);
}

const char* CVarSystemImpl::GetStringCVar(engine::utility::StringHash hash)
{
	return GetCVarCurrent<std::string>(hash)->c_str();
}

glm::vec3* CVarSystemImpl::GetVec3CVar(engine::utility::StringHash hash)
{
	return GetCVarCurrent<glm::vec3>(hash);
}

CVarSystem* CVarSystem::Get()
{
	static CVarSystemImpl cvarSys{};
	return &cvarSys;
}


CVarParameter* CVarSystemImpl::GetCVar(engine::utility::StringHash hash)
{
	std::shared_lock<std::shared_mutex> lock(mutex_);
	auto it = savedCVars.find(hash);

	if (it != savedCVars.end())
	{
		return &(*it).second;
	}

	return nullptr;
}

void CVarSystemImpl::SetFloatCVar(engine::utility::StringHash hash, double value)
{
	SetCVarCurrent<double>(hash, value);
}

void CVarSystemImpl::SetIntCVar(engine::utility::StringHash hash, int32_t value)
{
	SetCVarCurrent<int32_t>(hash, value);
}

void CVarSystemImpl::SetStringCVar(engine::utility::StringHash hash, const char* value)
{
	SetCVarCurrent<std::string>(hash, value);
}

void CVarSystemImpl::SetVec3CVar(engine::utility::StringHash hash, glm::vec3 value)
{
	SetCVarCurrent<glm::vec3>(hash, value);
}

CVarParameter* CVarSystemImpl::CreateFloatCVar(const char* name, const char* description, double defaultValue, double currentValue)
{
	std::unique_lock lock(mutex_);
	CVarParameter* param = InitCVar(name, description);
	if (!param) return nullptr;

	param->type = CVarType::FLOAT;

	GetCVarArray<double>()->Add(defaultValue, currentValue, param);

	return param;
}

CVarParameter* CVarSystemImpl::CreateIntCVar(const char* name, const char* description, int32_t defaultValue, int32_t currentValue)
{
	std::unique_lock lock(mutex_);
	CVarParameter* param = InitCVar(name, description);
	if (!param) return nullptr;

	param->type = CVarType::INT;

	GetCVarArray<int32_t>()->Add(defaultValue, currentValue, param);

	return param;
}

CVarParameter* CVarSystemImpl::CreateStringCVar(const char* name, const char* description, const char* defaultValue, const char* currentValue)
{
	std::unique_lock lock(mutex_);
	CVarParameter* param = InitCVar(name, description);
	if (!param) return nullptr;

	param->type = CVarType::STRING;

	GetCVarArray<std::string>()->Add(defaultValue, currentValue, param);

	return param;
}

CVarParameter* CVarSystemImpl::CreateVec3CVar(const char* name, const char* description, glm::vec3 defaultValue, glm::vec3 currentValue)
{
	std::unique_lock lock(mutex_);
	CVarParameter* param = InitCVar(name, description);
	if (!param) return nullptr;

	param->type = CVarType::VEC_3;

	GetCVarArray<glm::vec3>()->Add(defaultValue, currentValue, param);

	return param;
}

CVarParameter* CVarSystemImpl::InitCVar(const char* name, const char* description)
{
	//if (GetCVar(name)) return nullptr; //if exists return null

	uint32_t namehash = engine::utility::StringHash{ name };
	savedCVars[namehash] = CVarParameter{};

	CVarParameter& newparam = savedCVars[namehash];

	newparam.name = name;
	newparam.description = description;

	return &newparam;
}

//get the cvar data purely by type and array index
template<typename T>
T GetCVarCurrentByIndex(int32_t index)
{
	return CVarSystemImpl::Get()->GetCVarArray<T>()->GetCurrent(index);
}

template<typename T>
T* PtrGetCVarCurrentByIndex(int32_t index)
{
	return CVarSystemImpl::Get()->GetCVarArray<T>()->GetCurrentPtr(index);
}

//set the cvar data purely by type and index
template <typename T>
void SetCVarCurrentByIndex(int32_t index, const T& data)
{
	CVarSystemImpl::Get()->GetCVarArray<T>()->SetCurrent(data, index);
}


AutoCVar_Float::AutoCVar_Float(const char* name, const char* description, double defaultValue, CVarFlags flags)
{
	CVarParameter* cvar = CVarSystem::Get()->CreateFloatCVar(name, description, defaultValue, defaultValue);
	cvar->flags = flags;
	index = cvar->arrayIndex;
}

double AutoCVar_Float::Get()const
{
	return GetCVarCurrentByIndex<CVarType>(index);
}

double* AutoCVar_Float::GetPtr()
{
	return PtrGetCVarCurrentByIndex<CVarType>(index);
}

float AutoCVar_Float::GetFloat()const
{
	return static_cast<float>(Get());
}

float* AutoCVar_Float::GetFloatPtr()
{
	float* result = reinterpret_cast<float*>(GetPtr());
	return result;
}

void AutoCVar_Float::Set(double val)
{
	SetCVarCurrentByIndex<CVarType>(index, val);
}


AutoCVar_Vec3::AutoCVar_Vec3(const char* name, const char* description, glm::vec3 defaultValue, CVarFlags flags)
{
	CVarParameter* cvar = CVarSystem::Get()->CreateVec3CVar(name, description, defaultValue, defaultValue);
	cvar->flags = flags;
	index = cvar->arrayIndex;
}

glm::vec3 AutoCVar_Vec3::Get()const
{
	return GetCVarCurrentByIndex<CVarType>(index);
}

void AutoCVar_Vec3::Set(glm::vec3&& val)
{
	SetCVarCurrentByIndex<CVarType>(index, val);
}

AutoCVar_Int::AutoCVar_Int(const char* name, const char* description, int32_t defaultValue, CVarFlags flags)
{
	CVarParameter* cvar = CVarSystem::Get()->CreateIntCVar(name, description, defaultValue, defaultValue);
	cvar->flags = flags;
	index = cvar->arrayIndex;
}

int32_t AutoCVar_Int::Get()const
{
	return GetCVarCurrentByIndex<CVarType>(index);
}

int32_t* AutoCVar_Int::GetPtr()
{
	return PtrGetCVarCurrentByIndex<CVarType>(index);
}

void AutoCVar_Int::Set(int32_t val)
{
	SetCVarCurrentByIndex<CVarType>(index,val);
}

AutoCVar_String::AutoCVar_String(const char* name, const char* description, const char* defaultValue, CVarFlags flags)
{
	CVarParameter* cvar = CVarSystem::Get()->CreateStringCVar(name, description, defaultValue, defaultValue);
	cvar->flags = flags;
	index = cvar->arrayIndex;
}

const char* AutoCVar_String::Get()const
{
	return (*PtrGetCVarCurrentByIndex<CVarType>(index)).c_str();
}

void AutoCVar_String::Set(std::string&& val)
{
	SetCVarCurrentByIndex<CVarType>(index, val);
}


//----------------- imgui stuff --------------------------

void CVarSystemImpl::DrawImguiEditor()
{
	static std::string searchText = "";

	ImGui::InputText("Filter", &searchText);
	static bool bShowAdvanced = false;
	ImGui::Checkbox("Advanced", &bShowAdvanced);
	ImGui::Separator();
	cachedEditParameters.clear();

	auto addToEditList = [&](auto parameter)
	{
		bool bHidden = ((uint32_t)parameter->flags & (uint32_t)CVarFlags::Noedit);
		bool bIsAdvanced = ((uint32_t)parameter->flags & (uint32_t)CVarFlags::Advanced);

		if (!bHidden)
		{
			if (!(!bShowAdvanced && bIsAdvanced) && parameter->name.find(searchText) != std::string::npos)
			{
				cachedEditParameters.push_back(parameter);
			};
		}
	};

	for (int i = 0; i < GetCVarArray<int32_t>()->lastCVar; i++)
	{
		addToEditList(GetCVarArray<int32_t>()->cvars[i].parameter);
	}
	for (int i = 0; i < GetCVarArray<double>()->lastCVar; i++)
	{
		addToEditList(GetCVarArray<double>()->cvars[i].parameter);
	}
	for (int i = 0; i < GetCVarArray<std::string>()->lastCVar; i++)
	{
		addToEditList(GetCVarArray<std::string>()->cvars[i].parameter);
	}
	for (int i = 0; i < GetCVarArray<glm::vec3>()->lastCVar; i++)
	{
		addToEditList(GetCVarArray<glm::vec3>()->cvars[i].parameter);
	}

	if (cachedEditParameters.size() > 10)
	{
		std::unordered_map<std::string, std::vector<CVarParameter*>> categorizedParams;

		//insert all the edit parameters into the hashmap by category
		for (auto p : cachedEditParameters)
		{
			int dotPos = -1;
			//find where the first dot is to categorize
			for (int i = 0; i < p->name.length(); i++)
			{
				if (p->name[i] == '.')
				{
					dotPos = i;
					break;
				}
			}
			std::string category = "";
			if (dotPos != -1)
			{
				category = p->name.substr(0, dotPos);
			}

			auto it = categorizedParams.find(category);
			if (it == categorizedParams.end())
			{
				categorizedParams[category] = std::vector<CVarParameter*>();
				it = categorizedParams.find(category);
			}
			it->second.push_back(p);
		}

		for (auto [category, parameters] : categorizedParams)
		{
			//alphabetical sort
			std::sort(parameters.begin(), parameters.end(), [](CVarParameter* A, CVarParameter* B)
			{
				return A->name < B->name;
			});

			if (ImGui::BeginMenu(category.c_str()))
			{
				float maxTextWidth = 0;

				for (auto p : parameters)
				{
					maxTextWidth = std::max(maxTextWidth, ImGui::CalcTextSize(p->name.c_str()).x);
				}
				for (auto p : parameters)
				{
					EditParameter(p, maxTextWidth);
				}

				ImGui::EndMenu();
			}
		}
	}
	else
	{
		//alphabetical sort
		std::sort(cachedEditParameters.begin(), cachedEditParameters.end(), [](CVarParameter* A, CVarParameter* B)
		{
			return A->name < B->name;
		});
		float maxTextWidth = 0;
		for (auto p : cachedEditParameters)
		{
			maxTextWidth = std::max(maxTextWidth, ImGui::CalcTextSize(p->name.c_str()).x);
		}
		for (auto p : cachedEditParameters)
		{
			EditParameter(p, maxTextWidth);
		}
	}
}
void Label(const char* label, float textWidth)
{
	constexpr float Slack = 50;
	constexpr float EditorWidth = 100;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	const ImVec2 lineStart = ImGui::GetCursorScreenPos();
	const ImGuiStyle& style = ImGui::GetStyle();
	float fullWidth = textWidth + Slack;

	ImVec2 textSize = ImGui::CalcTextSize(label);

	ImVec2 startPos = ImGui::GetCursorScreenPos();

	ImGui::Text(label);

	ImVec2 finalPos = { startPos.x + fullWidth, startPos.y };

	ImGui::SameLine();
	ImGui::SetCursorScreenPos(finalPos);

	ImGui::SetNextItemWidth(EditorWidth);
}
void CVarSystemImpl::EditParameter(CVarParameter* p, float textWidth)
{
	const bool readonlyFlag = ((uint32_t)p->flags & (uint32_t)CVarFlags::EditReadOnly);
	const bool checkboxFlag = ((uint32_t)p->flags & (uint32_t)CVarFlags::EditCheckbox);
	const bool dragFlag = ((uint32_t)p->flags & (uint32_t)CVarFlags::EditFloatDrag);


	switch (p->type)
	{
		case CVarType::INT:

			if (readonlyFlag)
			{
				std::string displayFormat = p->name + "= %i";
				ImGui::Text(displayFormat.c_str(), GetCVarArray<int32_t>()->GetCurrent(p->arrayIndex));
			}
			else
			{
				if (checkboxFlag)
				{
					bool bCheckbox = GetCVarArray<int32_t>()->GetCurrent(p->arrayIndex) != 0;
					Label(p->name.c_str(), textWidth);

					ImGui::PushID(p->name.c_str());

					if (ImGui::Checkbox("", &bCheckbox))
					{
						GetCVarArray<int32_t>()->SetCurrent(bCheckbox ? 1 : 0, p->arrayIndex);
					}
					ImGui::PopID();
				}
				else
				{
					Label(p->name.c_str(), textWidth);
					ImGui::PushID(p->name.c_str());
					ImGui::InputInt("", GetCVarArray<int32_t>()->GetCurrentPtr(p->arrayIndex));
					ImGui::PopID();
				}
			}
			break;

		case CVarType::FLOAT:

			if (readonlyFlag)
			{
				std::string displayFormat = p->name + "= %f";
				ImGui::Text(displayFormat.c_str(), GetCVarArray<double>()->GetCurrent(p->arrayIndex));
			}
			else
			{
				Label(p->name.c_str(), textWidth);
				ImGui::PushID(p->name.c_str());
				if (dragFlag)
				{					
					ImGui::DragScalar("", ImGuiDataType_Double, GetCVarArray<double>()->GetCurrentPtr(p->arrayIndex), 0.1, 0, 0, "%.3f");
				}
				else
				{
					ImGui::InputDouble("", GetCVarArray<double>()->GetCurrentPtr(p->arrayIndex), 0, 0, "%.3f");
				}
				ImGui::PopID();
			}
			break;

		case CVarType::STRING:

			if (readonlyFlag)
			{
				std::string displayFormat = p->name + "= %s";
				ImGui::PushID(p->name.c_str());
				ImGui::Text(displayFormat.c_str(), GetCVarArray<std::string>()->GetCurrent(p->arrayIndex));

				ImGui::PopID();
			}
			else
			{
				Label(p->name.c_str(), textWidth);
				ImGui::InputText("", GetCVarArray<std::string>()->GetCurrentPtr(p->arrayIndex));

				ImGui::PopID();
			}
			break;

		case CVarType::VEC_3:

			if (readonlyFlag)
			{
				std::string displayFormat = p->name + "= (%0.3f,%0.3f,%0.3f)";
				glm::vec3 pos = GetCVarArray<glm::vec3>()->GetCurrent(p->arrayIndex);
				ImGui::Text(displayFormat.c_str(), pos.x, pos.y, pos.z);
			}
			else
			{
				Label(p->name.c_str(), textWidth);
				ImGui::PushID(p->name.c_str());
				if (dragFlag)
				{
					ImGui::DragFloat3("", &GetCVarArray<glm::vec3>()->GetCurrentPtr(p->arrayIndex)->x);
				}
				else
				{
					ImGui::InputFloat3("", &GetCVarArray<glm::vec3>()->GetCurrentPtr(p->arrayIndex)->x);
				}
				ImGui::PopID();
			}
			break;

		default:
			break;
	}

	if (ImGui::IsItemHovered())
	{
		ImGui::SetTooltip(p->description.c_str());
	}
}



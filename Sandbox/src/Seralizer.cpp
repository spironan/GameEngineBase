#include "Seralizer.h"
#include "Editor/RttrTypeID.h"

#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <istream>

#include "Engine/Transform/Transform3D.h"
#include "Engine/ECS/GameObject.h"
void Serializer::LoadObject(const std::string& prefab)
{
	std::ifstream ifs;
	ifs.open(prefab);
	if (!ifs.is_open())
		return;
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	for (auto& iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter)
	{
		auto& arr = iter->value.GetArray();
		engine::GameObject object{};
		engine::GameObject(0).AddChild(object);
		for (int count = 0; count < arr.Size(); ++count)
		{
			std::string component = arr[count].GetString();
			if (component == "Transform3D")
			{
				++count;
				auto& trans = arr[count].GetArray();
				object.AddComponent<engine::Transform3D>();
				engine::Transform3D& transform = object.GetComponent<engine::Transform3D>();

				auto& vec3arr = trans[0].GetArray();
				transform.SetPosition({ vec3arr[0].GetFloat(), vec3arr[1].GetFloat(), vec3arr[2].GetFloat() });
				vec3arr = trans[1].GetArray();
				transform.SetRotationAxis({ vec3arr[0].GetFloat(), vec3arr[1].GetFloat(), vec3arr[2].GetFloat() });
				transform.SetRotationAngle(trans[2].GetFloat());
			}
			
		}
	}
}

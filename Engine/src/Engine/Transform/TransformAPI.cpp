#include "pch.h"

#include "TransformAPI.h"
#include "Engine/Scripting/Scripting.h"
#include "Engine/Scene/SceneManager.h"

SCRIPT_API_LINK(Transform)

namespace engine
{
    namespace scriptAPI
    {
        SCRIPT_API void Transform_GetLocalPosition(Entity instanceID, float* x, float* y, float* z)
        {
            GameObject obj{ instanceID };
            Transform3D& transform = obj.GetComponent<Transform3D>();
            oom::vec3 position = transform.GetPosition();
            *x = position.x;
            *y = position.y;
            *z = position.z;
        }

        SCRIPT_API void Transform_SetLocalPosition(Entity instanceID, float x, float y, float z)
        {
            GameObject obj{ instanceID };
            Transform3D& transform = obj.GetComponent<Transform3D>();
            transform.SetPosition({ x, y, z });
        }

        //SCRIPT_API void Transform_GetGlobalPosition(Entity instanceID, float* x, float* y, float* z)
        //{
        //    GameObject obj{ instanceID };
        //    Transform3D& transform = obj.GetComponent<Transform3D>();
        //    oom::vec3 position = transform.GetGlobalPosition();
        //    *x = position.x;
        //    *y = position.y;
        //    *z = position.z;
        //}

        //SCRIPT_API void Transform_SetGlobalPosition(Entity instanceID, float x, float y, float z)
        //{
        //    GameObject obj{ instanceID };
        //    Transform3D& transform = obj.GetComponent<Transform3D>();
        //}



        SCRIPT_API float Transform_GetLocalAngle(Entity instanceID)
        {
            GameObject obj{ instanceID };
            Transform3D& transform = obj.GetComponent<Transform3D>();
            return transform.GetRotationAngle();
        }

        SCRIPT_API void Transform_SetLocalAngle(Entity instanceID, float angle)
        {
            GameObject obj{ instanceID };
            Transform3D& transform = obj.GetComponent<Transform3D>();
            transform.SetRotationAngle(angle);
        }



        SCRIPT_API void Transform_GetLocalScale(Entity instanceID, float* x, float* y, float* z)
        {
            GameObject obj{ instanceID };
            oom::vec3 const& scale = obj.GetComponent<Transform3D>().GetScale();
            *x = scale.x;
            *y = scale.y;
            *z = scale.z;
        }

        SCRIPT_API void Transform_SetLocalScale(Entity instanceID, float x, float y, float z)
        {
            GameObject obj{ instanceID };
            obj.GetComponent<Transform3D>().SetScale({ x, y, z });
        }



        SCRIPT_API int Transform_GetChildCount(Entity instanceID)
        {
            GameObject obj{ instanceID };
            return obj.GetComponent<Transform3D>().GetChildCount();
        }

        SCRIPT_API uint32_t Transform_GetChild(Entity instanceID, size_t childIndex)
        {
            GameObject obj{ instanceID };
            GameObject child{ obj.GetChildren()[childIndex] };
            return child.GetComponent<Scripting>().GetComponentInterface<Transform3D>();
        }

        SCRIPT_API void Transform_SetParent(Entity instanceID, Entity newParent, bool preserveTransforms)
        {
            GameObject obj{ instanceID };
            if (newParent == (Entity)-1)
            {
                SceneManager::GetActiveRoot().AddChild(obj, preserveTransforms);
                return;
            }
            GameObject parentObj{ newParent };
            parentObj.AddChild(obj, preserveTransforms);
        }

        SCRIPT_API uint32_t Transform_GetParent(Entity instanceID)
        {
            GameObject obj{ instanceID };
            GameObject parent{ obj.GetComponent<Transform3D>().GetParentId() };
            GameObject const& root = SceneManager::GetActiveRoot();
            if (parent == root)
                return 0;
            return parent.GetComponent<Scripting>().GetComponentInterface<Transform3D>();
        }
    }
}
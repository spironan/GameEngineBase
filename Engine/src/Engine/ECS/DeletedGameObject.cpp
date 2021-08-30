#pragma once
#include "pch.h"
#include "DeletedGameObject.h"
namespace engine
{
    DeletedGameObject::DeletedGameObject()
    {
        m_componentList.fill(nullptr);
    }

    DeletedGameObject::~DeletedGameObject()
    {
        for (auto i : m_componentList)
        {
            if (i != nullptr)
            {
                delete[] i;
                i = nullptr;
            }
        }
    }

}
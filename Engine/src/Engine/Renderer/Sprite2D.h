/************************************************************************************//*!
\file          Sprite2D.h
\project       <PROJECT_NAME>
\author        Jamie Kong, j.kong , 390004720
\par           email: j.kong\@digipen.edu
\date          August 1, 2021
\brief         File contains API for a Sprite2D component. 

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/ECS/Component.h"

#include "Engine/Asset/Asset.h"

#include <memory>
#include <limits>

//#include <glm/glm.hpp>
#include <oom/oom.hpp>

#include <rttr/type>


namespace engine
{

    /********************************************************************************//*!
     @brief    Defines a 2D transform component. A Transform Component is used to
               identify a position in space and how much to rotate and scale the
               object by.
               Refer to this as a template of how to implement a component.
    *//*********************************************************************************/
    class Sprite2D final : public Component
    {
    public:

      /*-----------------------------------------------------------------------------*/
      /* Constructors and Destructors                                                */
      /*-----------------------------------------------------------------------------*/
        Sprite2D() = delete;
        Sprite2D(Sprite2D const&) = default;
        Sprite2D(Sprite2D&&) = default;
        Sprite2D& operator=(Sprite2D const&) = default;
        Sprite2D& operator=(Sprite2D&&) = default;
        virtual ~Sprite2D() override = default;

        /****************************************************************************//*!
         @brief    Overloaded constructor for transform component

         @param[in]    _entityID
            The id that is attached to the entity.
         @param[in]    _active
            Determines if the component is active or not.
        *//*****************************************************************************/
        Sprite2D(Entity _entityID, bool _active = true);

        /*-----------------------------------------------------------------------------*/
        /* Getter Functions                                                            */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief    Returns the raw value of the texture to use for rendering
         
        \return   rendering texture ID
        *//**********************************************************************************/
        ooRendererID GetTextureRaw() { return m_texture?m_texture->GetID(): 0 ; }

        /*********************************************************************************//*!
        \brief    Gets the colour tint of the texture
         
        \return   Colour
        *//**********************************************************************************/
        oom::vec4 GetColor() { return m_color; }

        /*-----------------------------------------------------------------------------*/
        /* Setter Functions                                                            */
        /*-----------------------------------------------------------------------------*/

         /*********************************************************************************//*!
        \brief    Sets the texture ID using a texture2d asset

        \param    Texture asset
        *//**********************************************************************************/
        void SetTexture(std::shared_ptr<Texture2D> tex);

          /*********************************************************************************//*!
        \brief    Sets the texture ID using a texture asset

        \param    Texture asset
        *//**********************************************************************************/
        void SetTexture(std::shared_ptr<Texture> tex);

          /*********************************************************************************//*!
        \brief    Sets the texture ID using a texture asset

        \param    Texture asset
        *//**********************************************************************************/
        void SetTexture(AssetHandle tex);

        /*********************************************************************************//*!
        \brief    Sets the colour tint
         
        \param    col tint to set
        *//**********************************************************************************/
        void SetColor(oom::vec4 col) { m_color = col; }

        /*********************************************************************************//*!
        \brief    Sets the colour tint to the RGBA value
         
        \param    r red val
        \param    g green val
        \param    b blue val
        \param    alpha transparency value
        *//**********************************************************************************/
        void SetColor(float r, float g, float b, float alpha = 1.0f) { m_color.r = r; m_color.g = g; m_color.b = b; m_color.a = alpha;}

        //const bool   IsDirty()           const { return m_dirty; }
        //const bool   HasChanged()        const { return m_hasChanged; }

        /*-----------------------------------------------------------------------------*/
        /* Member Functions                                                            */
        /*-----------------------------------------------------------------------------*/

        AssetHandle GetHandle() const { return m_texture->GetHandle(); }

        RTTR_ENABLE();

    private:

        std::shared_ptr<Texture> m_texture;

        oom::vec4 m_color{1.0f,1.0f,1.0f,1.0f};

    };

    extern "C"
    {
        __declspec(dllexport) void Sprite2D_GetColor(Entity instanceID, float* r, float* g, float* b, float* a);
        __declspec(dllexport) void Sprite2D_SetColor(Entity instanceID, float r, float g, float b, float alpha = 1.0f);
    }

} // namespace Engine



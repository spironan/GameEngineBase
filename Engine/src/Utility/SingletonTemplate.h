/************************************************************************************//*!
\file           SingletonTemplate.h
\project        Tenebris
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           Jul 07, 2021
\brief          Template class that transforms inherited classes into a singleton.

                SingletonTemplate should only be used for something that should only has
                one instance of in the lifetime of the program. With this template,
                it allows classes to become one easily.
                Take note this comes with risk,
                everyone else having access and ability to change its data too.

                How to use : inherit from this template class and friend it inside
                the class

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/

#pragma once

namespace engine {
namespace utility{
    /********************************************************************************//*!
     @brief    Template class that allows for inherited class to become a singleton
     
               Template class that transforms inherited classes into a singleton.
            
               SingletonTemplate should only be used for something that should only has
               one instance of in the lifetime of the program. With this template,
               it allows classes to become one easily.
               Take note this comes with risk,
               everyone else having access and ability to change its data too.
     
               How to use : inherit from this template class and friend it inside
               the class
     
     @tparam   Type 
        The class to be made a singleton
    *//*********************************************************************************/
    template<class Type>
    class SingletonTemplate
    {
      public:
        
        /****************************************************************************//*!
         @brief     Main interface function to call that 
                    creates the singleton object (or return an existing one)
         
         @return    Instance of the object 
        *//*****************************************************************************/
        inline static Type& GetInstance()
        {
            if (instance == nullptr)
            {
                instance = new Type();
            }

            return *instance;
        }

        /****************************************************************************//*!
         @brief    Interface function to call that destroys the singleton
         
         @return   Whether the operation was successful or not, unsuccessful means
                   the instance was already previously destroyed.
        *//*****************************************************************************/
        inline static bool Destroy()
        {
            if (instance == nullptr)
            {
                return false;
            }
            delete instance;
            instance = nullptr;
            return true;
        }

      protected:
        //default Constructor(s) & Destructor
        SingletonTemplate() = default;
        virtual ~SingletonTemplate() = default;

      private:
        static Type* instance;
    };

    template<class Type>
    Type* SingletonTemplate<Type>::instance = nullptr;
}} // namespace engine::utility
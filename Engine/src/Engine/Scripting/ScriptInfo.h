/************************************************************************************//*!
\file           ScriptInfo.h
\project        <PROJECT_NAME>
\author         Solomon Tan Teng Shue, t.tengshuesolomon, 620010020
\par            email: t.tengshuesolomon\@digipen.edu
\date           August 3, 2021
\brief          Declares the structs required to contain all the information
                needed to create a new script instance during play mode using data
                set in the editor inspector durng edit mode

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include <string>
#include <vector>
#include <variant>

namespace engine
{
    // forward declaration
    struct ScriptFieldInfo;
    struct ScriptFieldValue;

    typedef std::variant<
        std::monostate,
        bool,
        int,
        float,
        std::string,
        std::vector<ScriptFieldInfo>,
        std::vector<ScriptFieldValue>
    > ScriptValue;

    enum class ScriptValueType
    {
        EMPTY = 0,
        BOOL,
        INT,
        FLOAT,
        STRING,
        CLASS,
        LIST,
    };

    /*********************************************************************************//*!
    \brief      overloads the << operator to output the ScriptValueType enum
                to a given output stream, mostly used for debugging
     
    \param      os
            the output stream to write the value type to
                value
            the ScriptValueType to write
    \return     the ouput stream given
    
    *//**********************************************************************************/
    std::ostream& operator<<(std::ostream& os, ScriptValueType value);

    struct ScriptFieldValue
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors                                                                */
        /*-----------------------------------------------------------------------------*/
        ScriptFieldValue() : value(std::monostate()) {};
        inline ScriptFieldValue(ScriptValue const& _value) : value(_value) {};

        /*-----------------------------------------------------------------------------*/
        /* Get Type Functions                                                          */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      gets the type of the field value as a ScriptValueType enum
         
        \return     the type of the field value as a ScriptValueType enum
        *//**********************************************************************************/
        inline ScriptValueType GetValueType() const
        {
            return static_cast<ScriptValueType>(value.index());
        }

        /*********************************************************************************//*!
        \brief      checks if the field value is of a specific type
         
        \return     true if the field value is of the given type, else false
        
        *//**********************************************************************************/
        template<typename T>
        bool IsValueType() const
        {
            return std::holds_alternative<T>(value);
        }

        /*-----------------------------------------------------------------------------*/
        /* getter and setter                                                           */
        /*-----------------------------------------------------------------------------*/

        /*********************************************************************************//*!
        \brief      gets the script field's value as a specified type
        
        \warning    This function assumes that the field value's type is the one given,
                    so a check should be done before using this function if the 
                    value's type is not definite
         
        \return     the field value as the specified type
        
        *//**********************************************************************************/
        template<typename T>
        T const GetValue() const
        {
            return std::get<T>(value);
        }

        /*********************************************************************************//*!
        \brief      sets the script field's value to a new value if the type of the new value
                    matches with the actual type of the field's value
         
        \param      newValue
                the new value to set the field value as
        \return     true if the value was successfully set, else false
        
        *//**********************************************************************************/
        template<typename T>
        bool SetValue(T const& newValue)
        {
            if (!std::holds_alternative<T>(value))
                return false;
            value = newValue;
            return true;
        }

    private:
        ScriptValue value;
    };

    /*********************************************************************************//*!
    \brief      overloads the << operator to output the Script Field's Value
                to a given output stream, mostly used for debugging

    \param      os
            the output stream to write the value to
                value
            the ScriptFieldvalue to write
    \return     the ouput stream given

    *//**********************************************************************************/
    std::ostream& operator<<(std::ostream& os, ScriptFieldValue const& value);

    struct ScriptFieldInfo
    {
        std::string name;
        ScriptFieldValue value;
    };

    /*********************************************************************************//*!
    \brief      overloads the << operator to output the Script Field's Information
                to a given output stream, mostly used for debugging

    \param      os
            the output stream to write the value to
                value
            the ScriptFieldInfo to write
    \return     the ouput stream given

    *//**********************************************************************************/
    std::ostream& operator<<(std::ostream& os, ScriptFieldInfo const& value);

    struct ScriptClassInfo
    {
        std::string name_space;
        std::string name;

        /*-----------------------------------------------------------------------------*/
        /* Default Constructor                                                         */
        /*-----------------------------------------------------------------------------*/
        ScriptClassInfo(std::string const& _namespace, std::string const& _name) : name_space(_namespace), name(_name) {}

        /*********************************************************************************//*!
        \brief      gets a list of the info of all public fields of the C# class
         
        \return     a vector containing the info of all public fields of the class
        *//**********************************************************************************/
        std::vector<ScriptFieldInfo> const GetScriptFieldInfoAll();

        /*********************************************************************************//*!
        \brief      gets a list of the info of all the C# classes that are used by a given
                    generic public field in the class
         
        \param      fieldName
                the name of the generic public field

        \return     a vector containing the info of all C# classes used by the given field
        *//**********************************************************************************/
        std::vector<ScriptClassInfo> const GetFieldGenericTypeParams(const char* fieldName);

        /*********************************************************************************//*!
        \brief      overloads the == operator to check if the info of two ScriptClassInfo match
         
        \param      rhs
                the other ScriptClassInfo to compare to

        \return     true if the namespace and name of the classes match, else false
        *//**********************************************************************************/
        bool operator==(ScriptClassInfo const& rhs);

        /*********************************************************************************//*!
        \brief      overloads the != operator to check if the info of two ScriptClassInfo do not match

        \param      rhs
                the other ScriptClassInfo to compare to

        \return     true if the namespace and name of the classes do not match, else false
        *//**********************************************************************************/
        bool operator!=(ScriptClassInfo const& rhs);
    };

    struct ScriptInfo
    {
        ScriptClassInfo classInfo;
        std::vector<ScriptFieldInfo> fieldList;

        /*********************************************************************************//*!
        \brief      constructs a C# script's info from the given class info
         
        \param      _classInfo
                the info of the C# script's class
        *//**********************************************************************************/
        ScriptInfo(ScriptClassInfo const& _classInfo);

        /*********************************************************************************//*!
        \brief      gets a pointer to the info of a desired field of the script by name
         
        \param      fieldName
                the name of the desired field
         * 
        \return     a pointer to the desired field's info, or null if it was not found
        *//**********************************************************************************/
        ScriptFieldInfo* FindFieldInfo(std::string const& fieldName);

        /*********************************************************************************//*!
        \brief      outputs all of the script's info to the standard output stream
                    for debugging purposes
        *//**********************************************************************************/
        void DebugPrint() const;
    };
}

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
    std::ostream& operator<<(std::ostream& os, ScriptValueType value);

    struct ScriptFieldValue
    {
    public:
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        ScriptFieldValue() : value(std::monostate()) {};
        inline ScriptFieldValue(ScriptValue const& _value) : value(_value) {};

        /*-----------------------------------------------------------------------------*/
        /* Get Type Functions                                                          */
        /*-----------------------------------------------------------------------------*/

        inline ScriptValueType GetValueType() const
        {
            return static_cast<ScriptValueType>(value.index());
        }

        template<typename T>
        bool IsValueType() const
        {
            return std::holds_alternative<T>(value);
        }

        /*-----------------------------------------------------------------------------*/
        /* getter and setter                                                           */
        /*-----------------------------------------------------------------------------*/

        template<typename T>
        bool GetValue(T* target) const
        {
            if (!std::holds_alternative<T>(value))
                return false;
            *target = std::get<T>(value);
            return true;
        }

        template<typename T>
        T const GetValue() const
        {
            return std::get<T>(value);
        }

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
    std::ostream& operator<<(std::ostream& os, ScriptFieldValue const& value);

    struct ScriptFieldInfo
    {
        std::string name;
        ScriptFieldValue value;
    };
    std::ostream& operator<<(std::ostream& os, ScriptFieldInfo const& value);

    struct ScriptClassInfo
    {
        std::string name_space;
        std::string name;

        ScriptClassInfo(std::string const& _namespace, std::string const& _name) : name_space(_namespace), name(_name) {}

        std::vector<ScriptFieldInfo> const GetScriptFieldInfoAll();
        std::vector<ScriptClassInfo> const GetFieldGenericTypeParams(const char* fieldName);

        bool operator==(ScriptClassInfo const& rhs);
        bool operator!=(ScriptClassInfo const& rhs);
    };

    struct ScriptInfo
    {
        ScriptClassInfo classInfo;
        std::vector<ScriptFieldInfo> fieldList;

        ScriptInfo(ScriptClassInfo const& _classInfo);

        ScriptFieldInfo* FindFieldInfo(std::string const& fieldName);
        void DebugPrint() const;
    };
}

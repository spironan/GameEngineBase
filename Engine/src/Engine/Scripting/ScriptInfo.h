#pragma once

#include <string>
#include <vector>
#include <rttr/variant.h>
#include <rttr/registration.h>

namespace engine
{
    struct ScriptClassInfo
    {
        std::string const name_space;
        std::string const name;

        ScriptClassInfo(std::string const& _namespace, std::string const& _name) : name_space(_namespace), name(_name) {}
    };

    struct ScriptFieldInfo
    {
        std::string const name;
        rttr::variant value;

        ScriptFieldInfo(std::string const& _name, rttr::variant _value) : name(_name), value(_value) {}
    };

    class ScriptInfo
    {
        ScriptClassInfo const classInfo;
        std::vector<ScriptFieldInfo> fieldList;
    };
}

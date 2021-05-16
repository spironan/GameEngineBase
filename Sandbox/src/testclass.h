#pragma once
#include "pch.h"
#include <rttr/type>
class testclass
{
public:
    testclass(int v = 20) :i1{ v }, i2{ v + 1 } { id += 1; uid = id; }
public:
    static int id;
    int i1;
    int i2;
    int uid;
    std::string name = "obj";
    testclass* parent = nullptr;
    std::vector<testclass*>childs;

    void addchild(testclass* c) { childs.emplace_back(c); c->parent = this; }

    void SetParent(testclass* p)
    {
        if (parent)
        {
            for (std::vector<testclass*>::iterator it = parent->childs.begin(); it < parent->childs.end();++it)
            {
                if ((*it) == this)
                {
                    parent->childs.erase(it);
                    break;
                }
            }
        }
        p->addchild(this);
    }
    void CreateTestClass(int a, int b, int c, std::string d)
    {
        auto list = rttr::type::get<testclass>().get_properties();
        list[1].set_value(this, a);
        list[1].set_value(this, b);
        list[2].set_value(this, c);
        list[3].set_value(this, d);
    }
    RTTR_ENABLE()
};


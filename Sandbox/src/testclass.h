#pragma once
#include "pch.h"
#include <rttr/type>
class testclass
{
public:
    testclass(int v = 20) :i1{ v }, f2{float( v + 1) } { id += 1; uid = id; }
    testclass(const testclass& c) :i1{ c.i1 }, f2{ c.f2 }, name{ c.name }, parent{ c.parent }, childs{ c.childs }, uid{ c.uid } {}
public:
    static int id;
    int i1;
    float f2;
    int uid;
    std::string name = "obj";
	std::string somestring = "abc";
    testclass* parent = nullptr;
    std::vector<testclass*>childs;

    void addchild(testclass* c) 
    {
        if (c)
        {
            childs.push_back(c);
            c->parent = this; 
        }
    }

    void SetParent(testclass* p)
    {
        if (parent)
        {
            for (std::vector<testclass*>::iterator it = parent->childs.begin(); it != parent->childs.end();++it)
            {
                if ((*it)->uid == uid)
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


#include "testclass.h"
#include <rttr/registration>

int testclass::id = 0;


RTTR_REGISTRATION
{
    using namespace rttr;
registration::class_<testclass>("test_class")
        .constructor<int>()
        .property("i1", &testclass::i1)
        .property("f2", &testclass::f2)
        .property("uid", &testclass::uid)
        .property("name", &testclass::name)
		.property("somestring", &testclass::somestring);
}

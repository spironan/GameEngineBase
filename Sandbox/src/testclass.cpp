#include "testclass.h"
#include <rttr/registration>

int testclass::id = 0;


RTTR_REGISTRATION
{
    using namespace rttr;
registration::class_<testclass>("test_class")
        .constructor<int>()
        .property("i1", &testclass::i1)
        .property("i2", &testclass::i2)
        .property("uid", &testclass::uid)
        .property("name", &testclass::name);
}

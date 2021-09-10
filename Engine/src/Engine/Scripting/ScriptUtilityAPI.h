#pragma once

#define SCRIPT_API extern "C" __declspec(dllexport)

#define SCRIPT_API_LINK(name) \
namespace engine \
{ \
    namespace scriptAPI \
    { \
        int force_link_##name = 0; \
    } \
}

#define SCRIPT_API_ANCHOR(name) \
namespace engine \
{ \
    namespace scriptAPI \
    { \
        void Force_Link_##name() { extern int force_link_##name; force_link_##name = 1; } \
    } \
}

//nam namespace scriptAPI void Force_Link_##name() { extern int force_link_##name; force_link_##name = 1; }

// this is stupid
//#define SCRIPT_API_LINK_API \
//namespace engine \
//{ \
//    namespace scriptAPI \
//    { \
//        void PreventAPFromBeingExcludedFromSandbox(); \
//    } \
//}
//
//#define SCRIPT_API_HOOK_API \
//namespace engine \
//{ \
//    namespace scriptAPI \
//    { \
//        void PreventAPFromBeingExcludedFromSandbox() {} \
//    } \
//}
//
//#define SCRIPT_API_ANCHOR_API \
//namespace engine \
//{ \
//    namespace scriptAPI \
//    { \
//        void ScriptAPIAnchor() \
//        { \
//            PreventAPFromBeingExcludedFromSandbox(); \
//        } \
//    } \
//}
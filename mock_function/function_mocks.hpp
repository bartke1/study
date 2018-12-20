#include <stdio.h>
#include <gmock/gmock.h>

#define FMOCK_T(name, ret_t, ...) _FMOCK_NAME(name)<ret_t(__VA_ARGS__)>

#define INSTALL_FMOCK(...) \
    DECLARE_FMOCK(__VA_ARGS__);\
    DEFINE_FMOCK(__VA_ARGS__);

#define DECLARE_FMOCK(name,...) \
    template<typename T>\
    struct _FMOCK_NAME(name);\
    template<>\
    struct _FMOCK_NAME(name)<_GET_FUN_TYPE(__VA_ARGS__)>\
    {\
        _FMOCK_NAME(name)()\
        {\
            obj = this;\
        }\
    \
        virtual ~_FMOCK_NAME(name)()\
        {\
            obj = NULL;\
        }\
    \
        _GET_MOCK_METHOD(name, __VA_ARGS__);\
    \
        static _FMOCK_NAME(name)<_GET_FUN_TYPE(__VA_ARGS__)>* obj;\
    }

#define DEFINE_FMOCK(name, ...) \
    _FMOCK_NAME(name)<_GET_FUN_TYPE(__VA_ARGS__)>* _FMOCK_NAME(name)<_GET_FUN_TYPE(__VA_ARGS__)>::obj = NULL;\
    \
    _GET_FIRST(__VA_ARGS__) name(_GET_IN_ARGS(__VA_ARGS__))\
    \
    {\
        _FMOCK_NAME(name)<_GET_FUN_TYPE(__VA_ARGS__)>::obj->name(_GET_OUT_ARGS(__VA_ARGS__));\
    }

#define _GET_OVERRIDE(_1, _2, _3, name, ...) name
#define _GET_MOCK_METHOD(name, ...) \
    _GET_OVERRIDE(__VA_ARGS__ , MOCK_METHOD2, MOCK_METHOD1, MOCK_METHOD0)(\
        name, _GET_FIRST(__VA_ARGS__)(_GET_ALL_BUT_FIRST(__VA_ARGS__))\
    )
#define _GET_IN_ARGS(...) \
    _GET_OVERRIDE(__VA_ARGS__, _GET_IN_PARAM2, _GET_IN_PARAM1, _GET_IN_PARAM0)(__VA_ARGS__)
#define _GET_OUT_ARGS(...) \
    _GET_OVERRIDE(__VA_ARGS__, _GET_OUT_PARAM2, _GET_OUT_PARAM1, _GET_OUT_PARAM0)(__VA_ARGS__)
#define _GET_IN_PARAM0(_)
#define _GET_IN_PARAM1(_, T1) T1 p1
#define _GET_IN_PARAM2(_, T1, T2) T1 p1, T2 p2
#define _GET_OUT_PARAM0(_)
#define _GET_OUT_PARAM1(_, T1) p1
#define _GET_OUT_PARAM2(_, T1, T2) p1, p2

#define _FMOCK_NAME(NAME) NAME##_FMOCK_IDENT
#define _GET_FIRST(first, ...) first
#define _GET_ALL_BUT_FIRST(first, ...) __VA_ARGS__
#define _GET_FUN_TYPE(...) \
    _GET_FIRST(__VA_ARGS__)(_GET_ALL_BUT_FIRST(__VA_ARGS__))

#include <stdio.h>
#include <gmock/gmock.h>

/*
 * This file contains macros to facilitate mocking free functions using gmock:
 * - DECLARE_FMOCK(function_name, return_arg_type, input_arg_type...)
 *      - declares structures needed by the mock; typically put in mock library header file, eg.
 *
 * - DEFINE_FMOCK(function_name, return_arg_type, input_arg_type...)
 *      - defines global objects needed by the mock; typically put in mock library cpp file
 *
 * - PREPARE_FMOCK(function_name, return_arg_type, input_arg_type...)
 *      - does both above steps; typically put in test cpp file
 *
 * - FMOCK_T(function_name, return_arg_type, input_arg_type...)
 *      - provides type of gmock object; once created function is mocked (until deletion of the object)
 */

#define FMOCK_T(name, ret_t, ...) _FMOCK_NAME(name)<ret_t(__VA_ARGS__)>

#define PREPARE_FMOCK(...) \
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

#define _GET_OVERRIDE(_1, _2, _3, _4, _5, name, ...) name
#define _GET_MOCK_METHOD(name, ...) \
    _GET_OVERRIDE(__VA_ARGS__ , MOCK_METHOD4, MOCK_METHOD3, MOCK_METHOD2, MOCK_METHOD1, MOCK_METHOD0)(\
        name, _GET_FIRST(__VA_ARGS__)(_GET_ALL_BUT_FIRST(__VA_ARGS__))\
    )
#define _GET_IN_ARGS(...) \
    _GET_OVERRIDE(__VA_ARGS__, _GET_IN_PARAM4, _GET_IN_PARAM3, _GET_IN_PARAM2, _GET_IN_PARAM1, _GET_IN_PARAM0)(__VA_ARGS__)
#define _GET_OUT_ARGS(...) \
    _GET_OVERRIDE(__VA_ARGS__, _GET_OUT_PARAM4, _GET_OUT_PARAM3, _GET_OUT_PARAM2, _GET_OUT_PARAM1, _GET_OUT_PARAM0)(__VA_ARGS__)
#define _GET_IN_PARAM0(_)
#define _GET_IN_PARAM1(_, T1) T1 p1
#define _GET_IN_PARAM2(_, T1, T2) T1 p1, T2 p2
#define _GET_IN_PARAM3(_, T1, T2, T3) T1 p1, T2 p2, T3 p3
#define _GET_IN_PARAM4(_, T1, T2, T3, T4) T1 p1, T2 p2, T3 p3, T4 p4
#define _GET_OUT_PARAM0(_)
#define _GET_OUT_PARAM1(_, T1) p1
#define _GET_OUT_PARAM2(_, T1, T2) p1, p2
#define _GET_OUT_PARAM3(_, T1, T2, T3) p1, p2, p3
#define _GET_OUT_PARAM4(_, T1, T2, T3, T4) p1, p2, p3, p4

#define _FMOCK_NAME(NAME) NAME##_FMOCK_IDENT
#define _GET_FIRST(first, ...) first
#define _GET_ALL_BUT_FIRST(first, ...) __VA_ARGS__
#define _GET_FUN_TYPE(...) \
    _GET_FIRST(__VA_ARGS__)(_GET_ALL_BUT_FIRST(__VA_ARGS__))

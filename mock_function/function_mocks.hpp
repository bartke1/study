#include <gmock/gmock.h>

#define _MOCK_SUFFIX _fmock

#define FMOCK(NAME) NAME ## _MOCK_SUFFIX

#define _GET_NTH_ARG(_1, _2, _3, _4, N, ...) N
#define COUNT_VARARGS(...) _GET_NTH_ARG(__VA_ARGS__, 4, 3, 2, 1)
#define _GET_OVERRIDE_M2(_1, _2, _3, NAME, ...) NAME
#define _GET_OVERRIDE(_1, NAME, ...) NAME

//#define DECLARE_FMOCK(...) \
//    _GET_OVERRIDE_M2(__VA_ARGS__, DECLARE_FMOCK1, DECLARE_FMOCK0)(__VA_ARGS__)

//#define DEFINE_FMOCK(...) \
        _GET_OVERRIDE_M2(__VA_ARGS__, DEFINE_FMOCK1, DEFINE_FMOCK0)(__VA_ARGS__)

#define _UNI_MOCK_METHOD(NAME, SIGNATURE, ...) \
    _GET_OVERRIDE(__VA_ARGS__, MOCK_METHOD1, MOCK_METHOD0)(NAME, SIGNATURE)

#define _GET_ARGS_BUT_FIRST(first, ...) __VA_ARGS__

#define DECLARE_FMOCK(NAME,RET,...) \
struct FMOCK(NAME)\
{\
    FMOCK(NAME)()\
    {\
        obj = this;\
    }\
\
    virtual ~FMOCK(NAME)()\
    {\
        obj = nullptr;\
    }\
\
    _UNI_MOCK_METHOD(NAME, RET(__VA_ARGS__), __VA_ARGS__);\
\
    static FMOCK(NAME)* obj;\
}

#define DEFINE_FMOCK(NAME) \
FMOCK(NAME)* FMOCK(NAME)::obj;\
\
void NAME(int x)\
{\
    FMOCK(NAME)::obj->NAME(x);\
}

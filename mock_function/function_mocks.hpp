#include <gmock/gmock.h>

//#define _MOCK_SUFFIX _fmock

#define FMOCK(NAME) NAME ## _MOCK_SUFFIX

#define _GET_NTH_ARG(_1, _2, _3, _4, N, ...) N
#define COUNT_VARARGS(...) _GET_NTH_ARG(__VA_ARGS__, 4, 3, 2, 1)
#define _GET_OVERRIDE(_1, _2, _3, NAME, ...) NAME

#define DECLARE_FMOCK(...) \
    _GET_OVERRIDE(__VA_ARGS__, DECLARE_FMOCK1, DECLARE_FMOCK0)(__VA_ARGS__)

#define DEFINE_FMOCK(...) \
    _GET_OVERRIDE(__VA_ARGS__, DEFINE_FMOCK1, DEFINE_FMOCK0)(__VA_ARGS__)

//#define _GET_ARGS_BUT_FIRST(first, ...) __VA_ARGS__

#define DECLARE_FMOCK1(NAME,RET,PARAM1) \
template<typename T>\
struct FMOCK(NAME);\
template<>\
struct foo_MOCK_SUFFIX<void(int)>\
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
    MOCK_METHOD1(NAME, RET(PARAM1));\
\
    static FMOCK(NAME)<void(int)>* obj;\
}

#define DEFINE_FMOCK1(NAME,RET,PARAM1) \
FMOCK(NAME)* FMOCK(NAME)::obj;\
\
void NAME(int x)\
{\
    FMOCK(NAME)::obj->NAME(x);\
}

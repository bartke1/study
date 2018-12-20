#include <gmock/gmock.h>

//#define _MOCK_SUFFIX _fmock

#define FMOCK(NAME) NAME ## _MOCK_SUFFIX
#define FMOCK_T(NAME, RET, ...) FMOCK(NAME)<RET(__VA_ARGS__)>

#define _GET_NTH_ARG(_1, _2, _3, _4, N, ...) N
#define COUNT_VARARGS(...) _GET_NTH_ARG(__VA_ARGS__, 4, 3, 2, 1)
#define _GET_OVERRIDE(_1, _2, _3, NAME, ...) NAME

#define DECLARE_FMOCK(...) \
    _GET_OVERRIDE(__VA_ARGS__, DECLARE_FMOCK1, DECLARE_FMOCK0)(__VA_ARGS__)

#define DEFINE_FMOCK(...) \
    _GET_OVERRIDE(__VA_ARGS__, DEFINE_FMOCK1, DEFINE_FMOCK0)(__VA_ARGS__)

//#define _GET_ARGS_BUT_FIRST(first, ...) __VA_ARGS__

#define DECLARE_FMOCK0(NAME,RET) \
template<typename T>\
struct FMOCK(NAME);\
template<>\
struct FMOCK(NAME)<RET()>\
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
    MOCK_METHOD0(NAME, RET());\
\
    static FMOCK(NAME)<RET()>* obj;\
}

#define DECLARE_FMOCK1(NAME,RET,PARAM1) \
template<typename T>\
struct FMOCK(NAME);\
template<>\
struct FMOCK(NAME)<RET(PARAM1)>\
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
    static FMOCK(NAME)<RET(PARAM1)>* obj;\
}

#define DEFINE_FMOCK0(NAME,RET) \
FMOCK(NAME)<RET()>* FMOCK(NAME)<RET()>::obj = nullptr;\
\
RET NAME()\
{\
    FMOCK(NAME)<RET()>::obj->NAME();\
}


#define DEFINE_FMOCK1(NAME,RET,PARAM1) \
FMOCK(NAME)<RET(PARAM1)>* FMOCK(NAME)<RET(PARAM1)>::obj = nullptr;\
\
RET NAME(PARAM1 x)\
{\
    FMOCK(NAME)<RET(PARAM1)>::obj->NAME(x);\
}

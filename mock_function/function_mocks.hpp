#include <gmock/gmock.h>

#define FMOCK(NAME) NAME ## _MOCK_SUFFIX
#define FMOCK_T(NAME, RET, ...) FMOCK(NAME)<RET(__VA_ARGS__)>

#define _GET_NTH_ARG(_1, _2, _3, _4, N, ...) N
#define COUNT_VARARGS(...) _GET_NTH_ARG(__VA_ARGS__, 4, 3, 2, 1)
#define _GET_OVERRIDE(_1, _2, _3, NAME, ...) NAME

#define _GET_FIRST(first, ...) first
#define _GET_ALL_BUT_FIRST(first, ...) __VA_ARGS__

#define GET_MOCK_METHOD(NAME, ...) \
    _GET_OVERRIDE(__VA_ARGS__ , MOCK_METHOD2, MOCK_METHOD1, MOCK_METHOD0)(\
        NAME, _GET_FIRST(__VA_ARGS__)(_GET_ALL_BUT_FIRST(__VA_ARGS__))\
    )

#define _GET_TYPE(...) \
    _GET_FIRST(__VA_ARGS__)(_GET_ALL_BUT_FIRST(__VA_ARGS__))

#define DECLARE_FMOCK(NAME,...) \
template<typename T>\
struct FMOCK(NAME);\
template<>\
struct FMOCK(NAME)<_GET_TYPE(__VA_ARGS__)>\
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
    GET_MOCK_METHOD(NAME, __VA_ARGS__);\
\
    static FMOCK(NAME)<_GET_TYPE(__VA_ARGS__)>* obj;\
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

#define MAKRO0(_)
#define MAKRO1(_, P1) P1 p1
#define MAKRO2(_, P1, P2) P1 p1, P2 p2
#define MAKRO3(_, P1, P2, P3) P1 p1, P2 p2, P3 p3
#define MAKROA0(_)
#define MAKROA1(_, P1) p1
#define MAKROA2(_, P1,P2) p1, p2
#define MAKROA3(_, P1, P2, P3) p1, p2, p3

#define _GET_OUT_ARGS(...) \
_GET_OVERRIDE(__VA_ARGS__, MAKROA2, MAKROA1, MAKROA0)(__VA_ARGS__)

#define _GET_IN_ARGS(...) \
_GET_OVERRIDE(__VA_ARGS__, MAKRO2, MAKRO1, MAKRO0)(__VA_ARGS__)

#define DEFINE_FMOCK(NAME, ...) \
FMOCK(NAME)<_GET_TYPE(__VA_ARGS__)>* FMOCK(NAME)<_GET_TYPE(__VA_ARGS__)>::obj = nullptr;\
\
_GET_FIRST(__VA_ARGS__) NAME(_GET_IN_ARGS(__VA_ARGS__))\
\
{\
    FMOCK(NAME)<_GET_TYPE(__VA_ARGS__)>::obj->NAME(_GET_OUT_ARGS(__VA_ARGS__));\
}

#ifndef __NSPLATFORMDEFINE_H__
#define __NSPLATFORMDEFINE_H__

#include <stdio.h>

#define NS_DLL 

#define NS_ASSERT(cond) \
{ \
    char content[256]; \
    sprintf(content, "%s function:%s line:%d", __FILE__, __FUNCTION__, __LINE__);  \
    sprintf(content, "Assert error"); \
}

#define NS_ASSERT_S(cond) \
{ \
	char content[256]; \
	sprintf_s(content, "%s function:%s line:%d", __FILE__, __FUNCTION__, __LINE__);  \
	sprintf_s(content, "Assert error"); \
}


#define NS_UNUSED_PARAM(unusedparam) (void)unusedparam

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif



#endif /* __NSPLATFORMDEFINE_H__*/

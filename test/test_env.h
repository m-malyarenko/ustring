#ifndef __TEST_ENV_H__
#define __TEST_ENV_H__

#include <stdio.h>
#include <assert.h>

#ifdef NDEBUG
#undef NDEBUG
#endif

#define test_passed printf("Test passed: %s\n", __func__)

#endif /* __TEST_ENV_H__ */
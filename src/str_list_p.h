/******************************************************************************
 * 
 * @file    str_list_p.h
 * @date    31 Aug 2021
 * @author  Mikhail Malyarenko <malyarenko.md@gmail.com>
 * 
 * @brief   String List library private header file
 * 
 *****************************************************************************/

#ifndef __STR_LIST_P_H__
#define __STR_LIST_P_H__

#include <stdbool.h>

#include "str_p.h"

#define STR_LIST_DEFAULT_CAPACITY ((size_t) 32)

struct str_list_t {
    struct str_t** buffer;
    size_t size;
    size_t cap;
};

#endif /* __STR_LIST_P_H__ */
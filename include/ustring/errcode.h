/**************************************************************************//**
 * 
 * @file    errcode.h
 * @date    Jan 2023
 * @author  Mikhail Malyarenko <malyarenko.md@gmail.com>
 * 
 * @brief   Error codes for str_t and str_list_t methods
 * 
 *****************************************************************************/

#ifndef __USTRING_ERRCODE_H__
#define __USTRING_ERRCODE_H__

#define USTRING_OK              ((int) 0) /**< Success */
#define USTRING_ERR_NULLPTR     ((int) 1) /**< Null pointer argument error */
#define USTRING_ERR_OOM         ((int) 2) /**< Out of memory error */

#endif /* __USTRING_ERRCODE_H__ */

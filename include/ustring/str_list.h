/**************************************************************************//**
 * 
 * @file    str_list.h
 * @date    31 Aug 2021
 * @author  Mikhail Malyarenko <malyarenko.md@gmail.com>
 * 
 * @brief   String List library API
 * 
 * The library provides dynamic heap-based string list data structure,
 * string list type and set of methods implementing basic operations,
 * such as adding/deleting strings, splitting and joining strings.
 * 
 *****************************************************************************/

#ifndef __USTRING_STR_LIST_H__
#define __USTRING_STR_LIST_H__

#include <stdbool.h>

#include "str.h"

/**
 * @addtogroup API
 * @{
 * 
 * @addtogroup StringList
 * 
 * String List library API.
 * 
 * The library provides dynamic heap-based string list data structure,
 * string list type and set of methods implementing basic operations,
 * such as adding/deleting strings, splitting and joining strings.
 * 
 * @{
 */

typedef struct __str_list str_list_t; /**< String list type */

/**
 * @brief Creates new instance of an empty string list
 * 
 * Constructs new heap-allocated string list (array) instance
 * 
 * @return On success, returns the pointer to the new string list instance.
 *      On failure, returns @c NULL
 */
str_list_t* str_list_new();

/**
 * @brief Created an empty string list with the given capacity
 * 
 * @param capacity Capacity of the buffer
 * @return On success, returns the pointer to the new string list instance. On failure, returns @c NULL
 * @note If capacity is 0 no memory allocation is performed and buffer is empty
 */
str_list_t* str_list_with_capacity(size_t capacity);

/**
 * @brief Creates the new copy of the string list
 * 
 * Function creates a copy of the given string list by 
 * creating deep copies of each string in the list buffer.
 * 
 * @param other Pointer to the initialized string list instance to be copied
 * @return On success, returns the pointer to the new string list instance. On failure, returns @c NULL
 * @note If the given pointer is @c NULL an empty string list is returned.
 */
str_list_t* str_list_copy(const str_list_t* other);

/**
 * @brief Deallocates string list instance and all its contents
 * 
 * @param self Pointer to the pointer to the initialized string list instance
 * @warning After string list is dropped it must not be used,
 *      the string list pointer passed to the funcion will be set to @c NULL
 */
void str_list_drop(str_list_t** self);

/**
 * @brief Adds string instance to the end of the list
 * 
 * @param self Pointer to the initialized string list instance
 * @param string Pointer to the initialized string instance to be added
 * @return On success returns zero. On failure returns non-zero value
 * @warning String list takes ownership of the string, string must not be dropped externally
 */
int str_list_push(str_list_t* self, str_t* string);

/**
 * @brief Returns the last string in the list and deletes it
 * 
 * @param self Pointer to the initialized string list instance
 * @return Pointer to the last string in the string list; @c NULL if string list is empty or @c NULL
 * @warning String list losses ownership of the string, string must be dropped implicitly
 */
str_t* str_list_pop(str_list_t* self);

/**
 * @brief Returns the size of the string list
 * 
 * @param self Pointer to the initialized string list instance
 * @return Size of the string list: 0 if the string list is empty or @c NULL
 */
size_t str_list_size(const str_list_t* self);

/**
 * @brief Returns the capacity of the string list
 * 
 * @param self Pointer to the initialized string list instance
 * @return Capacity of the string list;
 *         0 if the string list's buffer is empty or if string list is @c NULL
 */
size_t str_list_cap(const str_list_t* self);

/**
 * @brief Checks if the string list is empty
 * 
 * @param self Pointer to the initialized string list instance
 * @return @c true if string list has at least one element
 *         0 if the string list is empty or @c self is @c NULL
 */
bool str_list_is_empty(const str_list_t* self);

/**
 * @brief Returns the string at given index starting from 0.
 * 
 * @param self Pointer to the initialized string list instance
 * @param idx Position of the string in the given string list starting from 0
 * @return String of the string list with the given index;
 *        @c NULL is returned if
 *          - position violates the bounds or
 *          - string list is empty or
 *          - @c self is @c NULL
 */
str_t* str_list_at(const str_list_t* self, const size_t idx);

/**
 * @brief Splits the string around the given delimeter
 * 
 * Splits the string to one or more strings around any sequence of any
 * character provided by the given delimeter string.
 * Resulting string chunks are new string instances.
 * If string starts or ends with the delimeter character sequnce,
 * no additional empty string are added. 
 * 
 * @param string Pointer to the initialized string instance
 * @param delim Delimeter - NULL-terminated byte string of valid ASCII characters
 * @return Pointer to the new string list that contains all resulting string chunks.
 *      Empty string list if @c self is @c NULL or empty.
 *      On failure returns @c NULL
 * 
 * @code
 *      str_t* string = str_new(" .One, Two; Three");
 *      str_list_t* list = str_split(string, " ,;");
 *      // Pseudocode:
 *      // list = ["One", "Two", "Three"]
 * @endcode
 */
str_list_t* str_split(const str_t* string, const char* delim);

/**
 * @brief Joins all provided strings and puts delimeter sequence between them
 * 
 * Joins provided strings by putting them together with the
 * given delimeter sequence between items.
 * 
 * @param self Pointer to the initialized string ilist nstance
 * @param delim Delimeter - NULL-terminated byte string of valid ASCII characters
 * @return Pointer to the new instance of the string that contains joined strings.
 *      Empty string if @c self is @c NULL.
 *      On failure returns @c NULL
 * @note If delimeter is @c NULL it considered as an empty string 
 */
str_t* str_list_join(const str_list_t* self, const char* delim);

/**
 * @brief Same as @c str_split with whitespace characters as separartor
 * 
 * Function splits the string into string list by whitespace characters
 * @b space (' '), @b h-tab (\\t), @b v-tab (\\v),
 * @b newline (\\n), @b carriage return (\\r).
 * 
 * @param string Pointer to the initialized string instance
 * @return Pointer to the new string list that contains all resulting string chunks.
 *      Empty string list if @c self is @c NULL or empty.
 *      On failure returns @c NULL
 */
str_list_t* str_split_whitespace(const str_t* string);

/**
 * @brief Checks is string list contains specified string
 * 
 * Compares each element of the string list with
 * the given string to find an equal match.
 * 
 * @param self Pointer to the initialized string list instance
 * @param string String to find in the list - pointer to the initialized string instance
 * @return @c true if string list contains the given string;
 *      @c false otherwise or if either @c self or @c string is @c NULL
 */
bool str_list_contains(const str_list_t* self, const str_t* string);

/**
 * @}
 */ /* StringList */

/**
 * @}
 */ /* API */

#endif /* __USTRING_STR_LIST_H__ */

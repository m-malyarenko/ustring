/******************************************************************************
 * 
 * @file    str.h
 * @date    14 Nov 2021
 * @author  Mikhail Malyarenko <malyarenko.md@gmail.com>
 * 
 * @brief   String library API
 * 
 * API header for the C language string library 
 * 
 *****************************************************************************/

#ifndef __STR_H__
#define __STR_H__

#include <stdbool.h>

/**
 * @struct String type
 */
typedef struct str_t str_t;

/**
 * @brief Creates new instance of the string holding contents
 *        of the provided null-terminated byte string.
 * 
 * @param string Null-terminated byte string of valid ASCII characters
 * @return Pointer to the allocated instance of the string
 * @note If string is @c NULL an empty string with default capacity is created
 */
str_t* str_new(const char* string);

/**
 * @brief Created an empty string with the buffer of the given capacity.
 * 
 * @param capacity Capacity of the buffer
 * @return Pointer to the allocated instance of the string
 * @note If capacity is 0 no memory allocation is performed
 *       and buffer is empty
 */
str_t* str_with_capacity(const size_t capacity);

/**
 * @brief Creates the new copy of the string.
 * 
 * @param other String to be copied
 * @return Pointer to the new instance of @c str_t type which is
 *         a copy of the given string. If the given pointer is @c NULL
 *         returns an empty string.
 */
str_t* str_copy(const str_t* other);

/**
 * @brief Drops the buffer of the string and sets all attributes to 0.
 * 
 * @param self Pointer to the initialised string instance
 * @return void
 * @note If @code{self} is @c NULL function does nothing
 * @warning After string is dropped it must not be used
 */
void str_drop(str_t** self);

/**
 * @brief Returns the length of the string.
 * 
 * @return Length of the string. If @c self is @c NULL ,
 *         0 is returned.
 */
size_t str_len(const str_t* self);

/**
 * @brief Returns the capacity of the string buffer.
 * 
 * @return Capacity of the string's buffer. If @c self is @c NULL
 *         0 is returned.
 */
size_t str_cap(const str_t* self);

/**
 * @brief Checks if string is empty.
 * 
 * @param self Pointer to the initialised string instance
 * @return @c true if @c self is empty;
 *         @c false otherwise or if the string @c NULL
 */
bool str_is_empty(const str_t* self);

/**
 * @brief Returns character of the string at the given position
 * starting with 0. Function provides bound checking.
 * 
 * @param self Pointer to the initialised string instance
 * @param pos Position of the character in the given string
 * @return Character of the string on the given position; @c 0 if @c pos
 * is greater or equal to the length of the string or if the string is
 * empty or @c NULL
 */
char str_at(const str_t* self, const size_t pos);

/**
 * @brief Returns a pointer to the string buffer.
 * 
 * String buffer is holding valid Null-terminated ASCII string literal,
 * so pointer to the string buffer may be used as
 * normal @c const @c char* string literal.
 * 
 * @param self Pointer to the string instance
 * @return Pointer to the @c self buffer; @c NULL if @c self is @c NULL 
 */
const char* str_as_ptr(const str_t* self);

/**
 * @brief Appends one string to the end of another string.
 * 
 * Function changes the first string by adding all characters of
 * the second string to it. Second string stays unchanged.
 * 
 * @param self Pointer to the @c str_t instance, which will be altered
 * @param string Null-terminated string literal with valid ASCII characters
 * @return Pointer to the @c self string
 * @note Function does nothing to the @c self if @c string is @c NULL or empty
 */
str_t* str_append(str_t* self, const char* string);

/**
 * @brief Creates new string which is a result of concatenation of
 * the given strings.
 * 
 * @param str_a,str_b Pointesr to the @c str_t instances
 * @return Pointer to the new @c str_t instance - a result
 *         of concatenation of the given strings
 */
str_t* str_concat(const str_t* str_a, const str_t* str_b);

/**
 * @brief Trims leading and trailing whitespace characters in the string.
 * 
 * Trims leading and trailing @b space, @b h-tab, @b v-tab, @b newline
 * symbols if any exitsts. Function has linear complexity.
 * 
 * @param self Pointesr to the @c str_t instances
 * @returns Pointer to the altered @c self string
 */
str_t* str_trim(str_t* self);

#endif /* __STR_H__ */
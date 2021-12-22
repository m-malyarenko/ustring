/******************************************************************************
 * 
 * @file    str.h
 * @date    14 Nov 2021
 * @author  Mikhail Malyarenko <malyarenko.md@gmail.com>
 * 
 * @brief   String library API
 * 
 * API header for the String library.
 * 
 * Library provides dynamic heap-based string type and set of functions
 * implementing basic string operations.
 * 
 *****************************************************************************/

#ifndef __STR_H__
#define __STR_H__

#include <stddef.h>
#include <stdbool.h>

/**
 * @struct String type
 */
typedef struct str_t str_t;

/**
 * @brief Creates new instance of string.
 * 
 * Constructs new heap-allocated string instance based
 * on the provided null-terminated ASCII string.
 * Constructed string instance contains only valid ASCII
 * characters. Non-ASCII characters are replaced with '?' character. 
 * 
 * @param string Null-terminated byte string of valid ASCII characters
 * @return Pointer to the allocated instance of the string
 * @note If string is @c NULL an empty string with default capacity is created
 * @note Non-ASCII characters will be replaced with the '?' character
 * 
 * @example['6'] str_example.c
 */
str_t* str_new(const char* string);

/**
 * @brief Creates an empty string with the buffer of the given capacity.
 * 
 * @param capacity Capacity of the buffer
 * @return Pointer to the allocated instance of the string
 * @note If capacity is set to 0 no memory allocation
 *      is performed, string buffer is empty
 */
str_t* str_with_capacity(const size_t capacity);

/**
 * @brief Creates copy of the string.
 * 
 * @param other String instance to be copied
 * @return Pointer to the new instance of string which is
 *      a copy of the given string;
 *      If given pointer is @c NULL returns an empty string
 * 
 * @example['15'] str_example.c
 */
str_t* str_copy(const str_t* other);

/**
 * @brief Drops the string instance.
 * 
 * Frees the allocated memory and sets string
 * instance pointer to @c NULL  
 * 
 * @param self Pointer to pointer to the initialised string instance
 * @return void
 * @note If @c self is @c NULL or if @c *self is @c NULL function does nothing
 * @warning After string is dropped it must not be used
 * 
 * @example['32'] str_example.c
 */
void str_drop(str_t** self);

/**
 * @brief Returns the length of the string.
 * 
 * @return Length of the string. If @c self is @c NULL , 0 is returned.
 */
size_t str_len(const str_t* self);

/**
 * @brief Returns the capacity of the string buffer.
 * 
 * @return Capacity of the string's buffer;
 *      If @c self is @c NULL 0 is returned
 */
size_t str_cap(const str_t* self);

/**
 * @brief Checks if string is empty.
 * 
 * @param self Pointer to the initialised string instance
 * @return @c true if @c self is empty;
 *      @c false otherwise or if the string @c NULL
 */
bool str_is_empty(const str_t* self);

/**
 * @brief Returns character of the string at the given position
 * starting with 0. Function provides bound checking.
 * 
 * @param self Pointer to the initialised string instance
 * @param pos Position of the character in the given string
 * @return Character of the string on the given position;
 *      @c 0 if @c pos is greater or equal to the length
 *      of the string or if the string is empty or @c NULL
 * 
 * @example['41'] str_example.c
 */
char str_at(const str_t* self, const size_t pos);

/**
 * @brief Returns a pointer to the string buffer.
 * 
 * String buffer is holding valid null-terminated ASCII string literal,
 * so pointer to the string buffer may be used as
 * normal @c const @c char* string literal.
 * 
 * @param self Pointer to the string instance
 * @return Pointer to the @c self buffer; @c NULL if @c self is @c NULL
 * 
 * @example['51'] str_example.c
 */
const char* str_as_ptr(const str_t* self);

/**
 * @brief Appends one string to the end of another string.
 * 
 * Function changes the first string by adding all characters of
 * the second string to it. Second string stays unchanged.
 * 
 * @param self Pointer to the string instance, which will be altered
 * @param string Null-terminated string literal with valid ASCII characters
 * @return Pointer to the @c self string instance
 * @note Function does nothing to @c self if @c string is @c NULL or empty
 * 
 * @example['61'] str_example.c
 */
str_t* str_append(str_t* self, const char* string);
// TODO str_t* -> void

/**
 * @brief Creates new string which is a result of concatenation of
 * the given strings.
 * 
 * @param str_a,str_b Pointers to the string instances
 * @return Pointer to the new string instance - a result
 *      of concatenation of the given strings
 * 
 * @example['81'] str_example.c
 */
str_t* str_concat(const str_t* str_a, const str_t* str_b);

/**
 * @brief Trims leading and trailing whitespace characters in the string.
 * 
 * Trims leading and trailing @b space, @b h-tab, @b v-tab, @b newline
 * symbols if any exitsts. Function has linear complexity @itO(n).
 * 
 * @param self Pointesr to the @c str_t instances
 * @returns Pointer to the altered @c self string
 * 
 * @example['94'] str_example.c
 */
str_t* str_trim(str_t* self);
// TODO str_t* -> void

// TODO implement str_eq
// bool str_eq(const str_t* a, const str_t* b);

// TODO implement str_truncate
// void str_truncate(str_t* self, size_t len);

// TODO implement str_contains
// bool str_contains(const str_t* self, const char* pattern);

// TODO implement str_trim_matches
// void str_trim_matches(str_t* self, const char* pattern);

// TODO implement str_trim_start_matches
// void str_trim_start_matches(str_t* self, const char* pattern);

// TODO implement str_trim_end_matches
// void str_trim_end_matches(str_t* self, const char* pattern);

// TODO implement str_replace
// void str_replace(str_t* self, const char* pattern, const char* replacement);

// TODO implement str_replacen
// void str_replacen(str_t* self, const char* pattern, const char* replacement, size_t n);

// TODO implement str_shrink_to_fit
// void str_shrink_to_fit(str_t* self);

// TODO implement str_starts_with
// bool str_starts_with(const str_t* self, const char* pattern);

// TODO implement str_ends_with
// bool str_ends_with(const str_t* self, const char* pattern);

// TODO implement str_to_lowercase
// void str_to_lowercase(str_t* slef);

// TODO implement str_to_uppercase
// void str_to_uppercase(str_t*);

#endif /* __STR_H__ */
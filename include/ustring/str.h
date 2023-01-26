/**************************************************************************//**
 * 
 * @file    str.h
 * @date    14 Nov 2021
 * @author  Mikhail Malyarenko <malyarenko.md@gmail.com>
 * 
 * @brief   String library API
 * 
 * The library provides dynamic heap-based string data structure,
 * string type and set of methods implementing basic string operations.
 * 
 *****************************************************************************/

#ifndef __USTRING_STR_H__
#define __USTRING_STR_H__

#include <stddef.h>
#include <stdbool.h>

/**
 * @addtogroup API
 * @{
 * 
 * @addtogroup String
 * 
 * String library API.
 * 
 * The library provides dynamic heap-based string data structure,
 * string type and set of methods implementing basic string operations.
 * 
 * @{
 */

typedef struct __str str_t; /**< String type */

/**
 * @brief Creates new instance of string
 * 
 * Constructs new heap-allocated string instance based
 * on the provided null-terminated ASCII string.
 * Constructed string instance contains only valid ASCII characters.
 * 
 * @param string Null-terminated byte string of valid ASCII characters
 * @return On success, returns the pointer to the new string instance. On failure, returns @c NULL
 * @note If string is @c NULL an empty string is created
 * @note Non-ASCII characters will be replaced with the '?' character
 */
str_t* str_new(const char* string);

/**
 * @brief Creates an empty string with the buffer of the given capacity
 * 
 * @param capacity Capacity of the buffer
 * @return On success, returns the pointer to the new string instance. On failure, returns @c NULL
 * @note If capacity is set to 0 an empty string is created
 */
str_t* str_with_capacity(size_t capacity);

/**
 * @brief Creates copy of the string
 * 
 * @param other Pointer to the initialized string instance to be copied
 * @return On success, returns the pointer to the new instance of the string copy. On failure, returns @c NULL
 * @note If @c other is @c NULL , an empty string is created
 */
str_t* str_copy(const str_t* other);

/**
 * @brief Drops the string instance
 * 
 * Frees the allocated memory and sets string
 * instance pointer to @c NULL  
 * 
 * @param self Pointer to the pointer to the initialized string instance
 * @note If @c self or @c *self is @c NULL, function does nothing
 * @warning After string is dropped it must not be used,
 *      the string pointer passed to the function will be set to @c NULL 
 */
void str_drop(str_t** self);

/**
 * @brief Returns the length of the string
 * 
 * @return Length of the string. If @c self is @c NULL , 0 is returned.
 */
size_t str_len(const str_t* self);

/**
 * @brief Returns the capacity of the string buffer
 * 
 * @param self Pointer to the initialised string instance
 * @return Capacity of the string's buffer. If @c self is @c NULL , 0 is returned
 */
size_t str_cap(const str_t* self);

/**
 * @brief Checks if string is empty
 * 
 * @param self Pointer to the initialized string instance
 * @return @c true if string is empty; @c false otherwise or if @c self is @c NULL 
 */
bool str_is_empty(const str_t* self);

/**
 * @brief Returns character of the string at the given position starting from 0
 * 
 * @param self Pointer to the initialized string instance
 * @param pos Position of the character in the given string starting from 0
 * @return Character of the string on the given position; 0 is returned if
 *          - position violates the bounds or
 *          - string is empty or
 *          - @c self is @c NULL 
 */
char str_at(const str_t* self, const size_t pos);

/**
 * @brief Returns a constant pointer to the string buffer
 * 
 * String buffer is holding valid null-terminated ASCII string,
 * so pointer to the string buffer may be used as
 * normal C-style string.
 * 
 * @param self Pointer to the initialized string instance
 * @return Pointer to the string buffer. If @c self is @c NULL , @c NULL  is returned
 */
const char* str_as_ptr(const str_t* self);

/**
 * @brief Appends one string to the end of another string
 * 
 * Changes the @c self string by adding all characters of
 * the provided C-style string to it. Second string stays unchanged.
 * Function does nothing to @c self if @c string is @c NULL  or empty.
 * 
 * @note Appending string instance to itself, passing its
 * buffer as pointer is inefficient (string buffer is temporary duplicated).
 * For this purpose use str_concat method instead.
 * 
 * @param self Pointer to the initialized string instance
 * @param string NULL-terminated byte string of valid ASCII characters
 * @return On success, returns the pointer to the @c self string instance.
 *      On failure, returns @c NULL
 */
str_t* str_append(str_t* self, const char* string);

/**
 * @brief Clears contents of the string
 * 
 * Marks string as zero-length keeping buffer in place.
 * 
 * @param self Pointer to the initialized string instance
 * @return On success returns zero. On failure returns non-zero value
 */
int str_clear(str_t* self);

/**
 * @brief Creates new string which is a result of concatenation of two given strings.
 * 
 * @param str_a,str_b Pointers to the initialized string instances
 * @return On success, returns the pointer to the new string instance - a result
 *      of concatenation of the given strings. On failure, returns @c NULL
 */
str_t* str_concat(const str_t* str_a, const str_t* str_b);

/**
 * @brief Trims leading and trailing whitespace characters in the string.
 * 
 * Trims leading and trailing @b space (' '), @b h-tab (\\t),
 * @b v-tab (\\v), @b newline (\\n), @b carriage return (\\r) symbols.
 * 
 * @param self Pointer to the initialized string instance
 * @returns On success returns zero. On failure returns non-zero value
 */
int str_trim(str_t* self);

/**
 * @brief Checks two strings for equality
 * 
 * @param a,b Pointers to the initialized string instances
 * @returns @c true if strings are equal; @c false otherwise.
 *      If one of strings is @c NULL, @c false is returned
 */
bool str_eq(const str_t* a, const str_t* b);

/**
 * @brief Truncates the string to the given length.
 * 
 * Shortens the string to the given length keeping the beginning.
 * Function does nothing if @c len is greater then @c self length.
 * 
 * @param self Pointers to the initialized string instances
 * @param len Desired string length
 * @returns On success returns zero. On failure returns non-zero value
 */
int str_truncate(str_t* self, size_t len);

/**
 * @brief Checks if the string contains a string pattern
 * 
 * Checks if the entire pattern is included in the string at least once.
 * 
 * @note Following conventions are used:
 *          - Empty string contains only empty pattern (zero-length pattern)
 *          - All strings contains an empty pattern
 * 
 * @param self Pointer to the initialized string instance
 * @param pattern Search pattern - NULL-terminated byte string of valid ASCII characters
 * @return @c true if the string contains @c pattern ;
 *      @c false otherwise or if either @c self or @c pattern is @c NULL
 */
bool str_contains(const str_t* self, const char* pattern);

/**
 * @brief Checks if the string contains at least one character of class
 *      determined by the predicate function.
 * 
 * Function applies the predicate function to each character
 * in the string and checks if any character makes predicate function return @c true 
 * 
 * @param self Pointer to the initialized string instance
 * @param fn Predicate function
 * @return @c true if the string contains at least one character passed by predicate
 *      @c false otherwise or if either @c self or @c fn is @c NULL
 */
bool str_contains_fn(const str_t* self, bool (*fn) (char));

/**
 * @brief Removes all occurrences of the pattern in the string
 * 
 * Function removes all occurrences of the entire pattern in the string with greedy strategy:
 *      - string is scanned once from start to end
 *      - every time pattern occurs it is trimmed.
 * If either @c self or @c pattern is @c NULL function does nothing and returns error code.
 * 
 * @param self Pointer to the initialized string instance
 * @param pattern Pattern to be removed - NULL-terminated byte string of valid ASCII characters
 * @return On success returns zero. On failure returns non-zero value
 */
int str_trim_matches(str_t* self, const char* pattern);

/**
 * @brief Removes all characters matched by the predicate function
 * 
 * Scans the string from start to end, applies provided
 * predicate function to each character and removes all characters
 * that matched predicate.
 * 
 * If either @c self or @c fn is @c NULL function does nothing and returns error code.
 * 
 * @param self Pointer to the initialized string instance
 * @param fn Predicate function
 * @return On success returns zero. On failure returns non-zero value
 */
int str_trim_matches_fn(str_t* self, bool (*fn) (char));

/**
 * @brief Removes prefix of the string that matches pattern
 * 
 * Scans the string from the start and removes first characters if they matches the given pattern.
 * If either @c self or @c pattern is @c NULL function does nothing and returns error code.
 * 
 * @param self Pointer to the initialized string instance
 * @param pattern Pattern of prefix to be removed - NULL-terminated byte string of valid ASCII characters
 * @return On success returns zero. On failure returns non-zero value
 */
int str_trim_start_matches(str_t* self, const char* pattern);

/**
 * @brief Removes all characters from the start of the string matched by the predicate function
 * 
 * Scans the string from start and removes characters until they matches predicate function.
 * If either @c self or @c fn is @c NULL function does nothing and returns error code.
 * 
 * @param self Pointer to the initialized string instance
 * @param fn Predicate function
 * @return On success returns zero. On failure returns non-zero value
 */
int str_trim_start_matches_fn(str_t* self, bool (*fn) (char));

/**
 * @brief Removes suffix of the string that matches pattern
 * 
 * Scans the string from the end and removes last characters if they matches the given pattern.
 * If either @c self or @c pattern is @c NULL function does nothing and returns error code.
 * 
 * @param self Pointer to the initialized string instance
 * @param pattern pattern of suffix to be removed - NULL-terminated byte string of valid ASCII characters
 * @return On success returns zero. On failure returns non-zero value
 */
int str_trim_end_matches(str_t* self, const char* pattern);

/**
 * @brief Removes all characters from the end of the string matched by the predicate function
 * 
 * Scans the string from the end and removes characters until they matches predicate function.
 * If either @c self or @c fn is @c NULL function does nothing and returns error code.
 * 
 * @param self Pointer to the initialized string instance
 * @param fn Predicate function
 * @return On success returns zero. On failure returns non-zero value
 */
int str_trim_end_matches_fn(str_t* self, bool (*fn) (char));

/**
 * @brief Replaces all occurrences of the pattern with the provided replacement substring
 * 
 * Scans through string from start to end. If an entire
 * pattern occurrence is detected it is replaced with the provided substring.
 * Function uses greedy strategy and do not support recursive replacement.
 * 
 * If either @c self or @c pattern is @c NULL function does nothing and returns error code.
 * 
 * @note If @c replacement is @c NULL, it is treated as an empty string
 * 
 * @param self Pointer to the initialized string instance
 * @param pattern Pattern to be replaced - NULL-terminated byte string of valid ASCII characters
 * @param replacement Substring to be inserted in the string replacing pattern
 * @return On success, returns 0. On failure returns non-zero error code
 */
int str_replace(str_t* self, const char* pattern, const char* replacement);

/**
 * @brief Shrinks string buffer to fit its content
 * 
 * Reallocates or truncates the buffer of the string so it occupies
 * only necessary amount of memory to hold string content.
 * If @c self is @c NULL function does nothing and returns error code.
 * 
 * @param self Pointer to the initialized string instance
 * @return On success returns zero. On failure returns non-zero value
 */
int str_shrink_to_fit(str_t* self);

/**
 * @brief Checks if string starts with pattern
 * 
 * Checks if string matches the pattern from the start.
 * 
 * @note Every string begins with an empty pattern: @c "" .
 * 
 * @param self Pointer to the initialized string instance
 * @param pattern Pattern to match string prefix - NULL-terminated byte string of valid ASCII characters
 * @return @c true if string starts with the @c pattern ;
 *      @c false otherwise or if either self or pattern are @c NULL
 */
bool str_starts_with(const str_t* self, const char* pattern);

/**
 * @brief Checks if string starts with pattern
 * 
 * Checks if string matches the pattern from the end.
 * 
 * @note Every string ends with an empty pattern: @c "" .
 * 
 * @param self Pointer to the initialized string instance
 * @param pattern Pattern to match string suffix - NULL-terminated byte string of valid ASCII characters
 * @return @c true if string ends with the @c pattern ;
 *      @c false otherwise or if either self or pattern are @c NULL
 */
bool str_ends_with(const str_t* self, const char* pattern);

/**
 * @brief Converts string to lower case
 * 
 * Converts all ASCII letter characters in the string to lower case. 
 * 
 * @param self Pointer to the initialized string instance
 * @return On success returns zero. On failure returns non-zero value
 */
int str_to_lowercase(str_t* self);

/**
 * @brief Converts string to upper case
 * 
 * Converts all ASCII letter characters in the string to upper case. 
 * 
 * @param self Pointer to the initialized string instance
 * @return On success returns zero. On failure returns non-zero value
 */
int str_to_uppercase(str_t* self);

/**
 * @}
 */ /* String */

/**
 * @}
 */ /* API */

#endif /* __USTRING_STR_H__ */

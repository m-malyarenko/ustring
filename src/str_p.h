/******************************************************************************
 * 
 * @file    str_p.h
 * @date    15 Aug 2021
 * @author  Mikhail Malyarenko <malyarenko.md@gmail.com>
 * 
 * @brief   String library private header file
 * 
 *****************************************************************************/

#ifndef __STR_P_H__
#define __STR_P_H__

#define STR_DEFAULT_CAPACITY ((size_t) 32)
#define ASCII_LETTER_CASE_CODE_SHIFT ((char) 32)

struct __str {
    char* buffer;
    size_t len;
    size_t cap;
};

/**
 * @brief Checks if character is a whitespace character.
 * 
 * @param ch @c char ASCII character
 * @return @c true if @c ch is either space (0x20), form feed (0x0c), 
 * line feed (0x0a), carriage return (0x0d), horizontal tab (0x09) 
 * or vertical tab (0x0b); @c false otherwise
 */
#define __is_blank(ch) (                                                      \
    ((unsigned char) (ch) == (unsigned char) 0x20)                            \
        || (((unsigned char) (ch) >= (unsigned char) 0x09)                    \
            && ((unsigned char) (ch) <= (unsigned char) 0x0D))                \
)

/**
 * @brief Checks if character is a valid ASCII character.
 * 
 * @param ch @c char character
 * @return @c true if @c ch is valid ASCII character; @c false otherwise
 */
#define __is_ascii(ch) (                                                      \
    (unsigned char) (ch) <= (unsigned char) 0x7F                              \
)

/**
 * @brief Checks if character is a letter.
 * 
 * @param ch @c char character
 * @return @c true if @c ch is a letter; @c false otherwise
 */
#define __is_letter(ch) (                                                     \
    (((unsigned char) (ch) >= 0x41)                                           \
        && ((unsigned char) (ch) <= 0x5A))                                    \
    || (((unsigned char) (ch) >= 0x61)                                        \
        && ((unsigned char) (ch) <= 0x7A))                                    \
)

/**
 * @brief Returns the length of a C string.
 * 
 * Function is one to one analog of the strlen() fuction
 * from the string.h standart library.
 * 
 * @param string Null-terminated ASCII C string
 * @return Length of the C string
 */
size_t __str_literal_len(const char* string);

/**
 * @brief Checks if string contains the given character.
 * 
 * @param string Null-terminated ASCII C string
 * @param ch ASCII character
 * @return @c true if @c string contains @c ch character;
 *         @c false otherwise
 */
bool __str_literal_contains(const char* string, char ch);

/**
 * @brief Converts ASCII letter character to lower case.
 * 
 * @param ch ASCII character
 * @return lower-case version of the character is exists;
 *      @c ch itself otherwise 
 */
char __to_lower(char ch);

/**
 * @brief Converts ASCII letter character to upper case.
 * 
 * @param ch ASCII character
 * @return upper-case version of the character is exists;
 *      @c ch itself otherwise 
 */
char __to_upper(char ch);

#endif /* __STR_P_H__ */

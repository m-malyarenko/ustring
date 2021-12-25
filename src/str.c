/******************************************************************************
 * 
 * @file    str.c
 * @date    15 Aug 2021
 * @author  Mikhail Malyarenko <malyarenko.md@gmail.com>
 * 
 * @brief   String library implementation
 * 
 *****************************************************************************/

#include <stdlib.h>

#include "str.h"
#include "str_p.h"

str_t* str_new(const char* string) {   
    if (string == NULL) {
        return str_with_capacity(STR_DEFAULT_CAPACITY);
    }

    const size_t len = __str_literal_len(string);
    size_t cap = STR_DEFAULT_CAPACITY;

    while (len >= cap) {
        cap *= 2;
    }
    
    str_t* self = (str_t*) malloc(sizeof(str_t));
    self->buffer = (char*) malloc(cap);
    self->len = len;
    self->cap = cap;
    for (size_t i = 0; i < len; i++) {
        self->buffer[i] = __is_ascii(string[i]) ? string[i] : '?';
    }
    self->buffer[len] = '\0';

    return self;
}

str_t* str_with_capacity(size_t capacity) {
    str_t* self = (str_t*) malloc(sizeof(str_t));
    self->len = 0;
    
    if (capacity == 0) {
        self->cap = 0;
        self->buffer = NULL;
    }
    else {
        self->cap = capacity;
        self->buffer = (char*) malloc(capacity * sizeof(char));
        self->buffer[0] = '\0';
    }

    return self;
}

str_t* str_copy(const str_t* other) {
    if (other == NULL) {
        return str_with_capacity(STR_DEFAULT_CAPACITY);
    }

    str_t* self = (str_t*) malloc(sizeof(str_t));
    self->cap = other->len + 1;
    self->len = other->len;

    if (self->cap == 0) {
        self->buffer = NULL;
    }
    else {
        self->buffer = (char*) malloc(self->cap * sizeof(char));
        for (size_t i = 0; i < self->len; i++) {
            self->buffer[i] = other->buffer[i];
        }
        self->buffer[self->len] = '\0';
    }

    return self;
}

void str_drop(str_t** self) {
    if ((self == NULL) || (*self == NULL)) {
        return;
    }

    if ((*self)->buffer != NULL) {
        free((*self)->buffer);
        (*self)->buffer = NULL;
    }

    (*self)->cap = 0;
    (*self)->len = 0;
    *self = NULL;
}

size_t str_len(const str_t* self) {
    return (self != NULL) ? self->len : 0;
}

size_t str_cap(const str_t* self) {
    return (self != NULL) ? self->cap : 0;
}

bool str_is_empty(const str_t* self) {
    return (bool) ((self == NULL) || (self->len == 0));
}

char str_at(const str_t* self, const size_t pos) {
    if (((self == NULL) || (self->len == 0)) || (pos >= self->len)) {
        return 0;
    }
    else {
        return self->buffer[pos];
    }
}

const char* str_as_ptr(const str_t* self) {
    return (self->cap == 0) ? NULL : self->buffer; 
}

void str_append(str_t* self, const char* string) {
    if (self == NULL) {
        return;
    }

    if ((string == NULL) || (__str_literal_len(string) == 0)) {
        return;
    }

    const size_t string_len = __str_literal_len(string);
    const size_t new_len = self->len + string_len;

    if (new_len >= self->cap) {
        while (new_len >= self->cap) {
            self->cap *= 2;
        }

        if (self->buffer != NULL) {
            self->buffer = (char*) realloc(self->buffer, self->cap * sizeof(char));
        }
        else {
            self->buffer = (char*) malloc(self->cap * sizeof(char));
        }
    }

    for (size_t i = 0; i < string_len; i++) {
        self->buffer[self->len + i] = __is_ascii(string[i]) ? string[i] : '?';
    }
    self->buffer[new_len] = '\0';
    self->len = new_len;

    return;
}

str_t* str_concat(const str_t* str_a, const str_t* str_b) {
    const bool is_null_a = str_a == NULL;
    const bool is_null_b = str_b == NULL;
    const bool is_empty_a = is_null_a || (str_a->len == 0);
    const bool is_empty_b = is_null_b || (str_b->len == 0);

    /* Case: */
    /* a) Both strings are NULL or empty */
    if ((is_null_a && is_null_b) || (is_empty_a && is_empty_b)) {
        return str_with_capacity(STR_DEFAULT_CAPACITY);
    }

    /* b) String A normal & other is NULL or empty */
    if ((!is_null_a && !is_empty_a) && (is_null_b || is_empty_b)) {
        return str_copy(str_a);
    }

    /* c) String A normal & other is NULL or empty */
    if ((!is_null_b && !is_empty_b) && (is_null_a || is_empty_a)) {
        return str_copy(str_b);
    }

    /* d) Both strings are normal */
    const size_t len = str_a->len + str_b->len;
    size_t cap = STR_DEFAULT_CAPACITY;

    while (len >= cap) {
        cap *= 2;
    }

    str_t* result_str = (str_t*) malloc(sizeof(str_t));
    result_str->cap = cap;
    result_str->len = len;
    result_str->buffer = (char*) malloc(cap * sizeof(char));

    for (size_t i = 0; i < str_a->len; i++) {
        result_str->buffer[i] = str_a->buffer[i];
    }

    for (size_t i = 0; i < str_b->len; i++) {
        result_str->buffer[str_a->len + i] = str_b->buffer[i];
    }

    result_str->buffer[result_str->len] = '\0';

    return result_str;
}

void str_trim(str_t* self) {
    if ((self == NULL) || (self->len == 0)) {
        return;
    }

    const char* front_ptr = self->buffer;
    const char* back_ptr = self->buffer + self->len;

    while ((front_ptr != back_ptr) && __is_blank(*front_ptr)) {
        front_ptr++;
    }

    while ((back_ptr != front_ptr) && __is_blank(*(back_ptr - 1))) {
        back_ptr--;
    }

    const size_t new_len = back_ptr - front_ptr;

    if ((new_len != 0) && (front_ptr != self->buffer)) {
        size_t position = 0;
        while (front_ptr != back_ptr) {
            self->buffer[position] = *front_ptr;
            front_ptr++;
            position++;
        }   
    }
    
    self->buffer[new_len] = '\0';
    self->len = new_len;
}

bool str_eq(const str_t* a, const str_t* b) {
    if ((a == NULL) || (b == NULL)) {
        return false;
    }

    const size_t a_len = a->len;
    const size_t b_len = b->len;

    if (a_len != b_len) {
        return false;
    }

    for (size_t i = 0; i < a_len; i++) {
        if (a->buffer[i] != b->buffer[i]) {
            return false;
        }
    }
    
    return true;
}

void str_truncate(str_t* self, size_t len) {
    if (self == NULL) {
        return;
    }

    if (self->len > len) {
        self->buffer[len] = '\0';
        self->len = len;
    }
}

bool str_contains(const str_t* self, const char* pattern) {
    if ((self == NULL) || (pattern == NULL)) {
        return false;
    }

    const size_t pattern_len = __str_literal_len(pattern);

    if (self->len == 0) {
        /* Empty string contains only empty pattern */
        return pattern_len == 0;
    } else if (pattern_len == 0) {
        /* Non-empty string always contains empty pattern */
        return true;
    } else if (pattern_len > self->len) {
        /* Pattern length can not exceed string length */
        return false;
    } else if (pattern_len == 1) {
        /* If pattern is single character scan string */
        const char pattern_ch = pattern[0];
        for (size_t i = 0; i < self->len; i++) {
            if (self->buffer[i] == pattern_ch) {
                return true;
            }
        }
    }

    for (size_t i = 0; i < self->len; i++) {
        if ((i + pattern_len) > self->len) {
                return false;
        }

        if (self->buffer[i] == pattern[0]) {
            for (size_t j = 1; j < pattern_len; j++) {
                if (self->buffer[++i] != pattern[j]) {
                    /* Substring failed matching test */
                    continue;
                }
            }
            
            /* Substring passed matching test */
            return true;
        }
    }

    return false;
}

void str_shrink_to_fit(str_t* self) {
    if ((self == NULL) || (self->cap == 0)) {
        return;
    }

    self->buffer = (char*) realloc(self->buffer, sizeof(char) * (self->len + 1));
}

bool str_starts_with(const str_t* self, const char* pattern) {
    if ((self == NULL) || (pattern == NULL)) {
        return false;
    }

    const size_t pattern_len = __str_literal_len(pattern);

    if (self->len == 0) {
        /* Empty string starts only with empty pattern */
        return pattern_len == 0;
    } else if (pattern_len == 0) {
        /* Non-empty string always starts with empty pattern */
        return true;
    } else if (pattern_len > self->len) {
        /* Pattern length can not exceed string length */
        return false;
    }

    for (size_t i = 0; i < pattern_len; i++) {
        if (self->buffer[i] != pattern[i]) {
            return false;
        }
    }

    return true;
}

bool str_ends_with(const str_t* self, const char* pattern) {
    if ((self == NULL) || (pattern == NULL)) {
        return false;
    }

    const size_t pattern_len = __str_literal_len(pattern);

    if (self->len == 0) {
        /* Empty string ends only with empty pattern */
        return pattern_len == 0;
    } else if (pattern_len == 0) {
        /* Non-empty string always ends with empty pattern */
        return true;
    } else if (pattern_len > self->len) {
        /* Pattern length can not exceed string length */
        return false;
    }

    for (size_t i = 0; i < pattern_len; i++) {
        if (self->buffer[self->len - (i + 1)] != pattern[pattern_len - (i + 1)]) {
            return false;
        }
    }

    return true;
}

void str_to_lowercase(str_t* self) {
    if ((self == NULL) || (self->len == 0)) {
        return;
    }

    for (size_t i = 0; i < self->len; i++) {
        self->buffer[i] = __to_lower(self->buffer[i]);
    }
}

void str_to_uppercase(str_t* self) {
    if ((self == NULL) || (self->len == 0)) {
        return;
    }

    for (size_t i = 0; i < self->len; i++) {
        self->buffer[i] = __to_upper(self->buffer[i]);
    }
}

size_t __str_literal_len(const char* string) {
    if ((string == NULL) || (*string == '\0')) {
        return 0;
    }

    size_t len = 0;
    while (*(string + len) != '\0') {
        len++;
    }
    
    return len;
}

bool __str_literal_contains(const char* string, char ch) {
    const size_t len = __str_literal_len(string);

    if ((string == NULL) || (len == 0) || (ch == '\0')) {
        return false;
    }

    for (size_t i = 0; i < len; i++) {
        if (string[i] == ch) {
            return true;
        }
    }

    return false;
}

char __to_lower(char ch) {
    return (__is_letter(ch) && ((unsigned char) (ch) <= 0x5a)) ?
        ch + ASCII_LETTER_CASE_CODE_SHIFT :
        ch;
}

char __to_upper(char ch) {
    return (__is_letter(ch) && ((unsigned char) (ch) >= 0x61)) ?
        ch - ASCII_LETTER_CASE_CODE_SHIFT :
        ch;
}

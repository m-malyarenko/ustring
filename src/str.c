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

    const size_t len = _str_literal_len(string);
    size_t cap = STR_DEFAULT_CAPACITY;

    while (len >= cap) {
        cap *= 2;
    }
    
    str_t* self = (str_t*) malloc(sizeof(str_t));
    self->buffer = (char*) malloc(cap);
    self->len = len;
    self->cap = cap;
    for (size_t i = 0; i < len; i++) {
        self->buffer[i] = _is_ascii(string[i]) ? string[i] : '?';
    }
    self->buffer[len] = '\0';

    return self;
}

str_t* str_with_capacity(const size_t capacity) {
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

void str_drop(str_t* self) {
    if (self == NULL) {
        return;
    }

    if (self->buffer != NULL) {
        free(self->buffer);
        self->buffer = NULL;
    }

    self->cap = 0;
    self->len = 0;
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

str_t* str_append(str_t* self, const char* string) {
    if (self == NULL) {
        return NULL;
    }

    if ((string == NULL) || (_str_literal_len(string) == 0)) {
        return self;
    }

    const size_t string_len = _str_literal_len(string);
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
        self->buffer[self->len + i] = _is_ascii(string[i]) ? string[i] : '?';
    }
    self->buffer[new_len] = '\0';
    self->len = new_len;

    return self;
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

str_t* str_trim(str_t* self) {
    if ((self == NULL) || (self->len == 0)) {
        return self;
    }

    const char* front_ptr = self->buffer;
    const char* back_ptr = self->buffer + self->len;

    while ((front_ptr != back_ptr) && _is_blank(*front_ptr)) {
        front_ptr++;
    }

    while ((back_ptr != front_ptr) && _is_blank(*(back_ptr - 1))) {
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

    return self;
}

size_t _str_literal_len(const char* string) {
    if ((string == NULL) || (*string == '\0')) {
        return 0;
    }

    size_t len = 0;
    while (*(string + len) != '\0') {
        len++;
    }
    
    return len;
}

bool _str_literal_contains(const char* string, char ch) {
    const size_t len = _str_literal_len(string);

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

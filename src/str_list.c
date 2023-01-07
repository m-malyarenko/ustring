/******************************************************************************
 * 
 * @file    str_list.c
 * @date    15 Aug 2021
 * @author  Mikhail Malyarenko <malyarenko.md@gmail.com>
 * 
 * @brief   String list library implementation
 * 
 *****************************************************************************/

#include <stdlib.h>

#include <ustring/str_list.h>
#include "str_list_p.h"

str_list_t* str_list_new() {
    return str_list_with_capacity(STR_LIST_DEFAULT_CAPACITY);
}

str_list_t* str_list_with_capacity(size_t capacity) {
    str_list_t* self = (str_list_t*) malloc(sizeof(str_list_t));
    self->size = 0;
    
    if (capacity == 0) {
        self->cap = 0;
        self->buffer = NULL;
    }
    else {
        self->cap = capacity;
        self->buffer = (str_t**) malloc(capacity * sizeof(str_t*));
        
        for (size_t i = 0; i < capacity; i++) {
            self->buffer[i] = NULL;
        }
    }

    return self;
}

str_list_t* str_list_copy(const str_list_t* other) {
    if (other == NULL) {
        return str_list_with_capacity(STR_LIST_DEFAULT_CAPACITY);
    }

    str_list_t* self = (str_list_t*) malloc(sizeof(str_list_t));
    self->cap = other->size;
    self->size = other->size;

    if (self->cap == 0) {
        self->buffer = NULL;
    }
    else {
        self->buffer = (str_t**) malloc(self->cap * sizeof(str_t*));
        for (size_t i = 0; i < self->size; i++) {
            self->buffer[i] = str_copy(other->buffer[i]);
        }
    }

    return self;
}

void str_list_drop(str_list_t** self) {
    if ((self == NULL) || (*self == NULL)) {
        return;
    }

    if ((*self)->buffer != NULL) {
        const size_t len = (*self)->size;
        for (size_t i = 0; i < len; i++) {
            str_drop(&(*self)->buffer[i]);
        }

        free((*self)->buffer);
    }

    (*self)->cap = 0;
    (*self)->size = 0;
    *self = NULL;
}

void str_list_push(str_list_t* self, str_t* string) {
    if ((self == NULL) || (string == NULL)) {
        return;
    }

    if (self->size == self->cap) {
        self->cap *= 2;
        self->buffer = (str_t**) realloc(self->buffer, self->cap * sizeof(str_t*));
    }

    self->buffer[self->size++] = string;
}

str_t* str_list_pop(str_list_t* self) {
    if ((self == NULL) || (self->size == 0)) {
        return NULL;
    }

    str_t* string = self->buffer[--self->size];
    self->buffer[self->size] = NULL;
    return string;
}

size_t str_list_size(const str_list_t* self) {
    return (self != NULL) ? self->size : 0;
}

size_t str_list_cap(const str_list_t* self) {
    return (self != NULL) ? self->cap : 0;
}

bool str_list_is_empty(const str_list_t* self) {
    return (bool) ((self == NULL) || (self->size == 0));
}

str_t* str_list_at(const str_list_t* self, const size_t idx) {
    if ((self == NULL) || (self->size == 0) || (idx >= self->size)) {
        return NULL;
    }
    else {
        return self->buffer[idx];
    }
}

str_list_t* str_split(const str_t* self, const char* delim) {
    if ((self == NULL) || (self->len == 0)) {
        return str_list_new();
    }

    if ((delim == NULL) || (__str_literal_len(delim) == 0)) {
        str_list_t* new_list = str_list_new();
        str_list_push(new_list, str_copy(self));
        return new_list;
    }

    str_list_t* result_str_list = str_list_new();
    char* new_str_chunk = (char*) malloc((self->len + 1) * sizeof(char));
    const char* const bound_back = self->buffer + self->len;

    const char* front_ptr = self->buffer;
    const char* back_ptr = NULL;

    while (front_ptr != bound_back) {
        while (__str_literal_contains(delim, *front_ptr) && (front_ptr != bound_back)) {
            front_ptr++;
        }

        back_ptr = front_ptr;

        while (!__str_literal_contains(delim, *back_ptr) && (back_ptr != bound_back)) {
            back_ptr++;
        }

        if (front_ptr != back_ptr) {
            size_t i = 0;
            for (; (front_ptr + i) != back_ptr; i++) {
                new_str_chunk[i] = *(front_ptr + i);
            }

            new_str_chunk[i] = '\0';
            str_list_push(result_str_list, str_new(new_str_chunk));
            front_ptr = back_ptr;
        }
    }

    free(new_str_chunk);

    return result_str_list;
}

str_t* str_list_join(const str_list_t* self, const char* delim) {
    if ((self == NULL) || (self->size == 0)) {
        return str_new(NULL);
    }

    const size_t delim_len = __str_literal_len(delim);
    size_t total_len = delim_len * (self->size - 1);

    for (size_t i = 0; i < self->size; i++) {
        total_len += self->buffer[i]->len;
    }

    str_t* result_str = str_with_capacity(total_len + 1);
    for (size_t i = 0; i < self->size; i++) {
        str_append(result_str, str_as_ptr(self->buffer[i]));
        if (i < (self->size - 1)) {
            str_append(result_str, delim);
        }
    }

    return result_str;
}

str_list_t* str_split_whitespace(const str_t* self) {
    const char* whitespace_delim = " \t\v\n\r";

    return str_split(self, whitespace_delim);
}

bool str_list_contains(const str_list_t* self, const str_t* string) {
    if ((self == NULL)
            || (string == NULL)
            || (self->size == 0))
    {
        return false;
    }

    if (string->len == 0) {
        for (size_t i = 0; i < self->size; i++) {
            if (self->buffer[i]->len == 0) {
                return true;
            }
        }
    } else {
        for (size_t i = 0; i < self->size; i++) {
            if (str_eq(self->buffer[i], string)) {
                return true;
            }
        }
    }

    return false;
}

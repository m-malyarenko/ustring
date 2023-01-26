/**************************************************************************//**
 * 
 * @file    str_list.c
 * @date    15 Aug 2021
 * @author  Mikhail Malyarenko <malyarenko.md@gmail.com>
 * 
 *****************************************************************************/

#include <stdlib.h>

#include <ustring/str_list.h>
#include "str_list_p.h"

#define USTRING_OK  ((int) 0)
#define USTRING_ERR ((int) 1)

str_list_t* str_list_new() {
    return str_list_with_capacity(STR_LIST_DEFAULT_CAPACITY);
}

str_list_t* str_list_with_capacity(size_t capacity) {
    str_list_t* self = malloc(sizeof(str_list_t));
    if (self == NULL) {
        return NULL;
    }

    self->size = 0;
    if (capacity == 0) {
        self->cap = 0;
        self->buffer = NULL;
    } else {
        self->cap = capacity;
        self->buffer = malloc(self->cap * sizeof(str_t*));
        if (self->buffer == NULL) {
            free(self);
            return NULL;
        }
        
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

    str_list_t* self = malloc(sizeof(str_list_t));
    if (self == NULL) {
        return NULL;
    }

    self->cap = other->size;
    self->size = other->size;
    if (self->cap == 0) {
        self->buffer = NULL;
    } else {
        self->buffer = malloc(self->cap * sizeof(str_t*));
        if (self->buffer == NULL) {
            free(self);
            return NULL;
        }

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

    free(*self);
    *self = NULL;
}

int str_list_push(str_list_t* self, str_t* string) {
    if ((self == NULL) || (string == NULL)) {
        return USTRING_ERR;
    }

    if (self->size == self->cap) {
        self->cap *= 2;
        str_t** new_buffer = realloc(self->buffer, self->cap * sizeof(str_t*));
        if (new_buffer == NULL) {
            return USTRING_ERR;
        } else {
            self->buffer = new_buffer;
        }
    }

    self->buffer[self->size] = string;
    self->size += 1;

    return USTRING_OK;
}

str_t* str_list_pop(str_list_t* self) {
    if ((self == NULL) || (self->size == 0)) {
        return NULL;
    }

    self->size -= 1;
    str_t* string = self->buffer[self->size];
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
    return ((self == NULL) || (self->size == 0) || (idx >= self->size))
                ? NULL
                : self->buffer[idx];
}

str_list_t* str_split(const str_t* string, const char* delim) {
    if ((string == NULL) || (string->len == 0)) {
        return str_list_new();
    }

    if ((delim == NULL) || (__str_literal_len(delim) == 0)) {
        str_list_t* new_list = str_list_new();
        if (new_list == NULL) {
            return NULL;
        }

        str_t* self_copy = str_copy(string);
        if (self_copy == NULL) {
            str_list_drop(&new_list);
            return NULL;
        }

        const int status = str_list_push(new_list, self_copy);
        if (status != USTRING_OK) {
            str_drop(&self_copy);
            str_list_drop(&new_list);
            return NULL;
        }

        return new_list;
    }

    str_list_t* result_str_list = str_list_new();
    if (result_str_list == NULL) {
        return NULL;
    }

    char str_chunk[string->len + 1];
    const char* const bound_back = string->buffer + string->len;

    const char* front_ptr = string->buffer;
    const char* back_ptr = NULL;

    while (front_ptr != bound_back) {
        while ((front_ptr != bound_back) && __str_literal_contains(delim, *front_ptr)) {
            front_ptr++;
        }

        back_ptr = front_ptr;

        while ((back_ptr != bound_back) && !__str_literal_contains(delim, *back_ptr)) {
            back_ptr++;
        }

        if (front_ptr != back_ptr) {
            size_t i = 0;
            for (; (front_ptr + i) != back_ptr; i++) {
                str_chunk[i] = *(front_ptr + i);
            }
            str_chunk[i] = '\0';

            str_t* new_str_chunk = str_new(str_chunk);
            if (new_str_chunk == NULL) {
                str_list_drop(&result_str_list);
                return NULL;
            }

            const int status = str_list_push(result_str_list, new_str_chunk);
            if (status != USTRING_OK) {
                str_list_drop(&result_str_list);
                return NULL;
            }

            front_ptr = back_ptr;
        }
    }

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
    if (result_str == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < self->size; i++) {
        const str_t* status =
            str_append(result_str, str_as_ptr(self->buffer[i]));

        if (status == NULL) {
            str_drop(&result_str);
            return NULL;
        }

        if (i < (self->size - 1)) {
            if (str_append(result_str, delim) == NULL) {
                str_drop(&result_str);
                return NULL;
            }
        }
    }

    return result_str;
}

str_list_t* str_split_whitespace(const str_t* string) {
    const char* whitespace_delim = " \t\v\n\r";

    return str_split(string, whitespace_delim);
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

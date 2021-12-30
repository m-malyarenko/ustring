#include <string.h>

#include "test_env.h"

#include "../src/str.c"

static void str_new_test() {
    str_t* string = str_new("Pull & Bear");
    
    assert(str_len(string) == 11);
    assert(str_cap(string) == 32);
    assert(!str_is_empty(string));
    assert(str_as_ptr(string) != NULL);
    assert(strlen(str_as_ptr(string)) == str_len(string));
    assert(strcmp(str_as_ptr(string), "Pull & Bear") == 0);
    str_drop(string);

    string = str_new(NULL);

    assert(str_len(string) == 0);
    assert(str_cap(string) == 32);
    assert(str_as_ptr(string) != NULL);
    str_drop(string);

    test_passed;
}

static void str_with_capacity_test() {
    str_t* string = str_with_capacity(128);
    
    assert(str_cap(string) == 128);
    assert(str_len(string) == 0);
    assert(str_as_ptr(string) != NULL);

    string = str_with_capacity(0);
    assert(str_cap(string) == 0);
    assert(str_len(string) == 0);
    assert(str_as_ptr(string) == NULL);

    str_drop(string);

    test_passed;
}

static void str_copy_test() {
    str_t* string = str_new("Pull & Bear");
    str_t* copy = str_copy(string);

    assert(copy != NULL);
    assert(copy->buffer != string->buffer);
    assert(str_len(copy) == str_len(string));
    assert(strcmp(str_as_ptr(copy), str_as_ptr(string)) == 0);

    str_drop(string);
    str_drop(copy);

    copy = str_copy(NULL);
    assert(copy != NULL);
    assert(str_len(copy) == 0);
    assert(str_cap(copy) == 32);
    assert(str_as_ptr(copy) != NULL);

    str_drop(copy );

    test_passed;
}

static void str_drop_test() {
    str_t* string = str_new("One Two Three");
    str_drop(string);
    assert(str_as_ptr(string) == NULL);
    assert(str_len(string) == 0);
    assert(str_cap(string) == 0);

    test_passed;
}

static void str_len_test() {
    str_t* str1 = str_new("");
    str_t* str2 = str_new(NULL);
    str_t* str3 = str_new("ABC");
    str_t* str4 = str_new(" \tABC");

    assert(str_len(str1) == 0);
    assert(str_len(str2) == 0);
    assert(str_len(str3) == 3);
    assert(str_len(str4) == 5);

    str_drop(str1);
    str_drop(str2);
    str_drop(str3);
    str_drop(str4);

    test_passed;
}

static void str_cap_test() {
    str_t* str1 = str_new(NULL);
    str_t* str2 = str_with_capacity(0);
    str_t* str3 = str_with_capacity(30);

    assert(str_cap(str1) == STR_DEFAULT_CAPACITY);
    assert(str_cap(str2) == 0);
    assert(str_cap(str3) == 30);

    str_drop(str1);
    str_drop(str2);
    str_drop(str3);

    test_passed;
}

static void str_is_empty_test() {
    str_t* str1 = str_new(NULL);
    str_t* str2 = NULL;
    str_t* str3 = str_new("");
    str_t* str4 = str_with_capacity(0);
    str_t* str5 = str_with_capacity(5);
    str_t* str6 = str_new("ABCD");

    assert(str_is_empty(str1));
    assert(str_is_empty(str2));
    assert(str_is_empty(str3));
    assert(str_is_empty(str4));
    assert(str_is_empty(str5));
    assert(!str_is_empty(str6));

    str_drop(str1);
    str_drop(str2);
    str_drop(str3);
    str_drop(str4);
    str_drop(str5);
    str_drop(str6);

    test_passed;
}

static void str_at_test() {
    const char* str_lit = "abcdef";
    str_t* str1 = str_new(str_lit);
    str_t* str2 = str_new("");

    for (size_t i = 0; i < str_len(str1); i++) {
        assert(str_at(str1, i) == str_lit[i]);
    }

    assert(str_at(str1, 23) == 0);
    assert(str_at(NULL, 0) == 0);
    assert(str_at(str2, 0) == 0);
    assert(str_at(str2, 23) == 0);

    str_drop(str1);
    str_drop(str2);

    test_passed;
}

static void str_append_test() {
    str_t* str_a = str_new("One Two Three");
    size_t old_len_a = str_len(str_a);
    str_t* str_b = str_new(" Four");
    size_t old_len_b = str_len(str_b);
    str_t* str_c = str_with_capacity(0);

    assert(str_append(NULL, str_as_ptr(str_b)) == NULL);
    assert(str_append(NULL, NULL) == NULL);
    str_t* str_a_copy = str_copy(str_a);
    assert(str_append(str_a, NULL) != NULL);
    assert(str_len(str_a) == old_len_a);
    assert(strcmp(str_as_ptr(str_a), str_as_ptr(str_a_copy)) == 0);
    str_drop(str_a_copy);

    assert(str_append(str_a, str_as_ptr(str_b)) != NULL);
    assert(strcmp(str_as_ptr(str_a), "One Two Three Four") == 0);
    assert(strcmp(str_as_ptr(str_b), " Four") == 0);
    assert(str_len(str_a) == (old_len_a + old_len_b));

    assert(str_append(str_a, str_as_ptr(str_c)) != NULL);
    assert(strcmp(str_as_ptr(str_a), "One Two Three Four") == 0);

    assert(str_append(str_a, " Five") != NULL);
    assert(strcmp(str_as_ptr(str_a), "One Two Three Four Five") == 0);

    str_drop(str_a);
    str_drop(str_b);
    str_drop(str_c);

    test_passed;
}

static void str_concat_test() {
    str_t* string = str_concat(NULL, NULL);
    assert(string != NULL);
    assert(string->len == 0);
    assert(string->buffer != 0);
    str_drop(string);

    string = str_concat(NULL, str_with_capacity(5));
    assert(string != NULL);
    assert(string->len == 0);
    assert(string->buffer != 0);
    str_drop(string);

    string = str_concat(str_with_capacity(5), NULL);
    assert(string != NULL);
    assert(string->len == 0);
    assert(string->buffer != 0);
    str_drop(string);

    string = str_concat(str_with_capacity(5), str_with_capacity(5));
    assert(string != NULL);
    assert(string->len == 0);
    assert(string->buffer != 0);
    str_drop(string);

    str_t* str_a = str_new("One Two Three");
    size_t old_len_a = str_len(str_a);
    str_t* str_b = str_new(" Four");
    size_t old_len_b = str_len(str_b);
    str_t* concat_str = str_concat(str_a, str_b);
    assert(concat_str != NULL);
    assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
    assert(str_len(concat_str) == (str_len(str_a) + str_len(str_b)));
    assert(strcmp(str_as_ptr(concat_str), "One Two Three Four") == 0);
    str_drop(concat_str);

    concat_str = str_concat(str_b, str_a);
    assert(concat_str != NULL);
    assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
    assert(str_len(concat_str) == (str_len(str_a) + str_len(str_b)));
    assert(strcmp(str_as_ptr(concat_str), " FourOne Two Three") == 0);
    str_drop(concat_str);

    concat_str = str_concat(str_a, NULL);
    assert(concat_str != NULL);
    assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
    assert(str_len(concat_str) == str_len(str_a));
    assert(strcmp(str_as_ptr(concat_str), "One Two Three") == 0);
    str_drop(concat_str);

    concat_str = str_concat(str_a, str_with_capacity(5));
    assert(concat_str != NULL);
    assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
    assert(str_len(concat_str) == str_len(str_a));
    assert(strcmp(str_as_ptr(concat_str), "One Two Three") == 0);

    concat_str = str_concat(NULL, str_a);
    assert(concat_str != NULL);
    assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
    assert(str_len(concat_str) == str_len(str_a));
    assert(strcmp(str_as_ptr(concat_str), "One Two Three") == 0);
    str_drop(concat_str);

    concat_str = str_concat(str_with_capacity(5), str_a);
    assert(concat_str != NULL);
    assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
    assert(str_len(concat_str) == str_len(str_a));
    assert(strcmp(str_as_ptr(concat_str), "One Two Three") == 0);
    str_drop(concat_str);

    str_drop(str_a);
    str_drop(str_b);

    test_passed;
}

static void str_trim_test() {
    str_t* str1 = str_new("");
    str_t* str2 = str_new("  \t\n\vabc");
    str_t* str3 = str_new("  \t\nabc \t\v  \n\r");
    str_t* str4 = str_new("abc abc\t abc\n\t   ");
    str_t* str5 = str_new("  \t\n\t  \v\r   ");

    assert(strcmp(str_as_ptr(str_trim(str1)), "") == 0);
    assert(str_is_empty(str1));
    assert(strcmp(str_as_ptr(str_trim(str2)), "abc") == 0);
    assert(strcmp(str_as_ptr(str_trim(str3)), "abc") == 0);
    assert(strcmp(str_as_ptr(str_trim(str4)), "abc abc\t abc") == 0);
    assert(strcmp(str_as_ptr(str_trim(str5)), "") == 0);
    assert(str_is_empty(str5));

    str_drop(str1);
    str_drop(str2);
    str_drop(str3);
    str_drop(str4);
    str_drop(str5);

    test_passed;
}

static void _str_literal_len_test() {
    const char* str1 = NULL;
    const char* str2 = "";
    const char* str3 = "Hello";

    assert(__str_literal_len(str1) == 0);
    assert(__str_literal_len(str2) == 0);
    assert(__str_literal_len(str3) == 5);

    test_passed;
}

static void _str_literal_contains_test() {
    const char* str1 = NULL;
    const char* str2 = "";
    const char* str3 = "Hello";
    const char* str4 = "qwerty;qwerty?";

    assert(!__str_literal_contains(str1, 'a'));
    assert(!__str_literal_contains(str2, '\0'));
    assert(__str_literal_contains(str3, 'e'));
    assert(__str_literal_contains(str4, ';'));
    assert(__str_literal_contains(str4, '?'));
    assert(!__str_literal_contains(str4, 'z'));

    test_passed;
}

int main() {
    str_new_test();
    str_with_capacity_test();
    str_copy_test();
    str_drop_test();
    str_len_test();
    str_cap_test();
    str_is_empty_test();
    str_at_test();
    str_append_test();
    str_concat_test();
    str_trim_test();
    _str_literal_len_test();
    _str_literal_contains_test();

    return 0;
}
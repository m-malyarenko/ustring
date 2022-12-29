#include <criterion/criterion.h>

#include "../include/str.h"
#include "../src/str_p.h"

static str_t* string_a;
static str_t* string_b;
static str_t* string_c;
static str_t* string_empty_a;
static str_t* string_empty_b;
static str_t* string_null;

static void setup(void) {
    string_a = str_new("Pull & Bear");
    string_b = str_new("One Two Three");
    string_c = str_new("AbCd");
    string_empty_a = str_new(NULL);
    string_empty_b = str_new("");
    string_null = NULL;
}

static void teardown(void) {
    str_drop(&string_a);
    str_drop(&string_b);
    str_drop(&string_c);
    str_drop(&string_empty_a);
    str_drop(&string_empty_b);
}

TestSuite(str, .init = setup, .fini = teardown);

Test(str, new) {
    cr_assert_not_null(string_a);
    cr_assert_eq(string_a->len, 11);
    cr_assert_eq(string_a->cap, STR_DEFAULT_CAPACITY);
    cr_assert_str_eq(string_a->buffer, "Pull & Bear");

    cr_assert_not_null(string_empty_a);
    cr_assert_eq(string_empty_a->len, 0);
    cr_assert_eq(string_empty_a->cap, STR_DEFAULT_CAPACITY);
}

Test(str, with_capacity) {
    str_t* string = str_with_capacity(128);
    
    cr_assert_eq(string->cap, 128);
    cr_assert_eq(string->len, 0);
    cr_assert_not_null(string->buffer);

    string = str_with_capacity(0);
    cr_assert_eq(string->cap, 0);
    cr_assert_eq(string->len, 0);
    cr_assert_null(string->buffer);

    str_drop(&string);
}

Test(str, copy) {
    str_t* copy = str_copy(string_a);

    cr_assert_not_null(copy);
    cr_assert_neq(copy->buffer, string_a->buffer);
    cr_assert_eq(copy->len, string_a->len);
    cr_assert_str_eq(copy->buffer, string_a->buffer);

    str_drop(&copy);

    copy = str_copy(NULL);
    cr_assert_not_null(copy);
    cr_assert_eq(copy->len, 0);
    cr_assert_eq(copy->cap, STR_DEFAULT_CAPACITY);
    cr_assert_not_null(copy->buffer);

    str_drop(&copy);
}

Test(str, is_empty) {
    cr_assert_not(str_is_empty(string_a));
    cr_assert_not(str_is_empty(string_b));
    cr_assert_not(str_is_empty(string_c));
    cr_assert(str_is_empty(string_empty_a));
    cr_assert(str_is_empty(string_empty_b));
    cr_assert(str_is_empty(string_null));
}

Test(str, at) {
    str_t* expr_a = str_new("int x = 0 \ny = 0 \n( x != 0 ) || ( y != 0 ) \n");
    str_t* expr_b = str_new("y = y * 78 \ny > 90 \n");

    str_replace(expr_a, ">", "\\>");
    str_replace(expr_b, ">", "\\>");

    // str_replace(expr_a, "|", "\\|");
    str_replace(expr_b, "|", "\\|");

    cr_assert_str_eq(expr_a->buffer, "int x = 0 \ny = 0 \n( x != 0 ) \\|\\| ( y != 0 ) \n");
    cr_assert_str_eq(expr_b->buffer, "y = y * 78 \ny \\> 90 \n");

    // str_drop(&expr_a);
    str_drop(&expr_b);
}

Test(str, append) {

}

Test(str, concat) {

}

Test(str, trim) {

}

Test(str, replace) {

}

Test(str, literal_len) {

}

Test(str, literal_contains) {

}

// static void str_is_empty_test() {
//     str_t* str1 = str_new(NULL);
//     str_t* str2 = NULL;
//     str_t* str3 = str_new("");
//     str_t* str4 = str_with_capacity(0);
//     str_t* str5 = str_with_capacity(5);
//     str_t* str6 = str_new("ABCD");

//     assert(str_is_empty(str1));
//     assert(str_is_empty(str2));
//     assert(str_is_empty(str3));
//     assert(str_is_empty(str4));
//     assert(str_is_empty(str5));
//     assert(!str_is_empty(str6));

//     str_drop(str1);
//     str_drop(str2);
//     str_drop(str3);
//     str_drop(str4);
//     str_drop(str5);
//     str_drop(str6);
// }

// static void str_at_test() {
//     const char* str_lit = "abcdef";
//     str_t* str1 = str_new(str_lit);
//     str_t* str2 = str_new("");

//     for (size_t i = 0; i < str_len(str1); i++) {
//         assert(str_at(str1, i) == str_lit[i]);
//     }

//     assert(str_at(str1, 23) == 0);
//     assert(str_at(NULL, 0) == 0);
//     assert(str_at(str2, 0) == 0);
//     assert(str_at(str2, 23) == 0);

//     str_drop(str1);
//     str_drop(str2);
// }

// static void str_append_test() {
//     str_t* str_a = str_new("One Two Three");
//     size_t old_len_a = str_len(str_a);
//     str_t* str_b = str_new(" Four");
//     size_t old_len_b = str_len(str_b);
//     str_t* str_c = str_with_capacity(0);

//     assert(str_append(NULL, str_as_ptr(str_b)) == NULL);
//     assert(str_append(NULL, NULL) == NULL);
//     str_t* str_a_copy = str_copy(str_a);
//     assert(str_append(str_a, NULL) != NULL);
//     assert(str_len(str_a) == old_len_a);
//     assert(strcmp(str_as_ptr(str_a), str_as_ptr(str_a_copy)) == 0);
//     str_drop(str_a_copy);

//     assert(str_append(str_a, str_as_ptr(str_b)) != NULL);
//     assert(strcmp(str_as_ptr(str_a), "One Two Three Four") == 0);
//     assert(strcmp(str_as_ptr(str_b), " Four") == 0);
//     assert(str_len(str_a) == (old_len_a + old_len_b));

//     assert(str_append(str_a, str_as_ptr(str_c)) != NULL);
//     assert(strcmp(str_as_ptr(str_a), "One Two Three Four") == 0);

//     assert(str_append(str_a, " Five") != NULL);
//     assert(strcmp(str_as_ptr(str_a), "One Two Three Four Five") == 0);

//     str_drop(str_a);
//     str_drop(str_b);
//     str_drop(str_c);
// }

// static void str_concat_test() {
//     str_t* string = str_concat(NULL, NULL);
//     assert(string != NULL);
//     assert(string->len == 0);
//     assert(string->buffer != 0);
//     str_drop(string);

//     string = str_concat(NULL, str_with_capacity(5));
//     assert(string != NULL);
//     assert(string->len == 0);
//     assert(string->buffer != 0);
//     str_drop(string);

//     string = str_concat(str_with_capacity(5), NULL);
//     assert(string != NULL);
//     assert(string->len == 0);
//     assert(string->buffer != 0);
//     str_drop(string);

//     string = str_concat(str_with_capacity(5), str_with_capacity(5));
//     assert(string != NULL);
//     assert(string->len == 0);
//     assert(string->buffer != 0);
//     str_drop(string);

//     str_t* str_a = str_new("One Two Three");
//     size_t old_len_a = str_len(str_a);
//     str_t* str_b = str_new(" Four");
//     size_t old_len_b = str_len(str_b);
//     str_t* concat_str = str_concat(str_a, str_b);
//     assert(concat_str != NULL);
//     assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
//     assert(str_len(concat_str) == (str_len(str_a) + str_len(str_b)));
//     assert(strcmp(str_as_ptr(concat_str), "One Two Three Four") == 0);
//     str_drop(concat_str);

//     concat_str = str_concat(str_b, str_a);
//     assert(concat_str != NULL);
//     assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
//     assert(str_len(concat_str) == (str_len(str_a) + str_len(str_b)));
//     assert(strcmp(str_as_ptr(concat_str), " FourOne Two Three") == 0);
//     str_drop(concat_str);

//     concat_str = str_concat(str_a, NULL);
//     assert(concat_str != NULL);
//     assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
//     assert(str_len(concat_str) == str_len(str_a));
//     assert(strcmp(str_as_ptr(concat_str), "One Two Three") == 0);
//     str_drop(concat_str);

//     concat_str = str_concat(str_a, str_with_capacity(5));
//     assert(concat_str != NULL);
//     assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
//     assert(str_len(concat_str) == str_len(str_a));
//     assert(strcmp(str_as_ptr(concat_str), "One Two Three") == 0);

//     concat_str = str_concat(NULL, str_a);
//     assert(concat_str != NULL);
//     assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
//     assert(str_len(concat_str) == str_len(str_a));
//     assert(strcmp(str_as_ptr(concat_str), "One Two Three") == 0);
//     str_drop(concat_str);

//     concat_str = str_concat(str_with_capacity(5), str_a);
//     assert(concat_str != NULL);
//     assert((old_len_a == str_a->len) && (old_len_b == str_b->len));
//     assert(str_len(concat_str) == str_len(str_a));
//     assert(strcmp(str_as_ptr(concat_str), "One Two Three") == 0);
//     str_drop(concat_str);

//     str_drop(str_a);
//     str_drop(str_b);
// }

// static void str_trim_test() {
//     str_t* str1 = str_new("");
//     str_t* str2 = str_new("  \t\n\vabc");
//     str_t* str3 = str_new("  \t\nabc \t\v  \n\r");
//     str_t* str4 = str_new("abc abc\t abc\n\t   ");
//     str_t* str5 = str_new("  \t\n\t  \v\r   ");

//     str_trim(str1);
//     assert(strcmp(str_as_ptr(str1), "") == 0);
//     assert(str_is_empty(str1));

//     str_trim(str2);
//     assert(strcmp(str_as_ptr(str2), "abc") == 0);

//     str_trim(str3);
//     assert(strcmp(str_as_ptr(str3), "abc") == 0);

//     str_trim(str4);
//     assert(strcmp(str_as_ptr(str4), "abc abc\t abc") == 0);

//     str_trim(str5);
//     assert(strcmp(str_as_ptr(str5), "") == 0);
//     assert(str_is_empty(str5));

//     str_drop(str1);
//     str_drop(str2);
//     str_drop(str3);
//     str_drop(str4);
//     str_drop(str5);
// }

// static void _str_literal_len_test() {
//     const char* str1 = NULL;
//     const char* str2 = "";
//     const char* str3 = "Hello";

//     assert(__str_literal_len(str1) == 0);
//     assert(__str_literal_len(str2) == 0);
//     assert(__str_literal_len(str3) == 5);
// }

// static void _str_literal_contains_test() {
//     const char* str1 = NULL;
//     const char* str2 = "";
//     const char* str3 = "Hello";
//     const char* str4 = "qwerty;qwerty?";

//     assert(!__str_literal_contains(str1, 'a'));
//     assert(!__str_literal_contains(str2, '\0'));
//     assert(__str_literal_contains(str3, 'e'));
//     assert(__str_literal_contains(str4, ';'));
//     assert(__str_literal_contains(str4, '?'));
//     assert(!__str_literal_contains(str4, 'z'));
// }

// int main() {
//     str_new_test();
//     str_with_capacity_test();
//     str_copy_test();
//     str_drop_test();
//     str_len_test();
//     str_cap_test();
//     str_is_empty_test();
//     str_at_test();
//     str_append_test();
//     str_concat_test();
//     str_trim_test();
//     _str_literal_len_test();
//     _str_literal_contains_test();

//     return 0;
// }
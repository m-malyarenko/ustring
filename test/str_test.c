#include <criterion/criterion.h>

#include <ustring/str.h>
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
    cr_assert_eq(string->cap, 1);
    cr_assert_eq(string->len, 0);
    cr_assert_str_eq(string->buffer, "");

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
    cr_assert_eq(str_at(string_a, 0), 'P');
    cr_assert_eq(str_at(string_a, 5), '&');
    cr_assert_eq(str_at(string_a, 10), 'r');
    cr_assert_eq(str_at(string_a, 11), 0);
    cr_assert_eq(str_at(string_a, 100), 0);
    cr_assert_eq(str_at(NULL, 0), 0);
}

Test(str, append) {
    str_append(string_a, " & Break");
    cr_assert_str_eq(string_a->buffer, "Pull & Bear & Break");
    str_append(string_a, NULL);
    cr_assert_str_eq(string_a->buffer, "Pull & Bear & Break");
    str_append(string_a, "");
    cr_assert_str_eq(string_a->buffer, "Pull & Bear & Break");
    str_append(string_a, str_as_ptr(string_a));
    cr_assert_str_eq(string_a->buffer, "Pull & Bear & BreakPull & Bear & Break");
}

Test(str, clear) {
    const size_t old_cap = str_cap(string_a);
    str_clear(string_a);
    const size_t new_cap = str_cap(string_a);
    cr_assert_eq(string_a->len, 0);
    cr_assert_eq(old_cap, new_cap);
}

Test(str, concat) {
    str_t* string_a_b = str_concat(string_a, string_b);
    cr_assert_str_eq(string_a_b->buffer, "Pull & BearOne Two Three");
    str_t* string_a_a = str_concat(string_a, string_a);
    cr_assert_str_eq(string_a_a->buffer, "Pull & BearPull & Bear");
    str_t* string_a_b_c = str_concat(string_a_b, string_c);
    cr_assert_str_eq(string_a_b_c->buffer, "Pull & BearOne Two ThreeAbCd");
    str_t* string_b_null = str_concat(string_b, NULL);
    cr_assert_str_eq(string_b_null->buffer, "One Two Three");
    str_t* string_null_b = str_concat(NULL, string_b);
    cr_assert_str_eq(string_null_b->buffer, "One Two Three");
    str_t* string_null_null = str_concat(NULL, NULL);
    cr_assert_str_eq(string_null_null->buffer, "");
    cr_assert_eq(str_len(string_null_null), 0);

    str_drop(&string_a_b);
    str_drop(&string_a_a);
    str_drop(&string_a_b_c);
    str_drop(&string_b_null);
    str_drop(&string_null_b);
    str_drop(&string_null_null);
}

Test(str, trim) {
    str_t* string_white = str_new("  \t\n \vHello  \t\n\r World");
    str_trim(string_white);
    cr_assert_str_eq(string_white->buffer, "Hello  \t\n\r World");

    str_clear(string_white);
    str_append(string_white, "Hello  \t\n\r World  \t\n \v\n\r");
    str_trim(string_white);
    cr_assert_str_eq(string_white->buffer, "Hello  \t\n\r World");

    str_clear(string_white);
    str_append(string_white, "  \t\n \vHello  \t\n\r World  \t\n \v\n\r");
    str_trim(string_white);
    cr_assert_str_eq(string_white->buffer, "Hello  \t\n\r World");

    str_drop(&string_white);
}

Test(str, eq) {
    cr_assert(str_eq(string_a, string_a));
    cr_assert_not(str_eq(string_b, string_a));
    cr_assert_not(str_eq(string_b, NULL));
    cr_assert_not(str_eq(NULL, NULL));
    cr_assert_not(str_eq(NULL, string_b));
}

// Test(str, truncate) {

// }

// Test(str, contains) {

// }

// Test(str, contains_fn) {

// }

// Test(str, trim_matches) {

// }

// Test(str, trim_matches_fn) {

// }

// Test(str, trim_start_matches) {

// }

// Test(str, trim_start_matches_fn) {

// }

// Test(str, trim_end_matches) {

// }

// Test(str, trim_end_matches_fn) {

// }

// Test(str, replace) {

// }

// Test(str, starts_with) {

// }

// Test(str, ends_with) {

// }

// Test(str, to_lowercase) {

// }

// Test(str, to_uppercase) {

// }

// Test(str, literal_len) {

// }

// Test(str, literal_contains) {

// }

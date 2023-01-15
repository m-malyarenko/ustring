#include <ctype.h>

#include <criterion/criterion.h>

#include <ustring/str.h>
#include "../src/str_p.h"

static str_t* string_a;
static str_t* string_b;
static str_t* string_c;
static str_t* string_empty_a;
static str_t* string_empty_b;
static str_t* string_null;

static bool predicate_a(char c) {
    return (c == 'A') || (c == '&');
}

static bool predicate_b(char c) {
    return isdigit(c);
}

static bool predicate_c(char c) {
    return isalpha(c);
}

static void setup(void) {
    string_a = str_new("Pull & Bear");
    string_b = str_new("One Two Three");
    string_c = str_new("AbCd12\t&#\n");
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

    copy = str_copy(string_null);
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
    cr_assert_str_eq(string_a_b_c->buffer, "Pull & BearOne Two ThreeAbCd12\t&#\n");
    str_t* string_b_null = str_concat(string_b, string_null);
    cr_assert_str_eq(string_b_null->buffer, "One Two Three");
    str_t* string_null_b = str_concat(string_null, string_b);
    cr_assert_str_eq(string_null_b->buffer, "One Two Three");
    str_t* string_null_null = str_concat(string_null, string_null);
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
    cr_assert_not(str_eq(string_b, string_null));
    cr_assert_not(str_eq(string_null, string_null));
    cr_assert_not(str_eq(string_null, string_b));
}

Test(str, truncate) {
    size_t old_len = string_a->len;
    str_truncate(string_a, string_a->len + 10);
    size_t new_len = string_a->len;
    cr_assert_eq(old_len, new_len);
    cr_assert_str_eq(string_a->buffer, "Pull & Bear");

    str_truncate(string_a, 4);
    cr_assert_eq(string_a->len, 4);
    cr_assert_str_eq(string_a->buffer, "Pull");

    str_truncate(string_null, 4);
    cr_assert_null(string_null);
}

Test(str, contains) {
    cr_assert(str_contains(string_a, ""));
    cr_assert(str_contains(string_empty_a, ""));
    cr_assert(str_contains(string_b, "One"));
    cr_assert(str_contains(string_b, "e Two Th"));
    cr_assert(str_contains(string_b, "ree"));
    cr_assert_not(str_contains(string_b, "&all"));
    cr_assert_not(str_contains(string_b, NULL));
    cr_assert_not(str_contains(NULL, "&all"));
    cr_assert_not(str_contains(NULL, NULL));
}

Test(str, contains_fn) {
    cr_assert(str_contains_fn(string_a, predicate_a));
    cr_assert_not(str_contains_fn(string_b, predicate_a));
    cr_assert(str_contains_fn(string_c, predicate_a));
    cr_assert_not(str_contains_fn(string_a, NULL));
    cr_assert_not(str_contains_fn(NULL, predicate_a));
}

Test(str, trim_matches) {
    str_t* string_a_1 = str_copy(string_a);
    str_t* string_a_2 = str_copy(string_a);

    str_trim_matches(string_a, " & ");
    str_trim_matches(string_a, NULL);
    cr_assert_str_eq(string_a->buffer, "PullBear");

    str_trim_matches(string_a_1, "123");
    str_trim_matches(string_a_1, "ar");
    str_trim_matches(string_a_1, "ar");
    str_trim_matches(string_a_1, "Pull & Bear");
    cr_assert_str_eq(string_a_1->buffer, "Pull & Be");

    str_trim_matches(string_a_2, "Pu");
    str_trim_matches(string_a_2, "Pu");
    str_trim_matches(string_a_2, "Pull & Bear");
    cr_assert_str_eq(string_a_2->buffer, "ll & Bear");

    str_trim_matches(string_empty_a, "()");
    cr_assert_str_empty(string_empty_a->buffer);

    str_trim_matches(NULL, "CDT");

    str_drop(&string_a_1);
    str_drop(&string_a_2);
}

Test(str, trim_matches_fn) {
    str_trim_matches_fn(string_a, predicate_c);
    cr_assert_str_eq(string_a->buffer, " & ");

    str_trim_matches_fn(string_c, predicate_b);
    cr_assert_str_eq(string_c->buffer, "AbCd\t&#\n");

    str_trim_matches_fn(string_b, NULL);
    cr_assert_str_eq(string_b->buffer, "One Two Three");

    str_trim_matches_fn(string_empty_a, predicate_a);
    cr_assert_str_empty(string_empty_a->buffer);
}

Test(str, trim_start_matches) {
    str_t* string_d = str_new("123 abc89 98");
    str_t* string_e = str_new("--@\n X + Y");

    str_trim_start_matches(string_d, "12");
    cr_assert_str_eq(string_d->buffer, "3 abc89 98");

    str_trim_start_matches(string_d, "abc");
    str_trim_start_matches(string_d, "");
    cr_assert_str_eq(string_d->buffer, "3 abc89 98");

    str_trim_start_matches(string_e, "--@\n ");
    cr_assert_str_eq(string_e->buffer, "X + Y");

    str_trim_start_matches(string_empty_a, "()");
    cr_assert_str_empty(string_empty_a->buffer);

    str_trim_start_matches(NULL, "CDT");

    str_drop(&string_d);
    str_drop(&string_e);
}

Test(str, trim_start_matches_fn) {
    str_t* string_d = str_new("123 abc89 98");

    str_trim_start_matches_fn(string_d, predicate_b);
    cr_assert_str_eq(string_d->buffer, " abc89 98");

    str_trim_start_matches_fn(string_d, predicate_b);
    str_trim_start_matches_fn(string_d, NULL);
    cr_assert_str_eq(string_d->buffer, " abc89 98");

    str_trim_start_matches_fn(string_empty_a, predicate_b);
    cr_assert_str_empty(string_empty_a->buffer);

    str_trim_start_matches_fn(NULL, predicate_b);

    str_drop(&string_d);
}

Test(str, trim_end_matches) {
    str_t* string_d = str_new("123 abc89 98");
    str_t* string_e = str_new("--@\n X + Y\n");

    str_trim_end_matches(string_d, "9 98");
    cr_assert_str_eq(string_d->buffer, "123 abc8");

    str_trim_end_matches(string_d, "9 98");
    str_trim_end_matches(string_d, "");
    cr_assert_str_eq(string_d->buffer, "123 abc8");

    str_trim_end_matches(string_e, " + Y\n");
    cr_assert_str_eq(string_e->buffer, "--@\n X");

    str_trim_end_matches(string_empty_a, "()");
    cr_assert_str_empty(string_empty_a->buffer);

    str_trim_end_matches(NULL, "CDT");

    str_drop(&string_d);
    str_drop(&string_e);
}

Test(str, trim_end_matches_fn) {
    str_t* string_d = str_new("123 abc89 98");

    str_trim_end_matches_fn(string_d, predicate_b);
    cr_assert_str_eq(string_d->buffer, "123 abc89 ");

    str_trim_end_matches_fn(string_d, predicate_b);
    str_trim_end_matches_fn(string_d, NULL);
    cr_assert_str_eq(string_d->buffer, "123 abc89 ");

    str_trim_end_matches_fn(string_empty_a, predicate_b);
    cr_assert_str_empty(string_empty_a->buffer);

    str_trim_end_matches_fn(NULL, predicate_c);

    str_drop(&string_d);
}

Test(str, replace) {
    str_replace(string_a, " & ", " and ");
    cr_assert_str_eq(string_a->buffer, "Pull and Bear");

    str_replace(string_a, "Bear", "Push");
    cr_assert_str_eq(string_a->buffer, "Pull and Push");

    str_replace(string_a, "x", "y");
    cr_assert_str_eq(string_a->buffer, "Pull and Push");

    str_replace(string_a, " ", NULL);
    cr_assert_str_eq(string_a->buffer, "PullandPush");

    str_replace(string_a, "and", "");
    cr_assert_str_eq(string_a->buffer, "PullPush");

    str_replace(string_a, "PullPushPullPush", "");
    cr_assert_str_eq(string_a->buffer, "PullPush");

    str_replace(string_a, "Pu", "Bu");
    cr_assert_str_eq(string_a->buffer, "BullBush");
}

Test(str, starts_with) {
    cr_assert(str_starts_with(string_a, "Pull"));
    cr_assert_not(str_starts_with(string_a, "\nFool"));
    cr_assert(str_starts_with(string_a, ""));

    cr_assert(str_starts_with(string_c, "AbCd12\t"));
    cr_assert(str_starts_with(string_c, "AbCd12\t&#\n"));
    cr_assert(str_starts_with(string_c, ""));
    cr_assert_not(str_starts_with(string_c, "AbCd12\t&#\n12"));

    cr_assert_not(str_starts_with(string_empty_a, "abcd"));
    cr_assert(str_starts_with(string_empty_a, ""));
    cr_assert_not(str_starts_with(string_empty_a, NULL));

    cr_assert_not(NULL, "abc");
}

Test(str, ends_with) {
    cr_assert(str_ends_with(string_a, "& Bear"));
    cr_assert_not(str_ends_with(string_a, "r\n"));
    cr_assert(str_ends_with(string_a, ""));

    cr_assert(str_ends_with(string_c, "&#\n"));
    cr_assert(str_ends_with(string_c, "AbCd12\t&#\n"));
    cr_assert(str_ends_with(string_c, ""));
    cr_assert_not(str_ends_with(string_c, "\t&\n"));

    cr_assert_not(str_ends_with(string_empty_a, "abcd"));
    cr_assert(str_ends_with(string_empty_a, ""));
    cr_assert_not(str_ends_with(string_empty_a, NULL));

    cr_assert_not(NULL, "abc");
}

Test(str, to_lowercase) {
    str_to_lowercase(string_a);
    cr_assert_str_eq(string_a->buffer, "pull & bear");

    str_to_lowercase(string_b);
    cr_assert_str_eq(string_b->buffer, "one two three");

    str_to_lowercase(string_c);
    cr_assert_str_eq(string_c->buffer, "abcd12\t&#\n");

    str_to_lowercase(NULL);
}

Test(str, to_uppercase) {
    str_to_uppercase(string_a);
    cr_assert_str_eq(string_a->buffer, "PULL & BEAR");

    str_to_uppercase(string_b);
    cr_assert_str_eq(string_b->buffer, "ONE TWO THREE");

    str_to_uppercase(string_c);
    cr_assert_str_eq(string_c->buffer, "ABCD12\t&#\n");

    str_to_uppercase(NULL);
}

Test(str, literal_len) {
    cr_assert_eq(__str_literal_len(""), 0);
    cr_assert_eq(__str_literal_len("Godspeed"), 8);
    cr_assert_eq(__str_literal_len(NULL), 0);
}

Test(str, literal_contains) {
    cr_assert(__str_literal_contains("Godspeed", 'e'));
    cr_assert(__str_literal_contains("&*\nhhh\n", '\n'));
    cr_assert_not(__str_literal_contains("&*\nhhh\n", '\t'));
    cr_assert_not(__str_literal_contains(NULL, '\t'));
    cr_assert_not(__str_literal_contains("abc", '\0'));
}

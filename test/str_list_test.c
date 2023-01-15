#include <criterion/criterion.h>

#include <stdio.h>

#include <ustring/str_list.h>
#include "../src/str_list_p.h"

static str_list_t* list_a;
static str_list_t* list_b;
static str_list_t* list_empty;
static str_list_t* list_null;

static void setup(void) {
    list_a = str_list_new();
    str_list_push(list_a, str_new("foo"));
    str_list_push(list_a, str_new("bar"));
    str_list_push(list_a, str_new("baz"));

    list_b = str_list_new();
    str_list_push(list_b, str_new("Hello"));
    str_list_push(list_b, str_new("world"));

    list_empty = str_list_new();
    list_null = NULL;
}

static void teardown(void) {
    str_list_drop(&list_a);
    str_list_drop(&list_b);
    str_list_drop(&list_empty);
}

TestSuite(str_list, .init = setup, .fini = teardown);

Test(str_list, new) {
    cr_assert_eq(list_empty->size, 0);
    cr_assert_eq(list_empty->cap, STR_LIST_DEFAULT_CAPACITY);
    cr_assert_not_null(list_empty->buffer);
}

Test(str_list, with_capacity) {
    str_list_t* list_1 = str_list_with_capacity(15);
    cr_assert_eq(list_1->size, 0);
    cr_assert_eq(list_1->cap, 15);
    cr_assert_not_null(list_1->buffer);

    str_list_t* list_2 = str_list_with_capacity(0);
    cr_assert_eq(list_2->size, 0);
    cr_assert_eq(list_2->cap, 0);
    cr_assert_null(list_2->buffer);

    str_list_drop(&list_1);
    str_list_drop(&list_2);
}

Test(str_list, copy) {
    str_list_t* list_a_copy = str_list_copy(list_a);

    cr_assert_eq(list_a_copy->size, list_a->size);
    cr_assert_gt(list_a_copy->cap, 0);
    cr_assert_not_null(list_a_copy->buffer);
    cr_assert_str_eq(list_a->buffer[0]->buffer, list_a_copy->buffer[0]->buffer);
    cr_assert_str_eq(list_a->buffer[1]->buffer, list_a_copy->buffer[1]->buffer);
    cr_assert_str_eq(list_a->buffer[2]->buffer, list_a_copy->buffer[2]->buffer);

    str_list_t* list_empty_copy_1 = str_list_copy(NULL);
    cr_assert_not_null(list_empty_copy_1);
    cr_assert_eq(list_empty_copy_1->size, 0);
    cr_assert_eq(list_empty_copy_1->cap, STR_LIST_DEFAULT_CAPACITY);

    str_list_t* list_empty_copy_2 = str_list_copy(list_empty);
    cr_assert_not_null(list_empty_copy_2);
    cr_assert_eq(list_empty_copy_2->size, 0);

    str_list_drop(&list_a_copy);
    str_list_drop(&list_empty_copy_1);
    str_list_drop(&list_empty_copy_2);
}

Test(str_list, push) {
    cr_assert_eq(list_empty->size, 0);
    
    str_list_push(list_empty, str_new("one"));
    cr_assert_eq(list_empty->size, 1);

    for (unsigned int i = 0; i < 120; i++) {
        str_list_push(list_empty, str_new("string"));
    }

    cr_assert_eq(list_empty->size, 121);
    str_list_push(list_empty, NULL);
    cr_assert_eq(list_empty->size, 121);
    str_list_push(NULL, NULL);
}

Test(str_list, pop) {
    str_t* str_baz = str_list_pop(list_a);
    cr_assert_str_eq(str_baz->buffer, "baz");
    cr_assert_eq(list_a->size, 2);

    str_t* str_bar = str_list_pop(list_a);
    cr_assert_str_eq(str_bar->buffer, "bar");
    cr_assert_eq(list_a->size, 1);

    str_t* str_foo = str_list_pop(list_a);
    cr_assert_str_eq(str_foo->buffer, "foo");
    cr_assert_eq(list_a->size, 0);
}

Test(str_list, empty) {
    cr_assert(str_list_is_empty(list_empty));
    cr_assert(str_list_is_empty(NULL));
    cr_assert_not(str_list_is_empty(list_a));
    cr_assert_not(str_list_is_empty(list_b));
}

Test(str_list, at) {
    str_t* str_bar = str_list_at(list_a, 1);
    cr_assert_not_null(str_bar);
    cr_assert_str_eq(str_bar->buffer, "bar");

    str_t* str_baz = str_list_at(list_a, 2);
    cr_assert_not_null(str_baz);
    cr_assert_str_eq(str_baz->buffer, "baz");

    str_t* str_foo = str_list_at(list_a, 0);
    cr_assert_not_null(str_foo);
    cr_assert_str_eq(str_foo->buffer, "foo");

    cr_assert_null(str_list_at(list_a, 3));
    cr_assert_null(str_list_at(list_a, 4));
    cr_assert_null(str_list_at(list_empty, 0));
    cr_assert_null(str_list_at(NULL, 7));
}

Test(str_list, spilt) {
    str_t* file_path = str_new("/home/alex/documents/pdf/text.pdf");
    str_list_t* file_path_list = str_split(file_path, "/");

    cr_assert_eq(file_path_list->size, 5);
    cr_assert_str_eq(file_path_list->buffer[0]->buffer, "home");
    cr_assert_str_eq(file_path_list->buffer[1]->buffer, "alex");
    cr_assert_str_eq(file_path_list->buffer[2]->buffer, "documents");
    cr_assert_str_eq(file_path_list->buffer[3]->buffer, "pdf");
    cr_assert_str_eq(file_path_list->buffer[4]->buffer, "text.pdf");

    str_drop(&file_path);
    str_list_drop(&file_path_list);

    str_t* alnum_pattern = str_new("1233abc33231mkk111koo3");
    str_list_t* alnum_pattern_list = str_split(alnum_pattern, "123");

    cr_assert_eq(alnum_pattern_list->size, 3);
    cr_assert_str_eq(alnum_pattern_list->buffer[0]->buffer, "abc");
    cr_assert_str_eq(alnum_pattern_list->buffer[1]->buffer, "mkk");
    cr_assert_str_eq(alnum_pattern_list->buffer[2]->buffer, "koo");

    str_drop(&alnum_pattern);
    str_list_drop(&alnum_pattern_list);

    str_list_t* empty_pattern_list = str_split(NULL, "");
    cr_assert_eq(empty_pattern_list->size, 0);

    str_list_drop(&empty_pattern_list);

    str_t* single_pattern = str_new("lopilopilalalay");
    str_list_t* single_pattern_list = str_split(single_pattern, "");
    cr_assert_eq(single_pattern_list->size, 1);
    cr_assert_str_eq(single_pattern_list->buffer[0]->buffer, "lopilopilalalay");

    str_drop(&single_pattern);
    str_list_drop(&single_pattern_list);
}

Test(str_list, sort_whitespace) {
    str_t* ws_pattern = str_new(" hello \t\ndarkness, my  old \v\t friend  ");
    str_list_t* ws_pattern_list = str_split_whitespace(ws_pattern);

    cr_assert_eq(ws_pattern_list->size, 5);
    cr_assert_str_eq(ws_pattern_list->buffer[0]->buffer, "hello");
    cr_assert_str_eq(ws_pattern_list->buffer[1]->buffer, "darkness,");
    cr_assert_str_eq(ws_pattern_list->buffer[2]->buffer, "my");
    cr_assert_str_eq(ws_pattern_list->buffer[3]->buffer, "old");
    cr_assert_str_eq(ws_pattern_list->buffer[4]->buffer, "friend");

    str_drop(&ws_pattern);
    str_list_drop(&ws_pattern_list);
}

Test(str_list, join) {
    str_t* joined_hyphen_foobar = str_list_join(list_a, "-");
    cr_assert_str_eq(joined_hyphen_foobar->buffer, "foo-bar-baz");

    str_t* joined_ws_foobar = str_list_join(list_a, " \t");
    cr_assert_str_eq(joined_ws_foobar->buffer, "foo \tbar \tbaz");

    str_t* empty_string = str_list_join(NULL, "");
    cr_assert_eq(empty_string->len, 0);

    str_drop(&joined_hyphen_foobar);
    str_drop(&joined_ws_foobar);
    str_drop(&empty_string);
}

Test(str_list, contains) {
    str_t* string_foo = str_new("foo");
    str_t* string_baz = str_new("baz");
    str_t* string_beez = str_new("beez");

    cr_assert(str_list_contains(list_a, string_foo));
    cr_assert(str_list_contains(list_a, string_baz));
    cr_assert_not(str_list_contains(list_a, string_beez));
    cr_assert_not(str_list_contains(list_empty, string_beez));
    cr_assert_not(str_list_contains(NULL, string_beez));
    cr_assert_not(str_list_contains(list_a, NULL));

    str_drop(&string_foo);
    str_drop(&string_baz);
    str_drop(&string_beez);
}

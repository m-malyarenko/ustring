#include <criterion/criterion.h>

#include "../src/str_list_p.h"
#include "../include/str_list.h"

static void str_list_new_test() {
    str_list_t* list = str_list_new();

    assert(list->size == 0);
    assert(list->cap == STR_LIST_DEFAULT_CAPACITY);
    assert(list->buffer != NULL);

    str_list_drop(list);
}

static void str_list_with_capacity_test() {
    str_list_t* list = str_list_with_capacity(15);

    assert(list->size == 0);
    assert(list->cap == 15);
    assert(list->buffer != NULL);

    str_list_drop(list);
}

static void str_list_drop_test() {
    str_list_t* list = str_list_new();
    str_list_push(list, str_new("Hello"));
    str_list_push(list, str_new(" "));
    str_list_push(list, str_new("World"));

    str_list_drop(list);

    assert(list->buffer == NULL);
    assert(list->size == 0);
    assert(list->cap == 0);
}

static void str_list_push_test() {
    str_list_t* list = str_list_new();
    str_list_push(list, str_new("One"));
    str_list_push(list, NULL);
    str_list_push(list, str_new("Two"));
    str_list_push(list, str_new("Three"));

    assert(str_list_size(list) == 3);
    assert(strcmp(str_as_ptr(str_list_at(list, 0)), "One") == 0);
    assert(strcmp(str_as_ptr(str_list_at(list, 1)), "Two") == 0);
    assert(strcmp(str_as_ptr(str_list_at(list, 2)), "Three") == 0);

    str_list_drop(list);
}

static void str_list_pop_test() {
    str_list_t* list = str_list_new();
    str_list_push(list, str_new("One"));
    str_list_push(list, NULL);
    str_list_push(list, str_new("Two"));
    str_list_push(list, str_new("Three"));

    assert(str_list_size(list) == 3);

    str_t* string = str_list_pop(list);
    assert(string != NULL);
    assert(str_list_size(list) == 2);
    assert(strcmp(str_as_ptr(string), "Three") == 0);
    str_drop(string);
    string = str_list_pop(list);
    assert(string != NULL);
    assert(str_list_size(list) == 1);
    assert(strcmp(str_as_ptr(string), "Two") == 0);
    str_drop(string);
    string = str_list_pop(list);
    assert(string != NULL);
    assert(str_list_size(list) == 0);
    assert(strcmp(str_as_ptr(string), "One") == 0);
    str_drop(string);
    string = str_list_pop(list);
    assert(string == NULL);
    assert(str_list_size(list) == 0);

    string = str_list_pop(NULL);
    assert(string == NULL);
    string = str_list_pop(list);
    assert(string == NULL);

    str_list_drop(list);
}

static void str_list_size_test() {
    str_list_t* list = str_list_new();
    assert(str_list_size(list) == 0);

    str_list_push(list, str_new("One"));
    assert(str_list_size(list) == 1);

    str_list_push(list, NULL);
    assert(str_list_size(list) == 1);

    str_list_push(list, str_new(NULL));
    assert(str_list_size(list) == 2);

    str_drop(str_list_pop(list));
    assert(str_list_size(list) == 1);

    str_list_push(list, str_new("Two"));
    str_list_push(list, str_new("Three"));
    assert(str_list_size(list) == 3);

    str_list_drop(list);
}

static void str_list_cap_test() {
    str_list_t* list1 = str_list_new();
    assert(str_list_cap(list1) == STR_LIST_DEFAULT_CAPACITY);

    str_list_t* list2 = str_list_with_capacity(2);
    assert(str_list_cap(list2) == 2);

    str_list_push(list2, str_new("A"));
    str_list_push(list2, str_new("B"));
    str_list_push(list2, str_new("C"));
    assert(str_list_cap(list2) == 4);

    str_list_drop(list1);
    str_list_drop(list2);
}

static void str_list_is_empty_test() {
    str_list_t* list = str_list_new();
    assert(str_list_is_empty(list));
    assert(str_list_is_empty(NULL));

    str_list_push(list, str_new("ABC"));
    assert(!str_list_is_empty(list));
    str_drop(str_list_pop(list));
    assert(str_list_is_empty(list));
    assert(str_list_is_empty(NULL));

    str_list_drop(list);
}

static void str_list_at_test() {
    str_list_t* list = str_list_new();
    assert(str_list_at(list, 0) == NULL);
    assert(str_list_at(NULL, 0) == NULL);

    str_list_push(list, str_new("ABC"));
    str_list_push(list, str_new("CDE"));

    assert(str_list_at(list, 0) != NULL);
    assert(strcmp(str_as_ptr(str_list_at(list, 0)), "ABC") == 0);
    assert(str_list_at(list, 1) != NULL);
    assert(strcmp(str_as_ptr(str_list_at(list, 1)), "CDE") == 0);
    assert(str_list_at(list, 4) == NULL);

    str_list_drop(list);
}

static void str_split_test() {
    str_t* str1 = str_new(",  One, Two, Three, Four  ,");
    str_t* str2 = str_new(",;,;Hello");
    str_t* str3 = str_new("Hello,;,;");
    str_t* str4 = str_new(",;,;,;,");
    str_t* str5 = str_new(NULL);

    str_list_t* list1 = str_split(str1, " ,");
    assert(list1 != NULL);
    assert(str_list_size(list1) == 4);
    assert(strcmp(str_as_ptr(str_list_at(list1, 0)), "One") == 0);
    assert(strcmp(str_as_ptr(str_list_at(list1, 1)), "Two") == 0);
    assert(strcmp(str_as_ptr(str_list_at(list1, 2)), "Three") == 0);
    assert(strcmp(str_as_ptr(str_list_at(list1, 3)), "Four") == 0);

    str_list_t* list2 = str_split(NULL, " ,");
    assert(list2 != NULL);
    assert(str_list_is_empty(list2));

    str_list_t* list3 = str_split(str5, " ,");
    assert(list3 != NULL);
    assert(str_list_is_empty(list3));

    str_list_t* list4 = str_split(str5, " ,");
    assert(list4 != NULL);
    assert(str_list_is_empty(list3));

    str_list_t* list5 = str_split(str2, " ,;");
    assert(list5 != NULL);
    assert(str_list_size(list5) == 1);
    assert(strcmp(str_as_ptr(str_list_at(list5, 0)), "Hello") == 0);

    str_list_t* list6 = str_split(str3, " ,;");
    assert(list6 != NULL);
    assert(str_list_size(list6) == 1);
    assert(strcmp(str_as_ptr(str_list_at(list6, 0)), "Hello") == 0);

    str_list_drop(list1);
    str_list_drop(list2);
    str_list_drop(list3);
    str_list_drop(list4);
    str_list_drop(list5);
    str_list_drop(list6);

    str_drop(str1);
    str_drop(str2);
    str_drop(str3);
    str_drop(str4);
    str_drop(str5);
}

static void str_list_join_test() {
    str_list_t* list1 = str_list_new();
    str_list_push(list1, str_new("Extra"));
    str_list_push(list1, str_new("Mega"));
    str_list_push(list1, str_new("Super"));
    str_list_push(list1, str_new("Meta"));
    str_list_push(list1, str_new("Doughnut"));

    str_t* str1 = str_list_join(list1, "-");
    assert(strcmp(str_as_ptr(str1), "Extra-Mega-Super-Meta-Doughnut") == 0);

    str_t* str2 = str_list_join(list1, "; ");
    assert(strcmp(str_as_ptr(str2), "Extra; Mega; Super; Meta; Doughnut") == 0);

    str_t* str3 = str_list_join(NULL, "-");
    assert(str3 != NULL);
    assert(str_is_empty(str3));

    str_list_t* list2 = str_list_new();
    str_t* str4 = str_list_join(list2, "-");
    assert(str4 != NULL);
    assert(str_is_empty(str4));

    str_list_drop(list1);
    str_list_drop(list2);

    str_drop(str1);
    str_drop(str2);
    str_drop(str3);
    str_drop(str4);
}

int main() {
    str_list_new_test();
    str_list_with_capacity_test();
    str_list_drop_test();
    str_list_push_test();
    str_list_pop_test();
    str_list_size_test();
    str_list_cap_test();
    str_list_is_empty_test();
    str_list_at_test();
    str_split_test();
    str_list_join_test();

    return 0;
}

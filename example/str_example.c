#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../include/str.h"

int main() {
/* str_new example */
{
    str_t* s1 = str_new("Hello world");
    str_t* s2 = str_new(NULL);

    assert(strcmp(str_as_ptr(s1), "Hello world") == 0);
    assert(strcmp(str_as_ptr(s2), "") == 0);

    str_drop(&s1);
    str_drop(&s2);
}

/* str_copy example */
{
    str_t* s1 = str_new("Hello world");
    str_t* s2 = str_copy(s1);

    assert(strcmp(str_as_ptr(s1), "Hello world") == 0);
    assert(strcmp(str_as_ptr(s2), "Hello world") == 0);

    str_drop(&s1);
    str_drop(&s2);
}

/* str_drop example */
{
 str_t* s = str_new("foo");
 assert(strcmp(str_as_ptr(s), "foo") == 0);

 str_drop(&s);
 assert(s == NULL);
}

/* str_at example */
{
    str_t* s = str_new("foo");

    assert(str_at(s, 2) == 'o');
    assert(str_at(s, 7) == 0);

    str_drop(&s);
}

/* str_as_ptr example */
{
    str_t* s = str_new("Hello world");

    assert(strcmp(str_as_ptr(s), "Hello world") == 0);
    printf("String content: %s", str_as_ptr(s));

    str_drop(&s);
}

/* str_append example */
{
    str_t* s1 = str_new("In ");
    str_t* s2 = str_new("vino ");
    const char* s3 = "veritas";
    const char* s4 = NULL;

    str_append(s1, str_as_ptr(s2));
    assert(strcmp(str_as_ptr(s1), "In vino ") == 0);

    str_append(s1, s3);
    assert(strcmp(str_as_ptr(s1), "In vino veritas") == 0);

    str_append(s1, s4);
    assert(strcmp(str_as_ptr(s1), "In vino veritas") == 0);

    str_drop(&s1);
    str_drop(&s2);
}

/* str_concat example */
{
    str_t* s1 = str_new("Hello ");
    str_t* s2 = str_new("world");
    str_t* s3 = str_concat(s1, s2);

    assert(strcmp(str_as_ptr(s3), "Hello world") == 0);

    str_drop(&s1);
    str_drop(&s2);
    str_drop(&s3);
}

/* str_trim example */
{
    str_t* s = str_new("\n   foo bar\v  ");
    str_trim(s);

    assert(strcmp(str_as_ptr(s), "foo bar") == 0);

    str_drop(&s);
}
}
# ustring

## Description

__ustring__ is a small yet useful string library for C language. The design of the library was insired by the Rust standart string library. Currently, only ASCII character strings are supported.  

__ustring__ features:
- Dynamic heap-allocated string data structure and type `str_t`
- Dynamic heap-allocated string list data structure  and type `str_list_t`
- Plenty of string and string list manipulation methods
- Depends only on the standart C library

__ustring__ API tries to be as safe as it possible with C language:
- Required NULL pointer and memory allocation fail checks are provided
- Methods' behavior is defined for different values of the input parameters
- Error codes and error return values are provided

## Build and usage

The project uses the Meson build system.

__ustring__ may be used:
- As a subproject dependency for a Meson project
- As standalone static library (default is static library as it is rather small and can be embedded in an executable)

### Meson subproject

To use __ustring__ as Meson subproject, put this repository in the _subprojects_ directory and in the root of the _subprojects_ create an `ustring.wrap` file containing the folowing lines

    [provides]
    ustring = ustring_dep

Next add dependency in your `meson.build` as follows

    ustring_dep = dependency('ustring')

### Standalone static library

To use __ustring__ as standalone static library file buld it with following lines (in the root of the project):

    $meson setup builddir
    $meson compile -C builddir

Next you may find libustring.a archive in the `builddir` directory. To use the archive in your project you need to link against `ustring` library and include headers provided in the `include/ustring` directory.

## Documentation

To generate documentation the Doxygen utility is required.
Simply type in the root project directory:

    doxygen ./Doxyfile

and then you shall obtain html, man and RTF documentation in the `doc` directory.

## Example

```c
    #include<stdio.h>
    #include<ctype.h>

    #include<ustring/str.h>
    #include<ustring/str_list.h>

    static bool filter_fn(char ch) {
        return (ch == 'A') || (ch == 'O');
    }

    int main(int argc, char* argv[]) {
        // Create a string
        str_t* str_a = str_new("  You"); /* str_a = "  You" */
        str_t* str_b = str_new("Will"); /* str_b = "Will" */
        str_t* str_c = str_new("Not"); /* str_c = "Not" */

        //Append text to string
        str_append(str_c, " Pass"); /* str_c = "Not Pass" */
        
        //Concat strings
        str_* str_d = str_concat(str_a, str_b); /* str_d = "  You Will" */
        str_* str_e = str_concat(str_d, str_c); /* str_d = "  You Will Not Pass" */

        //Trim whitespace
        str_trim(str_e); /* str_e = "You Will Not Pass" */

        //Replace substring
        str_replace(str_e, "Will", "Shall"); /* str_e = "You Shall Not Pass" */

        //Convert to uppercase
        str_to_uppercase(str_e); /* str_d = "YOU SHALL NOT PASS" */

        //Split string by whitespace
        str_list_t* phrase = str_split_whitespace(str_e);
        /* phrase = ["YOU", "SHALL", "NOT", "PASS"] */

        //Add string to list
        str_list_push(phrase, str_new("!"));
        /* phrase = ["YOU", "SHALL", "NOT", "PASS", "!"] */

        //Get string at position
        const str_t* last_string = str_list_at(phrase, str_list_size(phrase) - 1);

        //Cast to C-style string
        printf("Last string in list is: %s\n", str_as_ptr(last_string));

        //Join string list with delimeter
        str_t* str_phrase = str_list_join(phrase, "-");
        /* str_phrase = "YOU-SHALL-NOT-PASS-!" */

        //Trim by predicate match
        str_trim_matches_fn(str_phrase, filter_fn); /* str_phrase = "YU-SHLL-NT-PSS-!" */

        //Drop strings and list
        str_drop(&str_a); /* str_a = NULL */ 
        str_drop(&str_b); /* str_b = NULL */
        str_drop(&str_c); /* str_c = NULL */
        str_drop(&str_d); /* str_d = NULL */
        str_drop(&str_e); /* str_e = NULL */

        str_list_drop(&phrase); /* phrase = NULL */
        str_drop(&str_phrase); /* str_phrase = NULL */

        return 0;
    }


```

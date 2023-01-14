criterion_dep = dependency('criterion', version : '>=2.4.0')

ustring_test_src = [
    'str_test.c',
    'str_list_test.c',
]

ustring_test_exe = executable('ustring_test', ustring_test_src,
    include_directories: ustring_inc,
    link_with: ustring_lib,
    dependencies: criterion_dep,
)

test('ustring_test', ustring_test_exe)

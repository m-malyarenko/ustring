criterion_dep = dependency('criterion', version : '>=2.4.0')

test_src = [
    'str_test.c',
    # 'str_list_test.c',
]

test_exe = executable('ustring_test',
    test_src,
    include_directories: ustring_inc,
    link_with: ustring_lib,
    dependencies: [criterion_dep],
)

test('str', test_exe)
# test('str_list', test_exe)

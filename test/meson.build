criterion_dep = dependency('criterion',
    version : '>=2.4.0',
    required: false
)

ustring_test_src = [
    'str_test.c',
    'str_list_test.c',
]

if criterion_dep.found()
    ustring_test_exe = executable('ustring_test', ustring_test_src,
        include_directories: ustring_inc,
        link_with: ustring_lib,
        dependencies: criterion_dep,
    )

    test('ustring_test', ustring_test_exe)
else
    message('To run tests, you need to install the Criterion framework.',
        'For installation details see https://criterion.readthedocs.io/en/master/'
    )
endif

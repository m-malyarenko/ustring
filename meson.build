project('ustring', 'c',
    version: '0.1.0',
    default_options: [
        'c_std=c11',
        'buildtype=release',
        'default_library=static',
    ]
)

ustring_inc = include_directories('include')
ustring_src = [
    'src/str.c',
    'src/str_list.c',
]

ustring_lib = library('ustring', ustring_src,
    include_directories: ustring_inc,
)

ustring_dep = declare_dependency(
    version: meson.project_version(),
    include_directories: ustring_inc,
    link_with: ustring_lib,
)

subdir('test')

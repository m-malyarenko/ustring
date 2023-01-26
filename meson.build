project('ustring', 'c',
    version: '1.1.0',
    default_options: [
        'c_std=c11',
        'buildtype=release',
        'default_library=static',
    ]
)

ustring_inc = include_directories('include')
subdir('src')
subdir('test')

ustring_dep = declare_dependency(
    version: meson.project_version(),
    include_directories: ustring_inc,
    link_with: ustring_lib,
)

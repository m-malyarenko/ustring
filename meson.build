project('ustring', 'c',
    version: '0.1.0',
    default_options: [
        'c_std=c11',
        'buildtype=release',
        'default_library=static',
    ]
)

inc = include_directories('include')
src = [
    'src/str.c',
    'src/str_list.c',
]

library('ustring', src,
    include_directories: inc
)

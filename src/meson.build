
bytesize_srcs = [ 'bytesize.c' ]
bytesize = shared_library('bytesize',
bytesize_srcs,
include_directories: inc,
dependencies: [deps])

bytesize_static = static_library('bytesize',
bytesize_srcs,
include_directories: inc,
dependencies: [deps])

dep_bytesize = declare_dependency(
            link_with: [ bytesize, deps ],
            include_directories: inc)

bytesize_bin = executable('bytesize',
[ bytesize_srcs, 'cli.c', 'main.c' ],
include_directories: [inc, inc_ver],
dependencies: [deps],
link_with: bytesize)

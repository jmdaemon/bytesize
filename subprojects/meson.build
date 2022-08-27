# Log.c
src_logc = 'log.c/src'
logc = shared_library('logc',
src_logc / 'log.c',
include_directories: [inc, src_logc])

dep_logc = declare_dependency(
link_with: logc,
include_directories: [inc, src_logc])

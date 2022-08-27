project('bytesize', 'c',
version: '1.10.0',
subproject_dir: 'meson-subprojects') # Don't cause conflict with subprojects directory
# Dependencies
#add_global_arguments('-lm', language : 'c')
cc = meson.get_compiler('c')
dep_m = cc.find_library('m', required : false)
dep_pcre = dependency('libpcre')
dep_gmp = dependency('gmp')
dep_mpfr = dependency('mpfr')

# Single source project version
project_version = meson.project_version().split('.')

conf_data = configuration_data()
conf_data.set('CMAKE_PROJECT_VERSION_MAJOR', project_version[0])
conf_data.set('CMAKE_PROJECT_VERSION_MINOR', project_version[1])
conf_data.set('CMAKE_PROJECT_VERSION_PATCH', project_version[2])

configure_file(input : 'include/version.h.in',
      output : 'version.h',
      configuration : conf_data)

inc = include_directories('include')
inc_ver = include_directories('.')

# Build subproject dependencies first
subdir('subprojects')

proj_utility = subproject('utility')
dep_utility = proj_utility.get_variable('dep_utility')

deps = [ dep_m, dep_pcre, dep_gmp, dep_mpfr, dep_utility, dep_logc ]

subdir('src')


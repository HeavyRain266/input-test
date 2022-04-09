project('input-test', 'c',
  version : '1.1',
  license : 'Unlicense',
  default_options : ['c_std=c99']
)

add_project_arguments(
  '-D_POSIX_C_SOURCE=200809L',
  language : 'c'
)

udev      = dependency('libudev')
libinput  = dependency('libinput')
xkbcommon = dependency('xkbcommon')

cc = meson.get_compiler('c')

if cc.get_id() == ('clang')
  extra_args = [
    '-static',
    '-pedantic',
    '-fuse-ld=mold'
  ]
else
  extra_args = [
    '-static',
    '-pedantic'
  ]
endif

executable('input-test', 'src/main.c',
  c_args : extra_args,
  dependencies : [
    udev,
    libinput,
    xkbcommon
  ]
)

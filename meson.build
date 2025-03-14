project(
  'sfml-widgets',
  'cpp',
  default_options: [
    'default_library=static',
    'cpp_std=c++11',
  ],
  license: 'MIT',
  meson_version: '>= 1.0.0',
  version: '0.0.0',
)

# dependencies
sfml_req_version = '>= 2.5.0' # required version
sfml_graphics_dep = dependency('sfml-graphics', version: sfml_req_version)
sfml_window_dep = dependency('sfml-window', version: sfml_req_version)
sfml_system_dep = dependency('sfml-system', version: sfml_req_version)

gl_dep = dependency('gl', method: 'pkg-config', required: true)

# build
subdir('src')

# declare dependency
# Usage: sfml_widgets_dep = subproject('sfml-widgets').get_variable('sfml_widgets_dep')
sfml_widgets_dep = declare_dependency(
  dependencies: [sfml_graphics_dep, sfml_window_dep, sfml_system_dep],
  include_directories: sfml_widgets_include_dir,
  link_with: sfml_widgets_lib,
)

# Demo
executable(
  'sfml-widgets-demo',
  'demo/demo.cpp',
  include_directories: sfml_widgets_include_dir,
  dependencies: [sfml_graphics_dep, sfml_window_dep, sfml_window_dep, gl_dep],
  link_with: [sfml_widgets_lib],
)

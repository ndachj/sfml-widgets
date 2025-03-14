# include
sfml_widgets_include_dir = include_directories('.')

# sources
sfml_widgets_src = [
  'Gui/Layouts/FormLayout.cpp',
  'Gui/Layouts/Layout.cpp',
  'Gui/Layouts/VBoxLayout.cpp',
  'Gui/Layouts/HBoxLayout.cpp',
  'Gui/Slider.cpp',
  'Gui/Label.cpp',
  'Gui/TextBox.cpp',
  'Gui/CheckBox.cpp',
  'Gui/Utils/Arrow.cpp',
  'Gui/Utils/Cross.cpp',
  'Gui/Utils/Box.cpp',
  'Gui/Widget.cpp',
  'Gui/SpriteButton.cpp',
  'Gui/ProgressBar.cpp',
  'Gui/Image.cpp',
  'Gui/Menu.cpp',
  'Gui/Button.cpp',
  'Gui/Theme.cpp',
]

sfml_widgets_lib = library(
  'sfml_widgets',
  sfml_widgets_src,
  include_directories: sfml_widgets_include_dir,
  dependencies: [sfml_graphics_dep, sfml_window_dep, sfml_system_dep],
)

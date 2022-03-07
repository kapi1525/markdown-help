project('markdown-help', 'cpp',
  version : '0.2.0',
  default_options : ['warning_level=3', 'cpp_std=c++17'],
  license : 'MIT'
)

appframework_dep = subproject('appframework').get_variable('appframework_dep')
json_dep         = subproject('json').get_variable('nlohmann_json_dep')
maddy_dep        = subproject('maddy').get_variable('maddy_dep')


files = files('src/main.cpp', 'src/project.cpp', 'src/build.cpp')
deps  = [dependency('threads'), appframework_dep, json_dep, maddy_dep]


markdown_help = executable('markdown-help', files,
  dependencies:   deps,
  win_subsystem:  'console',
  install:        true
)
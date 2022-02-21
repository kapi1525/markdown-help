project('markdown-help', 'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3', 'cpp_std=c++17'],
  license : 'MIT')

appframework_proj = subproject('appframework')
json_proj = subproject('json')
maddy_proj = subproject('maddy')

appframework_dep = appframework_proj.get_variable('appframework_dep')
json_dep = json_proj.get_variable('nlohmann_json_dep')
maddy_dep = maddy_proj.get_variable('maddy_dep')


files = files('src/main.cpp', 'src/project.cpp', 'src/prepare.cpp')

markdown_help = executable('markdown-help', files, dependencies: [dependency('threads'), appframework_dep, json_dep, maddy_dep])
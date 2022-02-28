markdown-help is a small tool that creates .chm (microsoft html help) files from markdown files, and overall is easier to use.
You can still use this if you want to use html files it wont touch those, it only converts md files.
Currently supports only [chmcmd compiler from free pascal](https://wiki.freepascal.org/htmlhelp_compiler), i'll add support for microsofts compiler later, after i cleanup this messy code...


## Usage

NOTE: markdown-help requires [chmcmd](https://wiki.freepascal.org/htmlhelp_compiler) help file compiler, its bundled with [free pascal](https://www.freepascal.org/download.html).
Most linux distros have chmcmd in "fpc" package.
On windows you can download chmcmd [here](https://www.freepascal.org/download.html) or using [chocolatey](https://community.chocolatey.org/packages/freepascal).


mh.json file contains configuration, for example config look [here](mh.json).
run `markdown-help` in directory with mh.json file and it will build the chm file using that config.


## Command line arguments

`markdown-help [options]`

Avaiable options:
- -v            Show version string.
- -h            Show help.
- -d            Wait for user to press enter before executing, usefull for ataching debugger.
- -p (path)     Path to directory with mh.json or file that should be used instead mh.json as config.
- -o (path)     Override output path.
- -t (path)     Override temporary path.
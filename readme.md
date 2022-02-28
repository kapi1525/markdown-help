

## markdown-help

markdown-help is a "build system" for .chm (microsoft html help) files, tho its more than a build system, it alows you to use markdown files instead of html, hence the name.
Configuration is stored in [mh.json](mh.json) file, comments are allowed in json.


## Supported chm compilers:

- [chmcmd](https://wiki.freepascal.org/htmlhelp_compiler) (bundled with free pascal) | [download](https://www.freepascal.org/download.html) | [chocolatey](https://community.chocolatey.org/packages/freepascal)

Support for Microsoft's hhc (i think it was called hhc?) compiler that is bundled with Microsoft Help Workshop will come later.


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
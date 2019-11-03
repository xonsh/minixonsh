# Minixonsh

Minixonsh is a subset of Xonsh, that can be fully parsed and compiled ahead of
time, implemented in C++, with no Python dependency.

Initially we aim to achieve the following goals:

* Very fast to start, compile and execute
* Ability to run both scripts and have an interactive prompt
* Runs natively on Linux, macOS, Windows (distribute as a single binary)
* Calling programs
* If statements
* Environment variables (including `$PATH`)

Down the road we would like to implement:

* Larger subset of Python and Xonsh that can be statically compiled (such as
  `xonsh.lib.os.rmtree`)
* Ability to have multiple backends that any minixonsh script could be compiled
  to, such as: binary (using LLVM), Bash, Windows Batch, C, C++, Python, etc.

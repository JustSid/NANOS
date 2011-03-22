##Overview
NANOS (Not Another Noobish Operating System) is a open source kernel for the i386 architecture. 
Its not meant to be a kernel that can be used as a base for some kind of serious OS, but as a learning project to gather deeper knowledge about low-level programming. Thus features might be added in some stupid order or without any deeper sense.

The whole architecture of the kernel might look a little bit wonky/wrong and really everything is subject to change, so I might end up inventing the wheel multiple times for NANOS. Beside that, NANOS is open source and you are free to contribute to this project or just do something completely different with it. Everything is licensed under the MIT license, no copy left bullshit, just real freedom.

A bootable image of the most recent version can be found [here] [dllink].

## Features
NANOS is currently in a very early stage, implemented are the following features:

* Memory manager (no virtual memory mapping atm)
* Scheduler (supports preemptive multitasking)
* Tasks and threads (a task might have one or more threads, threads run always in ring 3)
* Loading and starting of ELF binaries (this is currently done via GRUB modules because there is no filesystem or HDD driver)
* Syscalls
* PS/2 controller (+ german keyboard layout definition)
* Shell (the NANOS shell is currently pretty boring, it can start GRUB modules, print some debug informations and terminate itself)

## Bugs
NANOS ships with a lot of bugs, the most annoying bugs are listed here:

* When starting a binary via mishelle, the scheduler might return some invalid pid and mishelle ends in an endless loop waiting for the binary to exit
* Scrolling isn't really working, before you hit the end of the screen better enter "cls" into mishelle to clear the screen.
  * This bug might be caused by usage of the backspace key!

##Naming convetion
Beside that everything is higgledy-piggledy, I tried to introduce a naming convention for functions and types: Everything is diveded into components and every component has its very own two character namespace. As C functions always share the same namespace, the component namespace is used as prefix for functions followed by an underscore.

Example:

* Component: `Scheduler` (scheduler.c and .h)
* Namespace: `sd`
* Some function: `sd_foo()`;

The same also counts for structs, variables, defines, enums etc. However, default C library functions don't contain some special prefix or suffix and they are defined in the heades described by the C standard (NANOS uses ISO-C99).

##License
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

[dllink]: http://widerwille.com/stuff/nanos.zip

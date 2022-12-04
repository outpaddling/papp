# papp

## Description

PAPP (Portable Assembly PreProcessor) preprocesses assembly language code,
much as cpp does for C and C++.

Papp parses assembly language programs line-by-line, processing macro
definitions, macro references, and includes. Output is sent to the standard
output.

Papp can be used with any assembler, but was specifically designed to
enhance the programming experience with the SPIM MIPS simulator, and
follows a syntax taken from MIPS assembly language documentation.

PAPP is also a dependency for
[Plasm, the PortabLe ASseMbler](https://github.com/outpaddling/plasm),
an educational tool, meant to be used with
[EPC (Educational Personal Computer)](https:), a simulator for learning
assembly and machine language.

Macro definitions take the following general form:

    .macro name($arg1[,$arg2...])
    Macro text
    .end_macro

Argument names must begin with a $, and should use standard variable naming
conventions that do not conflict with any tokens in the assembly language
being preprocessed. References to arguments within the macro text are
replaced by the text from the macro invocation.

MIPS assembly language (MAL) example:

    .macro push($x)
    sw      $x, ($sp)
    addi    $sp, $sp, -4
    .end_macro

    .macro swap($r1,$r2,$temp)
    move    $temp, $r1
    move    $r1, $r2
    move    $r2, $temp
    .end_macro

Macros are invoked using parentheses surrounding the argument:

    lw      $t0, x
    push($t0)
    swap($a0, $a1, $t1)

Files can be included using the .include directive. This is especially
useful for storing commonly used predefined constants and macros in a
separate file for inclusion in other programs. For example, if the macro
examples above are stored in a file called mymacros.mal, then the following
could be used to import them to another program (in the same directory):

    .include("mymacros.mal")

## Design and Implementation

The code is organized following basic object-oriented design principals, but
implemented in C to minimize overhead and keep the source code accessible to
scientists who don't have time to master the complexities of C++.

Structures are treated as classes, with accessor macros and mutator functions
provided, so dependent applications and libraries need not access
structure members directly.  Since the C language cannot enforce this, it's
up to application programmers to exercise self-discipline.

For detailed coding standards, see
https://github.com/outpaddling/Coding-Standards/.

## Building and installing

papp is intended to build cleanly in any POSIX environment on any CPU
architecture.  Please don't hesitate to open an issue if you encounter
problems on any Unix-like system.

Primary development is done on FreeBSD with clang, but the code is periodically
tested on Linux, MacOS, NetBSD, and OpenIndiana as well. MS Windows is not
supported, unless using a POSIX environment such as Cygwin or Windows
Subsystem for Linux.

The Makefile is designed to be friendly to package managers, such as
[Debian packages](https://www.debian.org/distrib/packages),
[FreeBSD ports](https://www.freebsd.org/ports/),
[MacPorts](https://www.macports.org/), [pkgsrc](https://pkgsrc.org/), etc.
End users should install via one of these if at all possible.

I maintain a FreeBSD port and a pkgsrc package, which is sufficient to install
cleanly on virtually any POSIX platform.  If you would like to see a
package in another package manager, please consider creating a package
yourself.  This will be one of the easiest packages in the collection and
hence a good vehicle to learn how to create packages.

For an overview of available package managers, see the
[Repology website](https://repology.org/).

### Installing papp on FreeBSD:

FreeBSD is a highly underrated platform for scientific computing, with over
1,900 scientific libraries and applications in the FreeBSD ports collection
(of more than 30,000 total), modern clang compiler, fully-integrated ZFS
filesystem, and renowned security, performance, and reliability.
FreeBSD has a somewhat well-earned reputation for being difficult to set up
and manage compared to user-friendly systems like [Ubuntu](https://ubuntu.com/).
However, if you're a little bit Unix-savvy, you can very quickly set up a
workstation, laptop, or VM using
[desktop-installer](http://www.acadix.biz/desktop-installer.php).

To install the binary package on FreeBSD:

```
pkg install papp
```

You can just as easily build and install from source.  This is useful for
FreeBSD ports with special build options, for building with non-portable
optimizations such as -march=native, building with debugging info, and for 
[work-in-progress ports](https://github.com/outpaddling/freebsd-ports-wip),
for which binary packages are not maintained.

```
cd /usr/ports/devel/papp && env CFLAGS='-march=native -O2' make install
``` 

### Installing via pkgsrc

pkgsrc is a cross-platform package manager that works on any Unix-like
platform. It is native to [NetBSD](https://www.netbsd.org/) and well-supported
on [Illumos](https://illumos.org/), [MacOS](https://www.apple.com/macos/),
[RHEL](https://www.redhat.com)/[CentOS](https://www.centos.org/), and
many other Linux distributions.
Using pkgsrc does not require admin privileges.  You can install a pkgsrc
tree in any directory to which you have write access and easily install any
of the nearly 20,000 packages in the collection.

The
[auto-pkgsrc-setup](https://github.com/outpaddling/auto-admin/blob/master/Scripts/auto-pkgsrc-setup)
script will help you install pkgsrc in about 10 minutes.  Just download it
and run

```
sh auto-pkgsrc-setup
```

Then, assuming you selected current packages and the default prefix:

```
# Add pkgsrc to your PATH
source ~/Pkgsrc/pkg/etc/pkgsrc.sh   # Or pkgsrc.csh for csh or tcsh
# Install papp
cd ~/Pkgsrc/devel/papp
sbmake install clean clean-depends
```

See the pkgsrc documentation for more information.

Community support for pkgsrc is available through the
[pkgsrc-users](http://netbsd.org/mailinglists) mailing list.

### Building papp locally

Below are caveman install instructions for development purposes, not
recommended for regular use.

1. Clone papp and dependencies into sibling directories
2. In each dependency directory and then in papp:

    1. Run "make depend" to update Makefile.depend
    2. Run "./cave-man-install.sh" if present, otherwise "make install"

The default install prefix is ../local.  Clone papp,  and dependent
apps into sibling directories so that ../local represents a common path to all
of them.

To facilitate incorporation into package managers, the Makefile respects
standard make/environment variables such as CC, CFLAGS, PREFIX, etc.  

Add-on libraries required for the build, such as , should be found
under `${LOCALBASE}`, which defaults to ../local.
The library, headers, and man pages are installed under
`${DESTDIR}${PREFIX}`.  DESTDIR is empty by default and is primarily used by
package managers to stage installations.  PREFIX defaults to `${LOCALBASE}`.

To install directly to /myprefix, assuming  is installed there as well,
using a make variable:

```
make LOCALBASE=/myprefix clean depend install
```

Using an environment variable:

```
# C-shell and derivatives
setenv LOCALBASE /myprefix
make clean depend install

# Bourne shell and derivatives
LOCALBASE=/myprefix
export LOCALBASE
make clean depend install
```

View the Makefile for full details.

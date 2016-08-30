Madeup is a programming language for making things up -- literally. Programmers write sequences of commands to move and turn through space, tracing out the cross sections or skeletons of printable 3D shapes with algorithms and mathematical operations. The language is designed to teach computation from a tangible, first-person perspective and help students integrate computation back into the physical world.

Programs may be composed with either text or blocks. The language facilitates an imperative and functional style of programming using standard expressions, conditionals, loops, functions, arrays, and turtle geometry commands.

The easiest way to write Madeup programs is through its [web client](http://madeup.xyz). The client has only been tested under Chrome, Firefox, and Safari. The [documentation](http://madeup.xyz/docs/introduction.html) contains many examples and a language reference.

Madeup is still in alpha stage, so you will likely discover bugs, slowness, and missing features. We invite bug reports, contributions, and patience.

## Building the Interpreter
If you'd like to build the Madeup interpreter on your local machine, you'll need CMake and a compiler that supports `c++0x`.

Build on Ubuntu following these steps:

    $ git clone --recursive git@github.com:twodee/madeup.git
    $ cd madeup
    $ mkdir build
    $ cd build
    $ sudo apt-get install cmake libeigen3-dev libcgal-dev libmagick++-dev
    $ cmake ..
    $ make

If you accidentally omitted the `--recursive` option when cloning, follow these steps to get the submodules cloned:

    $ git submodule init
    $ git submodule update

## Building the Tests

First you'll need the Google Test library. These instructions install it for a single user:

    $ git clone https://github.com/google/googletest.git
    $ cd googletest
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=. ..
    $ make
    $ make install

## Building the Web Client
To build and run the web client locally, you'll also need PHP and Ruby installed.

When building, set `WWW_ROOT` to the location of your web server. On OS X this defaults to `/Library/WebServer/Documents`. On Linux to `/var/www/madeup`.

    $ cd build
    $ cmake -DWWW_ROOT=path/to/webserver ..

Then you'll want to grab several JavaScript libraries and copy over the documentation:

    $ make wwwdeps
    $ make wwwdocs

Finally, you can copy the actual web client into the web server directory:

    $ make www

## Building the Documentation
Copying the prebuilt documentation doesn't require anything but `make wwwdocs` from the build directory. If you want to make changes to the documentation and rebuild the HTML pages, you'll need Ruby and a couple of gems:

    $ gem install coderay kramdown

Then you'll need to let the Coderay gem know about the Madeup scanner:

    $ cd <madeup-source-directory>/docs
    $ ln -s madeup_scanner.rb $GEM_HOME/gems/coderay-<version>/lib/coderay/scanners/madeup.rb

To rebuild the HTML, run `make` from the `docs` source directory---not the build directory.

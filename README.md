Madeup is a programming language for making things up -- literally. Programmers write sequences of commands to move and turn through space, tracing out the cross sections or skeletons of printable 3D shapes with algorithms and mathematical operations. The language is designed to teach computation from a tangible, first-person perspective and help students integrate computation back into the physical world.

Programs may be composed with either text or blocks. The language facilitates an imperative and functional style of programming using standard expressions, conditionals, loops, functions, arrays, and turtle geometry commands.

The easiest way to write Madeup programs is through its [web client](http://madeup.xyz). The [documentation](http://madeup.xyz/docs/introduction.html) contains many examples and a language reference.

Madeup is still in alpha stage, so you will likely discover bugs, slowness, and missing features. We invite bug reports, contributions, and patience.

## Building the Interpreter
If you'd like to build the Madeup interpreter on your local machine, you can follow these steps:

    $ git clone git@github.com:twodee/madeup.git
    $ cd madeup
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

You'll need a compiler that supports `c++0x` and `cmake`.

## Building the Web Client
To build the web client locally, you'll want to set `WWW_ROOT` to the location of your web server. On OS X this defaults to `/Library/WebServer/Documents`. On Linux to `/var/www/madeup`.

    $ cd build
    $ cmake -DWWW_ROOT=path/to/webserver ..

Then you'll want to grab several JavaScript libraries and copy over the documentation:

    $ make wwwdeps
    $ make wwwdocs

Finally, you can copy the actual web client into the web server directory:

    $ make www

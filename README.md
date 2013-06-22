# Xauth++: C++ wrapper for Xauthority (Xau)

## Description

Xauth++ provides a C++ interface to Xauthority by wrapping libXau.


## Features

* Xauthority file I/O
* Automatic generation of MIT Magic Cookies
* Easy-to-use object-oriented API


## Licensing

All source files are distributed under the GNU LGPL except the example file,
`examples/testxauth.cc`, which is distributed under the GNU GPL.


## Usage

For example usage, see `examples/testxauth.cc`.

Compile and run with:

    g++ -Wall -pedantic -g -DHAVE_GCRYPT=1 -o testxauth examples/testxauth.cc \
	src/xauthxx.cc -I src \
        `pkg-config --cflags --libs xau` `libgcrypt-config --cflags --libs`

*Warning:* It will create two new files in the working directory: Xauthority-1 and
Xauthority-2.


## Status

Functional but not feature-complete compared to libXau. Feel free to contribute!


## Known Problems

The API is not documented, but the example file should be self-explanatory.


# Usage Examples


## Compiling and Linking

Just copy `src/xauthxx.h` and `src/xauthxx.cc` in your source directory.

Include Xauth++ header in your source file:

    #include "xauthxx.h"


### With libgcrypt Support for Magic Cookie Generation

Compile:

    g++ -O2 -Wall -g -c `pkg-config --cflags xau` `libgcrypt-config --cflags` -DHAVE_GCRYPT=1 xauthxx.cc

Link:

    g++ -o <your_program> <your_objects> xauthxx.o `pkg-config --libs xau` `libgcrypt-config --libs`


### Without libgcrypt support

Compile:

    g++ -O2 -Wall -g -c `pkg-config --cflags xau` xauthxx.cc

Link:

    g++ -o <your_program> <your_objects> xauthxx.o `pkg-config --libs xau`

You will not be able to generate Magic Cookies automatically without libgcrypt.


## Typical Usage Scenario

### Creating a Magic Cookie

To automatically generate a Magic Cookie:

    Xau::MagicCookie mcookie;

To create a cookie from its hexadecimal representation:

    Xau::MagicCookie mcookie("bfe6e9ebfadfd8c17b59efe936b92783");


### Adding Xauth entries in the user default files

    Xau::Display display(2);    // we will add new entries for display :2
    Xau::XauthList auth_list;   // derives from std::list
    auth_list.load_from_file(); // load from default file (~/.Xauthority)
    auth_list.push_back(Xau::Xauth(Xau::LocalAddress(), display, mcookie));
    auth_list.push_back(Xau::Xauth(Xau::InternetAddress(127,0,0,1), display, mcookie));
    auth_list.write_to_file();

* When using `Xau::LocalAddress()` with no parameters, the default local
  hostname obtained with `gethostname()` is used. You can always specify an
  optional hostname: `Xau::LocalAddress("myhost")`.
* `Xau::InternetAddress()` also accepts a `std::string` argument, e.g.
  `Xau::InternetAddress("192.168.1.1")`


### Removing Xauth entries for a particular cookie

If we want to remove all the entries with the particular cookie `mcookie`:

    Xau::XauthList auth_list;
    auth_list.load_from_file();
    auth_list.remove(mcookie);
    auth_list.write_to_file();

If we want to remove entries with the cookie `mcookie` on the display `:2`:

    Xau::XauthList auth_list;
    auth_list.load_from_file();
    auth_list.remove(Xau::Display(2) && mcookie);
    auth_list.write_to_file();

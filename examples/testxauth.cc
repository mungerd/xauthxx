/* testxauth.cc
 *
 * Copyright (C) 2008  David Munger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author(s):
 * 	David Munger <mungerd@gmail.com>
 */

// compile with:
// g++ -Wall -pedantic -g -o testxauth testxauth.cc xauthxx.cc `pkg-config --cflags --libs xau` `libgcrypt-config --cflags --libs` -DHAVE_GCRYPT=1

#include <iostream>
#include "xauthxx.h"

int main(int argc, char** argv)
{
    try {
        Xau::XauthList auth_list;
        auth_list.load_from_file();

        Xau::MagicCookie cookie;
        Xau::Display display(1);

        // Setup Scheme 1: Compact
        auth_list.push_back(Xau::Xauth(Xau::LocalAddress(), display, cookie));
        auth_list.push_back(Xau::Xauth(Xau::InternetAddress(127,0,0,1), display, cookie));

        auth_list.write_to_file("Xauthority-1");

        std::cout << "*** Xauthority-1" << std::endl;
        std::cout << "*** compare with:  xauth -nf Xauthority-1 list" << std::endl;
        for (Xau::XauthList::iterator it = auth_list.begin(); it != auth_list.end(); it++)
            std::cout << it->as_text() << std::endl;

        auth_list.remove(display && cookie);

        Xau::Xauth auth;

        // Setup Scheme 2: Explicit
        auth.display = Xau::Display(2);                     // display as number
        auth.address = Xau::InternetAddress(192,168,1,1);   // IP as numbers
        auth.cookie  = Xau::MagicCookie();                  // automatic cookie
        auth_list.push_back(auth);

        auth.address = Xau::LocalAddress();                 // automatic local hostname
        auth_list.push_back(auth);

        // Setup Scheme 3: Stream
        auth << Xau::Display("3")                           // display as string
             << Xau::InternetAddress("192.168.1.2")         // IP as string
             << Xau::MagicCookie("bfe6e9ebfadfd8c17b59efe936b92783");
                                                            // custom cookie
        auth_list.push_back(auth);

        auth << Xau::LocalAddress("myhost");                // custom local hostname
        auth_list.push_back(auth);

        auth_list.write_to_file("Xauthority-2");

        std::cout << "*** Xauthority-2" << std::endl;
        std::cout << "*** compare with:  xauth -nf Xauthority-2 list" << std::endl;
        for (Xau::XauthList::iterator it = auth_list.begin(); it != auth_list.end(); it++)
            std::cout << it->as_text() << std::endl;

        // More examples of remove():
        // auth_list.remove(Xau::LocalAddress() && display);
        // auth_list.remove(Xau::InternetAddress(127,0,0,1) && display);

    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}

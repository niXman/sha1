
// Copyright (c) 2014,2015, niXman (i dotty nixman doggy gmail dotty com)
// All rights reserved.
//
// This file is part of sha1(https://github.com/niXman/sha1) project.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or
//   other materials provided with the distribution.
//
//   Neither the name of the {organization} nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "sha1.hpp"
#include <iostream>
#include <cstring>

/***************************************************************************/

void usage(const char *argv0) {
	const char *p = std::strrchr(argv0, '/');
	const char *pn = (p != 0 ? p+1 : argv0);

	std::cerr << "usage:\n\t" << pn << " [-s string] | [-f filename]" << std::endl;
}

/***************************************************************************/

int main(int argc, char **argv) {
	std::string result;
	const char *name = argv[0];

	if ( argc != 3 ) {
		usage(name);
		return 1;
	}

	const char *opts = argv[1];
	const char *data = argv[2];

	if ( std::strcmp(opts, "-s") == 0 ) {
		result = get_string_sha1(data);
	} else if ( ! std::strcmp(opts, "-f") ) {
		std::ifstream file(data, std::ios::binary);
		if ( ! file ) {
			std::cerr << "file is not exists." << std::endl;
			return 1;
		}
		result = get_file_sha1(file);
	} else {
		usage(name);
		return 1;
	}

	std::cout << result << std::endl;
}

/***************************************************************************/

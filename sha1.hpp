
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

#include <boost/uuid/sha1.hpp>

#include <string>
#include <fstream>
#include <cstdio>

/***************************************************************************/

std::string sha1_to_string(boost::uuids::detail::sha1 &sha1) {
	unsigned int digest[5] = {0};
	const char *dig_it = reinterpret_cast<const char*>(&digest[0]);
	sha1.get_digest(digest);

	char bin[sizeof(digest)] = {0};
	char *bin_it = &bin[0];
	char *bin_end= &bin[sizeof(digest)];
	for ( std::size_t i = 0; i < sizeof(digest)/sizeof(digest[0]); ++i ) {
		bin[i * 4    ] = dig_it[i * 4 + 3];
		bin[i * 4 + 1] = dig_it[i * 4 + 2];
		bin[i * 4 + 2] = dig_it[i * 4 + 1];
		bin[i * 4 + 3] = dig_it[i * 4    ];
	}
	char out[(sizeof(digest)*2)+1] = {0};
	char *out_it = &out[0];
	for ( ;bin_it != bin_end; ++bin_it, out_it += 2) {
		std::sprintf(out_it, "%02x", ((unsigned int)(unsigned char)*bin_it));
	}

	return out;
}

/***************************************************************************/

std::string get_string_sha1(const std::string &str) {
	boost::uuids::detail::sha1 sha1;
	sha1.process_bytes(str.c_str(), str.length());

	return sha1_to_string(sha1);
}

/***************************************************************************/

std::string get_file_sha1(std::istream &file) {
	boost::uuids::detail::sha1 sha1;
	enum { buffer_size = 1024*64 };
	char buffer[buffer_size] = {0};

	std::size_t rd = 0;
	while ( (rd = file.read(buffer, buffer_size).gcount()) ) {
		sha1.process_bytes(buffer, rd);
	}

	return sha1_to_string(sha1);
}

/***************************************************************************/

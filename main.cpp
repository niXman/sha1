
#include <boost/uuid/sha1.hpp>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <cstring>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

/***************************************************************************/

std::string to_sha1(boost::uuids::detail::sha1 &sha1) {
	unsigned int digest[5] = {0};
	sha1.get_digest(digest);

	char bin[sizeof(digest)];
	const char* tmp = reinterpret_cast<char*>(&digest[0]);
	for ( size_t i = 0; i < sizeof(digest)/sizeof(digest[0]); ++i ) {
		bin[i * 4    ] = tmp[i * 4 + 3];
		bin[i * 4 + 1] = tmp[i * 4 + 2];
		bin[i * 4 + 2] = tmp[i * 4 + 1];
		bin[i * 4 + 3] = tmp[i * 4    ];
	}

	struct {
		std::string operator()(const char(&bin)[20]) {
			std::ostringstream os;
			const char *hptr = bin;

			for (int i = 0; i < sizeof(bin); i++) {
				os << std::setfill('0') << std::setw(2) << std::hex << (unsigned int)*hptr;
				++hptr;
			}
			return os.str();
		}
	} sha1_to_string;

	return sha1_to_string(bin);
}

/***************************************************************************/

std::string get_file_sha1(boost::uuids::detail::sha1 &sha1, const std::string &fname) {
	boost::interprocess::file_mapping file(fname.c_str(), boost::interprocess::read_only);
	boost::interprocess::mapped_region region(file, boost::interprocess::read_only);

	sha1.process_bytes(region.get_address(), region.get_size());

	return to_sha1(sha1);
}

/***************************************************************************/

void usage(const char *argv0) {
	const char *p = std::strrchr(argv0, '/');
	const char *pn = (p != 0 ? p+1 : argv0);

	std::cerr << "usage:\n\t" << pn << " [-s string] | [-f filename]" << std::endl;
}

/***************************************************************************/

int main(int argc, char **argv) {
	boost::uuids::detail::sha1 sha1;
	std::string result;

	enum mode {
		 mode_string
		,mode_file
		,mode_unknown
	};
	mode m = mode_unknown;

	if ( argc != 3 ) {
		usage(argv[0]);
		return 1;
	}

	if ( ! std::strcmp(argv[1], "-s") ) {
		m = mode_string;
	} else if ( ! std::strcmp(argv[1], "-f") ) {
		m = mode_file;
	} else {
		usage(argv[0]);
		return 1;
	}

	if ( m == mode_string ) {
		sha1.process_bytes(argv[2], std::strlen(argv[2]));
		result = to_sha1(sha1);
	} else {
		std::ifstream file(argv[2]);
		if ( ! file ) {
			std::cerr << "file is not exists." << std::endl;
			return 1;
		}
		result = get_file_sha1(sha1, argv[2]);
	}

	std::cout << result << std::endl;
}

/***************************************************************************/

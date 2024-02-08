#include <sstream>
using namespace std;

#include "Complex.hpp"

Complex Complex::parse(const char *arg) {
    istringstream coords_string_stream(arg);
    string re_string;
    string im_string;
    getline(coords_string_stream, re_string, ',');
    getline(coords_string_stream, im_string, ',');
    return Complex{stod(re_string), stod(im_string)};
}

#ifdef _WIN32
#pragma warning(disable: 5045)
#endif

#include <string>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#pragma warning(default: 5045)
#endif

using namespace std;

#include "rasterise_text.hpp"

int
main(
    int argc,
    char *argv[]
) try {
    throw_if_failed(
        argc == 3,
        [] { return "Need 3 arguments."s; }
    );
    const auto input_file = string{argv[1]};
    const auto output_dir = string{argv[2]};

    auto renderer = Renderer{argc, argv};

    auto input_stream = ifstream{input_file};
    auto line = string{};

    //
    // Starting at one, because technically 0 is a zero-digit number, so ostreaming a ‘0’
    // should be the empty-string, but it’s not, so the output filename for the zeroth
    // file looks wrong.
    //
    auto i = 1u;
    while (getline(input_stream, line)) {
        if (line.empty())
            continue;
        renderer(line, output_dir + "/"s + to_string(i) + ".bmp"s);
        ++i;
    }
}
catch (const exception &e) {
    cerr << "Exception thrown: "s << e.what() << '\n';
}

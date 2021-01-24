#ifdef _WIN32
#pragma warning(disable: 5045)
#endif

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

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
        [] {
            return "Need 3 arguments."s;
        }
    );
    const auto input_file = string{argv[1]};
    const auto output_dir = string{argv[2]};

    auto renderer = Renderer{
        argc,
        argv
    };

    auto input_stream = ifstream{input_file};
    auto line = string{};
    auto i = size_t{};
    while (
        getline(
            input_stream,
            line
        )
    ) {
        if (line.empty()) {
            continue;
        }
        auto stream = ostringstream{};
        stream << output_dir << "/"s << setfill('0')
            << setw(3) // We don't expect to have any test cases with more than a 1000 lines.
            << i << ".png"s;
        renderer(
            line,
            stream.str()
        );
        ++i;
    }
}
catch (
    const exception &e
) {
    cerr << "Exception thrown: "s << e.what() << '\n';
}

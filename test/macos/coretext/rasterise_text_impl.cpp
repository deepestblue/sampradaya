#include <filesystem>

//#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

using namespace std;

#include "../../rasterise_text.hpp"

const auto typeface_file_path = filesystem::path{"../../../src/Sampradaya.ttf"s};

void
throw_if_failed(bool exp) {
    static auto l = []() {
        return "Assertion failed."s;
    };
    throw_if_failed(
        exp,
        l
    );
}

class Renderer::impl {
public:
    impl(int , char *[])
    { }

    void
    operator()(const string &text, const string &output_filename) {
    }

private:
};

Renderer::Renderer(
    int argc,
    char *argv[]
) : p_impl{
    std::make_unique<impl>(
        argc,
        argv
    )
} {}

void
Renderer::operator()(
    const string &text,
    const string &output_filename) {
    (*p_impl)(
        text,
        output_filename
    );
}

Renderer::~Renderer() = default;

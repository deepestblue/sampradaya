#include <stdexcept>
#include <string>
#include <memory>
#include <filesystem>

const auto typeface_size_pt = 48u;

template <typename E>
auto
throw_if_failed(
    bool exp,
    const E &e
) {
    if (exp)
        return;

    throw runtime_error(e());
}

class Renderer {
    class impl;
    const unique_ptr<impl> p_impl;
public:
    void
    operator()(
        const string &text,
        const string &output_filename
    ) const;

    Renderer(
        int argc,
        char *argv[]
    );
    ~Renderer();
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
};

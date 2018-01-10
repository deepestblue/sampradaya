#include <string>
#include <fstream>

#ifdef DEBUG
#include <iostream>
#endif

#include <boost/format.hpp>

#include "QtWidgets/QApplication"
#include "QtGui/QImage"
#include "QtGui/QPainter"

using namespace std;
using boost::format;

template<typename T>
void throw_if_false(T exp) {
    if (! exp)
        throw "Ack!";
}

//
// Unfortunately, void is irregular in C++,
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0146r1.html
// so we're forced to use some hackery to regularise it.
// https://stackoverflow.com/questions/47996550/handling-void-assignment-in-c-generic-programming
//
struct Or_void {};
template<typename T>
T &&operator ,(T &&x, Or_void) { return std::forward<T>(x); }

class Text_to_image_renderer {
public:
    Text_to_image_renderer(int argc, char *argv[])
    : app(argc, argv)
    , font("Sampradaya")
    , dummy(1, 1, QImage::Format_RGB32)  // Has to be non-zero sized for QPainter::begin(...) to succeed
    {}

    void
    operator()(const string &text, const string &output_filename) {
        const auto qtext = QString::fromStdString(text);

        const auto bounding_rect = get_bounding_rect(qtext);
        const auto image = render_text(qtext, bounding_rect);

        throw_if_false(
            image.save(QString::fromStdString(output_filename), "PNG")
        );
    }

private:
    QRect
    get_bounding_rect(const QString &qtext) {
        return paint_on(dummy,
            [&]() {
                auto r = painter.boundingRect(QRect(), 0, qtext);
#ifdef DEBUG
                auto bounding_rect_format = format{"Left: %1%, Right: %2%, Top: %3%, Bottom: %4%."s};
                cout << bounding_rect_format % r.left() % r.right() % r.top() % r.bottom() << '\n';
#endif
                return r;
        });
    }

    QImage
    render_text(const QString &qtext, const QRect &bounding_rect) {
        //
        // These should be r.width and r.height below, but they don't seem to work.
        //
        auto image = QImage{512, 128, QImage::Format_RGB32};
        image.fill(Qt::white);
        paint_on(image,
            [&]() {
                painter.drawText(0, bounding_rect.height(), qtext);
            });
        return image;
    }

    template <typename F>
    auto
    paint_on(QImage &image, F fn) -> decltype(fn()) {
        throw_if_false(
            painter.begin(&image)
        );
        painter.setFont(font);
        auto result = (fn(), Or_void{});
        throw_if_false(
            painter.end()
        );
        return static_cast<decltype(fn())>(result);
    }

    QApplication app;
    QFont font;
    QPainter painter;
    QImage dummy;
};

int
main(int argc, char *argv[]) {
    throw_if_false(argc == 3);
    const auto input_file = string{argv[1]};
    const auto output_dir = string{argv[2]};

    Text_to_image_renderer text_to_image_renderer{argc, argv};

    auto output_dir_format = format{"%1%/%2%.png"};
    auto input_stream = ifstream{input_file};
    auto line = string{};

    //
    // Starting at one, because technically 0 is a zero-digit number, so ostreaming a '0'
    // should be the empty-string, but it's not, so the output filename for the zeroth
    // file looks wrong.
    //
    auto i = 1u;
    while (getline(input_stream, line)) {
        if (line.empty())
            continue;
        text_to_image_renderer(line, str(output_dir_format % output_dir % i));
        ++i;
    }

    return 0;
}

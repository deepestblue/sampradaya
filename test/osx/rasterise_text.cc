#include <string>
#include <fstream>

//#define DEBUG

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
void assert_and_throw(T exp) {
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
    , font("Sampradaya", typeface_size_pt)
    , metrics(font)
    , dummy(1, 1, QImage::Format_RGB32)  // Has to be non-zero sized for QPainter::begin(...) to succeed
    {
#ifdef DEBUG
        auto metrics_format = format{"For the Sampradaya font, ascent: %1%, descent: %2%, leading: %3%."s};
        cout << metrics_format % metrics.ascent() % metrics.descent() % metrics.leading() << '\n';
#endif
        font.setStyleStrategy(
            static_cast<QFont::StyleStrategy>(QFont::NoAntialias | QFont::NoFontMerging)
        );
    }

    void
    operator()(const string &text, const string &output_filename) {
        const auto qtext = QString::fromStdString(text);

        const auto bounding_rect = get_bounding_rect(qtext);
        const auto image = render_text(qtext, bounding_rect);

        assert_and_throw(
            image.save(QString::fromStdString(output_filename))
        );
    }

private:
    QRect
    get_bounding_rect(const QString &qtext) {
        return paint_on(
            dummy,
            [&]() {
                const auto r = painter.boundingRect(QRect{}, 0, qtext);
#ifdef DEBUG
                auto bounding_rect_format = format{"For string %1%, Width: %2%, Height: %3%."s};
                cout << bounding_rect_format % qtext.toStdString() % r.width() % r.height() << '\n';
#endif
                return r;
            }
        );
    }

    QImage
    render_text(const QString &qtext, const QRect &bounding_rect) {
        auto image = QImage{
            bounding_rect.width(),
            bounding_rect.height() + 20, // We seem to need a bit more height on Qt. Probably a typeface bug???
            QImage::Format_RGB32
        };
        image.fill(
            Qt::white
        );
        paint_on(
            image,
            [&]() {
                painter.drawText(
                    0,
                    bounding_rect.height() - metrics.descent() - metrics.leading() + 5, // What's going on here?
                    qtext
                );
            }
        );
        return image;
    }

    template <typename F>
    auto
    paint_on(QImage &image, F fn) -> decltype(fn()) {
        assert_and_throw(
            painter.begin(&image)
        );
        painter.setFont(font);
        const auto result = (fn(), Or_void{});
        assert_and_throw(
            painter.end()
        );
        return static_cast<decltype(fn())>(result);
    }

    QApplication app;
    QFont font;
    QFontMetrics metrics;
    QPainter painter;
    QImage dummy;
    static const int typeface_size_pt = 48;
};

int
main(int argc, char *argv[]) {
    assert_and_throw(argc == 3);
    const auto input_file = string{argv[1]};
    const auto output_dir = string{argv[2]};

    Text_to_image_renderer text_to_image_renderer{argc, argv};

    auto output_dir_format = format{"%1%/%2%.bmp"};
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

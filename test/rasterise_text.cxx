#include <string>
#include <fstream>
#include <iostream>

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

class Text_to_image_renderer {
public:
    Text_to_image_renderer(int argc, char *argv[])
    : app(argc, argv)
    , font("Sampradaya")
    {}

    void operator()(const string &text, const string &output_filename) {
        const auto qtext = QString::fromStdString(text);

        const auto bounding_rect = get_bounding_rect(qtext);
        const auto image = render_text(qtext, bounding_rect);

        throw_if_false(
            image.save(QString::fromStdString(output_filename), "PNG")
        );
    }

private:
    QRect get_bounding_rect(const QString &qtext) {

        // Has to be non-zero sized for QPainter::begin(...) to succeed
        auto dummy = QImage{1, 1, QImage::Format_RGB32};
        throw_if_false(
            painter.begin(&dummy)
        );
        painter.setFont(font);

        auto r = painter.boundingRect(QRect(), 0, qtext);
#ifdef DEBUG
        auto bounding_rect_format = format{"Left: %1%, Right: %2%, Top: %3%, Bottom: %4%."s};
        cout << bounding_rect_format % r.left() % r.right() % r.top() % r.bottom() << '\n';
#endif

        throw_if_false(
            painter.end()
        );

        return r;
    }

    QImage render_text(const QString &qtext, const QRect &bounding_rect) {

        // This should be r.width and r.height below, but they don't seem to work.
        auto image = QImage{512, 128, QImage::Format_RGB32};
        image.fill(Qt::white);
        throw_if_false(
            painter.begin(&image)
        );

        painter.setFont(font);
        painter.drawText(0, bounding_rect.height(), qtext);

        throw_if_false(
            painter.end()
        );

        return image;
    }

    QApplication app;
    QFont font;
    QPainter painter;
};

int main(int argc, char *argv[]) {
    throw_if_false(argc == 3);
    const auto input_file = string{argv[1]};
    const auto output_dir = string{argv[2]};

    Text_to_image_renderer text_to_image_renderer{argc, argv};

    auto output_dir_format = format{"%1%/%2%.png"};
    auto input_stream = ifstream{input_file};
    auto line = string{};

    // Starting at one, because technically 0 is a zero-digit number, so ostreaming a '0'
    // should be the empty-string, but it's not, so the output filename for the zeroth
    // file looks wrong.
    auto i = 1u;
    while (getline(input_stream, line)) {
        if (line == "")
            continue;
        text_to_image_renderer(line, str(output_dir_format % output_dir % i));
        ++i;
    }

    return 0;
}

#include <string>
#include <iostream>

#include <boost/format.hpp>

#include "QtWidgets/QApplication"
#include "QtGui/QImage"
#include "QtGui/QPainter"

using namespace std;

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
        const auto &qtext = QString::fromStdString(text);

        const auto bounding_rect = get_bounding_rect(qtext);
        const auto image = render_text(qtext, bounding_rect);

        throw_if_false(
            image.save(QString::fromStdString(output_filename), "PNG")
            );
    }

private:
    QRect get_bounding_rect(const QString &qtext) {
        auto dummy = QImage{1, 1, QImage::Format_RGB32};
        throw_if_false(
            painter.begin(&dummy)
            );
        painter.setFont(font);
        auto r = painter.boundingRect(QRect(), 0, qtext);
#ifdef DEBUG
        auto bounding_rect_format = boost::format{"Left: %1%, Right: %2%, Top: %3%, Bottom: %4%."s};
        cout << bounding_rect_format % r.left() % r.right() % r.top() % r.bottom() << '\n';
#endif
        throw_if_false(
            painter.end()
            );

        return r;
    }

    QImage render_text(const QString &qtext, const QRect &bounding_rect) {
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
    Text_to_image_renderer text_to_image_renderer{argc, argv};
    text_to_image_renderer("𑌮𑌮 𑌨𑌾𑌮 𑌅𑌮𑍍𑌬𑌰𑍀𑌷𑌃 ।"s, "foo.png"s);
    return 0;
}

#include <string>
#include "QtWidgets/QApplication"
#include "QtGui/QImage"
#include "QtGui/QPainter"

using namespace std;

template<typename T>
void throw_if_false(T exp) {
    if (! exp)
        throw "Ack!";
}

void draw_text_at(
    const string &text,
    int x, int y,
    QPainter &painter) {

    painter.drawText(x, y, QString::fromStdString(text));
}

void render_line_to_image(const string &line, const string &output_filename) {
    QPainter painter;
    QImage dummy;

    QImage image(200, 200, QImage::Format_RGB32);
    image.fill(Qt::white);

    throw_if_false(
        painter.begin(&image)
        );

    QFont font("Sampradaya");
    painter.setFont(font);

    draw_text_at(line, 10, 50, painter);

    throw_if_false(
        painter.end()
        );

    throw_if_false(
        image.save(QString::fromStdString(output_filename), "PNG")
        );
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    render_line_to_image("𑌮𑌮 𑌨𑌾𑌮 𑌅𑌮𑍍𑌬𑌰𑍀𑌷𑌃 ।", "foo.png");
    return 0;
}

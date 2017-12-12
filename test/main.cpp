#include "QtWidgets/QApplication"
#include "QtGui/QImage"
#include "QtGui/QPainter"

template<typename T>
void throw_if_false(T exp) {
    if (exp)
        throw "Ack!";
}

void drawText(
    const QString &typefaceName,
    int x, int y, const QString &text,
    QPainter &painter) {

    QFont font(typefaceName);
    painter.setFont(font);
    painter.drawText(x, y, text);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QPainter painter;
    QImage dummy;

    QImage image(200, 200, QImage::Format_RGB32);
    image.fill(Qt::white);

    {
        painter.begin(&image);

        drawText("Arial", 10, 10, "My name is Ambarish.", painter);
        drawText("Arial", 10, 30, "எனது பெயர் அம்பரீடன்.", painter);
        drawText("Sampradaya", 10, 50, "𑌮𑌮 𑌨𑌾𑌮 𑌅𑌮𑍍𑌬𑌰𑍀𑌷𑌃 ।", painter);

        painter.end();
    }

    if (! image.save("foo.png", "PNG")) {
        throw "Ack!";
    }

    return 0;
}

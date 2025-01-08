#include "rasterizer.h"
#include <QApplication>
#include <QImage>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Rasterizer rasterizer;
    QImage image("marius_color.jpg"); // Ersetze dies durch den Pfad zu deinem Bild

    if (image.isNull()) {
        qDebug() << "Bild konnte nicht geladen werden!";
        return 1;
    }

    // Beispiel 1: Lineare Skalierung (Standardwert)
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_average_linear.svg", false, 1, 3)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    // Beispiel 2: Konstante Skalierung
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_average_constant.svg", false, 0, 3)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    // Beispiel 3: Logarithmische Skalierung
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_average_logarithmic.svg", false, 2, 3)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    // Beispiel 4: Quadratwurzel-Skalierung
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_average_sqrt.svg", false, 3, 3)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    // Beispiel 5: Mit Medianfilter und mit linearer Skalierung
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_median_linear.svg", true, 1, 3)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    return 0;
}

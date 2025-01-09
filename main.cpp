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

    // Beispiel 1a: Konstante Skalierung Durchschnitt
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_constant_average.svg", false, 0, 1.0)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    // Beispiel 1b: Konstante Skalierung Mittewert
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_constant_median.svg", true, 0, 1.0)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    // Beispiel 2a: Lineare Skalierung (Standardwert)
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_linear_average.svg", false, 1, 1.3)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    // Beispiel 2b: Lineare Skalierung (Standardwert)
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_linear_median.svg", true, 1, 1.3)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    // Beispiel 3a: Logarithmische Skalierung
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_logarithmic_average.svg", false, 2, 1.1)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    // Beispiel 3b: Logarithmische Skalierung
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_logarithmic_median.svg", true, 2, 1.1)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    // Beispiel 4a: Quadratwurzel-Skalierung
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_sqrt_average.svg", false, 3, 1.1)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    // Beispiel 4b: Quadratwurzel-Skalierung
    if (!rasterizer.rasterize(image, 768, 1024, 300, 2, 0, 0, image.width(), image.height(), "output_sqrt_median.svg", true, 3, 1.1)) {
        qDebug() << "Rasterisierung fehlgeschlagen!";
        return 1;
    }

    return 0;
}

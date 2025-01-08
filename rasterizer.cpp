#include "rasterizer.h"
#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QStringConverter>
#include <algorithm>
#include <cmath>

Rasterizer::Rasterizer() {}
Rasterizer::~Rasterizer() {}

double adjustBrightness(double brightness, double gamma = 2.2) {
    return std::pow(brightness, 1.0 / gamma);
}

bool Rasterizer::rasterize(const QImage& originalImage, double outputWidthMM, double outputHeightMM, int dpi, double rasterSizeMM,
                           int xStartOriginal, int yStartOriginal, int selectedWidthOriginal, int selectedHeightOriginal,
                           const QString& outputFileName, bool useMedian, int scalingMode, double coverageFactor) {

    int originalWidth = originalImage.width();
    int originalHeight = originalImage.height();

    if(selectedWidthOriginal == 0) selectedWidthOriginal = originalWidth;
    if(selectedHeightOriginal == 0) selectedHeightOriginal = originalHeight;

    int outputWidthPx = static_cast<int>(outputWidthMM / MM_PER_INCH * dpi);
    int outputHeightPx = static_cast<int>(outputHeightMM / MM_PER_INCH * dpi);
    double squareSizePx = rasterSizeMM / MM_PER_INCH * dpi;

    double widthRatio = static_cast<double>(selectedWidthOriginal) / outputWidthPx;
    double heightRatio = static_cast<double>(selectedHeightOriginal) / outputHeightPx;

    QVector<QRectF> circles;
    QVector<QColor> colors;

    for (int y = 0; y < outputHeightPx / squareSizePx; ++y) {
        for (int x = 0; x < outputWidthPx / squareSizePx; ++x) {
            int originalX = static_cast<int>(x * squareSizePx * widthRatio) + xStartOriginal;
            int originalY = static_cast<int>(y * squareSizePx * heightRatio) + yStartOriginal;

            QColor color;
            if (useMedian) {
                color = calculateMedianColor(originalImage, originalX, originalY, squareSizePx * widthRatio, squareSizePx * heightRatio);
            } else {
                color = calculateAverageColor(originalImage, originalX, originalY, squareSizePx * widthRatio, squareSizePx * heightRatio);
            }

            // Helligkeit der Farbe (0.0 - 1.0)
            double brightness = color.lightnessF();

            // Helligkeit anpassen (Gamma-Korrektur)
            brightness = adjustBrightness(brightness, 1.5); // Gamma-Wert anpassen

            // Radius basierend auf dem Skalierungsmodus berechnen (angepasst)
            double radius = 0.0;
            switch (scalingMode) {
            case 0: // Konstant
                radius = squareSizePx / 2.0 * coverageFactor;
                break;
            case 1: // Linear (angepasst)
                radius = std::clamp((1.0 - brightness) * squareSizePx * 0.75 * coverageFactor, 0.0, squareSizePx / 2.0 * coverageFactor); // Faktor 0.75 und clamp hinzugefügt
                break;
            case 2: // Logarithmisch (angepasst)
                if (brightness > 0.0) {
                    radius = std::clamp((1.0 - std::log(brightness * 9.0 + 1.0) / std::log(10.0)) * squareSizePx * 0.9 * coverageFactor, 0.0, squareSizePx / 2.0 * coverageFactor); // Faktor 0.9 und clamp hinzugefügt
                } else {
                    radius = squareSizePx/2 * coverageFactor;
                }
                break;
            case 3: // Quadratwurzel (angepasst)
                radius = std::clamp((1.0 - std::sqrt(brightness)) * squareSizePx * coverageFactor, 0.0, squareSizePx / 2.0 * coverageFactor); // clamp hinzugefügt
                break;
            default:
                radius = squareSizePx / 2.0 * coverageFactor;
                break;
            }
            if(radius < 0) radius = 0;
            circles.append(QRectF(x * squareSizePx + (squareSizePx - 2 * radius)/2, y * squareSizePx + (squareSizePx - 2 * radius)/2, 2 * radius, 2 * radius));
            colors.append(color);
        }
    }
    return writeSvgToFile(outputFileName, outputWidthPx, outputHeightPx, circles, colors);
}


QColor Rasterizer::calculateAverageColor(const QImage& image, int x, int y, double width, double height) {
    int r = 0, g = 0, b = 0;
    int anzahlPixel = 0;
    for (int oy = y; oy < y + height && oy < image.height(); ++oy) {
        for (int ox = x; ox < x + width && ox < image.width(); ++ox) {
            if(ox < 0 || oy < 0 || ox >= image.width() || oy >= image.height()) continue;
            QColor pixelFarbe = image.pixelColor(ox, oy);
            r += pixelFarbe.red();
            g += pixelFarbe.green();
            b += pixelFarbe.blue();
            anzahlPixel++;
        }
    }
    if (anzahlPixel > 0) {
        return QColor(r / anzahlPixel, g / anzahlPixel, b / anzahlPixel);
    }
    return Qt::white; // Standardfarbe, falls keine Pixel gefunden wurden
}

QColor Rasterizer::calculateMedianColor(const QImage& image, int x, int y, double width, double height) {

    QVector<int> redValues;
    QVector<int> greenValues;
    QVector<int> blueValues;

    for (int oy = y; oy < y + height && oy < image.height(); ++oy) {
        for (int ox = x; ox < x + width && ox < image.width(); ++ox) {
            if(ox < 0 || oy < 0 || ox >= image.width() || oy >= image.height()) continue;
            QColor pixelColor = image.pixelColor(ox, oy);
            redValues.append(pixelColor.red());
            greenValues.append(pixelColor.green());
            blueValues.append(pixelColor.blue());
        }
    }

    if (redValues.isEmpty()) {
        return Qt::white; // Standardfarbe, falls keine Pixel gefunden wurden
    }

    std::sort(redValues.begin(), redValues.end());
    std::sort(greenValues.begin(), greenValues.end());
    std::sort(blueValues.begin(), blueValues.end());

    int medianRed = redValues[redValues.size() / 2];
    int medianGreen = greenValues[greenValues.size() / 2];
    int medianBlue = blueValues[blueValues.size() / 2];

    return QColor(medianRed, medianGreen, medianBlue);
}

bool Rasterizer::writeSvgToFile(const QString& fileName, int widthPx, int heightPx, const QVector<QRectF>& circles, const QVector<QColor>& colors) {

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        // Korrigierte Kodierungseinstellung:
        stream.setEncoding(QStringConverter::Utf8);

        stream << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
        stream << "<svg width=\"" << widthPx << "\" height=\"" << heightPx << "\" viewBox=\"0 0 " << widthPx << " " << heightPx << "\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n";

        stream << "<rect id=\"background\" x=\"0\" y=\"0\" width=\"" << widthPx << "\" height=\"" << heightPx << "\" fill=\"white\" />\n";
        stream << "<g id=\"circles\">\n";

        for (int i = 0; i < circles.size(); ++i) {
            QRectF circle = circles.at(i);
            QColor color = colors.at(i);
            stream << "<circle cx=\"" << circle.x() + circle.width() / 2 << "\" cy=\"" << circle.y() + circle.height() / 2 << "\" r=\"" << circle.width() / 2 << "\" fill=\"#" << color.toRgb().name().mid(1) << "\" />\n";
        }

        stream << "</g>\n";
        stream << "</svg>\n";
        file.close();
        return true;
    } else {
        qDebug() << "Konnte Datei nicht öffnen: " << fileName;
        return false;
    }
}

#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <QImage>
#include <QString>
#include <QVector>
#include <QRectF>
#include <QColor>

class Rasterizer
{
public:
    Rasterizer();
    ~Rasterizer();

    bool rasterize(const QImage& originalImage,
                   double outputWidthMM,
                   double outputHeightMM,
                   int dpi,
                   double rasterSizeMM,
                   int xStartOriginal,
                   int yStartOriginal,
                   int selectedWidthOriginal,
                   int selectedHeightOriginal,
                   const QString& outputFileName,
                   bool useMedian,
                   int scalingMode = 1,
                   double coverageFactor = 1.0); // Standardwert für scalingMode = 1 (Linear)

private:
    // Private Methoden
    QColor calculateAverageColor(const QImage& image, int x, int y, double width, double height);
    QColor calculateMedianColor(const QImage& image, int x, int y, double width, double height);
    bool writeSvgToFile(const QString& fileName, int widthPx, int heightPx, const QVector<QRectF>& circles, const QVector<QColor>& colors);
    double MM_PER_INCH = 25.4;
};

#endif // RASTERIZER_H

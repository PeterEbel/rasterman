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
                               const QString& outputFileName = "output.svg",
                               int xStartOriginal = 0,
                               int yStartOriginal = 0,
                               double outputWidthMM = 512.0,
                               double outputHeightMM = 1024.0,
                               double maxCircleSizeMM = 10.0,
                               int dpi = 100,
                               bool useMedian = false,
                               int scalingMode = 1,
                               double coverageFactor = 1.0,
                               bool useGrayscale = false,
                               bool useBlackCircles = false,
                               double gamma = 0.5);

private:
    QColor calculateAverageColor(const QImage& image, int x, int y, double width, double height, bool useGrayscale);
    QColor calculateMedianColor(const QImage& image, int x, int y, double width, double height, bool useGrayscale);
    bool writeSvgToFile(const QString& fileName, int widthPx, int heightPx, const QVector<QRectF>& circles, const QVector<QColor>& colors);
    double MM_PER_INCH = 25.4;
};

#endif // RASTERIZER_H

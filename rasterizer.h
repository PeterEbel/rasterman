// Rasterman Standalone Version 1.0
// Copyright (C) 2025 Peter Ebel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <QImage>
#include <QString>
#include <QVector>
#include <QRectF>
#include <QColor>
#include <QObject> // for signals and slots

class Rasterizer : public QObject
{
    Q_OBJECT // Macro for signals and slots
    Q_PROPERTY(double gamma READ gamma WRITE setGamma)

public:
    Q_INVOKABLE Rasterizer(QObject *parent = nullptr);
    ~Rasterizer() override;

    double gamma() const { return m_gamma; }
    void setGamma(double gamma) { m_gamma = gamma; }

    Q_SIGNAL void rasterizationFinished(bool success); // add signal

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
                   bool useBlackCircles = false);

private:
    QColor calculateAverageColor(const QImage& image, int x, int y, double width, double height, bool useGrayscale);
    QColor calculateMedianColor(const QImage& image, int x, int y, double width, double height, bool useGrayscale);
    bool writeSvgToFile(const QString& fileName, int widthPx, int heightPx, const QVector<QRectF>& circles, const QVector<QColor>& colors);
    double MM_PER_INCH = 25.4;
    double m_gamma = 1.0;
};

#endif // RASTERIZER_H

/****************************************************************************************
 * Program:     rasterman
 * Description: Converts bitmap images into scalable rasterized vector graphics
 * Author:      Peter Ebel, peter.ebel@outlook.de
 * Date:        2025-01-12
 * Uses:        Qt 6.8.1
 * Execution:   ./rasterman input-file -h <help>
 *              ./rasterman input-file -s <scaling mode>
 * Change History
 * Version    By         Date         Change
 * 1.0        Ebel       20025-01-12  initial version
 ****************************************************************************************/

#include "rasterizer.h"
#include <QApplication>
#include <QImage>
#include <QDebug>
#include <QString>
#include <QCoreApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Rasterman");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Converts a bitmap file into a rasterized, scalable vector graphic.");
    parser.addHelpOption();
    parser.addVersionOption();

    // Optionen definieren
    parser.addPositionalArgument("input", "Input file");

    QCommandLineOption outputFileNameOption(QStringList() << "o" << "output", "Name of output file", "output.svg");
    parser.addOption(outputFileNameOption);
    QCommandLineOption outputWidthOption(QStringList() << "w" << "width", "Output width in mm", "768");
    parser.addOption(outputWidthOption);
    QCommandLineOption outputHeightOption(QStringList() << "t" << "height", "Output height in mm", "1024");
    parser.addOption(outputHeightOption);
    QCommandLineOption maxCircleSizeOption(QStringList() << "m" << "maxcirclesize", "Circle diameter in mm.", "4");
    parser.addOption(maxCircleSizeOption);
    QCommandLineOption scalingModeOption(QStringList() << "s" << "scaling", "Scaling mode (0: Constant, 1: Linear, 2: Logarithmic, 3: Square root).", "1");
    parser.addOption(scalingModeOption);
    QCommandLineOption colorCalculationOption(QStringList() << "c" << "colorcalc", "Color calculatio method (average or median).", "average");
    parser.addOption(colorCalculationOption);
    QCommandLineOption outputDpiOption(QStringList() << "d" << "dpi", "Dots per inch", "300");
    parser.addOption(outputDpiOption);
    QCommandLineOption coverageFactorOption(QStringList() << "f" << "coverage", "Cornwer coverage factor (0.0 to 2.0).", "1.0");
    parser.addOption(coverageFactorOption);
    QCommandLineOption grayscaleOption(QStringList() << "g" << "grayscale", "Convert colors to grayscale");
    parser.addOption(grayscaleOption);
    QCommandLineOption gammaOption(QStringList() << "y" << "gamma", "Adapt luminance", "0.5");
    parser.addOption(gammaOption);


    parser.process(a);

    QStringList positionalArguments = parser.positionalArguments();
    if (positionalArguments.isEmpty()) {
        qDebug() << "Input file missing.";
        return 1;
    }
    QString inputFileName = positionalArguments.first();
    QString outputFileName = parser.value(outputFileNameOption);
    double outputWidthMM = parser.value(outputWidthOption).toDouble();
    double outputHeightMM = parser.value(outputHeightOption).toDouble();
    double maxCircleSizeMM = parser.value(maxCircleSizeOption).toDouble();
    int scalingMode = parser.value(scalingModeOption).toInt();
    QString colorCalculationMethod = parser.value(colorCalculationOption).toLower();
    bool useMedian = (colorCalculationMethod == "median");
    int outputDpi = parser.value(outputDpiOption).toInt();
    double coverageFactor = parser.value(coverageFactorOption).toDouble();
    bool useGrayscale = parser.isSet(grayscaleOption);
    double gamma = parser.value(gammaOption).toDouble();

    // Load image
    QImage image(inputFileName);
    if (image.isNull()) {
        qDebug() << "Couldn't load input file: " << inputFileName;
        return 1;
    }

    // Instantiate and execute
    Rasterizer rasterizer;
    if (rasterizer.rasterize(image, outputFileName, 0, 0, outputWidthMM, outputHeightMM, maxCircleSizeMM, outputDpi, useMedian, scalingMode, coverageFactor, useGrayscale, gamma)) {
        qDebug() << "SVG file successfully created: " << outputFileName;
    } else {
        qDebug() << "Error creating SVG file.";
    }

    return 0;
}

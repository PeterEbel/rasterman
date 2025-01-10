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
    parser.setApplicationDescription("Erzeugt ein SVG-Rasterbild aus einem Bild.");
    parser.addHelpOption();
    parser.addVersionOption();

    // Optionen definieren
    QCommandLineOption scalingModeOption(QStringList() << "s" << "scaling", "Skalierungsmodus (0: Konstant, 1: Linear, 2: Logarithmisch, 3: Quadratwurzel).", "1");
    parser.addOption(scalingModeOption);

    QCommandLineOption grayscaleOption(QStringList() << "g" << "grayscale", "Erzeugt das SVG in Graustufen.");
    parser.addOption(grayscaleOption);

    QCommandLineOption maxCircleSizeOption(QStringList() << "m" << "maxcirclesize", "Maximale Kreisgröße in mm.", "4");
    parser.addOption(maxCircleSizeOption);

    QCommandLineOption coverageFactorOption(QStringList() << "f" << "coverage", "Abdeckungsfaktor der Kreise (0.0 bis 1.0).", "1.0");
    parser.addOption(coverageFactorOption);

    QCommandLineOption colorCalculationOption(QStringList() << "c" << "colorcalc", "FarbBerechnungsmethode (average oder median).", "average");
    parser.addOption(colorCalculationOption);

    QCommandLineOption gammaOption(QStringList() << "y" << "gamma", "Gamma-Wert für die Helligkeitsanpassung.", "0.5");
    parser.addOption(gammaOption);

    QCommandLineOption outputDpiOption(QStringList() << "d" << "dpi", "Dots per inch", "300");
    parser.addOption(outputDpiOption);

    QCommandLineOption outputWidthOption(QStringList() << "w" << "width", "Breite der SVG-Datei in mm.", "768");
    parser.addOption(outputWidthOption);

    QCommandLineOption outputHeightOption(QStringList() << "t" << "height", "Höhe der SVG-Datei in mm.", "1024");
    parser.addOption(outputHeightOption);

    QCommandLineOption outputFileNameOption(QStringList() << "o" << "output", "Name der Ausgabedatei.", "output.svg");
    parser.addOption(outputFileNameOption);

    // Bilddatei als Argument
    parser.addPositionalArgument("input", "Eingabebilddatei.");

    parser.process(a);

    // Werte auslesen und überprüfen (mit Standardwerten)
    bool useGrayscale = parser.isSet(grayscaleOption);
    int scalingMode = parser.value(scalingModeOption).toInt();
    double gamma = parser.value(gammaOption).toDouble();
    double maxCircleSizeMM = parser.value(maxCircleSizeOption).toDouble();
    QString colorCalculationMethod = parser.value(colorCalculationOption).toLower();
    bool useMedian = (colorCalculationMethod == "median");
    int outputDpi = parser.value(outputDpiOption).toInt();
    double outputWidthMM = parser.value(outputWidthOption).toDouble();
    double outputHeightMM = parser.value(outputHeightOption).toDouble();
    QString outputFileName = parser.value(outputFileNameOption);
    double coverageFactor = parser.value(coverageFactorOption).toDouble();

    QStringList positionalArguments = parser.positionalArguments();
    if (positionalArguments.isEmpty()) {
        qDebug() << "Keine Eingabedatei angegeben.";
        return 1;
    }
    QString inputFileName = positionalArguments.first();

    // Bild laden
    QImage image(inputFileName);
    if (image.isNull()) {
        qDebug() << "Konnte Bilddatei nicht laden: " << inputFileName;
        return 1;
    }

    // Rasterizer instanziieren und ausführen
    Rasterizer rasterizer;
    if (rasterizer.rasterize(image, outputWidthMM, outputHeightMM, outputDpi, scalingMode, 0, 0, outputWidthMM, outputHeightMM, outputFileName, useMedian, scalingMode, coverageFactor, useGrayscale, gamma, maxCircleSizeMM)) {
        qDebug() << "SVG erfolgreich erstellt: " << outputFileName;
    } else {
        qDebug() << "Fehler beim Erstellen des SVGs.";
    }

    return 0;
}

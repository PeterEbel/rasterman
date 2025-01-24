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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtConcurrent>
#include <QFuture>
#include <QDebug>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    centerWindow();
    connect(ui->spbDpi, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::promoteChanges);
    connect(ui->spbGamma, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::promoteChanges);
    connect(ui->spbCoverage, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::promoteChanges);
    connect(ui->spbCircleSize, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::promoteChanges);
    connect(ui->cobScalingMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::promoteChanges);
    connect(ui->cobColorCalcMethod, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::promoteChanges);
    connect(ui->chbGrayscale, &QCheckBox::checkStateChanged, this, &MainWindow::promoteChanges);
    connect(ui->chbBlackCircles, &QCheckBox::checkStateChanged, this, &MainWindow::promoteChanges);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::centerWindow()
{
    // Obtain screen geometry
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // Obtain window geometry
    QRect windowGeometry = this->frameGeometry();

    // Calculate window position
    int x = (screenGeometry.width() - windowGeometry.width()) / 2;
    int y = (screenGeometry.height() - windowGeometry.height()) / 2;

    // move window
    this->move(x, y);
}

void MainWindow::on_btnSelect_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!fileName.isEmpty()) {
        ui->ledImagePath->setText(fileName);
        // set output filename
        QFileInfo fileInfo(fileName);
        int lastPoint = fileName.lastIndexOf(".");
        QString fileNameNoExt = fileName.left(lastPoint);
        ui->ledOutputFilename->setText( fileNameNoExt + ".svg");
    }
}

void MainWindow::on_btnSaveSvg_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Store SVG", ui->ledOutputFilename->text(), "SVG (*.svg)");
    if (!fileName.isEmpty()) {
        ui->ledOutputFilename->setText(fileName);
        outputFilename = fileName;
    }
}

// void MainWindow::on_btnPreview_clicked()
// {
//     ui->textEdit->appendPlainText("Loading image");
//     ui->svgPreview->load(ui->ledOutputFilename->text());
// }

void MainWindow::on_btnRasterize_clicked()
{

   // Read and store values from GUI elements in local variables
    QString inputImagePath = ui->ledImagePath->text();
    QString outputFilename = ui->ledOutputFilename->text(); // output filename
    if (inputImagePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select image first.");
        return;
    }

    QImage image(inputImagePath);
    if (image.isNull()) {
        QMessageBox::critical(this, "Error", "Could not load image.");
        return;
    }

    double outputWidthMM = ui->spbOutputWidth->value();
    double outputHeightMM = ui->spbOutputHeight->value();
    double maxCircleSizeMM = ui->spbCircleSize->value();
    int dpi = ui->spbDpi->value();
    bool useMedian = ui->cobColorCalcMethod->currentIndex() == 1;
    int scalingMode = ui->cobScalingMode->currentIndex();
    double coverageFactor = ui->spbCoverage->value();
    double gamma = ui->spbGamma->value();
    bool useGrayscale = ui->chbGrayscale->isChecked();
    bool useBlackCircles = ui->chbBlackCircles->isChecked();

    ui->txeStatus->appendPlainText("Rasterization started...");

    QElapsedTimer timer;
    timer.start();

    bool success = rasterizer.rasterize(image, outputFilename, 0, 0, outputWidthMM, outputHeightMM, maxCircleSizeMM, dpi, useMedian, scalingMode, coverageFactor, gamma, useGrayscale, useBlackCircles);

    if (success) {
        ui->txeStatus->appendPlainText("Rasterization finished!");
        ui->svgPreview->load(outputFilename);
    } else {
        ui->txeStatus->appendPlainText("Error during rasterization!");
    }

    int elapsedMs = timer.elapsed();
    ui->txeStatus->appendPlainText(QString("Zeit: %1 ms").arg(elapsedMs));

    // Multi-threading will be added later
    // QtConcurrent::run([this, image, outputFilename, outputWidthMM, outputHeightMM, maxCircleSizeMM, dpi, useMedian, scalingMode, coverageFactor, gamma, useGrayscale, useBlackCircles]() {
    //     bool success = rasterizer.rasterize(image, outputFilename, 0, 0, outputWidthMM, outputHeightMM, maxCircleSizeMM, dpi, useMedian, scalingMode, coverageFactor, gamma, useGrayscale, useBlackCircles);
    //     QMetaObject::invokeMethod(this, [this, success]() {
    //         onRasterizationFinished(success);
    //     });
    // });
}

void MainWindow::promoteChanges()
{

}


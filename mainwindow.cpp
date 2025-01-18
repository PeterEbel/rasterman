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
    connect(ui->sbDpi, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updatePreview);
    connect(ui->gammaDoubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onGammaChanged); //Neue Verbindung
    connect(ui->coverageDoubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatePreview);
    connect(ui->maxCircleSizeDoubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updatePreview);
    connect(ui->scalingModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updatePreview);
    connect(ui->colorCalcComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updatePreview);
    connect(ui->grayscaleCheckBox, &QCheckBox::checkStateChanged, this, &MainWindow::updatePreview);
    connect(ui->blackCirclesCheckBox, &QCheckBox::checkStateChanged, this, &MainWindow::updatePreview);
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

void MainWindow::on_selectImageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!fileName.isEmpty()) {
        ui->imagePathLineEdit->setText(fileName);
        // set output filename
        QFileInfo fileInfo(fileName);
        ui->outputFilenameLineEdit->setText(fileInfo.baseName() + ".svg");
    }
}

void MainWindow::on_saveSvgButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Store SVG", ui->outputFilenameLineEdit->text(), "SVG (*.svg)");
    if (!fileName.isEmpty()) {
        ui->outputFilenameLineEdit->setText(fileName);
        outputFilename = fileName;
    }
}

void MainWindow::on_rasterizeButton_clicked()
{

   // Read and store values from GUI elements in local variables
    QString inputImagePath = ui->imagePathLineEdit->text();
    QString outputFilename = ui->outputFilenameLineEdit->text(); // output filename
    if (inputImagePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select image first.");
        return;
    }

    QImage image(inputImagePath);
    if (image.isNull()) {
        QMessageBox::critical(this, "Error", "Could not load image.");
        return;
    }

    double outputWidthMM = ui->outputWidthDoubleSpinBox->value();
    double outputHeightMM = ui->outputHeightDoubleSpinBox->value();
    double maxCircleSizeMM = ui->maxCircleSizeDoubleSpinBox->value();
    int dpi = ui->sbDpi->value();
    bool useMedian = ui->colorCalcComboBox->currentIndex() == 1;
    int scalingMode = ui->scalingModeComboBox->currentIndex();
    double coverageFactor = ui->coverageDoubleSpinBox->value();
    bool useGrayscale = ui->grayscaleCheckBox->isChecked();
    bool useBlackCircles = ui->blackCirclesCheckBox->isChecked();
    // double gamma = ui->gammaDoubleSpinBox->value();

    ui->statusText->appendPlainText("Rasterization started...");

    QElapsedTimer timer;
    timer.start();

    bool success = rasterizer.rasterize(image, outputFilename, 0, 0, outputWidthMM, outputHeightMM, maxCircleSizeMM, dpi, useMedian, scalingMode, coverageFactor, useGrayscale, useBlackCircles);

    if (success) {
        ui->statusText->appendPlainText("Rasterization finished!");
        ui->svgPreview->load(outputFilename); // show SVG
    } else {
        ui->statusText->appendPlainText("Error during rasterization!");
    }

    int elapsedMs = timer.elapsed();
    ui->statusText->appendPlainText(QString("Zeit: %1 ms").arg(elapsedMs));

    // QtConcurrent::run([this, image, outputFilename, outputWidthMM, outputHeightMM, maxCircleSizeMM, dpi, useMedian, scalingMode, coverageFactor, useGrayscale, useBlackCircles]() {
    //     bool success = rasterizer.rasterize(image, outputFilename, 0, 0, outputWidthMM, outputHeightMM, maxCircleSizeMM, dpi, useMedian, scalingMode, coverageFactor, useGrayscale, useBlackCircles);
    //     QMetaObject::invokeMethod(this, [this, success]() {
    //         onRasterizationFinished(success);
    //     });
    // });
}

void MainWindow::onRasterizationFinished(bool success)
{

    if (success) {
        ui->statusText->appendPlainText("Rasterization finished!");
        ui->svgPreview->load(ui->outputFilenameLineEdit->text()); // show SVG
    } else {
        ui->statusText->appendPlainText("Error during rasterization!");
    }

    if (success) {
        ui->statusText->appendPlainText("Rasterization finished.");
    }
}

void MainWindow::updatePreview()
{
    //if(ui->imagePathLineEdit->text().isEmpty()) return;
    //on_rasterizeButton_clicked();
}

void MainWindow::onGammaChanged(double gamma)
{
    rasterizer.setGamma(gamma);
    updatePreview();
}

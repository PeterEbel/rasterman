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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rasterizer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectImageButton_clicked();
    void on_saveSvgButton_clicked();
    void on_rasterizeButton_clicked();
    void onRasterizationFinished(bool success);
    void updatePreview();
    void onGammaChanged(double gamma);

private:
    Ui::MainWindow *ui;
    Rasterizer rasterizer;
    QString outputFilename;
    void centerWindow();
};
#endif // MAINWINDOW_H

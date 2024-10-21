#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <complex>
#include <cmath>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class bodediagramm{
private:
    std::complex<float> constantK;
    std::vector<std::complex<float>> coeffNumerator = std::vector<std::complex<float>>(10);
    std::vector<std::complex<float>> coeffDenominator = std::vector<std::complex<float>>(10);
    float maxFrequency = 0;
    float minFrequency = 0;
    float AmplitudeReserve = 0;
    bool checkAmplitudeReserve = false;
    float PhaseReserve = 0;
    bool checkPhaseReserve = false;

    void calculateAmplitudeReserve();
    void calculatePhaseReserve();

public:
    bodediagramm(std::complex<float> constantK, std::vector<std::complex<float>>& coeffNumerator, std::vector<std::complex<float>>& coeffDenominator, float maxFreq, float minFreq);
    ~bodediagramm();
    std::complex<float> Auswertung(float frequency);
    float calculatePhase(float frequency);
    float getMaxFrequency();
    float getMinFrequency();
    float getPhaseReserve();
    bool getCheckAmplitudeReserve();
    float getAmplitudeReserve();
    bool getCheckPhaseReserve();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButtonAddNullstelle_clicked();

    void on_pushButtonRemoveNullstelle_clicked();

    void on_pushButtonAddPolstelle_clicked();

    void on_pushButtonRemovePolstelle_clicked();

    void on_pushButtonDraw_clicked();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <complex>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

bodediagramm::bodediagramm(std::complex<float> constantK,std::vector<std::complex<float>> *coeffNumerator, std::vector<std::complex<float>> *coeffDenominator, float maxFreq, float minFreq){
    this->maxFrequency = maxFreq;
    this->minFrequency = minFreq;
    this->constantK = constantK;
    this->coeffNumerator = coeffNumerator;
    this->coeffNumerator->shrink_to_fit();
    this->coeffDenominator = coeffDenominator;
    this->coeffDenominator->shrink_to_fit();
}

std::complex<float> bodediagramm::Auswertung(float Frequency){
    /** @brief Calculates the Transfer function given a Frequency.
     *  Input; float frequency
     *  Output: std::complex<float> Evaluation
     *  TODO Beschreibung fertig machen
     *
     *
    **/
    std::complex<float> omega(0, Frequency*2*M_PI);
    std::complex<float> numerator(1, 0);
    std::complex<float> denominator(1, 0);
    for(unsigned long long i = 0; i<coeffNumerator->size(); i++){
        numerator = numerator * (omega - coeffNumerator->at(i));
    }
    for (unsigned long long i = 0; i<coeffDenominator->size(); i++){
        denominator = denominator * (omega - coeffDenominator->at(i));
    }
    return constantK * (numerator/denominator);
}

float bodediagramm::calculatePhase(float Frequency){
    /** @brief Calculates the Phase of the Transfer function given a Frequency. Returns the Frequency in degree.
     * TODO Beschreibung fertig machen
     *
    **/
    float phase = 180 * arg(constantK) / M_PI; // Umwandlung von Rad in Grad
    std::complex<float> omega(0, Frequency*2*M_PI);
    for (unsigned long long i = 0; i<coeffNumerator->size(); i++){
        phase += 180 * arg(omega - coeffNumerator->at(i)) / M_PI ;
    }
    for (unsigned long long i = 0; i<coeffDenominator->size(); i++){
        phase -= 180 * arg(omega - coeffDenominator->at(i)) / M_PI;
    }
    return phase;
}

void bodediagramm::calculatePhaseReserve(){
    float lowerPoint = minFrequency;
    float middlePoint = (minFrequency+maxFrequency)/2;
    float higherPoint = maxFrequency;
    float frequencyDifferenz = higherPoint - lowerPoint;
    if (abs(Auswertung(minFrequency)) == 1){                                                // Case 1
        this->PhaseReserve = calculatePhase(minFrequency);
    } else if (abs(Auswertung(maxFrequency)) == 1){                                         // Case 2
        this->PhaseReserve = calculatePhase(maxFrequency);
    } else if (abs(Auswertung(minFrequency)) > 1 && abs(Auswertung(maxFrequency)) < 1){     // Case 3
//Intervallhalbierungverfahren wird angewandt. Dazu wird überprüft, ob die Amplitude den Wert Null zwischen der unteren und der mittleren Frequenz durchsticht
//Falls dies nicht zutrifft wird angenommen der Wert 1 der Amplitude liegt zwischen der mittleren und oberen Frequenz.
//Spezialfall falls die mittlere Frequenz auf die 1 trifft
        while (frequencyDifferenz > 0.1){
            if (abs(Auswertung(middlePoint))== 1){
                this->PhaseReserve = 180 + (calculatePhase(middlePoint));
                this->checkPhaseReserve = true;
                return;
            }
            else if (abs(Auswertung(lowerPoint)) > 1 && abs(Auswertung(middlePoint)) < 1){
                higherPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
                frequencyDifferenz = higherPoint - lowerPoint;
            }
            else {
                lowerPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
                frequencyDifferenz = higherPoint - lowerPoint;
            }
        }
        if (abs(Auswertung(middlePoint)) < 0.1){
            this->PhaseReserve = 180 + (calculatePhase(middlePoint));
            this->checkPhaseReserve = true;
            return;
        }
    } else if (abs(Auswertung(minFrequency)) < 1 && abs(Auswertung(maxFrequency)) > 1){     // Case 4
//Intevallhalbierungsverfahren wird angewandt. Fast gleiche Vorgehensweise wie in der else if Anweisung zuvor
        while (frequencyDifferenz > 0.1){
            if (abs(Auswertung(middlePoint))== 1){
                this->PhaseReserve = 180 + (calculatePhase(middlePoint));
                this->checkPhaseReserve = true;
                return;
            }
            else if (abs(Auswertung(lowerPoint)) < 1 && abs(Auswertung(middlePoint)) > 1){
                higherPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
                frequencyDifferenz = higherPoint - lowerPoint;
            }
            else {
                lowerPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
                frequencyDifferenz = higherPoint - lowerPoint;
            }
        }
        if (abs(Auswertung(middlePoint)) < 0.1){
            this->PhaseReserve = 180 + (calculatePhase(middlePoint));
            this->checkPhaseReserve = true;
            return;
        }
    }

};

float calculateAmplitudeReserve(){
    return 0.0;
};

MainWindow::~MainWindow()
{
    delete ui;
}

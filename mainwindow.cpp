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

bodediagramm::bodediagramm(std::complex<float> constantK, std::vector<std::complex<float>> *coeffNumerator, std::vector<std::complex<float>> *coeffDenominator, float maxFreq, float minFreq){
    this->maxFrequency = maxFreq;
    this->minFrequency = minFreq;
    this->constantK = constantK;
    this->coeffNumerator = coeffNumerator;
    this->coeffNumerator->shrink_to_fit();
    this->coeffDenominator = coeffDenominator;
    this->coeffDenominator->shrink_to_fit();
    calculateAmplitudeReserve();
    calculatePhaseReserve();
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
    for(unsigned long long i = 0; i<coeffNumerator->size(); i++){// Der Zähler wird durch das Produkt aller Nullstellenkomponenten erstellt.
        numerator = numerator * (omega - coeffNumerator->at(i));
    }
    for (unsigned long long i = 0; i<coeffDenominator->size(); i++){// Der Nenner wird durch das Produkt aller Polstellenkomponenten erstellt.
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
    if (abs(Auswertung(minFrequency)) == 1){
        this->PhaseReserve = 180 + calculatePhase(minFrequency);
        this->checkPhaseReserve = true;
        return;
    }
    else if (abs(Auswertung(maxFrequency)) == 1){
        this->PhaseReserve = 180 + calculatePhase(maxFrequency);
        this->checkPhaseReserve = true;
        return;
    }
    else if (abs(Auswertung(minFrequency)) > 1 && abs(Auswertung(maxFrequency)) > 1){
        return; // Keine Schnittstelle.
    }
    else if (abs(Auswertung(minFrequency)) < 1 && abs(Auswertung(maxFrequency)) < 1){
        return; // Keine Schnittstelle.
    }
    else if (abs(Auswertung(minFrequency)) > 1 && abs(Auswertung(maxFrequency)) < 1){
        while (frequencyDifferenz > 0.1){                           // TODO Die minimale Frequenz Differenz einstellen
            if (abs(Auswertung(middlePoint))== 1){
                this->PhaseReserve = 180 + (calculatePhase(middlePoint));
                this->checkPhaseReserve = true;
                return;
            }
            else if (abs(Auswertung(middlePoint)) < 1){
                higherPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
            }
            else {
                lowerPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
            }
            frequencyDifferenz = higherPoint - lowerPoint;
        }
    }
    else if (abs(Auswertung(minFrequency)) < 1 && abs(Auswertung(maxFrequency)) > 1){
        while (frequencyDifferenz > 0.1){
            if (abs(Auswertung(middlePoint))== 1){
                this->PhaseReserve = 180 + (calculatePhase(middlePoint));
                this->checkPhaseReserve = true;
                return;
            }
            else if (abs(Auswertung(middlePoint)) > 1){
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
    this->PhaseReserve = 180 +(calculatePhase(middlePoint));
    this->checkPhaseReserve = true;
    return;
};

void bodediagramm::calculateAmplitudeReserve(){
    /** @brief
     *
     *
     *
     *
    **/
    float lowerPoint = minFrequency;
    float higherPoint = maxFrequency;
    float middlePoint = (lowerPoint + higherPoint)/2;
    float frequencyDifferenz = higherPoint - lowerPoint;
    if (calculatePhase(lowerPoint) == -180){
        this->AmplitudeReserve = -log10(abs(Auswertung(lowerPoint)));
        this->checkAmplitudeReserve = true;
    }
    else if (calculatePhase(higherPoint) == -180){
        this->AmplitudeReserve = -log10(abs(Auswertung(higherPoint)));
        this->checkAmplitudeReserve = true;
    }
    else if(calculatePhase(lowerPoint) > -180 && calculatePhase(higherPoint) > -180){
        return; // Wird angenommen, dass keine Schnittstelle existiert, da beide Grenzwert über -180° liegen.
    }
    else if(calculatePhase(lowerPoint) < -180 && calculatePhase(higherPoint) < -180){
        return; // Wird angenommen, dass keine Schnittstelle existiert, da beide Grenzwerte unter -180° liegen.
    }
    else if(calculatePhase(lowerPoint) > -180 && calculatePhase(higherPoint) < -180){//Zwischen Beiden Punkten liegt eine Schnittstelle mit -180°
        while (frequencyDifferenz > 0.1){   // TODO Minial Wert setzen
            if (calculatePhase(middlePoint) == -180){
                //this->AmplitudeReserve = -log10(abs(Auswertung(middlePoint)));
                //this->checkAmplitudeReserve = true;
                break;
            }
            else if (calculatePhase(middlePoint) > -180){
                lowerPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
            }
            else if (calculatePhase(middlePoint) < -180){
                higherPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
            }
            frequencyDifferenz = higherPoint - lowerPoint;
        }
    }
    else if(calculatePhase(lowerPoint) < -180 && calculatePhase(higherPoint) > -180){
        while (frequencyDifferenz > 0.1) {
            if (calculatePhase(middlePoint) == -180){
                //this->AmplitudeReserve = -log10(abs(Auswertung(middlePoint))); // TODO Minus überprüfen
                //this->checkAmplitudeReserve = true;
                break;
            }
            else if (calculatePhase(middlePoint) > -180){
                higherPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
            }
            else if (calculatePhase(middlePoint) < -180){
                lowerPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
            }
            frequencyDifferenz = higherPoint - lowerPoint;
        }
    }
    this->AmplitudeReserve = -log10(abs(Auswertung(middlePoint)));
    this->checkAmplitudeReserve = true;
    return;
};

float bodediagramm::getAmplitudeReserve(){
    return AmplitudeReserve;
}

bool bodediagramm::getCheckAmplitudeReserve(){
    return checkAmplitudeReserve;
}
float bodediagramm::getPhaseReserve(){
    return PhaseReserve;
}

bool bodediagramm::getCheckPhaseReserve(){
    return checkPhaseReserve;
}
MainWindow::~MainWindow()
{
    delete ui;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <complex>
#include <cmath>

#include <QDebug>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPixmap>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //Spaltern des Table Widget setzen
    ui->tableWidgetNullstellen->setColumnWidth(0, 130);
    ui->tableWidgetNullstellen->setColumnWidth(1, 130);
    ui->tableWidgetPolstellen->setColumnWidth(0, 130);
    ui->tableWidgetPolstellen->setColumnWidth(1, 130);

    //Minimale Größe beider tableWidgets setzen
    ui->tableWidgetNullstellen->setMinimumSize(280, 200);
    ui->tableWidgetPolstellen->setMinimumSize(280, 200);


    //Minimale Größe von DoubleSpinBox, Label der Konstante K
    ui->doubleSpinBoxKonstantKReal->setMinimumSize(125,25);
    ui->doubleSpinBoxKonstantKImag->setMinimumSize(125,25);
    ui->labelKonstantK->setMaximumHeight(25);
    ui->doubleSpinBoxKonstantKReal->setRange(-1000,1000);
    ui->doubleSpinBoxKonstantKImag->setRange(-1000,1000);

    //Bodediagramm
    ui->graphicsView->setFixedSize(1000, 700);
    ui->toolBox->setMinimumSize(1000,760);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    auto scene= new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::white);
    ui->graphicsView->setScene(scene);
    QPen blackPen(Qt::black);
    blackPen.setWidth(1);

    QPolygon polygon;
    polygon << QPoint(-400,-250) << QPoint(-400,0) << QPoint(400,0) << QPoint(400,-250);// Oben Amplitude
    scene->addPolygon(polygon);
    QPolygon polygon2;
    polygon2 << QPoint(-400,300) << QPoint(-400,50) << QPoint(400,50) << QPoint(400,300);// Unten Phase
    scene->addPolygon(polygon2);

    float minFreqExponent = 1;  //Anfangs Werte für den Frequenzbereich
    float maxFreqExponent = 5;
    int expoDiff = maxFreqExponent - minFreqExponent;
    float step = 800/expoDiff;

    for (int horiz = 1; horiz<=expoDiff; horiz++){//x  Gestrichelte Linien im Amplituden Diagramm oben
        for (int j=0;j<25;j+=2){//y
            if (horiz < expoDiff)
                scene->addLine(horiz*step -400, -j*10, horiz*step -400,-(j+1)*10);
            scene->addLine(horiz*step -400 -step+step*log10(2), -j*10, horiz*step -400 -step+step*log10(2),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(3), -j*10, horiz*step -400 -step+step*log10(3),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(4), -j*10, horiz*step -400 -step+step*log10(4),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(5), -j*10, horiz*step -400 -step+step*log10(5),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(6), -j*10, horiz*step -400 -step+step*log10(6),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(7), -j*10, horiz*step -400 -step+step*log10(7),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(8), -j*10, horiz*step -400 -step+step*log10(8),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(9), -j*10, horiz*step -400 -step+step*log10(9),-j*10-2);
        }
    }
    for (int horiz = 1; horiz<=expoDiff; horiz++){// Gestrichelte Linien im Phasen Diagramm unten
        for (int j=29;j>4;j-=2){
            if(horiz < expoDiff)
                scene->addLine( horiz*step -400, j*10, horiz*step - 400,(j+1)*10);
            scene->addLine(horiz*step -400 -step+step*log10(2), j*10, horiz*step -400 -step+step*log10(2),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(3), j*10, horiz*step -400 -step+step*log10(3),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(4), j*10, horiz*step -400 -step+step*log10(4),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(5), j*10, horiz*step -400 -step+step*log10(5),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(6), j*10, horiz*step -400 -step+step*log10(6),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(7), j*10, horiz*step -400 -step+step*log10(7),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(8), j*10, horiz*step -400 -step+step*log10(8),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(9), j*10, horiz*step -400 -step+step*log10(9),j*10+2);
        }
    }

    /**QGraphicsLineItem* lintest = new QGraphicsLineItem;
    lintest->setLine(0,0,100,100);
    scene->addItem(lintest);
    **/
    //BEGIN : Hinzufügen einer Beschriftung
    QGraphicsTextItem * headline = new QGraphicsTextItem;
    headline->setPos(-420,-300);
    headline->setPlainText("Bode Diagram");
    headline->setDefaultTextColor(Qt::black);
    headline->setScale(1.4);
    scene->addItem(headline);

    QGraphicsTextItem * xLabelAmpl = new QGraphicsTextItem;
    xLabelAmpl->setPos(-475,-75);
    xLabelAmpl->setPlainText("Magnitude (dB)");
    xLabelAmpl->setDefaultTextColor(Qt::black);
    xLabelAmpl->setScale(1.3);
    xLabelAmpl->setRotation(-90);
    scene->addItem(xLabelAmpl);

    QGraphicsTextItem * xLabelDegr = new QGraphicsTextItem;
    xLabelDegr->setPos(-475, 225);
    xLabelDegr->setPlainText("Phase (deg)");
    xLabelDegr->setDefaultTextColor(Qt::black);
    xLabelDegr->setScale(1.3);
    xLabelDegr->setRotation(-90);
    scene->addItem(xLabelDegr);

    QGraphicsTextItem * xLabelFreq = new QGraphicsTextItem;
    xLabelFreq->setPos(-50, 335);
    xLabelFreq->setPlainText("Frequenz (Hz)");
    xLabelFreq->setDefaultTextColor(Qt::black);
    xLabelFreq->setScale(1.3);
    scene->addItem(xLabelFreq);
    //END: Hinzufügen einer Beschriftung

    //BEGIN: Hinzufügen der Dezibel und Degree Makierungen
    int dezibel = 40;
    int degree = 180;
    for (int var = -250; var <=0; var +=50){//-250 bis 0 in y Richtung oben
        scene->addLine(-400,var,-390,var);
        QString yLabel = QString::number(dezibel);
        QGraphicsTextItem* xLabel = new QGraphicsTextItem;
        xLabel->setPos(-440, var-10);
        xLabel->setPlainText(yLabel+" dB");
        xLabel->setDefaultTextColor(Qt::black);
        xLabel->setScale(0.9);
        scene->addItem(xLabel);
        dezibel -= 20;
    }
    for (int var = 50; var <= 300; var +=50) {//50 bis 300 in y Richtung unten
        scene->addLine(-400,var,-390,var);
        QString yLabel = QString::number(degree);
        QGraphicsTextItem* xLabel = new QGraphicsTextItem;
        xLabel->setPos(-440, var-10);
        xLabel->setPlainText(yLabel+"°");
        xLabel->setDefaultTextColor(Qt::black);
        xLabel->setScale(0.9);
        scene->addItem(xLabel);
        degree -= 90;
    }
    //END: Hinzufügen der Dezibel und Degree Makierung

    //BEGIN: Hinzufügen der Makierungen der Frequenzen und der Label
    int freqExpo = 1;
    for (int freq = -400; freq <= 400; freq +=200) {
        scene->addLine(freq, 0, freq, 10);
        scene->addLine(freq, 300, freq, 310);
        QString freqLabel = QString::number(freqExpo);
        QGraphicsTextItem* Label = new QGraphicsTextItem;
        Label->setPos(freq-10, 318);
        Label->setPlainText("10^" +freqLabel);
        Label->setDefaultTextColor(Qt::black);
        Label->setScale(0.9);
        scene->addItem(Label);
        freqExpo++;
    }
    //END: Hinzufügen der Makierungen der Frequenzen und der Label
    ui->toolBox->setCurrentIndex(0);
}


bodediagramm::bodediagramm(std::complex<float> constantK, std::vector<std::complex<float>>& coeffNumerator, std::vector<std::complex<float>>& coeffDenominator, float maxFreq, float minFreq){
    this->maxFrequency = maxFreq;
    this->minFrequency = minFreq;
    this->constantK = constantK;
    this->coeffNumerator = coeffNumerator;
    this->coeffNumerator.shrink_to_fit();
    this->coeffDenominator = coeffDenominator;
    this->coeffDenominator.shrink_to_fit();
    calculateAmplitudeReserve();
    calculatePhaseReserve();
}


std::complex<float> bodediagramm::Auswertung(float Frequency){
    /** @brief Calculates the Transfer function given a Frequency.
     *  Input; float frequency
     *  Output: std::complex<float> Evaluation
     *
     *
    **/
    std::complex<float> omega(0, Frequency*2*M_PI);
    std::complex<float> numerator(1, 0);
    std::complex<float> denominator(1, 0);
    if (coeffNumerator.size() > 0){     //Does a zero point exist
        for(int i = 0; i< (int) coeffNumerator.size(); i++){// Der Zähler wird durch das Produkt aller Nullstellenkomponenten erstellt.
            numerator = numerator * (omega - coeffNumerator.at(i));
        }
    }
    if (coeffDenominator.size() >0){    //Does a pole point exist
        for (int i = 0; i< (int) coeffDenominator.size(); i++){// Der Nenner wird durch das Produkt aller Polstellenkomponenten erstellt.
            denominator = denominator * (omega - coeffDenominator.at(i));
        }
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
    for (int i = 0; i<(int) coeffNumerator.size(); i++){
        phase += 180 * arg(omega - coeffNumerator.at(i)) / M_PI ;
    }
    for (int i = 0; i<(int) coeffDenominator.size(); i++){
        phase -= 180 * arg(omega - coeffDenominator.at(i)) / M_PI;
    }
    return phase;
}


void bodediagramm::calculatePhaseReserve(){
    float lowerPoint = minFrequency;
    float middlePoint = (minFrequency+maxFrequency)/2;
    float higherPoint = maxFrequency;
    float frequencyDifferenz = higherPoint - lowerPoint;
    if (abs(Auswertung(pow(10, minFrequency))) == 1){
        this->PhaseReserve = 180 + calculatePhase(pow(10, minFrequency));
        this->checkPhaseReserve = true;
        return;
    }
    else if (abs(Auswertung(pow(10, maxFrequency))) == 1){
        this->PhaseReserve = 180 + calculatePhase(pow(10, maxFrequency));
        this->checkPhaseReserve = true;
        return;
    }
    else if (abs(Auswertung(pow(10, minFrequency))) > 1 && abs(Auswertung(pow(10, maxFrequency))) > 1){
        return; // Keine Schnittstelle.
    }
    else if (abs(Auswertung(pow(10, minFrequency))) < 1 && abs(Auswertung(pow(10, maxFrequency))) < 1){
        return; // Keine Schnittstelle.
    }
    else if (abs(Auswertung(pow(10, minFrequency))) > 1 && abs(Auswertung(pow(10, maxFrequency))) < 1){
        while (frequencyDifferenz > 0.1){
            if (abs(Auswertung(pow(10, middlePoint)))== 1){
                this->PhaseReserve = 180 + (calculatePhase(pow(10, middlePoint)));
                this->checkPhaseReserve = true;
                return;
            }
            else if (abs(Auswertung(pow(10, middlePoint))) < 1){
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
    else if (abs(Auswertung(pow(10, minFrequency))) < 1 && abs(Auswertung(pow(10,maxFrequency))) > 1){
        while (frequencyDifferenz > 0.1){
            if (abs(Auswertung(pow(10, middlePoint)))== 1){
                this->PhaseReserve = 180 + (calculatePhase(pow(10, middlePoint)));
                this->checkPhaseReserve = true;
                return;
            }
            else if (abs(Auswertung(pow(10, middlePoint))) > 1){
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
        if (abs(Auswertung(pow(10, middlePoint))) < 0.1){
            this->PhaseReserve = 180 + (calculatePhase(pow(10, middlePoint)));
            this->checkPhaseReserve = true;
            return;
        }
    }
    this->PhaseReserve = 180 +(calculatePhase(pow(10, middlePoint)));
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
    if (calculatePhase(pow(10, lowerPoint)) == -180){
        this->AmplitudeReserve = -log10(abs(Auswertung(lowerPoint)));
        this->checkAmplitudeReserve = true;
    }
    else if (calculatePhase(pow(10,higherPoint)) == -180){
        this->AmplitudeReserve = -log10(abs(Auswertung(higherPoint)));
        this->checkAmplitudeReserve = true;
    }
    else if(calculatePhase(pow(10, lowerPoint)) > -180 && calculatePhase(pow(10, higherPoint)) > -180){
        return; // Wird angenommen, dass keine Schnittstelle existiert, da beide Grenzwert über -180° liegen.
    }
    else if(calculatePhase(pow(10, lowerPoint)) < -180 && calculatePhase(pow(10, higherPoint)) < -180){
        return; // Wird angenommen, dass keine Schnittstelle existiert, da beide Grenzwerte unter -180° liegen.
    }
    else if(calculatePhase(pow(10, lowerPoint)) > -180 && calculatePhase(pow(10, higherPoint)) < -180){//Zwischen Beiden Punkten liegt eine Schnittstelle mit -180°
        while (frequencyDifferenz > 0.1){
            if (calculatePhase(pow(10, middlePoint)) == -180){
                this->AmplitudeReserve = -20*log10(abs(Auswertung(pow(10, middlePoint))));
                this->checkAmplitudeReserve = true;
                break;
            }
            else if (calculatePhase(pow(10, middlePoint)) > -180){
                lowerPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
            }
            else if (calculatePhase(pow(10, middlePoint)) < -180){
                higherPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
            }
            frequencyDifferenz = higherPoint - lowerPoint;
        }
    }
    else if(calculatePhase(pow(10, lowerPoint)) < -180 && calculatePhase(pow(10, higherPoint)) > -180){
        while (frequencyDifferenz > 0.1) {
            if (calculatePhase(pow(10, middlePoint)) == -180){
                this->AmplitudeReserve = -20*log10(abs(Auswertung(pow(10, middlePoint))));
                this->checkAmplitudeReserve = true;
                break;
            }
            else if (calculatePhase(pow(10, middlePoint)) > -180){
                higherPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
            }
            else if (calculatePhase(pow(10, middlePoint)) < -180){
                lowerPoint = middlePoint;
                middlePoint = (lowerPoint + higherPoint)/2;
            }
            frequencyDifferenz = higherPoint - lowerPoint;
        }
    }
    this->AmplitudeReserve = -20*log10(abs(Auswertung(pow(10, middlePoint))));
    this->checkAmplitudeReserve = true;
    return;
};


float bodediagramm::getMaxFrequency(){
    return maxFrequency;
}


float bodediagramm::getMinFrequency(){
    return minFrequency;
}


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


void MainWindow::on_pushButtonAddNullstelle_clicked()
{
    /** @brief Adds a Row to the table Widget, which contains zero points
    **/

    int rowCount = ui->tableWidgetNullstellen->rowCount();
    ui->tableWidgetNullstellen->setRowCount(rowCount + 1);
    QDoubleSpinBox* realPart = new QDoubleSpinBox;
    QDoubleSpinBox* imgPart = new QDoubleSpinBox;
    realPart->setRange(-100,100);
    realPart->setDecimals(4);
    imgPart->setRange(-100,100);
    imgPart->setDecimals(4);
    ui->tableWidgetNullstellen->setCellWidget(rowCount, 0, realPart);
    ui->tableWidgetNullstellen->setCellWidget(rowCount, 1, imgPart);

    // The number of zero points must be smaller or equal to the number of poles
    if (ui->tableWidgetPolstellen->rowCount() >= ui->tableWidgetNullstellen->rowCount())
        ui->labelErrorPolNullstellen->setText("");
    if (ui->tableWidgetPolstellen->rowCount() < ui->tableWidgetNullstellen->rowCount())
        ui->labelErrorPolNullstellen->setText("Es muss mindestens gleich viele Pol- wie Nullstellen geben.");
}


void MainWindow::on_pushButtonRemoveNullstelle_clicked()
{
    /** @brief Removes the selected Row
     *
    **/
    if (ui->tableWidgetNullstellen->currentRow() != -1)
        ui->tableWidgetNullstellen->removeRow(ui->tableWidgetNullstellen->currentRow());

    // The number of zero points must be smaller or equal to the number of poles
    if (ui->tableWidgetPolstellen->rowCount() < ui->tableWidgetNullstellen->rowCount())
        ui->labelErrorPolNullstellen->setText("Es muss mindestens gleich viele Pol- wie Nullstellen geben.");
    if (ui->tableWidgetPolstellen->rowCount() >= ui->tableWidgetNullstellen->rowCount())
        ui->labelErrorPolNullstellen->setText("");
}


void MainWindow::on_pushButtonAddPolstelle_clicked()
{
    int rowCount = ui->tableWidgetPolstellen->rowCount();
    ui->tableWidgetPolstellen->setRowCount(rowCount + 1);
    QDoubleSpinBox* realPart = new QDoubleSpinBox;
    QDoubleSpinBox* imgPart = new QDoubleSpinBox;
    realPart->setRange(-100,100);
    realPart->setDecimals(4);
    imgPart->setRange(-100,100);
    imgPart->setDecimals(4);
    ui->tableWidgetPolstellen->setCellWidget(rowCount, 0, realPart);
    ui->tableWidgetPolstellen->setCellWidget(rowCount, 1, imgPart);

    if (ui->tableWidgetPolstellen->rowCount() >= ui->tableWidgetNullstellen->rowCount())
        ui->labelErrorPolNullstellen->setText("");
    if (ui->tableWidgetPolstellen->rowCount() < ui->tableWidgetNullstellen->rowCount())
        ui->labelErrorPolNullstellen->setText("Es muss mindestens gleich viele Pol- wie Nullstellen geben.");
}


void MainWindow::on_pushButtonRemovePolstelle_clicked()
{
    if(ui->tableWidgetPolstellen->currentRow() !=-1)
        ui->tableWidgetPolstellen->removeRow(ui->tableWidgetPolstellen->currentRow());
    if (ui->tableWidgetPolstellen->rowCount() < ui->tableWidgetNullstellen->rowCount())
        ui->labelErrorPolNullstellen->setText("Es muss mindestens gleich viele Pol- wie Nullstellen geben.");
}


void MainWindow::on_pushButtonDraw_clicked()
{
    /** @brief Verifies the Input. Draws the Bode-Diagramm
     *
     *
     *
    **/

    if (ui->spinBoxHigherFrequency->value() == ui->spinBoxLowerFrequency->value()){
        ui->labelErrorExponent->setText("Frequenzbereich muss größer sein als Null.");
        return;
    }
    else{
        ui->labelErrorExponent->clear();
    }
    if (ui->tableWidgetNullstellen->rowCount()>ui->tableWidgetPolstellen->rowCount()){
        return;
    }




    //BEGIN Check and Read User Input

    // Überprüfung welche der Exponenten größer ist.
    float minFreqExponent = 0;
    float maxFreqExponent = 0;
    if (ui->spinBoxHigherFrequency->value() > ui->spinBoxLowerFrequency->value()){
        minFreqExponent = ui->spinBoxLowerFrequency->value();
        maxFreqExponent = ui->spinBoxHigherFrequency->value();
    }
    else{
        minFreqExponent = ui->spinBoxHigherFrequency->value();
        maxFreqExponent = ui->spinBoxLowerFrequency->value();
        int reassignMaxFreq = int (maxFreqExponent);
        int reassignMinFreq = int (minFreqExponent);
        ui->spinBoxHigherFrequency->setValue(reassignMaxFreq);
        ui->spinBoxLowerFrequency->setValue(reassignMinFreq);
    }


    std::vector<std::complex<float>> vectorNull(ui->tableWidgetNullstellen->rowCount());
    std::vector<std::complex<float>> vectorPol(ui->tableWidgetPolstellen->rowCount());
    for (int i = 0; i < ui->tableWidgetNullstellen->rowCount(); ++i) {
        QDoubleSpinBox* rP = (QDoubleSpinBox*)ui->tableWidgetNullstellen->cellWidget(i,0);
        QDoubleSpinBox* iP = (QDoubleSpinBox*)ui->tableWidgetNullstellen->cellWidget(i,1);
        vectorNull[i] = std::complex<float> (rP->value(), iP->value());
    }
    for (int i = 0; i < ui->tableWidgetPolstellen->rowCount(); ++i) {
        QDoubleSpinBox* rP = (QDoubleSpinBox*)ui->tableWidgetPolstellen->cellWidget(i,0);
        QDoubleSpinBox* iP = (QDoubleSpinBox*)ui->tableWidgetPolstellen->cellWidget(i,1);
        vectorPol[i] = std::complex<float> (rP->value(), iP->value());
    }
    //END Check and Read User Input

    //START Calculation Bode Diagramm
    std::complex<float> constantK(ui->doubleSpinBoxKonstantKReal->value(), ui->doubleSpinBoxKonstantKImag->value());
    auto transferFunction = new bodediagramm(constantK, vectorNull, vectorPol, ui->spinBoxHigherFrequency->value(), ui->spinBoxLowerFrequency->value());

    if (transferFunction->getCheckAmplitudeReserve()){
        ui->labelAmplitudenReserve->setText("Amplituden Reserve: " + QString::number(transferFunction->getAmplitudeReserve()));
    }
    else{
        ui->labelAmplitudenReserve->setText("Amplituden Reserve nicht vorhanden.");
    }
    if (transferFunction->getCheckPhaseReserve()){
        ui->labelPhasenReserve->setText("Phasen Reserve: " + QString::number(transferFunction->getPhaseReserve()));
    }
    else{
        ui->labelPhasenReserve->setText("Phasen Reserve nicht vorhanden.");
    }
    std::vector<float> amplitudeGraph(((transferFunction->getMaxFrequency()-transferFunction->getMinFrequency())) * 9 + 1, 0);
    std::vector<float> phaseGraph(((transferFunction->getMaxFrequency()-transferFunction->getMinFrequency())) * 9 + 1, 0);

    int position = 0;
    amplitudeGraph.at(position) = 20*log10(abs(transferFunction->Auswertung(pow(10, transferFunction->getMinFrequency()))));
    phaseGraph.at(position) = transferFunction->calculatePhase(pow(10, transferFunction->getMinFrequency()));
    position++;
    for (int i = transferFunction->getMinFrequency(); i < transferFunction->getMaxFrequency(); i++){
        for (float j = 2.0; j < 11.0; j++){
            amplitudeGraph.at(position) = 20*log10(abs(transferFunction->Auswertung((j/10.0) * pow(10, i+1))));
            phaseGraph.at(position) = transferFunction->calculatePhase((j/10.0) * pow(10, i+1));
            position++;
        }
    }
    //END Calculation Bode Diagramm


    auto scene= new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->clear();
    scene->setBackgroundBrush(Qt::white);
    QPolygon polygon;   // Graph Amplitude at the top
    polygon << QPoint(-400,-250) << QPoint(-400,0) << QPoint(400,0) << QPoint(400,-250);
    scene->addPolygon(polygon);
    QPolygon polygon2;  // Graph Phase at the bottom
    polygon2 << QPoint(-400,300) << QPoint(-400,50) << QPoint(400,50) << QPoint(400,300);
    scene->addPolygon(polygon2);

    //float widthCoorSys = 800;
    //float heightCoordSys = 250;




    int expoDiff = maxFreqExponent - minFreqExponent;
    float step = 800/expoDiff;

    for (int horiz = 1; horiz<=expoDiff; horiz++){//x  Gestrichelte Linien oben
        for (int j=0;j<25;j+=2){//y
            if (horiz < expoDiff)
                scene->addLine(horiz*step -400, -j*10, horiz*step -400,-(j+1)*10);// Linie bei ganzen Potenzen
            //Linien zwischen zwei Potenzen
            scene->addLine(horiz*step -400 -step+step*log10(2), -j*10, horiz*step -400 -step+step*log10(2),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(3), -j*10, horiz*step -400 -step+step*log10(3),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(4), -j*10, horiz*step -400 -step+step*log10(4),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(5), -j*10, horiz*step -400 -step+step*log10(5),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(6), -j*10, horiz*step -400 -step+step*log10(6),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(7), -j*10, horiz*step -400 -step+step*log10(7),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(8), -j*10, horiz*step -400 -step+step*log10(8),-j*10-2);
            scene->addLine(horiz*step -400 -step+step*log10(9), -j*10, horiz*step -400 -step+step*log10(9),-j*10-2);
        }
    }


    for (int horiz = 1; horiz<=expoDiff; horiz++){// Gestrichelte Linien unten
        for (int j=29;j>4;j-=2){
            if(horiz < expoDiff)
                scene->addLine( horiz*step -400, j*10, horiz*step - 400,(j+1)*10);// Linie bei ganzen Potenzen
            //Linien zwischen zwei Potenzen
            scene->addLine(horiz*step -400 -step+step*log10(2), j*10, horiz*step -400 -step+step*log10(2),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(3), j*10, horiz*step -400 -step+step*log10(3),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(4), j*10, horiz*step -400 -step+step*log10(4),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(5), j*10, horiz*step -400 -step+step*log10(5),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(6), j*10, horiz*step -400 -step+step*log10(6),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(7), j*10, horiz*step -400 -step+step*log10(7),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(8), j*10, horiz*step -400 -step+step*log10(8),j*10+2);
            scene->addLine(horiz*step -400 -step+step*log10(9), j*10, horiz*step -400 -step+step*log10(9),j*10+2);
        }
    }

    /**QGraphicsLineItem* lintest = new QGraphicsLineItem;
    lintest->setLine(0,0,100,100);
    scene->addItem(lintest);
    **/

    QGraphicsTextItem * headline = new QGraphicsTextItem;
    headline->setPos(-420,-300);
    headline->setPlainText("Bode Diagram");
    headline->setDefaultTextColor(Qt::black);
    headline->setScale(1.4);
    scene->addItem(headline);

    QGraphicsTextItem * xLabelAmpl = new QGraphicsTextItem;
    xLabelAmpl->setPos(-475,-75);
    xLabelAmpl->setPlainText("Magnitude (dB)");
    xLabelAmpl->setDefaultTextColor(Qt::black);
    xLabelAmpl->setScale(1.3);
    xLabelAmpl->setRotation(-90);
    scene->addItem(xLabelAmpl);

    QGraphicsTextItem * xLabelDegr = new QGraphicsTextItem;
    xLabelDegr->setPos(-475, 225);
    xLabelDegr->setPlainText("Phase (deg)");
    xLabelDegr->setDefaultTextColor(Qt::black);
    xLabelDegr->setScale(1.3);
    xLabelDegr->setRotation(-90);
    scene->addItem(xLabelDegr);

    QGraphicsTextItem * xLabelFreq = new QGraphicsTextItem;
    xLabelFreq->setPos(-50, 335);
    xLabelFreq->setPlainText("Frequenz (Hz)");
    xLabelFreq->setDefaultTextColor(Qt::black);
    xLabelFreq->setScale(1.3);
    scene->addItem(xLabelFreq);

    //BEGIN: Hinzufügen der Dezibel und Degree Makierungen
    int maxAmplitudeYValue = 20;
    int minAmplitudeYValue = 0;
    int maxPhaseYValue = 90;
    int minPhaseYValue = 90;
    std::vector<float>::iterator iterAmplGraph = amplitudeGraph.begin();
    std::vector<float>::iterator iterPhasGraph = phaseGraph.begin();
    float maxAmplitudeSearch = *iterAmplGraph;
    float minAmplitudeSearch = *iterAmplGraph;
    float maxPhaseSearch = *iterPhasGraph;
    float minPhaseSearch = *iterPhasGraph;
    iterAmplGraph++;
    iterPhasGraph++;
    while (iterAmplGraph != amplitudeGraph.end()) {
        if (*iterAmplGraph > maxAmplitudeSearch){
            maxAmplitudeSearch = *iterAmplGraph;
        }
        else if (*iterAmplGraph < minAmplitudeSearch){
            minAmplitudeSearch = *iterAmplGraph;
        }
        iterAmplGraph++;
    }
    maxAmplitudeYValue = 20*(int)(maxAmplitudeSearch/20+1);//(maxAmplitudeSearch>0) ? 20*(int)(maxAmplitudeSearch/20+1) : 20*(int)(maxAmplitudeSearch/20+1);
    if (maxAmplitudeSearch == -std::numeric_limits<float>::infinity())
        maxAmplitudeYValue = 20;
    minAmplitudeYValue = (minAmplitudeSearch>0) ? 20*(int)(minAmplitudeSearch/20) : 20*(int)(minAmplitudeSearch/20 - 1);
    if (minAmplitudeSearch == -std::numeric_limits<float>::infinity())
        minAmplitudeYValue = -20;

    while (iterPhasGraph != phaseGraph.end()) {
        if (*iterPhasGraph > maxPhaseSearch){
            maxPhaseSearch= *iterPhasGraph;
        }
        else if (*iterPhasGraph < minPhaseSearch){
            minPhaseSearch = *iterPhasGraph;
        }
        iterPhasGraph++;
    }
    maxPhaseYValue = 90*(int)(maxPhaseSearch/90+1);
    minPhaseYValue = (minPhaseSearch>0) ? 90*(int)(minPhaseSearch/90) : 90*(int)(minPhaseSearch/90-1);

    int dezibel = maxAmplitudeYValue;
    float yAxisStep = (maxAmplitudeYValue == 0 && minAmplitudeYValue == 0) ? 250/2 :250/((maxAmplitudeYValue-minAmplitudeYValue)/20);
    for (int var = 0; var <= (int)(maxAmplitudeYValue-minAmplitudeYValue)/20; ++var) {
        scene->addLine(-400, -250+var*yAxisStep, -390, -250+var*yAxisStep);
        QString yLabel = QString::number(dezibel);
        QGraphicsTextItem* xLabel = new QGraphicsTextItem;
        xLabel->setPos(-440, -250+var*yAxisStep-10);
        xLabel->setPlainText(yLabel+" dB");
        xLabel->setDefaultTextColor(Qt::black);
        xLabel->setScale(0.9);
        scene->addItem(xLabel);
        dezibel -= 20;
    }

    int degree = maxPhaseYValue;
    yAxisStep = 250/((maxPhaseYValue-minPhaseYValue)/90);// max und min Phase bei gleich falls nur ein wert
    for (int var = 0; var <= (int)(maxPhaseYValue-minPhaseYValue)/90; var ++) {//50 bis 300 in y Richtung unten
        scene->addLine(-400, 50+var*yAxisStep,-390, 50+var*yAxisStep);
        QString yLabel = QString::number(degree);
        QGraphicsTextItem* xLabel = new QGraphicsTextItem;
        xLabel->setPos(-440, 50+var*yAxisStep-10);
        xLabel->setPlainText(yLabel+"°");
        xLabel->setDefaultTextColor(Qt::black);
        xLabel->setScale(0.9);
        scene->addItem(xLabel);
        degree -= 90;
    }
    //END: Hinzufügen der Dezibel und Degree Makierung

    //BEGIN: Hinzufügen der Graphen
    int indexDraw = 0;
    for (int i = transferFunction->getMinFrequency(); i < transferFunction->getMaxFrequency(); i++){
        for (int j = 1; j < 10; j++){
            scene->addLine(-400 + step*(log10(j)+i-transferFunction->getMinFrequency()),    ((amplitudeGraph.at(indexDraw) - minAmplitudeYValue) /(maxAmplitudeYValue-minAmplitudeYValue)) *(-250),
                           -400 + step*(log10(j+1)+i-transferFunction->getMinFrequency()),  ((amplitudeGraph.at(indexDraw+1) - minAmplitudeYValue) /(maxAmplitudeYValue-minAmplitudeYValue)) *(-250));
            indexDraw++;
        }
    }

    indexDraw = 0;
    for (int i = transferFunction->getMinFrequency(); i < transferFunction->getMaxFrequency(); i++){
        for (int j = 1; j < 10; j++){
            scene->addLine(-400 + step*(log10(j)+i-transferFunction->getMinFrequency()),    ((phaseGraph.at(indexDraw) - minPhaseYValue) /(maxPhaseYValue-minPhaseYValue)) *(-250) + 300,
                           -400 + step*(log10(j+1)+i-transferFunction->getMinFrequency()),  ((phaseGraph.at(indexDraw+1) - minPhaseYValue) /(maxPhaseYValue-minPhaseYValue)) *(-250) + 300);
            indexDraw++;
        }
    }


    //END: Hinzufügen der Graphen

    //BEGIN: Hinzufügen der Makierungen der Frequenzen und der Label
    int freqExpo = ui->spinBoxLowerFrequency->value();
    for (int freq = -400; freq <= 400; freq +=step) {//float step = 800/expoDiff;
        scene->addLine(freq, 0, freq, 10);
        scene->addLine(freq, 300, freq, 310);
        QString freqLabel = QString::number(freqExpo);
        QGraphicsTextItem* Label = new QGraphicsTextItem;
        Label->setPos(freq-10, 318);
        Label->setPlainText("10^" +freqLabel);
        Label->setDefaultTextColor(Qt::black);
        Label->setScale(0.9);
        scene->addItem(Label);
        freqExpo++;
    }
    //END: Hinzufügen der Makierungen der Frequenzen und der Label

    ui->toolBox->setCurrentIndex(1);
}

void MainWindow::on_pushButton_clicked()
{
    QPixmap pixMap = this->ui->graphicsView->grab();
    pixMap.save("C:/Users/Stephan/OneDrive/Desktop/BodeDiagrammViewer/file_Name.png"); // Funktioniert, Der Ort ist im C:\Users\Stephan\OneDrive\Desktop\BodeDiagrammViewer\build\Desktop_Qt_6_7_2_MinGW_64_bit-Debug Ordner
}


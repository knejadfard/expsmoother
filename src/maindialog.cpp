#include "maindialog.h"
#include "ses.h"
#include "des.h"
#include "tes.h"
#include "functions.h"
#include "output.h"
#include <QTextStream>
#include <QMessageBox>
#include <QLocale>

MainDialog::MainDialog()
{
    //ALLOCATION OF WIDGETS
    vBoxOptions = new QVBoxLayout;
    paramLayout = new QHBoxLayout;
    countLayout = new QHBoxLayout;
    tableLayout = new QHBoxLayout;
    alphaDSpin = new QDoubleSpinBox;
    betaDSpin = new QDoubleSpinBox;
    gammaDSpin = new QDoubleSpinBox;
    methodLabel = new QLabel("Method: Exponential Smoothing");
    alphaLabel = new QLabel("Alpha:");
    countLabel = new QLabel("Data Count:");
    fcountLabel = new QLabel("Number of forecasts:");
    betaCheckBox = new QCheckBox("Beta:");
    gammaCheckBox = new QCheckBox("Gamma:");
    countSpin = new QSpinBox;
    fcountSpin = new QSpinBox;
    freqSpin = new QSpinBox;
    freqLabel = new QLabel("Frequency:");
    table = new QTableWidget;
    resultText = new QPlainTextEdit;
    debugText = new QPlainTextEdit;
    calculateButton = new QPushButton("Calculate");
    sgLayout = new QHBoxLayout;
    sDSpin = new QDoubleSpinBox;
    gDSpin = new QDoubleSpinBox;
    sLabel = new QLabel("Initial Intercept(S0):");
    gLabel = new QLabel("Initial Slope(G0):");
    //SETTINGS OF WIDGETS
    alphaDSpin->setRange(0.0, 1.0);
    alphaDSpin->setSingleStep(0.1);
    alphaDSpin->setDecimals(4);
    alphaDSpin->setMinimumWidth(60);
    betaDSpin->setRange(0.0, 1.0);
    betaDSpin->setSingleStep(0.1);
    betaDSpin->setDecimals(4);
    betaDSpin->setEnabled(false);
    betaDSpin->setMinimumWidth(60);
    gammaDSpin->setRange(0.0, 1.0);
    gammaDSpin->setSingleStep(0.1);
    gammaDSpin->setEnabled(false);
    gammaDSpin->setDecimals(4);
    gammaDSpin->setMinimumWidth(60);
    freqSpin->setRange(2, 100);
    sDSpin->setEnabled(false);
    gDSpin->setEnabled(false);
    sDSpin->setRange(-999999.0, 999999.0);
    gDSpin->setRange(-999999.0, 999999.0);
    sDSpin->setDecimals(4);
    gDSpin->setDecimals(4);
    countSpin->setMinimum(1);
    fcountSpin->setMinimum(1);
    fcountSpin->setEnabled(false);
    freqSpin->setEnabled(false);
    table->setColumnCount(1);
    table->setRowCount(1);
    resultText->setReadOnly(true);
    debugText->setReadOnly(true);
    //table->
    _method = 1;
    //LAYOUT ADJUSTMENTS
    paramLayout->addWidget(alphaLabel);
    paramLayout->addWidget(alphaDSpin);
    paramLayout->addWidget(betaCheckBox);
    paramLayout->addWidget(betaDSpin);
    paramLayout->addWidget(gammaCheckBox);
    paramLayout->addWidget(gammaDSpin);
    countLayout->addWidget(countLabel);
    countLayout->addWidget(countSpin);
    countLayout->addWidget(fcountLabel);
    countLayout->addWidget(fcountSpin);
    countLayout->addWidget(freqLabel);
    countLayout->addWidget(freqSpin);
    tableLayout->addWidget(table);
    tableLayout->addWidget(resultText);
    tableLayout->addWidget(debugText);
    sgLayout->addWidget(sLabel);
    sgLayout->addWidget(sDSpin);
    sgLayout->addWidget(gLabel);
    sgLayout->addWidget(gDSpin);
    vBoxOptions->addWidget(methodLabel);
    vBoxOptions->addLayout(paramLayout);
    vBoxOptions->addLayout(sgLayout);
    vBoxOptions->addLayout(countLayout);
    vBoxOptions->addLayout(tableLayout);
    vBoxOptions->addWidget(calculateButton);
    paramLayout->setAlignment(Qt::AlignLeft);
    countLayout->setAlignment(Qt::AlignLeft);
    sgLayout->setAlignment(Qt::AlignLeft);
    //vBoxOptions->addWidget(resultText);
    QWidget *central = new QWidget;
    central->setLayout(vBoxOptions);
    setCentralWidget(central);
    //CONNECTIONS
    QObject::connect(betaCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleBetaDSpin(bool)));
    QObject::connect(betaCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleMethod()));
    QObject::connect(gammaCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleGammaDSpin(bool)));
    QObject::connect(gammaCheckBox, SIGNAL(toggled(bool)), this, SLOT(toggleMethod()));
    QObject::connect(countSpin, SIGNAL(valueChanged(int)), this, SLOT(setTableRowCount(int)));
    //QObject::connect(countSpin, SIGNAL(editingFinished()), this, SLOT(setTableRowCount()));
    QObject::connect(calculateButton, SIGNAL(clicked()), this, SLOT(calculate()));
}

MainDialog::~MainDialog()
{
    //delete alphaDSpin, betaDSpin, gammaDSpin, vBoxOptions, alphaLayout, betaLayout, gammaLayout;
    /*for(int s = 0; s < table->rowCount(); s++) {
        delete table->item(s, 0);
    }*/
}

/*********************
 * SLOTS DEFINITIONS *
 ********************/
void MainDialog::toggleBetaDSpin(bool b) {
    if(b) {
        betaDSpin->setEnabled(b);
        sDSpin->setEnabled(true);
        gDSpin->setEnabled(true);
        fcountSpin->setEnabled(true);
        _method = 2;
    } else {
        gammaDSpin->setEnabled(false);
        gammaCheckBox->setChecked(false);
        sDSpin->setEnabled(false);
        gDSpin->setEnabled(false);
        fcountSpin->setEnabled(false);
        _method = 1;
    }
}

void MainDialog::toggleGammaDSpin(bool b) {
    if(b) {
        freqSpin->setEnabled(true);
        gammaDSpin->setEnabled(true);
        betaDSpin->setEnabled(true);
        betaCheckBox->setChecked(true);
        sDSpin->setEnabled(false);
        gDSpin->setEnabled(false);
        _method = 3;
    } else {
        freqSpin->setEnabled(false);
        sDSpin->setEnabled(true);
        gDSpin->setEnabled(true);
        _method = 2;
    }
}

void MainDialog::toggleMethod() {
    if(_method == 1) {
        methodLabel->setText("Method: Exponential Smoothing");
    } else if(_method == 2) {
        methodLabel->setText("Method: Double Exponential Smoothing");
    } else if(_method == 3) {
        methodLabel->setText("Method: Triple Exponential Smoothing");
    }
}

void MainDialog::setTableRowCount(int a) {
    table->setRowCount(a);
}

/*void MainDialog::getTableRowCount(int a) {
    _rows = a;
}*/

void MainDialog::calculate() {
    //get data
    std::vector<double> data;
    QLocale c(QLocale::system());
    for(int i=0; i<table->rowCount(); i++) {
        if(!table->item(i, 0)) {
            //item has never been set
            warn("Fill out all the data cells");
            return;
        } else if(table->item(i, 0)->text().isEmpty()) {
            //item has been set but has been emptied again
            warn("Fill out all the data cells");
            return;
        } else {
            //item is good
            bool check;
            double tmp = c.toDouble(table->item(i, 0)->text(), &check);
            if(!check) {
                warn("Conversion error!");
                return;
            }
            data.push_back(tmp);
        }
    }
    bool check;
    double alpha = c.toDouble(alphaDSpin->text(), &check);
    if(!check) {
        warn("Error converting alpha's value to double");
        return;
    }
    double beta = c.toDouble(betaDSpin->text(), &check);
    if(!check) {
        warn("Error converting beta's value to double");
        return;
    }
    double gamma = c.toDouble(gammaDSpin->text(), &check);
    if(!check) {
        warn("Error converting gamma's value to double");
        return;
    }
    double s0 = c.toDouble(sDSpin->text(), &check);
    if(!check) {
        warn("Error converting S0's value to double");
        return;
    }
    double g0 = c.toDouble(gDSpin->text(), &check);
    if(!check) {
        warn("Error converting G0's value to double");
        return;
    }
    int forecast_count = c.toInt(fcountSpin->text(), &check);
    if(!check) {
        warn("Error converting the number of forecasts to int");
        return;
    }
    int frequency = c.toInt(freqSpin->text(), &check);
    if(!check) {
        warn("Error converting frequency's value to int");
        return;
    }
    //call calculators
    if(_method == 1) {
        single_es es;
        es.seq_update(data, alpha);
        QString str;
        ses_forecast_summary(str, data, es);
        resultText->setPlainText(str);
    } else if(_method ==2) {
        double_es d(data, s0, g0);
        d.seq_update(alpha, beta);
        d.evaluate();
        d.future_forecast(forecast_count);
        QString str, debug;
        //des_update_summary(str, d);
        des_forecast_summary(str, debug, d);
        resultText->setPlainText(str);
        debugText->setPlainText(debug);
    } else {
        if(data.size()/frequency < 2) {
            warn("There aren't enough data points for the selected frequency value");
            return;
        }
        triple_es tes(data, frequency, alpha, beta, gamma);
        QString str, dstr;
        tes_initiate_summary(str, dstr, tes);
        tes.initial_forecast();
        tes.periodic_update();
        tes.evaluate();
        tes.future_forecast(forecast_count);
        tes_forecast_summary(str, dstr, tes);
        resultText->setPlainText(str);
        debugText->setPlainText(dstr);
    }
    //output the results
}

void MainDialog::warn(const QString& txt) {
    QMessageBox msg;
    msg.setText(txt);
    msg.setIcon(QMessageBox::Warning);
    msg.exec();
}

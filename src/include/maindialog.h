#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QString>
#include <QPlainTextEdit>
#include <QPushButton>

class MainDialog : public QMainWindow
{
    Q_OBJECT
    
public:
    MainDialog();
    ~MainDialog();

private:
    QVBoxLayout *vBoxOptions;
    QHBoxLayout *paramLayout, *countLayout, *tableLayout, *sgLayout;
    QDoubleSpinBox *alphaDSpin, *betaDSpin, *gammaDSpin, *sDSpin, *gDSpin;
    QSpinBox *countSpin, *fcountSpin, *freqSpin;
    QLabel *methodLabel, *alphaLabel, *countLabel, *fcountLabel, *sLabel, *gLabel, *freqLabel;
    QCheckBox *betaCheckBox, *gammaCheckBox;
    QTableWidget *table;
    QPlainTextEdit *resultText, *debugText;
    QPushButton *calculateButton;

    int _method; //1: ES 2: DES 3: TES
    void warn(const QString&);

public slots:
    void toggleBetaDSpin(bool);
    void toggleGammaDSpin(bool);
    void toggleMethod();
    void setTableRowCount(int);
    //void getTableRowCount(int);
    void calculate();
};

#endif // MAINDIALOG_H

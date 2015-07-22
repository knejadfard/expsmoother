#ifndef OUTPUT_H
#define OUTPUT_H

#include "des.h"
#include "ses.h"
#include "tes.h"
#include <QString>
#include <vector>

void des_update_summary(QString&, double_es&);
void des_forecast_summary(QString&, QString&, double_es&);
void ses_forecast_summary(QString&, std::vector<double>&, single_es&);
void tes_initiate_summary(QString&, QString&, triple_es&);
void tes_forecast_summary(QString&, QString&, triple_es&);

#endif // OUTPUT_H

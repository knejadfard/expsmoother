#include "output.h"
#include "functions.h"
#include <vector>
#include <sstream>

void ses_forecast_summary(QString& s, std::vector<double>& d, single_es& h) {
    std::stringstream out;
    out<<"Forecast with Alpha="<<h.get_alpha()<<"\n";
    int future = 1;
    std::vector<double> &fvec = h.forecast_vector_ref(), &errvec = h.error_vector_ref();
    for(std::vector<double>::iterator i1=fvec.begin(), i2=errvec.begin();\
        i1 != fvec.end(), i2 != errvec.end(); i1++, i2++) {
        out<<"   "<<future<<"->"<<*i1<<"   "<<"E->"<<*i2<<"\n";
        future++;
    }
    out<<"Forecast for all future periods\n   "<<fvec.back()<<"\n";
    out<<"\nForecast Evaluation\n";
    out<<"   MAD->"<<calc_mae(errvec)<<"\n";
    out<<"   MSE->"<<calc_mse(errvec)<<"\n";
    out<<"   MAPE->"<<calc_mape(d, errvec)<<"\n";
    s += out.str().c_str();
}

//ELIMINATE OVER ITERATION BY OUTPUTING DEBUG INFO WHILE UPDATING
void des_update_summary(QString& s, double_es& h) {
    std::stringstream out;
    out<<("Estimates of intercept and slope\n");
    std::vector<double> &v_s = h.intercept_vector_ref(), &v_g = h.slope_vector_ref();
    std::vector<double>::iterator i_s, i_g;
    for(i_s = v_s.begin(), i_g = v_g.begin(); i_s != v_s.end(), i_g != v_g.end(); i_s++, i_g++) {
        out<<"   S"<<i_s-v_s.begin()<<"->"<<*i_s<<"   G"<<i_g-v_g.begin()<<"->"<<*i_g<<"\n";
    }
    out<<"\n";
    s += out.str().c_str();
}
//CHANGE THE FUTURE FORECAST LOOP, SHOULD HAPPEN INSIDE DES
void des_forecast_summary(QString& s, QString& d, double_es& des) {
    std::stringstream out;
    out<<"Forecast with Alpha="<<des.get_alpha()<<" and Beta="<<des.get_beta()<<"\n";
    //Below forecasts are from period 1, one step ahead
    int future = 1;
    std::vector<double> &fvec = des.forecast_vector_ref(), &errvec = des.error_vector_ref();
    for(std::vector<double>::iterator i1=fvec.begin(), i2=errvec.begin();\
        i1 != fvec.end(), i2 != errvec.end(); i1++, i2++) {
        out<<"   "<<future<<"->"<<*i1<<"   "<<"E->"<<*i2<<"\n";
        future++;
    }
    //Below, multiple step ahead forecasts are calculated for future periods
    for(std::vector<double>::iterator i=des.get_newf().begin(); i!=des.get_newf().end(); i++, future++) {
        out<<"   "<<future<<"->"<<*i<<"\n";
    }
    out<<"\nForecast Evaluation\n";
    out<<"   MAD->"<<des.get_mae()<<"\n";
    out<<"   MSE->"<<des.get_mse()<<"\n";
    out<<"   MAPE->"<<des.get_mape()<<"\n";
    s += out.str().c_str();
    d = des.get_debug().str().c_str();
}

void tes_initiate_summary(QString& s, QString& d, triple_es& tes) {
    std::stringstream out;
    out<<"Smoothing Coefficients\n";
    out<<"    Alpha: "<<tes.get_alpha();
    out<<"\n    Beta: "<<tes.get_beta();
    out<<"\n    Gamma: "<<tes.get_gamma();
    out<<"\nInitial Intercept\n    "<<tes.get_s();
    out<<"\nInitial Slope\n    "<<tes.get_g();
    out<<"\nInitial Seasonal Factors\n";
    std::vector<double> &cref = tes.get_c();
    for(std::vector<double>::iterator i = cref.begin(); i!=cref.end(); i++) {
        out<<"    C["<<i-cref.begin()+1-tes.get_freq()<<"]="<<*i<<"\n";
    }
    s += out.str().c_str();
    d = tes.get_debug().str().c_str();
}

void tes_forecast_summary(QString& s, QString& d, triple_es& tes) {
    std::stringstream out;
    out<<"\nForecasts";
    for(std::vector<double>::iterator i = tes.get_f().begin(); i != tes.get_f().end(); i++) {
        out<<"\n    "<<i-tes.get_f().begin()+1<<"->"<<*i;
        out<<"   E->"<<tes.get_e().at(i-tes.get_f().begin());
    }
    for(std::vector<double>::iterator j = tes.get_newf().begin(); j != tes.get_newf().end(); j++) {
        out<<"\n    "<<j-tes.get_newf().begin()+tes.get_f().size()+1<<"->"<<*j;
    }
    out<<"\n\nForecast Evaluation\n";
    out<<"   MAD->"<<tes.get_mae()<<"\n";
    out<<"   MSE->"<<tes.get_mse()<<"\n";
    out<<"   MAPE->"<<tes.get_mape()<<"\n";
    s += out.str().c_str();
    d = tes.get_debug().str().c_str();
}

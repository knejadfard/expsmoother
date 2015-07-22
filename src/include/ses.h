#ifndef SES_H
#define SES_H

#include <vector>

class single_es {

    std::vector<double> _f; //forecasts for past periods
    std::vector<double> _e; //errors of forecasting past periods
    double _alpha; //the smoothing parameter

    void update(const double&, const double&);

public:

    single_es() {}

    void seq_update(std::vector<double>&, const double&);
    //double forecast(const unsigned int&);
    std::vector<double>& forecast_vector_ref();
    std::vector<double>& error_vector_ref();
    //int now() const; //return the current period number
    double get_alpha() const;

};

#endif // SES_H

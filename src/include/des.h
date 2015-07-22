#ifndef DES_H
#define DES_H

#include <vector>
#include <sstream>

class double_es {

    /*******************
     * Storage Vectors *
     *******************/
    std::vector<double> _s; //latest estimation of intercept
    std::vector<double> _g; //latest estimation of slope
    std::vector<double> _f; //forecasts for past periods
    std::vector<double> _newf; //future period forecasts (multiple-step-ahead)
    std::vector<double> _e; //errors of forecasting past periods
    std::vector<double>& _d;
    /************************
     * Smoothing Parameters *
     ************************/
    double _alpha;
    double _beta;
    /********************
     * Evaluation Stats *
     *******************/
    double _mae;
    double _mse;
    double _mape;
    /***************
     * Debug Text  *
     ***************/
    std::stringstream _debug;

public:

    double_es(std::vector<double>&, const double&, const double&);
    void update(const double&, const double&, const double&);
    void seq_update(const double&, const double&);
    double forecast(const unsigned int&, const unsigned int&);
    void future_forecast(const int&); //use forecast(int, int) to forecast for a known number of future periods
    void evaluate(); //must be called after seq_update
    /**********************
     * Accessor Functions *
     **********************/
    double latest_intercept() const;
    double latest_slope() const;
    std::vector<double>& intercept_vector_ref();
    std::vector<double>& slope_vector_ref();
    std::vector<double>& forecast_vector_ref();
    std::vector<double>& error_vector_ref();
    int now() const; //return the current period number
    double get_alpha() const;
    double get_beta() const;
    double get_mae() const;
    double get_mse() const;
    double get_mape() const;
    std::vector<double>& get_data();
    std::vector<double>& get_newf();
    std::stringstream& get_debug();

};

#endif // DES_H

#ifndef TES_H
#define TES_H

#include <vector>
#include <sstream>

class triple_es {
    /*******************
     * Storage Vectors *
     *******************/
    //std::vector<double> _s; //intercepts
    //std::vector<double> _g; //slopes
    std::vector<double> _c; //seasonal factors
    //NEW EFFICIENT VERSION
    double _s, _g;
    std::vector<double> _f; //forecasts for periods with data
    std::vector<double> _newf; //forecasts for future periods
    std::vector<double> _e; //forecast errors
    std::vector<double> *_d; //data
    /**************************
     * Smoothing Coefficients *
     **************************/
    double _alpha;
    double _beta;
    double _gamma;
    /************************************
     * Forecast Evaluation Measurements *
     ************************************/
    double _mae;
    double _mse;
    double _mape;
    /********************
     * Other Properties *
     ********************/
    int _freq; //number of data points in each season
    double _dfreq; //used in the calculations
    std::stringstream _debugtext;
    bool _updated; //set to true if periodic update happens
    /*********************
     * Private Functions *
     *********************/
    //NOTE: all initializations are based on the two first seasons
    //Additional data points are used to update the estimates
    void initiate_s(double&); //calculate the initial value of intercept
    void initiate_g(double&, double&); //calculate the initial value of slope
    void initiate_c(double&, double&); //calculate the initial seasonal factors
    void normalize_c(); //changes the seasonal factors in a way that they add up to _freq
    void initiate(); //prepare & call all the initialization functions in the right order
public:
    triple_es(std::vector<double>&, int&, double&, double&, double&);
    void initial_forecast(); //forecast for all available data points, based on the initial estimates
    void periodic_update(); //use the additional data points to update the estimates
    void future_forecast(const int&); //forecast for future periods based on the latest available estimates
    void evaluate(); //calculate forecast evaluations, to be called after initial_forecast and periodic_update
    /**********************
     * Accessor Functions *
     **********************/
    double get_alpha() const;
    double get_beta() const;
    double get_gamma() const;
    double get_s() const;
    double get_g() const;
    int get_freq() const;
    std::vector<double>& get_c();
    std::vector<double>& get_f();
    std::vector<double>& get_newf();
    std::vector<double>& get_e();
    std::vector<double>& get_data();
    std::stringstream& get_debug();
    double get_mae() const;
    double get_mse() const;
    double get_mape() const;
};

#endif // TES_H

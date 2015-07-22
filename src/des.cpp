#include "des.h"
#include "functions.h"

double_es::double_es(std::vector<double> &d, const double& s, const double& g): _alpha(0), _beta(0), _d(d) {
    _s.push_back(s);
    _g.push_back(g);
    _f.push_back(_s.back() + _g.back());
    _debug<<"Initial: S="<<s<<" G="<<g<<"\n";
    _debug<<"Forecast: (S="<<s<<")+(G="<<g<<")="<<_f.back()<<"\n";
}

double double_es::forecast(const unsigned int& future_index, const unsigned int& now_index) {
    if(future_index <= now_index) {
        /*std::cerr<<"Requested period number \""<<future_index\
        <<"\" is larger than provided current period \""<<now_index<<"\""<<std::endl;*/
        return 0;
    }
    double result = _s.at(now_index) + (future_index-now_index)*_g.at(now_index);
    _debug<<"F.F. (S="<<_s.back()<<")+"<<future_index-now_index<<"*(G="<<_g.back()<<")="<<result<<"\n";
    return result;
}

void double_es::update(const double& d, const double& a, const double& b) {
    _alpha = a;
    _beta = b;
    double new_s = a * d + (1-a) * (_s.back() + _g.back()); //update the intercept
    double new_g = b * (new_s - _s.back()) + (1-b) * _g.back(); //update the slope
    _debug<<"Update S="<<a<<"*"<<d<<"+"<<1-a<<"*"<<_s.back() + _g.back()<<"="<<new_s<<"\n";
    _debug<<"Update G="<<b<<"*("<<new_s-_s.back()<<")+"<<1-b<<"*"<<_g.back()<<"="<<new_g<<"\n";
    _e.push_back(_f.back() - d);
    _debug<<"Error: (F="<<_f.back()<<")-(D="<<d<<")="<<_e.back()<<"\n";
    _s.push_back(new_s);
    _g.push_back(new_g);
    _f.push_back(_s.back() + _g.back()); //forecast for the next period
    _debug<<"Forecast: (S="<<new_s<<")+(G="<<new_g<<")="<<_f.back()<<"\n";
}

void double_es::seq_update(const double& a, const double& b) {
    _alpha = a;
    _beta = b;
    for(std::vector<double>::iterator i = _d.begin(); i != _d.end(); i++) {
        update(*i, a, b);
    }
}

void double_es::future_forecast(const int& c) {
    for(int i = 1; i<=c; i++) {
        _newf.push_back(forecast(i+now(), now()));
    }
}

void double_es::evaluate() {
    _mae = calc_mae(_e);
    _mse = calc_mse(_e);
    _mape = calc_mape(_d, _e);
}

double double_es::latest_intercept() const {
    return _s.back();
}

double double_es::latest_slope() const {
    return _g.back();
}

std::vector<double>& double_es::slope_vector_ref() {
    return _g;
}

std::vector<double>& double_es::intercept_vector_ref() {
    return _s;
}

std::vector<double>& double_es::forecast_vector_ref() {
    return _f;
}

std::vector<double>& double_es::error_vector_ref() {
    return _e;
}

int double_es::now() const {
    return _s.size()-1;
}

double double_es::get_alpha() const {
    return _alpha;
}

double double_es::get_beta() const {
    return _beta;
}

double double_es::get_mae() const {
    return _mae;
}

double double_es::get_mse() const {
    return _mse;
}

double double_es::get_mape() const {
    return _mape;
}

std::vector<double>& double_es::get_data() {
    return _d;
}

std::vector<double>& double_es::get_newf() {
    return _newf;
}

std::stringstream& double_es::get_debug() {
    return _debug;
}

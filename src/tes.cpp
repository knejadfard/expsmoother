#include "tes.h"
#include <numeric>
#include <iostream>
#include "functions.h"

triple_es::triple_es(std::vector<double> &data, int &frequency, double &alpha, double &beta, double &gamma):
    _d(&data), _freq(frequency), _alpha(alpha), _beta(beta), _gamma(gamma), _dfreq(double(frequency)), _updated(false)
{
    initiate();
}

void triple_es::initiate() {
    double v1 = std::accumulate(_d->begin(), _d->begin()+_freq, 0) / _dfreq;
    double v2 = std::accumulate(_d->begin()+_freq, _d->begin()+2*_freq, 0) / _dfreq;
    _debugtext<<"V1="<<v1<<" V2="<<v2<<"\n";
    initiate_g(v1, v2);
    _debugtext<<"G0=("<<v2<<"-"<<v1<<")/"<<_freq<<"="<<_g<<"\n";
    initiate_s(v2);
    _debugtext<<"S0="<<v2<<"+"<<_g<<"*("<<_freq<<"-1)/2"<<"="<<_s<<"\n";
    initiate_c(v1, v2);
    normalize_c();
}

void triple_es::initiate_g(double &v1, double &v2) {
    _g = (v2-v1)/_dfreq;
}

void triple_es::initiate_s(double &v2) {
    _s = v2 + (_dfreq-1)*_g/2;
}

void triple_es::initiate_c(double &v1, double &v2) {
    std::vector<double> ctmp;
    for(int i=0; i<_freq; i++) {
        ctmp.push_back( _d->at(i)/(v1-((_dfreq+1)/2-i-1)*_g) );
    }
    for(int i=0; i<_freq; i++) {
        ctmp.push_back( _d->at(i+_freq)/(v2-((_dfreq+1)/2-i-1)*_g) );
    }
    for(int i=0; i<_freq; i++) {
        _c.push_back( (ctmp.at(i) + ctmp.at(i+_freq))/2 );
    }
}

void triple_es::normalize_c() {
    double sum = std::accumulate(_c.begin(), _c.end(), 0.0);
    _debugtext<<"Norming ";
    std::stringstream tmp;
    for(std::vector<double>::iterator i=_c.begin(); i!=_c.end(); i++) {
        _debugtext<<"("<<*i<<") ";
        *i = *i * _dfreq/sum;
        tmp<<"("<<*i<<") ";
    }
    _debugtext<<"\nNormed: "<<tmp.str()<<"\n";
}

double triple_es::get_alpha() const {
    return _alpha;
}

double triple_es::get_beta() const {
    return _beta;
}

double triple_es::get_gamma() const {
    return _gamma;
}

std::vector<double>& triple_es::get_c() {
    return _c;
}

std::vector<double>& triple_es::get_f() {
    return _f;
}

std::vector<double>& triple_es::get_newf() {
    return _newf;
}

std::vector<double>& triple_es::get_e() {
    return _e;
}

double triple_es::get_s() const {
    return _s;
}

double triple_es::get_g() const {
    return _g;
}

int triple_es::get_freq() const {
    return _freq;
}

std::vector<double>& triple_es::get_data() {
    return *_d;
}

std::stringstream& triple_es::get_debug() {
    return _debugtext;
}

void triple_es::initial_forecast() {
    int j = 0;
    for(std::vector<double>::iterator i = _d->begin(); i != _d->begin()+2*_freq; i++, j++) {
        if(j == _freq) {
            j = 0;
        }
        unsigned int index = i - _d->begin();
        _f.push_back((_s + (index+1)*_g)*_c.at(j));
        _e.push_back(_f.back() - *i);
        _debugtext<<"I.F. [(S="<<_s<<")+"<<(index+1)<<"*(G="<<_g<<")]*(C="<<_c.at(j)<<")="<<_f.back()<<"\n";
    }
}

void triple_es::periodic_update() {
    if(_d->size() > 2*_freq) {
        _updated = true;
    }
    bool flag = false; //to differ calculation coefficient from second iteration onwards
    for(int i=2*_freq, j=0; i<_d->size(); i++, j++) {
        if(j == _freq) {
            j = 0;
        }
        if(!flag) {
            _f.push_back( (_s+(i+1)*_g) * _c.at(j) );
            _debugtext<<"\nP.F. [(S="<<_s<<"+"<<i+1<<"*(G="<<_g<<")]*(C="<<_c.at(j)<<")="<<_f.back()<<"\n";
        } else {
            _f.push_back( (_s+(i-2*_freq+1)*_g) * _c.at(j) );
            _debugtext<<"\nP.F. [(S="<<_s<<"+"<<i-2*_freq<<"*(G="<<_g<<")]*(C="<<_c.at(j)<<")="<<_f.back()<<"\n";
        }
        _e.push_back( _f.back() - _d->at(i) );
        double new_s = _alpha*_d->at(i)/_c.at(j) + (1-_alpha)*(_s+_g);
        double new_g = _beta*(new_s-_s) + (1-_beta)*_g;
        _c.at(j) = _gamma*_d->at(i)/new_s + (1-_gamma) * _c.at(j);
        _debugtext<<"\nP.U. New S="<<new_s<<" New G="<<new_g<<" New C="<<_c.at(j)<<"\n";
        normalize_c();
        _s = new_s;
        _g = new_g;
        flag = true;
    }
}

void triple_es::future_forecast(const int &count) {
    if(!_updated) {
        for(int i=0, j=0; i<count; i++, j++) {
            if(j == _freq) {
                j = 0;
            }
            _newf.push_back( (_s + (i+2*_freq+1)*_g) * _c.at(j) );
            _debugtext<<"\nF.F. [(S="<<_s<<")+"<<i+2*_freq+1<<"*(G="<<_g<<")]*(C="<<_c.at(j)<<")="<<_newf.back();
        }
    } else {
        for(int i=0, j=0; i<count; i++, j++) {
            if(j == _freq) {
                j = 0;
            }
            _newf.push_back( (_s + (i+1)*_g) * _c.at(j) );
            _debugtext<<"\nF.F. [(S="<<_s<<")+"<<i+1<<"*(G="<<_g<<")]*(C="<<_c.at(j)<<")="<<_newf.back();
        }
    }
}

void triple_es::evaluate() {
    _mae = calc_mae(_e);
    _mse = calc_mse(_e);
    _mape = calc_mape(*_d, _e);
}

double triple_es::get_mae() const {
    return _mae;
}

double triple_es::get_mse() const {
    return _mse;
}

double triple_es::get_mape() const {
    return _mape;
}

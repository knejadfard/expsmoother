#include "ses.h"

void single_es::update(const double& d, const double& alpha) {
    _f.push_back(alpha*d + (1-alpha)*_f.back());
}

void single_es::seq_update(std::vector<double>& dvec, const double& alpha) {
    _alpha = alpha;
    _f.push_back(dvec.front());
    _e.push_back(0);
    for(std::vector<double>::iterator it = dvec.begin(); it != dvec.end()-1; it++) {
        _f.push_back(alpha*(*it) + (1-alpha)*(_f.back()));
        _e.push_back(_f.back() - (*(it+1)));
    }
    _f.push_back(alpha*dvec.back() + (1-alpha)*_f.back());
}

/*************
 * Accessors *
 *************/
std::vector<double>& single_es::forecast_vector_ref() {
    return _f;
}

std::vector<double>& single_es::error_vector_ref() {
    return _e;
}

double single_es::get_alpha() const {
    return _alpha;
}

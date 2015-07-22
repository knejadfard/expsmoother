#include "functions.h"
#include <cmath>
#include <vector>
#include <numeric>

double calc_mae(std::vector<double>& errors) {
    double mae = 0.0;
    for(std::vector<double>::iterator it = errors.begin(); it != errors.end(); it++) {
        mae += std::abs(*it);
    }
    return mae/errors.size();
}

double calc_mse(std::vector<double>& errors) {
    double mse = 0.0;
    for(std::vector<double>::iterator it = errors.begin(); it != errors.end(); it++) {
        mse += pow(*it, 2);
    }
    return mse/errors.size();
}

double calc_mape(std::vector<double>& data, std::vector<double>& errors) {
    double mape = 0.0;
    for(std::vector<double>::iterator it1 = errors.begin(), it2 = data.begin();\
    it1 != errors.end(), it2 != data.end(); it1++, it2++) {
        mape += std::abs(*it1 / *it2);
    }
    return mape/errors.size();
}

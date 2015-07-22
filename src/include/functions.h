#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <istream>

template<typename T> bool generic_read(T& d, std::istream& f) {
    return f>>d && f.ignore();
}

double calc_mae(std::vector<double>&);
double calc_mse(std::vector<double>&);
double calc_mape(std::vector<double>&, std::vector<double>&);

#endif // FUNCTIONS_H

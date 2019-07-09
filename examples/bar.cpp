#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <matplotlibcpp.h>
namespace plt = matplotlibcpp;

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

int main(int argc, char **argv) {
    xt::xtensor<double, 1> test_data = xt::arange<double>(20);

    plt::bar(test_data);
    plt::show();

    return (0);
}

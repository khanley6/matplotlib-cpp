#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include "../matplotlibcpp.h"

#include <vector>

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

namespace plt = matplotlibcpp;

std::vector<double> get_yv(std::size_t len) {
    std::vector<double> res(len);
    for (std::size_t i = 0; i < len; ++i) {
        res[i] = i*i + 2;
    }
    return res;
}

template <class E>
auto get_fy(E&& e) {
    auto res = e + 2.0;
    return std::move(res);
}

int main(int argc, char **argv) {
    std::size_t len = 10;

    /***** vectors *****/
    std::vector<double> xv(len), yv(len);
    for (std::size_t i = 0; i < len; ++i) {
        xv[i] = i;
        yv[i] = i*i;
    }

    plt::named_plot("vector - lvalue", xv, yv);
    plt::named_plot("vector - rvalue", xv, get_yv(len));

    /***** xtensors *****/
    xt::xtensor<double, 1> xx = xt::linspace<double>(0, len-1, len);
    xt::xtensor<double, 1> xy = xt::square(xx) + 4;

    plt::named_plot("xtensor - lvalue", xx, xy, "--");
    plt::named_plot("xtensor - rvalue", xx, xy+1, "--");

    /***** xfunction*****/
    auto fx = xt::linspace<double>(0, len-1, len);
    auto fy = xt::square(xx) + 6;

    plt::named_plot("xfunction - lvalue", fx, fy, "-.");
    plt::named_plot("xfunction - rvalue", fx, get_fy(fy), "--");

    plt::legend();
    plt::show();

    return 0;
}


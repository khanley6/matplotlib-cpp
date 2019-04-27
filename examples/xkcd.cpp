#define _USE_MATH_DEFINES
#include <cmath>
#include "../matplotlibcpp.h"
#include <vector>

namespace plt = matplotlibcpp;

int main() {
    xt::xtensor<double, 1> t = xt::linspace<double>(0, 10, 1000);
    xt::xtensor<double, 1> y = xt::sin(2.0 * xt::numeric_constants<double>::PI * t);

    plt::xkcd();
    plt::plot(t, y);
    plt::title("AN ORDINARY SIN WAVE");
    plt::show();
}


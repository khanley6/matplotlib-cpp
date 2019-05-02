#define _USE_MATH_DEFINES
#include "../matplotlibcpp.h"
#include <cmath>

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

using namespace xt::placeholders;
namespace plt = matplotlibcpp;

// Example fill plot taken from:
// https://matplotlib.org/gallery/misc/fill_spiral.html
int main() {
    // Prepare data.
    xt::xtensor<double, 1> theta = xt::arange<double>(0, 8*xt::numeric_constants<double>::PI, 0.1);

    const int a = 1;
    const double b = 0.2;

    for (double dt = 0; dt < 2 * M_PI; dt += M_PI/2.0) {
        xt::xtensor<double, 1> x1 = a*xt::cos(theta + dt) * xt::exp(b*theta);
        xt::xtensor<double, 1> y1 = a*xt::sin(theta + dt) * xt::exp(b*theta);
        xt::xtensor<double, 1> x2 = a*xt::cos(theta + dt + M_PI/4.0) * xt::exp(b*theta);
        xt::xtensor<double, 1> y2 = a*xt::sin(theta + dt + M_PI/4.0) * xt::exp(b*theta);

        x1 = xt::concatenate(xt::xtuple(x1, xt::view(x2, xt::range(x2.size(), _, -1))));
        y1 = xt::concatenate(xt::xtuple(y1, xt::view(y2, xt::range(y2.size(), _, -1))));

        plt::fill(x1+50, y1+50, {});
    }
    plt::show();
}

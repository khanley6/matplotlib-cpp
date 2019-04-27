#define _USE_MATH_DEFINES
#include "../matplotlibcpp.h"
#include <cmath>

using namespace std;
namespace plt = matplotlibcpp;

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

// Example fill plot taken from:
// https://matplotlib.org/gallery/misc/fill_spiral.html
int main() {
    // Prepare data.
    xt::xtensor<double, 1> theta = xt::arange<double>(0, 8*xt::numeric_constants<double>::PI, 0.1);

    const int a = 1;
    const double b = 0.2;

    for (double dt = 0; dt < 2 * M_PI; dt += M_PI/2.0) {
        xt::xtensor<double, 1> x1 = xt::zeros_like(theta);
        xt::xtensor<double, 1> y1 = xt::zeros_like(theta);
        xt::xtensor<double, 1> x2 = xt::zeros_like(theta);
        xt::xtensor<double, 1> y2 = xt::zeros_like(theta);
        std::size_t idx = 0;
        for (double th : theta) {
            x1[idx] =  a*cos(th + dt) * exp(b*th);
            y1[idx] =  a*sin(th + dt) * exp(b*th);

            x2[idx] =  a*cos(th + dt + M_PI/4.0) * exp(b*th);
            y2[idx] =  a*sin(th + dt + M_PI/4.0) * exp(b*th);
        }
        //xt::xtensor<double, 1> x1 = a*xt::cos(theta + dt) * xt::exp(b*theta);

        x1.insert(x1.end(), x2.rbegin(), x2.rend());
        y1.insert(y1.end(), y2.rbegin(), y2.rend());

        plt::fill(x1, y1, {});
    }
    plt::show();
}

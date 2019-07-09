#define _USE_MATH_DEFINES
#include <matplotlibcpp.h>
#include <cmath>
#include <iostream>

using namespace std;
namespace plt = matplotlibcpp;

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

int main() {
    // Prepare data.
    int n = 5000;
    xt::xtensor<double, 1> i = xt::linspace<double>(0, (n-1), n);
    std::array<std::size_t,1> shape{static_cast<std::size_t>(n)};
    xt::xtensor<double, 1> w(shape);
    w.fill(2.0);
    xt::xtensor<double, 1> x = xt::square(i);
    xt::xtensor<double, 1> y = xt::sin(2 * xt::numeric_constants<double>::PI * i / 360.0);
    xt::xtensor<double, 1> z = xt::log(i);

    // Prepare keywords to pass to PolyCollection. See
    // https://matplotlib.org/api/_as_gen/matplotlib.axes.Axes.fill_between.html
    std::map<string, string> keywords;
    keywords["alpha"] = "0.4";
    keywords["color"] = "grey";
    keywords["hatch"] = "-";

    plt::fill_between(x, y+2, z, keywords);
    plt::show();
}

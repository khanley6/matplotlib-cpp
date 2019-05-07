#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <matplotlibcpp.h>

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

namespace plt = matplotlibcpp;

int main()
{
    // Prepare data.
    int n = 5000;
    xt::xtensor<double, 1> i = xt::linspace<double>(0, (n-1), n);
    std::array<std::size_t,1> shape{static_cast<std::size_t>(n)};
    xt::xtensor<double, 1> w(shape);
    w.fill(2.0);
    xt::xtensor<double, 1> x = xt::square(i);
    xt::xtensor<double, 1> y = xt::sin(2 * xt::numeric_constants<double>::PI * i / 360.0);
    xt::xtensor<double, 1> z = xt::log(i);

    // Plot line from given x and y data. Color is selected automatically.
    plt::subplot(2,2,1);
    plt::plot(x, y);

    // Plot a red dashed line from given x and y data.
    plt::subplot(2,2,2);
    plt::plot(x, w,"r--");

    // Plot a line whose name will show up as "log(x)" in the legend.
    plt::subplot(2,2,3);
    plt::named_plot("log(x)", x, z);

    // Set x-axis to interval [0,1000000]
    plt::xlim(0, 1000*1000);

    // Add graph title
    plt::title("Sample figure");
    // Enable legend.
    plt::legend();

    plt::show(false);

    std::cout << "matplotlibcpp::show() is working in an non-blocking mode" << std::endl;
    std::getchar();
}

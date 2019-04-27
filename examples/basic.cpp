#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include "../matplotlibcpp.h"

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
    
    // Set the size of output image = 1200x780 pixels
    plt::figure_size(1200, 780);

    // Plot line from given x and y data. Color is selected automatically.
    plt::plot(x, y);

    // Plot a red dashed line from given x and y data.
    plt::plot(x, w,"r--");

    // Plot a line whose name will show up as "log(x)" in the legend.
    plt::named_plot("log(x)", x, z);

    // Set x-axis to interval [0,1000000]
    plt::xlim(0, 1000*1000);

    // Add graph title
    plt::title("Sample figure");

    // Enable legend.
    plt::legend();

    plt::show();
    // save figure
    //const char* filename = "./basic.png";
    //std::cout << "Saving result to " << filename << std::endl;;
    //plt::save(filename);
}

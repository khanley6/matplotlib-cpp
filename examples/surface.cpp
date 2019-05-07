#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <matplotlibcpp.h>

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

namespace plt = matplotlibcpp;

int main()
{
    std::vector<std::vector<double>> x, y, z;
    for (double i = -5; i <= 5;  i += 0.25) {
        std::vector<double> x_row, y_row, z_row;
        for (double j = -5; j <= 5; j += 0.25) {
            x_row.push_back(i);
            y_row.push_back(j);
            z_row.push_back(::std::sin(::std::hypot(i, j)));
        }
        x.push_back(x_row);
        y.push_back(y_row);
        z.push_back(z_row);
    }

    std::array<std::size_t,2> shape{x.size(), x[0].size()};
    xt::xtensor<double, 2> xx(shape), xy(shape), xz(shape);

    for (std::size_t i = 0; i < x.size(); ++i) {
        xt::view(xx, i, xt::all()) = xt::adapt(x[i]);
        xt::view(xy, i, xt::all()) = xt::adapt(y[i]);
        xt::view(xz, i, xt::all()) = xt::adapt(z[i]);
    }

    //plt::plot_surface(xx, xy, xz);
    plt::plot_surface(xx+2, xy+5, xz);
    plt::show();
}

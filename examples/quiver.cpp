#include <matplotlibcpp.h>

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

namespace plt = matplotlibcpp;

int main()
{
    // u and v are respectively the x and y components of the arrows we're plotting
    std::array<std::size_t, 1> shape{121};
    xt::xtensor<int,1> x(shape), y(shape), u(shape), v(shape);
    std::size_t idx = 0;
    for (int i = -5; i <= 5; i++) {
        for (int j = -5; j <= 5; j++) {
            x[idx] = (i);
            u[idx] = (-i);
            y[idx] = (j);
            v[idx] = (-j);
            ++idx;
        }
    }

    plt::quiver(x+2, y+2, u+3, v+1);
    plt::show();
}

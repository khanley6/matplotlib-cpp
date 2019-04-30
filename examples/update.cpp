#define _USE_MATH_DEFINES
#include <cmath>
#include "../matplotlibcpp.h"
#include <chrono>

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

namespace plt = matplotlibcpp;

template<class E1, class E2>
void update_window(const double x, const double y, const double t,
                   E1& xt, E2& yt)
{
    const double target_length = 300;
    const double half_win = (target_length/(2.*sqrt(1.+t*t)));

    xt[0] = x - half_win;
    xt[1] = x + half_win;
    yt[0] = y - half_win*t;
    yt[1] = y + half_win*t;
}


int main()
{
    size_t n = 1000;

    const double w = 0.05;
    const double a = n/2;

    xt::xtensor<double, 1> x = xt::linspace<double>(0, n-1, n);
    xt::xtensor<double, 1> y = a * xt::sin(w * x);

    xt::xtensor<double, 1> xt{0,0};
    xt::xtensor<double, 1> yt{0,0};

    plt::title("Tangent of a sine curve");
    // Plot sin once and for all.
    plt::named_plot("sin", x, y);

    plt::xlim(x[0], x[n-1]);
    plt::ylim(-a, a);
    plt::axis("equal");

    // Prepare plotting the tangent.
    plt::Plot plot("tangent");

    plt::legend();

    for (size_t i=0; i<n; i++) {
        if (i % 10 == 0) {
            update_window(x[i], y[i], a*w*cos(w*x[i]), xt, yt);

            // Just update data for this plot.
            plot.update(xt, yt);

            // Small pause so the viewer has a chance to enjoy the animation.
            plt::pause(0.1);
        }
   }
}

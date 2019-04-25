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
        vector<double> x1, y1, x2, y2;
        for (double th : theta) {
            x1.push_back( a*cos(th + dt) * exp(b*th) );
            y1.push_back( a*sin(th + dt) * exp(b*th) );

            x2.push_back( a*cos(th + dt + M_PI/4.0) * exp(b*th) );
            y2.push_back( a*sin(th + dt + M_PI/4.0) * exp(b*th) );
        }

        x1.insert(x1.end(), x2.rbegin(), x2.rend());
        y1.insert(y1.end(), y2.rbegin(), y2.rend());

        plt::fill(x1, y1, {});
    }
    plt::show();
}

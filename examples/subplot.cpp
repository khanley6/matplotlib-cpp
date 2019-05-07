#define _USE_MATH_DEFINES
#include <cmath>
#include <matplotlibcpp.h>

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

using namespace std;
namespace plt = matplotlibcpp;

int main() 
{
    // Prepare data
	int n = 500;
    xt::xtensor<double, 1> i = xt::linspace<double>(0, (n-1), n);
    std::array<std::size_t,1> shape{static_cast<std::size_t>(n)};
    xt::xtensor<double, 1> w(shape);
    w.fill(2.0);
    xt::xtensor<double, 1> x = xt::square(i);
    xt::xtensor<double, 1> y = xt::sin(2 * xt::numeric_constants<double>::PI * i / 360.0);
    xt::xtensor<double, 1> z = xt::log(i);

    // Set the "super title"
    plt::suptitle("My plot");
    plt::subplot(1, 2, 1);
	plt::plot(x, y+2, "r-");
    plt::subplot(1, 2, 2);
    plt::plot(x-50000, z-2, "k-");
    // Add some text to the plot
    plt::text(100, 90, "Hello!");


	// Show plots
	plt::show();
}

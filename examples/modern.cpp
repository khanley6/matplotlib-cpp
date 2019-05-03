#define _USE_MATH_DEFINES
#include <cmath>
#include "../matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xio.hpp>

int main() 
{
	// plot(y) - the x-coordinates are implicitly set to [0,1,...,n)
	//plt::plot({1,2,3,4}); 
	
	// Prepare data for parametric plot.
	int n = 5000; // number of data points
    xt::xtensor<double, 1> i = xt::linspace<double>(0, (n-1), n);
    xt::xtensor<double, 1> t = 2 * xt::numeric_constants<double>::PI * i / n;
    xt::xtensor<double, 1> x = 16*xt::sin(t)*xt::sin(t)*xt::sin(t);
    xt::xtensor<double, 1> y = 13*xt::cos(t) - 5*xt::cos(2*t) - 2*xt::cos(3*t) - xt::cos(4*t);

	// plot() takes an arbitrary number of (x,y,format)-triples. 
	// x must be iterable (that is, anything providing begin(x) and end(x)),
	// y must either be callable (providing operator() const) or iterable. 
    plt::plot(x, y, "r-", x, [](double d) { return 12.5+abs(sin(d)); }, "k-");


	// show plots
	plt::show();
}

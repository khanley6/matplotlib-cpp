#define _USE_MATH_DEFINES
#include <cmath>
#include "../matplotlibcpp.h"

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

namespace plt = matplotlibcpp;

int main()
{
	int n = 1000;
    xt::xtensor<double,1> x, y, z;

	for(int i=0; i<n; i++) {
        x = xt::concatenate(xt::xtuple(x, xt::xtensor<double,1>{static_cast<double>(i*i)}));
        y = xt::concatenate(xt::xtuple(y, xt::xtensor<double,1>{sin(2*M_PI*i/360.0)}));
        z = xt::concatenate(xt::xtuple(z, xt::xtensor<double,1>{log(i)}));

		if (i % 10 == 0) {
			// Clear previous plot
			plt::clf();
			// Plot line from given x and y data. Color is selected automatically.
			plt::plot(x, y);
			// Plot a line whose name will show up as "log(x)" in the legend.
			plt::named_plot("log(x)", x, z);

			// Set x-axis to interval [0,1000000]
			plt::xlim(0, n*n);

			// Add graph title
			plt::title("Sample figure");
			// Enable legend.
			plt::legend();
			// Display plot continuously
			plt::pause(0.01);
		}
	}
}

matplotlib-cpp-xt
=================

This is a fork of ![matplotlib-cpp](https://github.com/lava/matplotlib-cpp)
that supports plotting ![xtensor](https://github.com/QuantStack/xtensor) objects

The README below has been edited slightly to exhibit xtensor plotting capabilities.



matplotlib-cpp
==============

Welcome to matplotlib-cpp, possibly the simplest C++ plotting library.
It is built to resemble the plotting API used by Matlab and matplotlib.



Usage
-----
Complete minimal example:
```cpp
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;
int main() {
    plt::plot({1,3,2,4});
    plt::show();
}
```
    g++ minimal.cpp -std=c++14 -I/usr/include/python3.6 -lpython3.6


A more comprehensive example:
```cpp
#include "matplotlibcpp.h"
#include <cmath>

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

namespace plt = matplotlibcpp;

int main() 
{
    // Prepare data.
    int n = 5000;
    xt::xtensor<double, 1> i = xt::linspace<double>(0, (n-1), n);
    std::array<std::size_t,1> shape{static_cast<std::size_t>(n)};
    xt::xtensor<double, 1> w(shape); w.fill(2.0);
    xt::xtensor<double, 1> x = xt::square(i);
    xt::xtensor<double, 1> y = xt::sin(2 * xt::numeric_constants<double>::PI * i / 360.0);
    xt::xtensor<double, 1> z = xt::log(i);

    // Set the size of output image to 1200x780 pixels
    plt::figure_size(1200, 780);
    // Plot line from given x and y data. Color is selected automatically.
    plt::plot(x, y + 1.0); // rvalues are supported
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
    // Save the image (file format is determined by the extension)
    plt::save("./basic.png");
}
```
    g++ basic.cpp -std=c++14 -I/usr/include/python3.6 -lpython3.6


Alternatively, matplotlib-cpp also supports some C++11-powered syntactic sugar:
```cpp
#include <cmath>
#include "matplotlibcpp.h"

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

namespace plt = matplotlibcpp;

int main() 
{    
    // Prepare data.
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
```
    g++ modern.cpp -std=c++14 -I/usr/include/python3.6 -lpython3.6


Or some *funny-looking xkcd-styled* example:
```cpp
#include "matplotlibcpp.h"
#include <vector>
#include <cmath>

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

namespace plt = matplotlibcpp;

int main() {
    xt::xtensor<double, 1> t = xt::linspace<double>(0, 10, 1000);
    xt::xtensor<double, 1> x = xt::sin(2.0 * xt::numeric_constants<double>::PI * t);

    plt::xkcd();
    plt::plot(t, x);
    plt::title("AN ORDINARY SIN WAVE");
    plt::save("xkcd.png");
}

```
    g++ xkcd.cpp -std=c++14 -I/usr/include/python3.6 -lpython3.6


When working with vector fields, you might be interested in quiver plots:
```cpp
#include "../matplotlibcpp.h"

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
            x[idx] = (i)
            u[idx] = (-i);
            y[idx] = (j);
            v[idx] = (-j);
            ++idx;
        }
    }

    plt::quiver(x, y, u, v);
    plt::show();
}
```
    g++ quiver.cpp -std=c++14 -I/usr/include/python3.6 -lpython3.6


When working with 3d functions, you might be interested in 3d plots:
```cpp
#include "../matplotlibcpp.h"

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>
#include <xtensor/xview.hpp>

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

    plt::plot_surface(xx, xy, xz);
    plt::show();
}
```


Installation
------------

matplotlib-cpp works by wrapping the popular python plotting library matplotlib. (matplotlib.org)
This means you have to have a working python installation, including development headers.
On Ubuntu:

    sudo apt-get install python3-matplotlib python3-numpy python3-dev

If, for some reason, you're unable to get a working installation of numpy on your system,
you can add the define `WITHOUT_NUMPY` to erase this dependency.

The C++-part of the library consists of the single header file `matplotlibcpp.h` which can be placed
anywhere.

Since a python interpreter is opened internally, it is necessary to link against `libpython` in order to use
matplotlib-cpp.

# CMake

If you prefer to use CMake as build system, you will want to add something like this to your
CMakeLists.txt:
```cmake
find_package(PythonLibs 2.7)
target_include_directories(myproject PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(myproject ${PYTHON_LIBRARIES})
```

# C++14

c++14 is now required as `xtensor` uses c++14 features.

Currently, c++11 is required to build matplotlib-cpp. The last working commit that did
not have this requirement was `717e98e752260245407c5329846f5d62605eff08`.

Note that support for c++98 was dropped more or less accidentally, so if you have to work
with an ancient compiler and still want to enjoy the latest additional features, I'd
probably merge a PR that restores support.

# Python 2

This library supports both python2 and python3 (although the python3 support is probably far less tested,
so it is recommended to prefer python2.7). To switch the used python version, simply change
the compiler flags accordingly.

    g++ example.cpp -I/usr/include/python3.6 -lpython3.6

The same technique can be used for linking against a custom build of python

    g++ example.cpp -I/usr/local/include/fancy-python4 -L/usr/local/lib -lfancy-python4


Why?
----
I initially started this library during my diploma thesis. The usual approach of 
writing data from the c++ algorithm to a file and afterwards parsing and plotting
it in python using matplotlib proved insufficient: Keeping the algorithm
and plotting code in sync requires a lot of effort when the C++ code frequently and substantially 
changes. Additionally, the python yaml parser was not able to cope with files that
exceed a few hundred megabytes in size.

Therefore, I was looking for a C++ plotting library that was extremely easy to use
and to add into an existing codebase, preferably header-only. When I found
none, I decided to write one myself, which is basically a C++ wrapper around
matplotlib. As you can see from the above examples, plotting data and saving it
to an image file can be done as few as two lines of code.

The general approach of providing a simple C++ API for utilizing python code
was later generalized and extracted into a separate, more powerful
library in another project of mine, [wrappy](http://www.github.com/lava/wrappy).


Todo/Issues/Wishlist
--------------------
* This library is not thread safe. Protect all concurrent access with a mutex.
  Sadly, this is not easy to fix since it is not caused by the library itself but
  by the python interpreter, which is itself not thread-safe.

* It would be nice to have a more object-oriented design with a Plot class which would allow
  multiple independent plots per program.

* Right now, only a small subset of matplotlibs functionality is exposed. Stuff like xlabel()/ylabel() etc. should
  be easy to add.

* If you use Anaconda on Windows, you might need to set PYTHONHOME to Anaconda home directory and QT_QPA_PLATFORM_PLUGIN_PATH to %PYTHONHOME%Library/plugins/platforms. The latter is for especially when you get the error which says 'This application failed to start because it could not find or load the Qt platform plugin "windows"
in "".'

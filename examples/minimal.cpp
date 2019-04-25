#include "../matplotlibcpp.h"

namespace plt = matplotlibcpp;

#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor.hpp>

int main() {
    plt::plot({1,3,2,4});
    plt::show();
}

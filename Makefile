#examples: minimal basic modern animation nonblock xkcd quiver bar surface fill_inbetween fill update
examples: minimal basic animation nonblock xkcd quiver bar surface fill_inbetween fill update

minimal: examples/minimal.cpp matplotlibcpp.h
	cd examples && g++ -DWITHOUT_NUMPY minimal.cpp -I/usr/include/python3.7m -lpython3.7m -o minimal -std=c++17

basic: examples/basic.cpp matplotlibcpp.h
#all: examples/basic.cpp matplotlibcpp.h
	cd examples && g++ basic.cpp -I/usr/include/python3.7m -I/usr/lib/python3.7/site-packages/numpy/core/include -lpython3.7m -o basic -std=c++17

modern: examples/modern.cpp matplotlibcpp.h
	cd examples && g++ modern.cpp -I/usr/include/python3.7m -I/usr/lib/python3.7/site-packages/numpy/core/include -lpython3.7m -o modern -std=c++17

animation: examples/animation.cpp matplotlibcpp.h
	cd examples && g++ animation.cpp -I/usr/include/python3.7m -I/usr/lib/python3.7/site-packages/numpy/core/include -lpython3.7m -o animation -std=c++17

nonblock: examples/nonblock.cpp matplotlibcpp.h
	cd examples && g++ nonblock.cpp -I/usr/include/python3.7m -I/usr/lib/python3.7/site-packages/numpy/core/include -lpython3.7m -o nonblock -std=c++17

quiver: examples/quiver.cpp matplotlibcpp.h
	cd examples && g++ quiver.cpp -I/usr/include/python3.7m -I/usr/lib/python3.7/site-packages/numpy/core/include -lpython3.7m -o quiver -std=c++17

xkcd: examples/xkcd.cpp matplotlibcpp.h
	cd examples && g++ xkcd.cpp -I/usr/include/python3.7m -I/usr/lib/python3.7/site-packages/numpy/core/include -lpython3.7m -o xkcd -std=c++17

bar: examples/bar.cpp matplotlibcpp.h
	cd examples && g++ bar.cpp -I/usr/include/python3.7m -I/usr/lib/python3.7/site-packages/numpy/core/include -lpython3.7m -o bar -std=c++17

surface: examples/surface.cpp matplotlibcpp.h
	cd examples && g++ surface.cpp -I/usr/include/python3.7m -I/usr/lib/python3.7/site-packages/numpy/core/include -lpython3.7m -o surface -std=c++17

fill_inbetween: examples/fill_inbetween.cpp matplotlibcpp.h
	cd examples && g++ fill_inbetween.cpp -I/usr/include/python3.7m -I/usr/lib/python3.7/site-packages/numpy/core/include -lpython3.7m -o fill_inbetween -std=c++17

fill: examples/fill.cpp matplotlibcpp.h
	cd examples && g++ fill.cpp -I/usr/include/python3.7m -I/usr/lib/python3.7/site-packages/numpy/core/include -lpython3.7m -o fill -std=c++17
	
update: examples/update.cpp matplotlibcpp.h
	cd examples && g++ update.cpp -I/usr/include/python3.7m -I/usr/lib/python3.7/site-packages/numpy/core/include -lpython3.7m -o update -std=c++17

clean:
	rm -f examples/{minimal,basic,modern,animation,nonblock,xkcd,quiver,bar,surface,fill_inbetween,fill,update}

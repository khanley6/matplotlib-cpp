#pragma once

#include <vector>
#include <map>
#include <array>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstdint> // <cstdint> requires c++11 support
#include <functional>

#include <Python.h>

#ifndef WITHOUT_NUMPY
#  define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#  include <numpy/arrayobject.h>
#endif // WITHOUT_NUMPY

#if PY_MAJOR_VERSION >= 3
#  define PyString_FromString PyUnicode_FromString
#  define PyInt_FromLong PyLong_FromLong
#  define PyString_FromString PyUnicode_FromString
#endif

#include <xtensor/xadapt.hpp>
#include <xtensor/xbuilder.hpp>
#include <xtensor/xtensor_forward.hpp>
#include <xtensor/xview.hpp>

namespace matplotlibcpp {
namespace detail {

    static std::string s_backend;

    struct _interpreter {
        PyObject *s_python_function_imshow;
        PyObject *s_python_function_show;
        PyObject *s_python_function_close;
        PyObject *s_python_function_draw;
        PyObject *s_python_function_pause;
        PyObject *s_python_function_save;
        PyObject *s_python_function_figure;
        PyObject *s_python_function_fignum_exists;
        PyObject *s_python_function_plot;
        PyObject *s_python_function_quiver;
        PyObject *s_python_function_semilogx;
        PyObject *s_python_function_semilogy;
        PyObject *s_python_function_loglog;
        PyObject *s_python_function_fill;
        PyObject *s_python_function_fill_between;
        PyObject *s_python_function_hist;
        PyObject *s_python_function_scatter;
        PyObject *s_python_function_subplot;
        PyObject *s_python_function_legend;
        PyObject *s_python_function_xlim;
        PyObject *s_python_function_ion;
        PyObject *s_python_function_ginput;
        PyObject *s_python_function_ylim;
        PyObject *s_python_function_title;
        PyObject *s_python_function_axis;
        PyObject *s_python_function_xlabel;
        PyObject *s_python_function_ylabel;
        PyObject *s_python_function_xticks;
        PyObject *s_python_function_yticks;
        PyObject *s_python_function_grid;
        PyObject *s_python_function_clf;
        PyObject *s_python_function_errorbar;
        PyObject *s_python_function_annotate;
        PyObject *s_python_function_tight_layout;
        PyObject *s_python_colormap;
        PyObject *s_python_empty_tuple;
        PyObject *s_python_function_stem;
        PyObject *s_python_function_xkcd;
        PyObject *s_python_function_text;
        PyObject *s_python_function_suptitle;
        PyObject *s_python_function_bar;
        PyObject *s_python_function_subplots_adjust;


        /* For now, _interpreter is implemented as a singleton since its currently not possible to have
           multiple independent embedded python interpreters without patching the python source code
           or starting a separate process for each.
        http://bytes.com/topic/python/answers/793370-multiple-independent-python-interpreters-c-c-program
        */

        static _interpreter& get() {
            static _interpreter ctx;
            return ctx;
        }

    private:

#ifndef WITHOUT_NUMPY
#  if PY_MAJOR_VERSION >= 3

        void *import_numpy() {
            import_array(); // initialize C-API
            return NULL;
        }

#  else

        void import_numpy() {
            import_array(); // initialize C-API
        }

#  endif
#endif

        _interpreter() {

            // optional but recommended
#if PY_MAJOR_VERSION >= 3
            wchar_t name[] = L"plotting";
#else
            char name[] = "plotting";
#endif
            Py_SetProgramName(name);
            Py_Initialize();

#ifndef WITHOUT_NUMPY
            import_numpy(); // initialize numpy C-API
#endif

            PyObject* matplotlibname = PyString_FromString("matplotlib");
            PyObject* pyplotname = PyString_FromString("matplotlib.pyplot");
            PyObject* cmname  = PyString_FromString("matplotlib.cm");
            PyObject* pylabname  = PyString_FromString("pylab");
            if (!pyplotname || !pylabname || !matplotlibname || !cmname) {
                throw std::runtime_error("couldnt create string");
            }

            PyObject* matplotlib = PyImport_Import(matplotlibname);
            Py_DECREF(matplotlibname);
            if (!matplotlib) {
                PyErr_Print();
                throw std::runtime_error("Error loading module matplotlib!");
            }

            // matplotlib.use() must be called *before* pylab, matplotlib.pyplot,
            // or matplotlib.backends is imported for the first time
            if (!s_backend.empty()) {
                PyObject_CallMethod(matplotlib, const_cast<char*>("use"), const_cast<char*>("s"), s_backend.c_str());
            }

            PyObject* pymod = PyImport_Import(pyplotname);
            Py_DECREF(pyplotname);
            if (!pymod) { throw std::runtime_error("Error loading module matplotlib.pyplot!"); }

            s_python_colormap = PyImport_Import(cmname);
            Py_DECREF(cmname);
            if (!s_python_colormap) { throw std::runtime_error("Error loading module matplotlib.cm!"); }

            PyObject* pylabmod = PyImport_Import(pylabname);
            Py_DECREF(pylabname);
            if (!pylabmod) { throw std::runtime_error("Error loading module pylab!"); }

            s_python_function_imshow = PyObject_GetAttrString(pymod, "imshow");
            s_python_function_show = PyObject_GetAttrString(pymod, "show");
            s_python_function_close = PyObject_GetAttrString(pymod, "close");
            s_python_function_draw = PyObject_GetAttrString(pymod, "draw");
            s_python_function_pause = PyObject_GetAttrString(pymod, "pause");
            s_python_function_figure = PyObject_GetAttrString(pymod, "figure");
            s_python_function_fignum_exists = PyObject_GetAttrString(pymod, "fignum_exists");
            s_python_function_plot = PyObject_GetAttrString(pymod, "plot");
            s_python_function_quiver = PyObject_GetAttrString(pymod, "quiver");
            s_python_function_semilogx = PyObject_GetAttrString(pymod, "semilogx");
            s_python_function_semilogy = PyObject_GetAttrString(pymod, "semilogy");
            s_python_function_loglog = PyObject_GetAttrString(pymod, "loglog");
            s_python_function_fill = PyObject_GetAttrString(pymod, "fill");
            s_python_function_fill_between = PyObject_GetAttrString(pymod, "fill_between");
            s_python_function_hist = PyObject_GetAttrString(pymod,"hist");
            s_python_function_scatter = PyObject_GetAttrString(pymod,"scatter");
            s_python_function_subplot = PyObject_GetAttrString(pymod, "subplot");
            s_python_function_legend = PyObject_GetAttrString(pymod, "legend");
            s_python_function_ylim = PyObject_GetAttrString(pymod, "ylim");
            s_python_function_title = PyObject_GetAttrString(pymod, "title");
            s_python_function_axis = PyObject_GetAttrString(pymod, "axis");
            s_python_function_xlabel = PyObject_GetAttrString(pymod, "xlabel");
            s_python_function_ylabel = PyObject_GetAttrString(pymod, "ylabel");
            s_python_function_xticks = PyObject_GetAttrString(pymod, "xticks");
            s_python_function_yticks = PyObject_GetAttrString(pymod, "yticks");
            s_python_function_grid = PyObject_GetAttrString(pymod, "grid");
            s_python_function_xlim = PyObject_GetAttrString(pymod, "xlim");
            s_python_function_ion = PyObject_GetAttrString(pymod, "ion");
            s_python_function_ginput = PyObject_GetAttrString(pymod, "ginput");
            s_python_function_save = PyObject_GetAttrString(pylabmod, "savefig");
            s_python_function_annotate = PyObject_GetAttrString(pymod,"annotate");
            s_python_function_clf = PyObject_GetAttrString(pymod, "clf");
            s_python_function_errorbar = PyObject_GetAttrString(pymod, "errorbar");
            s_python_function_tight_layout = PyObject_GetAttrString(pymod, "tight_layout");
            s_python_function_stem = PyObject_GetAttrString(pymod, "stem");
            s_python_function_xkcd = PyObject_GetAttrString(pymod, "xkcd");
            s_python_function_text = PyObject_GetAttrString(pymod, "text");
            s_python_function_suptitle = PyObject_GetAttrString(pymod, "suptitle");
            s_python_function_bar = PyObject_GetAttrString(pymod,"bar");
            s_python_function_subplots_adjust = PyObject_GetAttrString(pymod,"subplots_adjust");

            if (   !s_python_function_show
                || !s_python_function_imshow
                || !s_python_function_close
                || !s_python_function_draw
                || !s_python_function_pause
                || !s_python_function_figure
                || !s_python_function_fignum_exists
                || !s_python_function_plot
                || !s_python_function_quiver
                || !s_python_function_semilogx
                || !s_python_function_semilogy
                || !s_python_function_loglog
                || !s_python_function_fill
                || !s_python_function_fill_between
                || !s_python_function_subplot
                || !s_python_function_legend
                || !s_python_function_ylim
                || !s_python_function_title
                || !s_python_function_axis
                || !s_python_function_xlabel
                || !s_python_function_ylabel
                || !s_python_function_grid
                || !s_python_function_xlim
                || !s_python_function_ion
                || !s_python_function_ginput
                || !s_python_function_save
                || !s_python_function_clf
                || !s_python_function_annotate
                || !s_python_function_errorbar
                || !s_python_function_errorbar
                || !s_python_function_tight_layout
                || !s_python_function_stem
                || !s_python_function_xkcd
                || !s_python_function_text
                || !s_python_function_suptitle
                || !s_python_function_bar
                || !s_python_function_subplots_adjust
                ) { throw std::runtime_error("Couldn't find required function!"); }

            if (   !PyFunction_Check(s_python_function_show)
                || !PyFunction_Check(s_python_function_imshow)
                || !PyFunction_Check(s_python_function_close)
                || !PyFunction_Check(s_python_function_draw)
                || !PyFunction_Check(s_python_function_pause)
                || !PyFunction_Check(s_python_function_figure)
                || !PyFunction_Check(s_python_function_fignum_exists)
                || !PyFunction_Check(s_python_function_plot)
                || !PyFunction_Check(s_python_function_quiver)
                || !PyFunction_Check(s_python_function_semilogx)
                || !PyFunction_Check(s_python_function_semilogy)
                || !PyFunction_Check(s_python_function_loglog)
                || !PyFunction_Check(s_python_function_fill)
                || !PyFunction_Check(s_python_function_fill_between)
                || !PyFunction_Check(s_python_function_subplot)
                || !PyFunction_Check(s_python_function_legend)
                || !PyFunction_Check(s_python_function_annotate)
                || !PyFunction_Check(s_python_function_ylim)
                || !PyFunction_Check(s_python_function_title)
                || !PyFunction_Check(s_python_function_axis)
                || !PyFunction_Check(s_python_function_xlabel)
                || !PyFunction_Check(s_python_function_ylabel)
                || !PyFunction_Check(s_python_function_grid)
                || !PyFunction_Check(s_python_function_xlim)
                || !PyFunction_Check(s_python_function_ion)
                || !PyFunction_Check(s_python_function_ginput)
                || !PyFunction_Check(s_python_function_save)
                || !PyFunction_Check(s_python_function_clf)
                || !PyFunction_Check(s_python_function_tight_layout)
                || !PyFunction_Check(s_python_function_errorbar)
                || !PyFunction_Check(s_python_function_stem)
                || !PyFunction_Check(s_python_function_xkcd)
                || !PyFunction_Check(s_python_function_text)
                || !PyFunction_Check(s_python_function_suptitle)
                || !PyFunction_Check(s_python_function_bar)
                || !PyFunction_Check(s_python_function_subplots_adjust)
                ) { throw std::runtime_error("Python object is unexpectedly not a PyFunction."); }

            s_python_empty_tuple = PyTuple_New(0);
        }

        ~_interpreter() {
            Py_Finalize();
        }
    };

} // end namespace detail

    // must be called before the first regular call to matplotlib to have any effect
    inline void backend(const std::string& name) {
        detail::s_backend = name;
    }


    inline bool annotate(std::string annotation, double x, double y) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject * xy = PyTuple_New(2);
        PyObject * str = PyString_FromString(annotation.c_str());

        PyTuple_SetItem(xy,0,PyFloat_FromDouble(x));
        PyTuple_SetItem(xy,1,PyFloat_FromDouble(y));

        PyObject* kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "xy", xy);

        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, str);

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_annotate, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);

        return res;
    }


#ifndef WITHOUT_NUMPY
    // Type selector for numpy array conversion
    template <typename T> struct select_npy_type { const static NPY_TYPES type = NPY_NOTYPE; }; //Default
    template <> struct select_npy_type<double> { const static NPY_TYPES type = NPY_DOUBLE; };
    template <> struct select_npy_type<float> { const static NPY_TYPES type = NPY_FLOAT; };
    template <> struct select_npy_type<bool> { const static NPY_TYPES type = NPY_BOOL; };
    template <> struct select_npy_type<int8_t> { const static NPY_TYPES type = NPY_INT8; };
    template <> struct select_npy_type<int16_t> { const static NPY_TYPES type = NPY_SHORT; };
    template <> struct select_npy_type<int32_t> { const static NPY_TYPES type = NPY_INT; };
    template <> struct select_npy_type<int64_t> { const static NPY_TYPES type = NPY_INT64; };
    template <> struct select_npy_type<uint8_t> { const static NPY_TYPES type = NPY_UINT8; };
    template <> struct select_npy_type<uint16_t> { const static NPY_TYPES type = NPY_USHORT; };
    template <> struct select_npy_type<uint32_t> { const static NPY_TYPES type = NPY_ULONG; };
    template <> struct select_npy_type<uint64_t> { const static NPY_TYPES type = NPY_UINT64; };
#endif

#ifdef WITHOUT_NUMPY
    // vector implementation
    template <typename Numeric>
    PyObject* get_array_impl(const std::vector<Numeric>& v) {
        PyObject* list = PyList_New(v.size());
        for(size_t i = 0; i < v.size(); ++i) {
            PyList_SetItem(list, i, PyFloat_FromDouble(v.at(i)));
        }
        return list;
    }

    // lvalue xexpression implementation
    template <class E>
    PyObject* get_array_impl(const xt::xexpression<E>& e) {
        const E& de = e.derived_cast();
        // Implementation for evaluated lvalue using de

        PyObject* list;
        if (de.dimension() == 1) {
            list = PyList_New(de.size());
            for(size_t i = 0; i < de.size(); ++i) {
                PyList_SetItem(list, i, PyFloat_FromDouble(de.at(i)));
            }
        } else {
            std::size_t num_rows = de.shape()[0];
            std::size_t num_cols = de.shape()[1];
            list = PyList_New(num_rows);
            for(size_t i = 0; i < num_rows; ++i) {
                PyObject* row = PyList_New(num_cols);
                PyList_SetItem(list, i, row);
                for(size_t j = 0; j < num_cols; ++j) {
                    PyList_SetItem(row, j, PyFloat_FromDouble(de.at(i,j)));
                }
            }
        }
        return list;
    }

#else // With numpy
    // vector implementation
    template <typename Numeric>
    PyObject* get_array_impl(const std::vector<Numeric>& v) {
        detail::_interpreter::get();
        NPY_TYPES type = select_npy_type<Numeric>::type;
        if (type == NPY_NOTYPE) {
            std::vector<double> vd(v.size());
            npy_intp vsize = v.size();
            std::copy(v.begin(), v.end(), vd.begin());
            PyObject* varray = PyArray_SimpleNewFromData(1, &vsize, NPY_DOUBLE, (void*)(vd.data()));
            return varray;
        }

        npy_intp vsize = v.size();
        PyObject* varray = PyArray_SimpleNewFromData(1, &vsize, type, (void*)(v.data()));
        return varray;
    }

    // lvalue xexpression implementation
    template <class E>
    PyObject* get_array_impl(const xt::xexpression<E>& e) {
        const E& de = e.derived_cast();
        // Implementation for evaluated lvalue using de
        assert(de.dimension() <= 2);
        detail::_interpreter::get();    //interpreter needs to be initialized for the numpy commands to work

        if (de.dimension() == 1) {
            NPY_TYPES type = select_npy_type<typename std::decay_t<E>::value_type>::type;
            if (type == NPY_NOTYPE) {
                std::vector<double> vd(de.size());
                npy_intp vsize = de.size();
                std::copy(de.begin(), de.end(), vd.begin());
                PyObject* varray = PyArray_SimpleNewFromData(1, &vsize, NPY_DOUBLE, (void*)(vd.data()));
                return varray;
            }

            npy_intp vsize = de.size();
            PyObject* varray = PyArray_SimpleNewFromData(1, &vsize, type, (void*)(de.data()));
            return varray;
        } else {
            if (de.size() < 1) throw std::runtime_error("get_2d_array v too small");

            npy_intp vsize[2] = {static_cast<npy_intp>(de.shape()[0]),
                                 static_cast<npy_intp>(de.shape()[1])};

            PyArrayObject *varray =
                (PyArrayObject *)PyArray_SimpleNew(2, vsize, NPY_DOUBLE);

            double *vd_begin = static_cast<double *>(PyArray_DATA(varray));

            for (const std::size_t v_row_num : xt::arange(de.shape()[0])) {
                auto v_row = xt::view(de, v_row_num);
                if (v_row.size() != static_cast<size_t>(vsize[1]))
                    throw std::runtime_error("Missmatched array size");
                std::copy(v_row.begin(), v_row.end(), vd_begin);
                vd_begin += vsize[1];
            }

            return reinterpret_cast<PyObject *>(varray);
        }
    }

#endif // WITHOUT_NUMPY

    // rvalue xexpression implementation
    template <class E>
    PyObject* get_array_impl(xt::xexpression<E>&& e) {
        E de = e.derived_cast();

        PyObject* list;
        if (de.dimension() == 1) {
            list = PyList_New(de.size());
            for(size_t i = 0; i < de.size(); ++i) {
                PyList_SetItem(list, i, PyFloat_FromDouble(de.at(i)));
            }
        } else {
            std::size_t num_rows = de.shape()[0];
            std::size_t num_cols = de.shape()[1];
            list = PyList_New(num_rows);
            for(size_t i = 0; i < num_rows; ++i) {
                PyObject* row = PyList_New(num_cols);
                PyList_SetItem(list, i, row);
                for(size_t j = 0; j < num_cols; ++j) {
                    PyList_SetItem(row, j, PyFloat_FromDouble(de.at(i,j)));
                }
            }
        }
        return list;
    }

    // xtensor objects
    template <class E, XTL_REQUIRES(xt::is_xexpression<E>)>
    PyObject* get_array(E&& e) {
        return get_array_impl(xt::eval(std::forward<E>(e)));
    }

    // vectors
    template <class E, XTL_REQUIRES(xtl::negation<xt::is_xexpression<E>>)>
    PyObject* get_array(E&& e) {
        return get_array_impl(std::forward<E>(e));
    }


    template<class E1, class E2>
    bool plot(E1&& x, E2&& y, const std::map<std::string, std::string>& keywords) {
        assert(x.size() == y.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        // using numpy arrays
        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        // construct positional args
        PyObject* args = PyTuple_New(2);
        PyTuple_SetItem(args, 0, xarray);
        PyTuple_SetItem(args, 1, yarray);

        // construct keyword args
        PyObject* kwargs = PyDict_New();
        for(auto it = keywords.cbegin(); it != keywords.cend(); ++it)
        {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyString_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_plot, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);

        return res;
    }

    template<class E>
    bool imshow(E&& im, const std::map<std::string, std::string> &keywords =
                        std::map<std::string, std::string>()) {
        assert(im.dimension() == 2);

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        // using numpy arrays
        PyObject* imarray = get_array(std::forward<E>(im));

        // construct positional args
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, imarray);

        // construct keyword args
        PyObject* kwargs = PyDict_New();
        for(auto it = keywords.cbegin(); it != keywords.cend(); ++it)
        {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyString_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_imshow, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);

        return res;
    }


#ifndef WITHOUT_NUMPY
    template <class E1, class E2, class E3>
    void plot_surface(E1&& x, E2&& y, E3&& z,
                      const std::map<std::string, std::string> &keywords =
                      std::map<std::string, std::string>()) {
        // Passing a list of lists as the z argument results in python throwing:
        //   `AttributeError: 'list' object has no attribute 'ndim'`
        static_assert(std::is_lvalue_reference<E3>::value,
                "z argument to surface is required to be an lvalue");

        // We lazily load the modules here the first time this function is called
        // because I'm not sure that we can assume "matplotlib installed" implies
        // "mpl_toolkits installed" on all platforms, and we don't want to require
        // it for people who don't need 3d plots.

        // interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        static PyObject *mpl_toolkitsmod = nullptr, *axis3dmod = nullptr;
        if (!mpl_toolkitsmod) {
            PyObject* mpl_toolkits = PyString_FromString("mpl_toolkits");
            PyObject* axis3d = PyString_FromString("mpl_toolkits.mplot3d");
            if (!mpl_toolkits || !axis3d) 
                throw std::runtime_error("couldnt create string");

            mpl_toolkitsmod = PyImport_Import(mpl_toolkits);
            Py_DECREF(mpl_toolkits);
            if (!mpl_toolkitsmod)
                throw std::runtime_error("Error loading module mpl_toolkits!");

            axis3dmod = PyImport_Import(axis3d);
            Py_DECREF(axis3d);
            if (!axis3dmod)
                throw std::runtime_error("Error loading module mpl_toolkits.mplot3d!");
        }

        assert(x.size() == y.size());
        assert(y.size() == z.size());

        // using numpy arrays
        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));
        PyObject* zarray = get_array(std::forward<E3>(z));

        // construct positional args
        PyObject *args = PyTuple_New(3);
        PyTuple_SetItem(args, 0, xarray);
        PyTuple_SetItem(args, 1, yarray);
        PyTuple_SetItem(args, 2, zarray);

        // Build up the kw args.
        PyObject *kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "rstride", PyInt_FromLong(1));
        PyDict_SetItemString(kwargs, "cstride", PyInt_FromLong(1));

        PyObject *python_colormap_coolwarm = PyObject_GetAttrString(
                detail::_interpreter::get().s_python_colormap, "coolwarm");

        PyDict_SetItemString(kwargs, "cmap", python_colormap_coolwarm);

        for (auto it = keywords.cbegin(); it != keywords.cend(); ++it) {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyString_FromString(it->second.c_str()));
        }


        PyObject *fig = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_figure,
                detail::_interpreter::get().s_python_empty_tuple);

        if (!fig) throw std::runtime_error("Call to figure() failed.");

        PyObject *gca_kwargs = PyDict_New();
        PyDict_SetItemString(gca_kwargs, "projection", PyString_FromString("3d"));

        PyObject *gca = PyObject_GetAttrString(fig, "gca");
        if (!gca) throw std::runtime_error("No gca");
        
        Py_INCREF(gca);
        PyObject *axis = PyObject_Call(
                gca, detail::_interpreter::get().s_python_empty_tuple, gca_kwargs);

        if (!axis) throw std::runtime_error("No axis");
        Py_INCREF(axis);

        Py_DECREF(gca);
        Py_DECREF(gca_kwargs);

        PyObject *plot_surface = PyObject_GetAttrString(axis, "plot_surface");
        if (!plot_surface) throw std::runtime_error("No surface");
        Py_INCREF(plot_surface);
        PyObject *res = PyObject_Call(plot_surface, args, kwargs);
        if (!res) throw std::runtime_error("failed surface");
        Py_DECREF(plot_surface);

        Py_DECREF(axis);
        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);
    }
#endif // WITHOUT_NUMPY


    template<class E1, class E2>
    bool stem(E1&& x, E2&& y, const std::map<std::string, std::string>& keywords) {
        assert(x.size() == y.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        // using numpy arrays
        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        // construct positional args
        PyObject* args = PyTuple_New(2);
        PyTuple_SetItem(args, 0, xarray);
        PyTuple_SetItem(args, 1, yarray);

        // construct keyword args
        PyObject* kwargs = PyDict_New();
        for (auto it = keywords.cbegin(); it != keywords.cend(); ++it) {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyString_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_stem, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2>
    bool fill(E1&& x, E2&& y, const std::map<std::string, std::string>& keywords) {
        assert(x.size() == y.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        // using numpy arrays
        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        // construct positional args
        PyObject* args = PyTuple_New(2);
        PyTuple_SetItem(args, 0, xarray);
        PyTuple_SetItem(args, 1, yarray);

        // construct keyword args
        PyObject* kwargs = PyDict_New();
        for (auto it = keywords.cbegin(); it != keywords.cend(); ++it) {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyUnicode_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_fill, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2, class E3>
    bool fill_between(E1&& x, E2&& y1, E3&& y2, const std::map<std::string, std::string>& keywords) {
        assert(x.size() == y1.size());
        assert(x.size() == y2.size());
        detail::_interpreter::get();    //interpreter needs to be initialized for the numpy commands to work

        // using numpy arrays
        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* y1array = get_array(std::forward<E2>(y1));
        PyObject* y2array = get_array(std::forward<E3>(y2));

        // construct positional args
        PyObject* args = PyTuple_New(3);
        PyTuple_SetItem(args, 0, xarray);
        PyTuple_SetItem(args, 1, y1array);
        PyTuple_SetItem(args, 2, y2array);

        // construct keyword args
        PyObject* kwargs = PyDict_New();
        for(auto it = keywords.cbegin(); it != keywords.cend(); ++it) {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyUnicode_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_fill_between, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E>
    bool hist(E&& y, long bins=10, std::string color="b", double alpha=1.0, bool cumulative=false) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* yarray = get_array(std::forward<E>(y));

        PyObject* kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "bins", PyLong_FromLong(bins));
        PyDict_SetItemString(kwargs, "color", PyString_FromString(color.c_str()));
        PyDict_SetItemString(kwargs, "alpha", PyFloat_FromDouble(alpha));
        PyDict_SetItemString(kwargs, "cumulative", cumulative ? Py_True : Py_False);

        PyObject* plot_args = PyTuple_New(1);

        PyTuple_SetItem(plot_args, 0, yarray);

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_hist, plot_args, kwargs);

        Py_DECREF(plot_args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2>
    bool scatter(E1&& x, E2&& y, const double s=1.0) {
        assert(x.size() == y.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        PyObject* kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "s", PyLong_FromLong(s));

        PyObject* plot_args = PyTuple_New(2);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_scatter, plot_args, kwargs);

        Py_DECREF(plot_args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E>
    bool bar(E&& y, std::string ec = "black", std::string ls = "-", double lw = 1.0,
            const std::map<std::string, std::string>& keywords = {}) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* yarray = get_array(std::forward<E>(y));
        PyObject* xarray = get_array(xt::arange<typename std::decay_t<E>::value_type>(y.size()));

        PyObject* kwargs = PyDict_New();

        PyDict_SetItemString(kwargs, "ec", PyString_FromString(ec.c_str()));
        PyDict_SetItemString(kwargs, "ls", PyString_FromString(ls.c_str()));
        PyDict_SetItemString(kwargs, "lw", PyFloat_FromDouble(lw));

        PyObject* plot_args = PyTuple_New(2);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_bar, plot_args, kwargs);

        Py_DECREF(plot_args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);

        return res;
    }


    inline bool subplots_adjust(const std::map<std::string, double>& keywords = {}) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* kwargs = PyDict_New();
        for (auto it = keywords.cbegin(); it != keywords.cend(); ++it) {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyFloat_FromDouble(it->second));
        }

        PyObject* plot_args = PyTuple_New(0);

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_subplots_adjust, plot_args, kwargs);

        Py_DECREF(plot_args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E>
    bool named_hist(std::string label, E&& y, long bins=10, std::string color="b", double alpha=1.0) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* yarray = get_array(std::forward<E>(y));

        PyObject* kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "label", PyString_FromString(label.c_str()));
        PyDict_SetItemString(kwargs, "bins", PyLong_FromLong(bins));
        PyDict_SetItemString(kwargs, "color", PyString_FromString(color.c_str()));
        PyDict_SetItemString(kwargs, "alpha", PyFloat_FromDouble(alpha));

        PyObject* plot_args = PyTuple_New(1);
        PyTuple_SetItem(plot_args, 0, yarray);

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_hist, plot_args, kwargs);

        Py_DECREF(plot_args);
        Py_DECREF(kwargs);
        if (res) Py_DECREF(res);

        return res;
    }


    // enable_if used to fix overload resolution on variadic call to plot (see below) when the
    // last argument is a callable, such as a lambda
    template<class E1, class E2, typename = std::enable_if_t<xt::is_xexpression<E2>::value>>
    bool plot(E1&& x, E2&& y, const std::string& s = "") {
        assert(x.size() == y.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        PyObject* pystring = PyString_FromString(s.c_str());

        PyObject* plot_args = PyTuple_New(3);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);
        PyTuple_SetItem(plot_args, 2, pystring);

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_plot, plot_args);

        Py_DECREF(plot_args);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2, class E3, class E4>
    bool quiver(E1&& x, E2&& y, E3&& u, E4&& w,
                const std::map<std::string, std::string>& keywords = {}) {
        assert(x.size() == y.size() && x.size() == u.size() && u.size() == w.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));
        PyObject* uarray = get_array(std::forward<E3>(u));
        PyObject* warray = get_array(std::forward<E4>(w));

        PyObject* plot_args = PyTuple_New(4);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);
        PyTuple_SetItem(plot_args, 2, uarray);
        PyTuple_SetItem(plot_args, 3, warray);

        // construct keyword args
        PyObject* kwargs = PyDict_New();
        for(auto it = keywords.cbegin(); it != keywords.cend(); ++it)
        {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyUnicode_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_quiver, plot_args, kwargs);

        Py_DECREF(kwargs);
        Py_DECREF(plot_args);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2>
    bool stem(E1&& x, E2&& y, const std::string& s = "") {
        assert(x.size() == y.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        PyObject* pystring = PyString_FromString(s.c_str());

        PyObject* plot_args = PyTuple_New(3);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);
        PyTuple_SetItem(plot_args, 2, pystring);

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_stem, plot_args);

        Py_DECREF(plot_args);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2>
    bool semilogx(E1&& x, E2&& y, const std::string& s = "") {
        assert(x.size() == y.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        PyObject* pystring = PyString_FromString(s.c_str());

        PyObject* plot_args = PyTuple_New(3);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);
        PyTuple_SetItem(plot_args, 2, pystring);

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_semilogx, plot_args);

        Py_DECREF(plot_args);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2>
    bool semilogy(E1&& x, E2&& y, const std::string& s = "") {
        assert(x.size() == y.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        PyObject* pystring = PyString_FromString(s.c_str());

        PyObject* plot_args = PyTuple_New(3);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);
        PyTuple_SetItem(plot_args, 2, pystring);

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_semilogy, plot_args);

        Py_DECREF(plot_args);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2>
    bool loglog(E1&& x, E2&& y, const std::string& s = "") {
        assert(x.size() == y.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        PyObject* pystring = PyString_FromString(s.c_str());

        PyObject* plot_args = PyTuple_New(3);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);
        PyTuple_SetItem(plot_args, 2, pystring);

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_loglog, plot_args);

        Py_DECREF(plot_args);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2, class E3>
    bool errorbar(E1&& x, E2&& y, E3&& yerr,
                  const std::map<std::string, std::string> &keywords = {}) {
        assert(x.size() == y.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));
        PyObject* yerrarray = get_array(std::forward<E3>(yerr));

        // construct keyword args
        PyObject* kwargs = PyDict_New();
        for(auto it = keywords.cbegin(); it != keywords.cend(); ++it)
        {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyString_FromString(it->second.c_str()));
        }

        PyDict_SetItemString(kwargs, "yerr", yerrarray);

        PyObject *plot_args = PyTuple_New(2);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);

        PyObject *res = PyObject_Call(
                detail::_interpreter::get().s_python_function_errorbar, plot_args, kwargs);

        Py_DECREF(kwargs);
        Py_DECREF(plot_args);
        if (!res) throw std::runtime_error("Call to errorbar() failed.");
        Py_DECREF(res);

        return res;
    }


    template <class E>
    bool named_plot(const std::string& name, E&& y, const std::string& format = "") {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

        PyObject* yarray = get_array(std::forward<E>(y));

        PyObject* pystring = PyString_FromString(format.c_str());

        PyObject* plot_args = PyTuple_New(2);

        PyTuple_SetItem(plot_args, 0, yarray);
        PyTuple_SetItem(plot_args, 1, pystring);

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_plot, plot_args, kwargs);

        Py_DECREF(kwargs);
        Py_DECREF(plot_args);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2>
    bool named_plot(const std::string& name, E1&& x, E2&& y, const std::string& format = "") {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        PyObject* pystring = PyString_FromString(format.c_str());

        PyObject* plot_args = PyTuple_New(3);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);
        PyTuple_SetItem(plot_args, 2, pystring);

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_plot, plot_args, kwargs);

        Py_DECREF(kwargs);
        Py_DECREF(plot_args);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2>
    bool named_semilogx(const std::string& name, E1&& x, E2&& y, const std::string& format = "") {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        PyObject* pystring = PyString_FromString(format.c_str());

        PyObject* plot_args = PyTuple_New(3);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);
        PyTuple_SetItem(plot_args, 2, pystring);

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_semilogx, plot_args, kwargs);

        Py_DECREF(kwargs);
        Py_DECREF(plot_args);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2>
    bool named_semilogy(const std::string& name, E1&& x, E2&& y, const std::string& format = "") {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        PyObject* pystring = PyString_FromString(format.c_str());

        PyObject* plot_args = PyTuple_New(3);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);
        PyTuple_SetItem(plot_args, 2, pystring);

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_semilogy, plot_args, kwargs);

        Py_DECREF(kwargs);
        Py_DECREF(plot_args);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E1, class E2>
    bool named_loglog(const std::string& name, E1&& x, E2&& y, const std::string& format = "") {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

        PyObject* xarray = get_array(std::forward<E1>(x));
        PyObject* yarray = get_array(std::forward<E2>(y));

        PyObject* pystring = PyString_FromString(format.c_str());

        PyObject* plot_args = PyTuple_New(3);
        PyTuple_SetItem(plot_args, 0, xarray);
        PyTuple_SetItem(plot_args, 1, yarray);
        PyTuple_SetItem(plot_args, 2, pystring);

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_loglog, plot_args, kwargs);

        Py_DECREF(kwargs);
        Py_DECREF(plot_args);
        if (res) Py_DECREF(res);

        return res;
    }


    template<class E>
    bool plot(E&& y, const std::string& format = "") {
        xt::xtensor<double, 1> x = xt::arange(y.size());
        return plot(std::move(x), std::forward<E>(y), format);
    }


    template<class E>
    bool stem(E&& y, const std::string& format = "") {
        xt::xtensor<double, 1> x = xt::arange(y.size());
        return stem(std::move(x), std::forward<E>(y), format);
    }


    template<typename Numeric>
    void text(Numeric x, Numeric y, const std::string& s = "") {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* args = PyTuple_New(3);
        PyTuple_SetItem(args, 0, PyFloat_FromDouble(x));
        PyTuple_SetItem(args, 1, PyFloat_FromDouble(y));
        PyTuple_SetItem(args, 2, PyString_FromString(s.c_str()));

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_text, args);

        Py_DECREF(args);
        if (!res) throw std::runtime_error("Call to text() failed.");
        Py_DECREF(res);
    }


    inline long figure(long number = -1) {
        PyObject *res;
        if (number == -1) {
            res = PyObject_CallObject(
                    detail::_interpreter::get().s_python_function_figure,
                    detail::_interpreter::get().s_python_empty_tuple);
        } else {
            assert(number > 0);

            // Make sure interpreter is initialised
            detail::_interpreter::get();

            PyObject *args = PyTuple_New(1);
            PyTuple_SetItem(args, 0, PyLong_FromLong(number));
            res = PyObject_CallObject(
                    detail::_interpreter::get().s_python_function_figure, args);
            Py_DECREF(args);
        }

        if (!res) throw std::runtime_error("Call to figure() failed.");

        PyObject* num = PyObject_GetAttrString(res, "number");
        if (!num) throw std::runtime_error("Could not get number attribute of figure object");
        const long figureNumber = PyLong_AsLong(num);

        Py_DECREF(num);
        Py_DECREF(res);

        return figureNumber;
    }


    inline bool fignum_exists(long number) {
        // Make sure interpreter is initialised
        detail::_interpreter::get();

        PyObject *args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, PyLong_FromLong(number));
        PyObject *res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_fignum_exists, args);

        Py_DECREF(args);
        if (!res) throw std::runtime_error("Call to fignum_exists() failed.");
        bool ret = PyObject_IsTrue(res);
        Py_DECREF(res);

        return ret;
    }


    inline void figure_size(size_t w, size_t h) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        const size_t dpi = 100;
        PyObject* size = PyTuple_New(2);
        PyTuple_SetItem(size, 0, PyFloat_FromDouble((double)w / dpi));
        PyTuple_SetItem(size, 1, PyFloat_FromDouble((double)h / dpi));

        PyObject* kwargs = PyDict_New();
        PyDict_SetItemString(kwargs, "figsize", size);
        PyDict_SetItemString(kwargs, "dpi", PyLong_FromSize_t(dpi));

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_figure,
                detail::_interpreter::get().s_python_empty_tuple, kwargs);

        Py_DECREF(kwargs);
        if (!res) throw std::runtime_error("Call to figure_size() failed.");
        Py_DECREF(res);
    }


    inline void legend() {
        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_legend,
                detail::_interpreter::get().s_python_empty_tuple);

        if (!res) throw std::runtime_error("Call to legend() failed.");
        Py_DECREF(res);
    }


    template<typename Numeric>
    void ylim(Numeric left, Numeric right) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* list = PyList_New(2);
        PyList_SetItem(list, 0, PyFloat_FromDouble(left));
        PyList_SetItem(list, 1, PyFloat_FromDouble(right));

        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, list);

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_ylim, args);

        Py_DECREF(args);
        if (!res) throw std::runtime_error("Call to ylim() failed.");
        Py_DECREF(res);
    }


    template<typename Numeric>
    void xlim(Numeric left, Numeric right) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* list = PyList_New(2);
        PyList_SetItem(list, 0, PyFloat_FromDouble(left));
        PyList_SetItem(list, 1, PyFloat_FromDouble(right));

        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, list);

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_xlim, args);

        Py_DECREF(args);
        if (!res) throw std::runtime_error("Call to xlim() failed.");
        Py_DECREF(res);
    }


    inline std::array<double,2> xlim() {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* args = PyTuple_New(0);
        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_xlim, args);
        PyObject* left = PyTuple_GetItem(res,0);
        PyObject* right = PyTuple_GetItem(res,1);

        std::array<double, 2> arr{PyFloat_AsDouble(left), PyFloat_AsDouble(right)};

        if (!res) throw std::runtime_error("Call to xlim() failed.");
        Py_DECREF(res);

        return arr;
    }


    inline std::array<double,2> ylim() {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* args = PyTuple_New(0);
        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_ylim, args);
        PyObject* left = PyTuple_GetItem(res,0);
        PyObject* right = PyTuple_GetItem(res,1);

        std::array<double, 2> arr{PyFloat_AsDouble(left), PyFloat_AsDouble(right)};

        if (!res) throw std::runtime_error("Call to ylim() failed.");
        Py_DECREF(res);

        return arr;
    }


    template<class E>
    inline void xticks(E&& ticks, const std::vector<std::string> &labels = {},
                       const std::map<std::string, std::string>& keywords = {}) {
        assert(labels.size() == 0 || ticks.size() == labels.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        // using numpy array
        PyObject* ticksarray = get_array(std::forward<E>(ticks));

        PyObject* args;
        if (labels.size() == 0) {
            // construct positional args
            args = PyTuple_New(1);
            PyTuple_SetItem(args, 0, ticksarray);
        } else {
            // make tuple of tick labels
            PyObject* labelstuple = PyTuple_New(labels.size());
            for (size_t i = 0; i < labels.size(); i++)
                PyTuple_SetItem(labelstuple, i, PyUnicode_FromString(labels[i].c_str()));

            // construct positional args
            args = PyTuple_New(2);
            PyTuple_SetItem(args, 0, ticksarray);
            PyTuple_SetItem(args, 1, labelstuple);
        }

        // construct keyword args
        PyObject* kwargs = PyDict_New();
        for(auto it = keywords.cbegin(); it != keywords.cend(); ++it)
        {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyString_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_xticks, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (!res) throw std::runtime_error("Call to xticks() failed");
        Py_DECREF(res);
    }


    template<class E>
    inline void xticks(E&& ticks, const std::map<std::string, std::string>& keywords) {
        xticks(std::forward<E>(ticks), {}, keywords);
    }


    template<class E>
    inline void yticks(E&& ticks, const std::vector<std::string> &labels = {},
                       const std::map<std::string, std::string>& keywords = {}) {
        assert(labels.size() == 0 || ticks.size() == labels.size());

        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        // using numpy array
        PyObject* ticksarray = get_array(std::forward<E>(ticks));

        PyObject* args;
        if (labels.size() == 0) {
            // construct positional args
            args = PyTuple_New(1);
            PyTuple_SetItem(args, 0, ticksarray);
        } else {
            // make tuple of tick labels
            PyObject* labelstuple = PyTuple_New(labels.size());
            for (size_t i = 0; i < labels.size(); i++)
                PyTuple_SetItem(labelstuple, i, PyUnicode_FromString(labels[i].c_str()));

            // construct positional args
            args = PyTuple_New(2);
            PyTuple_SetItem(args, 0, ticksarray);
            PyTuple_SetItem(args, 1, labelstuple);
        }

        // construct keyword args
        PyObject* kwargs = PyDict_New();
        for(auto it = keywords.cbegin(); it != keywords.cend(); ++it)
        {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyString_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_yticks, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (!res) throw std::runtime_error("Call to yticks() failed");
        Py_DECREF(res);
    }


    template<class E>
    inline void yticks(E&& ticks, const std::map<std::string, std::string>& keywords) {
        yticks(std::forward<E>(ticks), {}, keywords);
    }


    inline void subplot(long nrows, long ncols, long plot_number) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        // construct positional args
        PyObject* args = PyTuple_New(3);
        PyTuple_SetItem(args, 0, PyFloat_FromDouble(nrows));
        PyTuple_SetItem(args, 1, PyFloat_FromDouble(ncols));
        PyTuple_SetItem(args, 2, PyFloat_FromDouble(plot_number));

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_subplot, args);

        Py_DECREF(args);
        if (!res) throw std::runtime_error("Call to subplot() failed.");
        Py_DECREF(res);
    }


    inline void title(const std::string &titlestr,
                      const std::map<std::string, std::string> &keywords = {}) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* pytitlestr = PyString_FromString(titlestr.c_str());
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, pytitlestr);

        PyObject* kwargs = PyDict_New();
        for (auto it = keywords.cbegin(); it != keywords.cend(); ++it) {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyUnicode_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_title, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (!res) throw std::runtime_error("Call to title() failed.");
        Py_DECREF(res);
    }


    inline void suptitle(const std::string &suptitlestr,
                         const std::map<std::string, std::string> &keywords = {}) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* pysuptitlestr = PyString_FromString(suptitlestr.c_str());
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, pysuptitlestr);

        PyObject* kwargs = PyDict_New();
        for (auto it = keywords.cbegin(); it != keywords.cend(); ++it) {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyUnicode_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_suptitle, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (!res) throw std::runtime_error("Call to suptitle() failed.");
        Py_DECREF(res);
    }


    inline void axis(const std::string &axisstr) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* str = PyString_FromString(axisstr.c_str());
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, str);

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_axis, args);

        Py_DECREF(args);
        if (!res) throw std::runtime_error("Call to title() failed.");
        Py_DECREF(res);
    }


    inline void xlabel(const std::string &str,
                       const std::map<std::string, std::string> &keywords = {}) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* pystr = PyString_FromString(str.c_str());
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, pystr);

        PyObject* kwargs = PyDict_New();
        for (auto it = keywords.cbegin(); it != keywords.cend(); ++it) {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyUnicode_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call
            (detail::_interpreter::get().s_python_function_xlabel, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (!res) throw std::runtime_error("Call to xlabel() failed.");
        Py_DECREF(res);
    }


    inline void ylabel(const std::string &str,
                       const std::map<std::string, std::string>& keywords = {}) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* pystr = PyString_FromString(str.c_str());
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, pystr);

        PyObject* kwargs = PyDict_New();
        for (auto it = keywords.cbegin(); it != keywords.cend(); ++it) {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyUnicode_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call
            (detail::_interpreter::get().s_python_function_ylabel, args, kwargs);

        Py_DECREF(args);
        Py_DECREF(kwargs);
        if (!res) throw std::runtime_error("Call to ylabel() failed.");
        Py_DECREF(res);
    }


    inline void grid(bool flag) {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* pyflag = flag ? Py_True : Py_False;
        Py_INCREF(pyflag);

        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, pyflag);

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_grid, args);

        Py_DECREF(args);
        if (!res) throw std::runtime_error("Call to grid() failed.");
        Py_DECREF(res);
    }


    inline void show(const bool block = true) {
        PyObject* res;
        if (block) {
            res = PyObject_CallObject(
                    detail::_interpreter::get().s_python_function_show,
                    detail::_interpreter::get().s_python_empty_tuple);
        } else {
            PyObject *kwargs = PyDict_New();
            PyDict_SetItemString(kwargs, "block", Py_False);
            res = PyObject_Call(
                    detail::_interpreter::get().s_python_function_show,
                    detail::_interpreter::get().s_python_empty_tuple, kwargs);
            Py_DECREF(kwargs);
        }

        if (!res) throw std::runtime_error("Call to show() failed.");
        Py_DECREF(res);
    }


    inline void close() {
        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_close,
                detail::_interpreter::get().s_python_empty_tuple);

        if (!res) throw std::runtime_error("Call to close() failed.");
        Py_DECREF(res);
    }


    inline void xkcd() {
        //interpreter needs to be initialized for the numpy commands to work
        detail::_interpreter::get();

        PyObject* res;
        PyObject *kwargs = PyDict_New();

        res = PyObject_Call(
                detail::_interpreter::get().s_python_function_xkcd,
                detail::_interpreter::get().s_python_empty_tuple, kwargs);

        Py_DECREF(kwargs);
        if (!res) throw std::runtime_error("Call to show() failed.");
        Py_DECREF(res);
    }


    inline void draw() {
        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_draw,
                detail::_interpreter::get().s_python_empty_tuple);

        if (!res) throw std::runtime_error("Call to draw() failed.");
        Py_DECREF(res);
    }


    template<typename Numeric>
    inline void pause(Numeric interval) {
        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, PyFloat_FromDouble(interval));

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_pause, args);

        Py_DECREF(args);
        if (!res) throw std::runtime_error("Call to pause() failed.");
        Py_DECREF(res);
    }


    inline void save(const std::string& filename) {
        PyObject* pyfilename = PyString_FromString(filename.c_str());

        PyObject* args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, pyfilename);

        PyObject* res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_save, args);

        Py_DECREF(args);
        if (!res) throw std::runtime_error("Call to save() failed.");
        Py_DECREF(res);
    }


    inline void clf() {
        PyObject *res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_clf,
                detail::_interpreter::get().s_python_empty_tuple);

        if (!res) throw std::runtime_error("Call to clf() failed.");
        Py_DECREF(res);
    }


    inline void ion() {
        PyObject *res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_ion,
                detail::_interpreter::get().s_python_empty_tuple);

        if (!res) throw std::runtime_error("Call to ion() failed.");
        Py_DECREF(res);
    }


    inline std::vector<std::array<double, 2>> ginput(const int numClicks = 1,
            const std::map<std::string, std::string>& keywords = {}) {
        PyObject *args = PyTuple_New(1);
        PyTuple_SetItem(args, 0, PyLong_FromLong(numClicks));

        // construct keyword args
        PyObject* kwargs = PyDict_New();
        for(auto it = keywords.cbegin(); it != keywords.cend(); ++it)
        {
            PyDict_SetItemString(kwargs, it->first.c_str(),
                    PyUnicode_FromString(it->second.c_str()));
        }

        PyObject* res = PyObject_Call(
                detail::_interpreter::get().s_python_function_ginput, args, kwargs);

        Py_DECREF(kwargs);
        Py_DECREF(args);
        if (!res) throw std::runtime_error("Call to ginput() failed.");

        const size_t len = PyList_Size(res);
        std::vector<std::array<double, 2>> out;
        out.reserve(len);
        for (size_t i = 0; i < len; i++) {
            PyObject *current = PyList_GetItem(res, i);
            std::array<double, 2> position;
            position[0] = PyFloat_AsDouble(PyTuple_GetItem(current, 0));
            position[1] = PyFloat_AsDouble(PyTuple_GetItem(current, 1));
            out.push_back(position);
        }

        Py_DECREF(res);

        return out;
    }


    // Actually, is there any reason not to call this automatically for every plot?
    inline void tight_layout() {
        PyObject *res = PyObject_CallObject(
                detail::_interpreter::get().s_python_function_tight_layout,
                detail::_interpreter::get().s_python_empty_tuple);

        if (!res) throw std::runtime_error("Call to tight_layout() failed.");
        Py_DECREF(res);
    }


    // Support for variadic plot() and initializer lists:
namespace detail {

    template<typename T>
    using is_function = typename std::is_function<std::remove_pointer<std::remove_reference<T>>>::type;

    template<bool obj, typename T>
    struct is_callable_impl;

    template<typename T>
    struct is_callable_impl<false, T> {
        typedef is_function<T> type;
    }; // a non-object is callable iff it is a function

    template<typename T>
    struct is_callable_impl<true, T> {
        struct Fallback { void operator()(); };
        struct Derived : T, Fallback { };

        template<typename U, U> struct Check;

        // use a variadic function to make sure (1) it accepts everything and (2) its always the worst match
        template<typename U>
        static std::true_type test( ... );

        template<typename U>
        static std::false_type test( Check<void(Fallback::*)(), &U::operator()>* );

    public:
        typedef decltype(test<Derived>(nullptr)) type;
        typedef decltype(&Fallback::operator()) dtype;
        static constexpr bool value = type::value;
    }; // an object is callable iff it defines operator()

    template<typename T>
    struct is_callable {
        // dispatch to is_callable_impl<true, T> or is_callable_impl<false, T>
        // depending on whether T is of class type or not
        typedef typename is_callable_impl<std::is_class<T>::value, T>::type type;
    };

    template<typename IsYDataCallable>
    struct plot_impl { };

    template<>
    struct plot_impl<std::false_type> {
        template<typename IterableX, typename IterableY>
        bool operator()(IterableX&& x, IterableY&& y, const std::string& format) {
            // 2-phase lookup for distance, begin, end
            using std::distance;
            using std::begin;
            using std::end;

            auto xs = distance(begin(x), end(x));
            auto ys = distance(begin(y), end(y));
            assert(xs == ys && "x and y data must have the same number of elements!");

            //interpreter needs to be initialized for the numpy commands to work
            detail::_interpreter::get();

            PyObject* xlist = PyList_New(xs);
            PyObject* ylist = PyList_New(ys);
            PyObject* pystring = PyString_FromString(format.c_str());

            auto itx = x.begin();
            auto ity = y.begin();
            for(size_t i = 0; i < xs; ++i) {
                PyList_SetItem(xlist, i, PyFloat_FromDouble(*itx++));
                PyList_SetItem(ylist, i, PyFloat_FromDouble(*ity++));
            }

            PyObject* plot_args = PyTuple_New(3);
            PyTuple_SetItem(plot_args, 0, xlist);
            PyTuple_SetItem(plot_args, 1, ylist);
            PyTuple_SetItem(plot_args, 2, pystring);

            PyObject* res = PyObject_CallObject(
                    detail::_interpreter::get().s_python_function_plot, plot_args);

            Py_DECREF(plot_args);
            if (res) Py_DECREF(res);

            return res;
        }
    };


    template<>
    struct plot_impl<std::true_type> {
        template<typename Iterable, typename Callable>
        bool operator()(Iterable&& ticks, Callable&& f, const std::string& format) {
            if (begin(ticks) == end(ticks)) return true;

            // We could use additional meta-programming to deduce the correct element type of y,
            // but all values have to be convertible to double anyways
            std::size_t size = std::distance(begin(ticks), end(ticks));
            std::array<std::size_t, 1> shape{size};
            xt::xtensor<double, 1> y(shape);
            std::size_t idx = 0;
            for (auto x : ticks) y[idx++] = f(x);
            return plot_impl<std::false_type>()(std::forward<Iterable>(ticks), std::move(y), format);
        }
    };

} // end namespace detail

    // recursion stop for the above
    template<typename... Args>
    bool plot() { return true; }

    template<class E1, class E2, typename... Args>
    bool plot(E1&& a, E2&& b, const std::string& format, Args... args) {

        return detail::plot_impl<
                std::integral_constant<bool, xtl::conjunction<
                    typename detail::is_callable<typename std::decay_t<E2>>::type,
                    xtl::negation<xt::is_xexpression<E2>>
                >::value>
            >()(std::forward<E1>(a), std::forward<E2>(b), format) && plot(std::forward<Args>(args)...);
    }

    //
    // This group of plot() functions is needed to support initializer lists, i.e. calling
    //    plot( {1,2,3,4} )
    ///
    inline bool plot(std::vector<double> x, std::vector<double> y, const std::string& format = "") {
        xt::xtensor<double, 1> x_adapt = xt::adapt(x);
        xt::xtensor<double, 1> y_adapt = xt::adapt(y);
        return plot(std::move(x_adapt), std::move(y_adapt), format);
    }

    inline bool plot(std::vector<double> y, const std::string& format = "") {
        xt::xtensor<double, 1> y_adapt = xt::adapt(y);
        return plot(std::move(y_adapt), format);
    }


    inline bool plot(std::vector<double> x, std::vector<double> y,
                     const std::map<std::string, std::string>& keywords) {
        xt::xtensor<double, 1> x_adapt = xt::adapt(x);
        xt::xtensor<double, 1> y_adapt = xt::adapt(y);
        return plot(std::move(x_adapt), std::move(y_adapt), keywords);
    }


    //
    // This class allows dynamic plots, ie changing the plotted data without clearing and re-plotting
    //
    class Plot {
    public:
        // default initialization with plot label, some data and format
        template<class E1, class E2>
        Plot(const std::string& name, E1&& x, E2&& y, const std::string& format = "") {
            assert(x.size() == y.size());

            PyObject* kwargs = PyDict_New();
            if (name != "")
                PyDict_SetItemString(kwargs, "label", PyString_FromString(name.c_str()));

            PyObject* xarray = get_array(std::forward<E1>(x));
            PyObject* yarray = get_array(std::forward<E2>(y));

            PyObject* pystring = PyString_FromString(format.c_str());

            PyObject* plot_args = PyTuple_New(3);
            PyTuple_SetItem(plot_args, 0, xarray);
            PyTuple_SetItem(plot_args, 1, yarray);
            PyTuple_SetItem(plot_args, 2, pystring);

            PyObject* res = PyObject_Call(
                    detail::_interpreter::get().s_python_function_plot, plot_args, kwargs);

            Py_DECREF(kwargs);
            Py_DECREF(plot_args);

            if (res) {
                line= PyList_GetItem(res, 0);

                if (line)
                    set_data_fct = PyObject_GetAttrString(line,"set_data");
                else
                    Py_DECREF(line);
                Py_DECREF(res);
            }
        }

        // shorter initialization with name or format only
        // basically calls line, = plot([], [])
        Plot(const std::string& name = "", const std::string& format = "")
            : Plot(name, xt::xtensor<double,1>(), xt::xtensor<double,1>(), format) {}

        template<class E1, class E2>
        bool update(E1&& x, E2&& y) {
            assert(x.size() == y.size());
            if (set_data_fct) {
                PyObject* xarray = get_array(std::forward<E1>(x));
                PyObject* yarray = get_array(std::forward<E2>(y));

                PyObject* plot_args = PyTuple_New(2);
                PyTuple_SetItem(plot_args, 0, xarray);
                PyTuple_SetItem(plot_args, 1, yarray);

                PyObject* res = PyObject_CallObject(set_data_fct, plot_args);
                if (res) Py_DECREF(res);
                return res;
            }
            return false;
        }

        // clears the plot but keep it available
        bool clear() {
            return update(xt::xtensor<double,1>(), xt::xtensor<double,1>());
        }

        // definitely remove this line
        void remove() {
            if (line) {
                auto remove_fct = PyObject_GetAttrString(line,"remove");
                PyObject* args = PyTuple_New(0);
                PyObject* res = PyObject_CallObject(remove_fct, args);
                if (res) Py_DECREF(res);
            }
            decref();
        }

        ~Plot() {
            decref();
        }
    private:

        void decref() {
            if (line)
                Py_DECREF(line);
            if (set_data_fct)
                Py_DECREF(set_data_fct);
        }

        PyObject* line = nullptr;
        PyObject* set_data_fct = nullptr;
    };
} // end namespace matplotlibcpp

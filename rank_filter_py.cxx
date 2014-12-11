#define PY_ARRAY_UNIQUE_SYMBOL rank_filter_PyArray_API
//#define NO_IMPORT_ARRAY


#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include "rank_filter.hxx"

#include <Python.h>
#include <boost/python.hpp>
#include <vigra/numpy_array.hxx>
#include <vigra/numpy_array_converters.hxx>


namespace rank_filter
{

    template < unsigned int N, class SrcPixelType >
    vigra::NumpyAnyArray pythonLineRankOrderFilter(const vigra::NumpyArray< N, vigra::Singleband<SrcPixelType> > & image,
            unsigned long half_length, double rank, unsigned int axis = N - 1,
            vigra::NumpyArray< N, vigra::Singleband<SrcPixelType> > res = boost::python::object())
    {
        std::string description("rank order filter over 1-Dimension, axis=");
        description += vigra::asString(axis);

        vigra_precondition((half_length + 1) <= image.shape(axis),
                "lineRankOrderFilter(): Window must be no bigger than the image.");

        res.reshapeIfEmpty(image.taggedShape().setChannelDescription(description),
                "lineRankOrderFilter(): Output array has wrong shape.");

        {
            vigra::PyAllowThreads _pythread;

            rank_filter::lineRankOrderFilter(image, res, half_length, rank, axis);
        }
        return(res);
    }


    void defineRankFilter()
    {
        using namespace boost::python;

        docstring_options doc_options(true, true, false);

        def("lineRankOrderFilter",
                vigra::registerConverters(&pythonLineRankOrderFilter<1, float>),
                (arg("image"), arg("half_length"), arg("rank"), arg("axis")=(1-1), arg("out")=object()),
                "Convolution along a single dimension of a 1D scalar or multiband image. "
                        "'kernel' must be an instance of Kernel1D.\n"
                        "\n"
                        "For details see convolveMultiArrayOneDimension_ in the vigra C++ documentation.\n");

        def("lineRankOrderFilter",
                vigra::registerConverters(&pythonLineRankOrderFilter<2, float>),
                (arg("image"), arg("half_length"), arg("rank"), arg("axis")=(2-1), arg("out")=object()),
                "Convolution along a single dimension of a 2D scalar or multiband image. "
                        "'kernel' must be an instance of Kernel1D.\n"
                        "\n"
                        "For details see convolveMultiArrayOneDimension_ in the vigra C++ documentation.\n");

        def("lineRankOrderFilter",
                vigra::registerConverters(&pythonLineRankOrderFilter<3, float>),
                (arg("image"), arg("half_length"), arg("rank"), arg("axis")=(3-1), arg("out")=object()),
                "Convolution along a single dimension of a 3D scalar or multiband image. "
                        "'kernel' must be an instance of Kernel1D.\n"
                        "\n"
                        "For details see convolveMultiArrayOneDimension_ in the vigra C++ documentation.\n");

        def("lineRankOrderFilter",
                vigra::registerConverters(&pythonLineRankOrderFilter<4, float>),
                (arg("image"), arg("half_length"), arg("rank"), arg("axis")=(4-1), arg("out")=object()),
                "Convolution along a single dimension of a 4D scalar or multiband image. "
                        "'kernel' must be an instance of Kernel1D.\n"
                        "\n"
                        "For details see convolveMultiArrayOneDimension_ in the vigra C++ documentation.\n");

        def("lineRankOrderFilter",
                vigra::registerConverters(&pythonLineRankOrderFilter<5, float>),
                (arg("image"), arg("half_length"), arg("rank"), arg("axis")=(5-1), arg("out")=object()),
                "Convolution along a single dimension of a 5D scalar or multiband image. "
                        "'kernel' must be an instance of Kernel1D.\n"
                        "\n"
                        "For details see convolveMultiArrayOneDimension_ in the vigra C++ documentation.\n");

        def("lineRankOrderFilter",
                vigra::registerConverters(&pythonLineRankOrderFilter<1, double>),
                (arg("image"), arg("half_length"), arg("rank"), arg("axis")=(1-1), arg("out")=object()),
                "Convolution along a single dimension of a 1D scalar or multiband image. "
                        "'kernel' must be an instance of Kernel1D.\n"
                        "\n"
                        "For details see convolveMultiArrayOneDimension_ in the vigra C++ documentation.\n");

        def("lineRankOrderFilter",
                vigra::registerConverters(&pythonLineRankOrderFilter<2, double>),
                (arg("image"), arg("half_length"), arg("rank"), arg("axis")=(2-1), arg("out")=object()),
                "Convolution along a single dimension of a 2D scalar or multiband image. "
                        "'kernel' must be an instance of Kernel1D.\n"
                        "\n"
                        "For details see convolveMultiArrayOneDimension_ in the vigra C++ documentation.\n");

        def("lineRankOrderFilter",
                vigra::registerConverters(&pythonLineRankOrderFilter<3, double>),
                (arg("image"), arg("half_length"), arg("rank"), arg("axis")=(3-1), arg("out")=object()),
                "Convolution along a single dimension of a 3D scalar or multiband image. "
                        "'kernel' must be an instance of Kernel1D.\n"
                        "\n"
                        "For details see convolveMultiArrayOneDimension_ in the vigra C++ documentation.\n");

        def("lineRankOrderFilter",
                vigra::registerConverters(&pythonLineRankOrderFilter<4, double>),
                (arg("image"), arg("half_length"), arg("rank"), arg("axis")=(4-1), arg("out")=object()),
                "Convolution along a single dimension of a 4D scalar or multiband image. "
                        "'kernel' must be an instance of Kernel1D.\n"
                        "\n"
                        "For details see convolveMultiArrayOneDimension_ in the vigra C++ documentation.\n");

        def("lineRankOrderFilter",
                vigra::registerConverters(&pythonLineRankOrderFilter<5, double>),
                (arg("image"), arg("half_length"), arg("rank"), arg("axis")=(5-1), arg("out")=object()),
                "Convolution along a single dimension of a 5D scalar or multiband image. "
                        "'kernel' must be an instance of Kernel1D.\n"
                        "\n"
                        "For details see convolveMultiArrayOneDimension_ in the vigra C++ documentation.\n");
    }


    void translate_std_exception(const std::exception& e)
    {
        using namespace boost::python;

        // Use the Python 'C' API to set up an exception object
        PyErr_SetString(PyExc_RuntimeError, e.what());
    }

}


BOOST_PYTHON_MODULE_INIT(rank_filter)
{
    try
    {
        boost::python::register_exception_translator<std::exception>(&rank_filter::translate_std_exception);
        vigra::import_vigranumpy();
        rank_filter::defineRankFilter();
    } catch(std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }
}
from rank_filter cimport lineRankOrderFilter1D_floating_inplace, ndindex

cimport cython

cimport numpy
import numpy

include "version.pxi"

numpy.import_array()


@cython.boundscheck(False)
def lineRankOrderFilter(numpy.ndarray image not None,
                        size_t half_length,
                        double rank,
                        int axis=-1,
                        numpy.ndarray out=None):
    """
        Runs a linear rank filter kernel along one dimension of an array.

        Args:
            image(numpy.ndarray):      array to run the rank filter over.

            half_length(size_t):       half the window size for the kernel.

            rank(double):              quantile to use from ``0.0`` to ``1.0``.

            axis(int):                 which axis to direct and run the kernel
                                       over defaults to the last one
                                       (e.g. ``-1``).

            out(numpy.ndarray):        where to store the results to. Creates a
                                       new array if not specified. If it is the
                                       same as image, it will run in-place.

        Returns:
            out(numpy.ndarray):        result of running the linear rank filter.
    """

    cdef int image_type_num = numpy.PyArray_TYPE(image)
    cdef int out_type_num

    cdef numpy.ndarray out_swap

    if -image.ndim <= axis < 0:
        axis += image.ndim
    elif not (0 <= axis < image.ndim):
        raise ValueError("`axis` needs to be within `image.ndim`")

    if not ((half_length + 1) <= image.shape[axis]):
        raise ValueError("Window must be no bigger than the image.")

    if not (0.0 <= rank <= 1.0):
        raise ValueError("The rank must be between 0.0 and 1.0.")

    if out is None:
        out = numpy.PyArray_NewCopy(image, numpy.NPY_CORDER)
        out_type_num = image_type_num
    else:
        out_type_num = numpy.PyArray_TYPE(out)
        assert (image_type_num == out_type_num), \
                "Both `image` and `out` must have the same type."
        assert numpy.PyArray_SAMESHAPE(image, out), \
                "Both `image` and `out` must have the same shape."
        if numpy.PyArray_CopyInto(out, image) == -1:
            raise RuntimeError("Unable to copy `image` to `out`.")

    out_swap = numpy.PyArray_SwapAxes(out, axis, out.ndim - 1)
    out_swap = numpy.PyArray_GETCONTIGUOUS(out_swap)

    if out_type_num == numpy.NPY_FLOAT32:
        lineRankOrderFilter1D_floating_inplace_loop[float](
            out_swap, half_length, rank, <float*>NULL
        )
    elif out_type_num == numpy.NPY_FLOAT64:
        lineRankOrderFilter1D_floating_inplace_loop[double](
            out_swap, half_length, rank, <double*>NULL
        )
    else:
        raise TypeError(
            "Only `float32` and `float64` are supported for `image` and `out`."
        )

    out_swap = numpy.PyArray_SwapAxes(out_swap, out.ndim - 1, axis)
    if numpy.PyArray_CopyInto(out, out_swap) == -1:
        raise RuntimeError("Unable to copy `out_swap` to `out`.")

    return(out)


cdef extern from "numpy/arrayobject.h":
    void* PyArray_GetPtr(numpy.ndarray, numpy.npy_intp*) nogil


@cython.boundscheck(False)
@cython.initializedcheck(False)
@cython.nonecheck(False)
cdef inline void lineRankOrderFilter1D_floating_inplace_loop(numpy.ndarray out,
                                                             size_t half_length,
                                                             double rank,
                                                             floating* null) nogil:
    cdef numpy.npy_intp i

    cdef numpy.npy_intp out_size_1 = 1
    for i in range(out.ndim - 1):
        out_size_1 *= out.shape[i]

    cdef floating* out_ptr = <floating*>out.data
    cdef numpy.npy_intp out_step = out.shape[out.ndim - 1]
    for i in range(out_size_1):
        lineRankOrderFilter1D_floating_inplace[floating](
            out_ptr, out_step, half_length, rank
        )
        out_ptr += out_step

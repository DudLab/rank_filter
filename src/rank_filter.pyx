from rank_filter cimport lineRankOrderFilter1D_floating_inplace

cimport cython

cimport numpy
import numpy

include "version.pxi"

numpy.import_array()


@cython.boundscheck(False)
def lineRankOrderFilter(image not None,
                        size_t half_length,
                        double rank,
                        int axis=-1,
                        out=None):
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

    cdef numpy.ndarray image_arr = numpy.asarray(image)
    cdef numpy.ndarray out_arr = out

    cdef numpy.ndarray out_swap

    assert ((half_length + 1) <= image.shape[axis]), \
            "Window must be no bigger than the image."

    assert (0.0 <= rank <= 1.0), \
            "The rank must be between 0.0 and 1.0."

    if out is None:
        out = out_arr = numpy.PyArray_NewCopy(image_arr, numpy.NPY_CORDER)
    else:
        assert (image_arr.descr.type_num == out_arr.descr.type_num), \
                "Both `image` and `out` must have the same type."
        assert numpy.PyArray_SAMESHAPE(image_arr, out_arr), \
                "Both `image` and `out` must have the same shape."
        if numpy.PyArray_CopyInto(out_arr, image_arr) == -1:
            raise RuntimeError("Unable to copy `image` to `out`.")

    out_swap = numpy.ascontiguousarray(out.swapaxes(axis, -1))
    out_strip_indices = numpy.ndindex(out_swap.shape[:-1])

    if out_arr.descr.type_num == numpy.NPY_FLOAT32:
        for idx in out_strip_indices:
            lineRankOrderFilter1D_floating_inplace[float](
                out_swap[idx], half_length, rank
            )
    elif out_arr.descr.type_num == numpy.NPY_FLOAT64:
        for idx in out_strip_indices:
            lineRankOrderFilter1D_floating_inplace[double](
                out_swap[idx], half_length, rank
            )
    else:
        raise TypeError(
            "Only `float32` and `float64` are supported for `image` and `out`."
        )

    numpy.copyto(out, out_swap.swapaxes(-1, axis))

    return(out)

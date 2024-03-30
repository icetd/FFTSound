//   complex.cpp - impelementation of template
//   of complex number
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

#ifndef _COMPLEX_CPP_
#define _COMPLEX_CPP_

//   Include header file
#include "complex.h"

//   Imaginary unity constants
template<class T> const complex<T> complex<T>::i(T(0.), T(1.));
template<class T> const complex<T> complex<T>::j(T(0.), T(1.));

#endif

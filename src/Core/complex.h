//   complex.h - declaration of template
//   of complex number
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

#ifndef _COMPLEX_H_
#define _COMPLEX_H_

//   Complex number template, double by default
template <class T = double> class complex
{
protected:
	//   Internal presentation - real and imaginary parts
	T m_re;
	T m_im;

public:
	//   Imaginary unity
	static const complex<T> i;
	static const complex<T> j;

	//   Constructors
	complex(): m_re(T(0.)), m_im(T(0.)) {}
	complex(T re, T im): m_re(re), m_im(im) {}
	complex(T val): m_re(val), m_im(T(0.)) {}

	//   Assignment
	complex& operator= (const T val)
	{
		m_re = val;
		m_im = T(0.);
		return *this;
	}

	//   Basic operations - taking parts
	T re() const { return m_re; }
	T im() const { return m_im; }

	//   Conjugate number
	complex<T> conjugate() const
	{
		return complex<T>(m_re, -m_im);
	}

	//   Norm   
	T norm() const
	{
		return m_re * m_re + m_im * m_im;
	}

	//   Arithmetic operations
	complex<T> operator+ (const complex<T> &other) const
	{
		return complex<T>(m_re + other.m_re, m_im + other.m_im);
	}

	complex<T> operator- (const complex<T> &other) const
	{
		return complex<T>(m_re - other.m_re, m_im - other.m_im);
	}

	complex<T> operator* (const complex<T> &other) const
	{
		return complex<T>(m_re * other.m_re - m_im * other.m_im,
			m_re * other.m_im + m_im * other.m_re);
	}

	complex<T> operator/ (const complex<T> &other) const
	{
		const T denominator = other.m_re * other.m_re + other.m_im * other.m_im;
		return complex<T>((m_re * other.m_re + m_im * other.m_im) / denominator,
			(m_im * other.m_re - m_re * other.m_im) / denominator);
	}

	complex<T>& operator+= (const complex<T> &other)
	{
		m_re += other.m_re;
		m_im += other.m_im;
		return *this;
	}

	complex<T>& operator-= (const complex<T> &other)
	{
		m_re -= other.m_re;
		m_im -= other.m_im;
		return *this;
	}

	complex<T>& operator*= (const complex<T> &other)
	{
		const T temp = m_re;
		m_re = m_re * other.m_re - m_im * other.m_im;
		m_im = m_im * other.m_re + temp * other.m_im;
		return *this;
	}

	complex<T>& operator/= (const complex<T> &other)
	{
		const T denominator = other.m_re * other.m_re + other.m_im * other.m_im;
		const T temp = m_re;
		m_re = (m_re * other.m_re + m_im * other.m_im) / denominator;
		m_im = (m_im * other.m_re - temp * other.m_im) / denominator;
		return *this;
	}

	complex<T>& operator++ ()
	{
		++m_re;
		return *this;
	}

	complex<T> operator++ (int)
	{
		complex<T> temp(*this);
		++m_re;
		return temp;
	}

	complex<T>& operator-- ()
	{
		--m_re;
		return *this;
	}

	complex<T> operator-- (int)
	{
		complex<T> temp(*this);
		--m_re;
		return temp;
	}

	complex<T> operator+ (const T val) const
	{
		return complex<T>(m_re + val, m_im);
	}

	complex<T> operator- (const T val) const
	{
		return complex<T>(m_re - val, m_im);
	}

	complex<T> operator* (const T val) const
	{
		return complex<T>(m_re * val, m_im * val);
	}

	complex<T> operator/ (const T val) const
	{
		return complex<T>(m_re / val, m_im / val);
	}

	complex<T>& operator+= (const T val)
	{
		m_re += val;
		return *this;
	}

	complex<T>& operator-= (const T val)
	{
		m_re -= val;
		return *this;
	}

	complex<T>& operator*= (const T val)
	{
		m_re *= val;
		m_im *= val;
		return *this;
	}

	complex<T>& operator/= (const T val)
	{
		m_re /= val;
		m_im /= val;
		return *this;
	}

	friend complex<T> operator+ (const T left, const complex<T> &right)
	{
		return complex<T>(left + right.m_re, right.m_im);
	}

	friend complex<T> operator- (const T left, const complex<T> &right)
	{
		return complex<T>(left - right.m_re, -right.m_im);
	}

	friend complex<T> operator* (const T left, const complex<T>& right)
	{
		return complex<T>(left * right.m_re, left * right.m_im);
	}

	friend complex<T> operator/ (const T left, const complex<T>& right)
	{
		const T denominator = right.m_re * right.m_re + right.m_im * right.m_im;
		return complex<T>(left * right.m_re / denominator,
			-left * right.m_im / denominator);
	}

	//   Boolean operators
	bool operator== (const complex<T> &other) const
	{
		return m_re == other.m_re && m_im == other.m_im;
	}

	bool operator!= (const complex<T> &other) const
	{
		return m_re != other.m_re || m_im != other.m_im;
	}

	bool operator== (const T val) const
	{
		return m_re == val && m_im == (0.);
	}

	bool operator!= (const T val) const
	{
		return m_re != val || m_im != T(0.);
	}

	friend bool operator== (const T left, const complex<T> &right)
	{
		return left == right.m_re && right.m_im == T(0.);
	}

	friend bool operator!= (const T left, const complex<T> &right)
	{
		return left != right.m_re || right.m_im != T(0.);
	}
};

//   Include implementation file
#include "complex.cpp"

#endif

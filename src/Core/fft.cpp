//   fft.cpp - impelementation of STL template
//   of fast Fourier transform - FFT
//
//   The code is property of LIBROW
//   You can use it on your own
//   When utilizing credit LIBROW site

#ifndef _FFT_CPP_
#define _FFT_CPP_

//   Include declaration file
#include "fft.h"
//   Include math library
#include <math.h>

//   FORWARD FOURIER TRANSFORM
//     Input  - input data
//     Output - transform result
//     N      - length of both input data and result
template <class T> bool TFFT<T>::Forward(const complex<T> *const Input, complex<T> *const Output, const unsigned int N)
{
	//   Check input parameters
	if (!Input || !Output || N < 1 || N & (N - 1))
		return false;
	//   Initialize data
	Rearrange(Input, Output, N);
	//   Call FFT implementation
	Perform(Output, N);
	//   Succeeded
	return true;
}

//   FORWARD FOURIER TRANSFORM, INPLACE VERSION
//     Data - both input data and output
//     N    - length of input data
template <class T> bool TFFT<T>::Forward(complex<T> *const Data, const unsigned int N)
{
	//   Check input parameters
	if (!Data || N < 1 || N & (N - 1))
		return false;
	//   Rearrange
	Rearrange(Data, N);
	//   Call FFT implementation
	Perform(Data, N);
	//   Succeeded
	return true;
}

//   INVERSE FOURIER TRANSFORM
//     Input  - input data
//     Output - transform result
//     N      - length of both input data and result
//     Scale  - if to scale result
template <class T> bool TFFT<T>::Inverse(const complex<T> *const Input, complex<T> *const Output, const unsigned int N, const bool Scale /* = true */)
{
	//   Check input parameters
	if (!Input || !Output || N < 1 || N & (N - 1))
		return false;
	//   Initialize data
	Rearrange(Input, Output, N);
	//   Call FFT implementation
	Perform(Output, N, true);
	//   Scale if necessary
	if (Scale)
		TFFT<T>::Scale(Output, N);
	//   Succeeded
	return true;
}

//   INVERSE FOURIER TRANSFORM, INPLACE VERSION
//     Data  - both input data and output
//     N     - length of both input data and result
//     Scale - if to scale result
template <class T> bool TFFT<T>::Inverse(complex<T> *const Data, const unsigned int N, const bool Scale /* = true */)
{
	//   Check input parameters
	if (!Data || N < 1 || N & (N - 1))
		return false;
	//   Rearrange
	Rearrange(Data, N);
	//   Call FFT implementation
	Perform(Data, N, true);
	//   Scale if necessary
	if (Scale)
		TFFT<T>::Scale(Data, N);
	//   Succeeded
	return true;
}

//   Rearrange function
template <class T> void TFFT<T>::Rearrange(const complex<T> *const Input, complex<T> *const Output, const unsigned int N)
{
	//   Data entry position
	unsigned int Target = 0;
	//   Process all positions of input signal
	for (unsigned int Position = 0; Position < N; ++Position)
	{
		//  Set data entry
		Output[Target] = Input[Position];
		//   Bit mask
		unsigned int Mask = N;
		//   While bit is set
		while (Target & (Mask >>= 1))
			//   Drop bit
			Target &= ~Mask;
		//   The current bit is 0 - set it
		Target |= Mask;
	}
}

//   Inplace version of rearrange function
template <class T> void TFFT<T>::Rearrange(complex<T> *const Data, const unsigned int N)
{
	//   Swap position
	unsigned int Target = 0;
	//   Process all positions of input signal
	for (unsigned int Position = 0; Position < N; ++Position)
	{
		//   Only for not yet swapped entries
		if (Target > Position)
		{
			//   Swap entries
			const complex<T> Temp(Data[Target]);
			Data[Target] = Data[Position];
			Data[Position] = Temp;
		}
		//   Bit mask
		unsigned int Mask = N;
		//   While bit is set
		while (Target & (Mask >>= 1))
			//   Drop bit
			Target &= ~Mask;
		//   The current bit is 0 - set it
		Target |= Mask;
	}
}

//   FFT implementation
template <class T> void TFFT<T>::Perform(complex<T> *const Data, const unsigned int N, const bool Inverse /* = false */)
{
	//   Pi constant
	const T pi = Inverse ? T(3.14159265358979323846) : T(-3.14159265358979323846);
	//   Iteration through dyads, quadruples, octads and so on...
	for (unsigned int Step = 1; Step < N; Step <<= 1)
	{
		//   Jump to the next entry of the same transform factor
		const unsigned int Jump = Step << 1;
		//   Angle increment
		const T delta = pi / T(Step);
		//   Auxiliary sin(delta / 2)
		const T Sine = sin(delta * T(.5));
		//   Multiplier for trigonometric recurrence
		const complex<T> Multiplier(T(-2.) * Sine * Sine, sin(delta));
		//   Start value for transform factor, fi = 0
		complex<T> Factor(T(1.));
		//   Iteration through groups of different transform factor
		for (unsigned int Group = 0; Group < Step; ++Group)
		{
			//   Iteration within group 
			for (unsigned int Pair = Group; Pair < N; Pair += Jump)
			{
				//   Match position
				const unsigned int Match = Pair + Step;
				//   Second term of two-point transform
				const complex<T> Product(Factor * Data[Match]);
				//   Transform for fi + pi
				Data[Match] = Data[Pair] - Product;
				//   Transform for fi
				Data[Pair] += Product;
			}
			//   Successive transform factor via trigonometric recurrence
			Factor = Multiplier * Factor + Factor;
		}
	}
}

//   Scaling of inverse FFT result
template <class T> void TFFT<T>::Scale(complex<T> *const Data, const unsigned int N)
{
	//   Scale factor
	const T Factor = T(1.) / T(N);
	//   Scale all data entries
	for (unsigned int Position = 0; Position < N; ++Position)
		Data[Position] *= Factor;
}

#endif

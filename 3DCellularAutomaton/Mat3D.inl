#pragma once

#include "Mat3D.h"

#include <algorithm>

template<typename T>
Mat3D<T>::Mat3D(size_t dimension) : 
	dimension(dimension)
{
	data = new T[dimension*dimension*dimension];
	M = new T**[dimension];
	for (int i = 0; i < dimension; i++) {
		M[i] = new T*[dimension];
		for (int j = 0; j < dimension; j++) {
			M[i][j] = &(data[i*dimension*dimension + j*dimension]);
		}
	}
}

template<typename T>
Mat3D<T>::~Mat3D()
{
	delete[] data;
	for (int i = 0; i < dimension; i++) {
		delete[] M[i];
	}
	delete[] M;
}

template<typename T>
Mat3D<T>::Mat3D(const Mat3D<T>& other)
{
	*this = other;
}

template<typename T>
Mat3D<T>& Mat3D<T>::operator=(const Mat3D<T>& other)
{
	if (dimension != other.dimension) {
		if (data) {
			delete[] data;
			for (int i = 0; i < dimension; i++) {
				delete[] M[i];
			}
			delete[] M;
		}

		dimension = other.dimension;
		data = new T[dimension*dimension*dimension];
		M = new T**[dimension];
		for (int i = 0; i < dimension; i++) {
			M[i] = new T*[dimension];
			for (int j = 0; j < dimension; j++) {
				M[i][j] = &(data[i*dimension*dimension + j*dimension]);
			}
		}
	}

	std::memcpy(data, other.data, sizeof(T)*dimension*dimension*dimension);

	return *this;
}

template<typename T>
Mat3D<T>::Mat3D(Mat3D<T>&& other)
{
	*this = std::move(other);
}

template<typename T>
Mat3D<T>& Mat3D<T>::operator=(Mat3D<T>&& other)
{
	M = other.M;
	data = other.data;
	dimension = other.dimension;

	other.M = nullptr;
	other.data = nullptr;
	other.dimension = 0;

	return *this;
}

template<typename T>
T ** Mat3D<T>::operator[](int index)
{
	return M[index];
}

template<typename T>
T * Mat3D<T>::getDataPtr() const
{
	return data;
}

template<typename T>
size_t Mat3D<T>::getDimension() const
{
	return dimension;
}

template<typename T>
inline void Mat3D<T>::toZero()
{
	std::fill(data, data + dimension*dimension*dimension, 0);
}

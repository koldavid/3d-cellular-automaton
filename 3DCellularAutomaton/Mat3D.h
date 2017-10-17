#pragma once

template<typename T>
class Mat3D
{
public:
	Mat3D(size_t dimension);
	~Mat3D();

	Mat3D(const Mat3D<T>& other);
	Mat3D<T>& operator=(const Mat3D<T>& other);

	Mat3D(Mat3D<T>&& other);
	Mat3D<T>& operator=(Mat3D<T>&& other);

	T** operator[](int index);

	T* getDataPtr() const;

	size_t getDimension() const;

	void toZero();

private:
	T* data;
	T*** M;

	size_t dimension;
};

#include "Mat3D.inl"
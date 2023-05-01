#pragma once

template <typename T>
struct FastList
{
	void* klass;
	void* monitor;
	Array<T>* Array;
	int Count;
	int mCapacity;
};
#pragma once

template <typename T>
struct Array
{
	void* klass;
	void* monitor;
	void* bounds;
	size_t length;
	T items[1024];
};

template <typename T>
struct List
{
	void* klass;
	void* monitor;
	Array<T>* items;
	int size;
	int version;
};

template <typename TKey, typename TValue>
struct Dictionary_entry
{
	void* monitor;
	TKey key;
	TValue value;
};

template <typename TKey, typename TValue>
struct Dictionary
{
	void* klass;
	void* monitor;
	void* buckets;
	Array<Dictionary_entry<TKey, TValue>>* entries;
	uint32_t count;
	uint32_t version;
	uint32_t freeList;
	uint32_t freeCount;
	void* comparer;
	void* keys;
	void* values;
	unsigned char _syncRoot;
};

struct WString
{
	void* klass;
	void* monitor;
	size_t length;
	wchar_t string[1024];

	WString()
	{
		klass = nullptr;
		monitor = nullptr;
		length = 0;
		string[0] = 0;
	}

	WString(std::string str)
	{
		length = str.length();
		for (size_t i = 0; i < length; i++)
			string[i] = str[i];
	}
	std::string ToStr()
	{
		std::wstring wstr = std::wstring(string);
		return std::string(wstr.begin(), wstr.end());
	}
};

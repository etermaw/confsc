#pragma once
#include <vector>
#include <array>
#include <cassert>
#include <stdlib.h>

static constexpr size_t NOT_FOUND = -1;

class string_view
{
	private:
		const char* ptr;
		size_t len;

	public:
		string_view() : ptr(nullptr), len(0) {}
		string_view(const char* c_str) : ptr(c_str), len(strlen(c_str)) {}
		string_view(const char* begin, size_t size) : ptr(begin), len(size) {}
		
		string_view substr(size_t begin, size_t offset) const;
		string_view trim_front(char c) const;
		string_view trim_back(char c) const;
		string_view trim(char c) const;
		string_view trim_whitespaces() const;

		std::vector<string_view> split(char delim, size_t split_count = -1) const;

		template<size_t split_count>
		std::array<string_view, split_count + 1> split(char delim) const;

		size_t find(char c, size_t offset = 0) const;
		size_t find(const char* c, size_t offset = 0) const;

		size_t rfind(char c, size_t offset = 0) const;

		size_t find_first_of(const char* c, size_t offset = 0) const;
		size_t find_last_of(const char* c, size_t offset = 0) const;
		size_t find_first_not_of(const char* c, size_t offset = 0) const;
		size_t find_last_not_of(const char* c, size_t offset = 0) const;

		bool is_number() const;
		bool is_text() const;

		size_t count(char c) const;

		const char* begin() const;
		const char* end() const;

		char front() const;
		char back() const;

		size_t length() const;

		bool operator== (const char* str) const;
		bool operator== (const string_view& other) const;

		char operator[] (size_t index) const;

		int to_int() const;
		uint32_t to_uint() const;
		float to_float() const;
};

inline const char* string_view::begin() const
{
	return ptr;
}

inline const char* string_view::end() const
{
	return ptr + len;
}

inline char string_view::front() const
{
	return *ptr;
}

inline char string_view::back() const
{
	return *(ptr + len - 1);
}

inline size_t string_view::length() const
{
	return len;
}

inline string_view string_view::substr(size_t begin, size_t offset) const
{
	return string_view(ptr + begin, offset);
}

inline string_view string_view::trim_front(char c) const
{
	size_t i = 0;

	while (ptr[i] == c)
		++i;

	return string_view(ptr + i, len - i);
}

inline string_view string_view::trim_back(char c) const
{
	size_t i = len - 1;

	while (ptr[i] == c)
		--i;

	return string_view(ptr, i + 1);
}

inline string_view string_view::trim(char c) const
{
	return trim_front(c).trim_back(c);
}

string_view string_view::trim_whitespaces() const
{
	size_t i = 0;
	size_t j = 0;

	while (i < len && isspace(ptr[len - i - 1]))
		++i;

	while (j < (len - i) && isspace(ptr[j]))
		++j;

	return string_view(ptr + j, len - i - j);
}


template<size_t split_count>
std::array<string_view, split_count + 1> string_view::split(char delim) const
{
	std::array<string_view, split_count + 1> holder;

	size_t last_pos = 0;
	size_t splits = 0;

	for (size_t i = 0; i < len && splits < split_count; ++i)
	{
		if (ptr[i] == delim)
		{
			holder[splits++] = string_view(ptr + last_pos, i - last_pos);

			last_pos = i + 1;
		}
	}

	if (last_pos < len)
		holder[splits] = string_view(ptr + last_pos, len - last_pos);

	return holder;
}

inline std::vector<string_view> string_view::split(char delim, size_t splits_count) const
{
	std::vector<string_view> holder;
	size_t last_pos = 0;
	size_t splits = 0;

	for (size_t i = 0; i < len && splits < splits_count; ++i)
	{
		if (ptr[i] == delim)
		{
			holder.push_back(string_view(ptr + last_pos, i - last_pos));

			last_pos = i + 1;
			++splits;
		}
	}

	if (last_pos < len)
		holder.push_back(string_view(ptr + last_pos, len - last_pos));

	return holder;
}

inline size_t string_view::find(char c, size_t offset) const
{
	for (size_t i = offset; i < len; ++i)
		if (ptr[i] == c)
			return i;

	return NOT_FOUND;
}

inline size_t string_view::find(const char* c, size_t offset) const
{
	size_t str_len = std::strlen(c);
	size_t max_len = str_len > len ? 0 : len - str_len;

	for (size_t i = offset; i < max_len; ++i)
		if (std::memcmp(c, ptr + i, str_len) == 0)
			return i;

	return NOT_FOUND;
}

size_t string_view::rfind(char c, size_t offset) const
{
	for (size_t i = len - offset - 1; i != static_cast<size_t>(-1); --i)
		if (ptr[i] == c)
			return i;

	return NOT_FOUND;
}

inline size_t string_view::find_first_of(const char* c, size_t offset) const
{
	for (size_t i = offset; i < len; ++i)
	{
		const char* j = c;

		while (*j)
			if (ptr[i] == *j++)
				return i;
	}

	return NOT_FOUND;
}

inline size_t string_view::find_last_of(const char* c, size_t offset) const
{
	for (size_t i = len - offset - 1; i != static_cast<size_t>(-1); --i)
	{
		const char* j = c;

		while (*j)
			if (ptr[i] == *j++)
				return i;
	}

	return NOT_FOUND;
}

inline size_t string_view::find_first_not_of(const char* c, size_t offset) const
{
	for (size_t i = offset; i < len; ++i)
	{
		const char* j = c;

		while (*j)
			if (ptr[i] != *j++)
				return i;
	}

	return NOT_FOUND;
}

inline size_t string_view::find_last_not_of(const char* c, size_t offset) const
{
	for (size_t i = len - offset - 1; i != static_cast<size_t>(-1); --i)
	{
		const char* j = c;

		while (*j)
			if (ptr[i] != *j++)
				return i;
	}

	return NOT_FOUND;
}

inline bool string_view::is_number() const
{
	size_t i = 0;

	if (ptr[i] == '-')
		++i;

	for (; i < len; ++i)
		if (!isdigit(ptr[i]))
			return false;

	return true;
}

inline bool string_view::is_text() const
{
	for (size_t i = 0; i < len; ++i)
		if (!isalpha(ptr[i]))
			return false;

	return true;
}

inline size_t string_view::count(char c) const
{
	size_t count = 0;

	for (size_t i = 0; i < len; ++i)
		if (ptr[i] == c)
			count++;

	return count;
}

inline bool string_view::operator== (const char* str) const
{
	size_t length = strlen(str);

	if (len != length)
		return false;

	else
		return (std::memcmp(ptr, str, len) == 0);
}

inline bool string_view::operator== (const string_view& other) const
{
	if (len != other.length())
		return false;

	else
		return (std::memcmp(ptr, other.ptr, len) == 0);
}

inline char string_view::operator[](size_t index) const
{
	assert(index < len && "Index out of range in string_view!");
	return ptr[index];
}

inline int string_view::to_int() const
{
	int value = 0,
		sign = 1,
		cond = len;

	size_t i = 0;

	assert(len < 11 && "String too long to convert into int!");

	if (ptr[i] == '-')
	{
		sign = -1;
		--cond;
		++i;
	}

	switch (cond)
	{
		case 10: value += (ptr[i++] - '0') * 1000000000;
		case 9:	 value += (ptr[i++] - '0') * 100000000;
		case 8:  value += (ptr[i++] - '0') * 10000000;
		case 7:  value += (ptr[i++] - '0') * 1000000;
		case 6:  value += (ptr[i++] - '0') * 100000;
		case 5:  value += (ptr[i++] - '0') * 10000;
		case 4:  value += (ptr[i++] - '0') * 1000;
		case 3:  value += (ptr[i++] - '0') * 100;
		case 2:  value += (ptr[i++] - '0') * 10;
		case 1:  value += (ptr[i] - '0');
	}

	return value * sign;
}

inline unsigned string_view::to_uint() const
{
	size_t value = 0;
	size_t i = 0;

	assert(len < 11 && "String too long to convert into uint!");

	switch (len)
	{
		case 10: value += (ptr[i++] - '0') * 1000000000;
		case 9:	 value += (ptr[i++] - '0') * 100000000;
		case 8:  value += (ptr[i++] - '0') * 10000000;
		case 7:  value += (ptr[i++] - '0') * 1000000;
		case 6:  value += (ptr[i++] - '0') * 100000;
		case 5:  value += (ptr[i++] - '0') * 10000;
		case 4:  value += (ptr[i++] - '0') * 1000;
		case 3:  value += (ptr[i++] - '0') * 100;
		case 2:  value += (ptr[i++] - '0') * 10;
		case 1:  value += (ptr[i] - '0');
	}

	return value;
}

inline float string_view::to_float() const
{
	auto parts = split<1>('.');

	float sign = parts[0].front() == '-' ? -1.0f : 1.0f;
	parts[0] = parts[0].trim_front('-');

	float int_part = static_cast<float>(parts[0].to_uint());

	if (parts[1].length() != 0)
	{
		float frac_part = static_cast<float>(parts[1].to_uint());

		return sign * (int_part + (frac_part * pow(0.1f, parts[1].length())));
	}

	else
		return sign * int_part;
}

namespace std
{
	template<>
	struct hash<string_view>
	{
		using result_type = size_t;
		using argument_type = string_view;

		size_t operator() (const argument_type& val) const
		{
			static constexpr size_t offset = 2166136261U;
			static constexpr size_t prime = 16777619U;

			auto hash = offset;

			for (auto i : val)
			{
				hash ^= static_cast<size_t>(i);
				hash *= prime;
			}

			return hash;
		}
	};
}
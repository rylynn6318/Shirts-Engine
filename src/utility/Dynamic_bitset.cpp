#include "utility/Dynamic_bitset.h"
#include <algorithm>
#include <string>

se::Dynamic_bitset::Dynamic_bitset() : _bitset(0), numbits(se::Dynamic_bitset::bit_size)
{
	bitPtr = new bitset[bit_size];
}

se::Dynamic_bitset::~Dynamic_bitset()
{
	delete[] bitPtr;
	bitPtr = nullptr;
}

size_t se::Dynamic_bitset::size() const noexcept
{
	return numbits;
}

void se::Dynamic_bitset::clear()
{
	bitPtr->reset();
}

void se::Dynamic_bitset::resize(size_t size)
{
	if (numbits <= size)
	{
		size_t block_size = size / bit_size;
		numbits = bit_size * (block_size + 1);
		auto newPtr = new bitset[block_size + 1];
		std::copy(bitPtr, bitPtr + block_size + 1, newPtr);
		delete[] bitPtr;

		bitPtr = newPtr;
	}
}

void se::Dynamic_bitset::set(size_t pos)
{
	if (numbits <= pos)
	{
		resize(pos);
	}
	auto& bits = bitPtr[pos / bit_size];
	bits[pos % bit_size] = pos;
}

bool se::Dynamic_bitset::get(size_t pos) const
{
	if (numbits <= pos)
		return false;

	auto& bits = bitPtr[pos / bit_size];
	return bits[pos % bit_size];
}
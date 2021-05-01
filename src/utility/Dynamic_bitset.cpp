#include "utility/Dynamic_bitset.h"
#include <algorithm>
#include <string>
#include <iostream>

se::dynamic_bitset::dynamic_bitset() :
	numbits(bit_size),
	bitArray{ bitset() }
{

}

std::size_t se::dynamic_bitset::size() const noexcept
{
	return numbits;
}

bool se::dynamic_bitset::operator==(const dynamic_bitset& other) const
{
	return is_same(other);
}

void se::dynamic_bitset::clear()
{
	for (auto& bitset : bitArray)
	{
		bitset.reset();
	}
}

void se::dynamic_bitset::resize(std::size_t size)
{
	if (numbits <= size)
	{
		std::size_t block_size = size / bit_size + 1 - bitArray.size();
		numbits = bit_size * (block_size + 1);
		bitArray.reserve(bitArray.size() + block_size);
		for (int i = 0; i < block_size; i++)
		{
			bitArray.push_back(bitset());
		}
	}
}

void se::dynamic_bitset::set(std::size_t pos)
{
	if (numbits <= pos)
	{
		resize(pos);
	}

	auto& bits = bitArray[pos / bit_size];
	bits[pos % bit_size] = true;
}

void se::dynamic_bitset::reset(std::size_t pos)
{
	if (numbits > pos)
	{
		auto& bits = bitArray[pos / bit_size];
		bits[pos % bit_size] = false;
	}
}

bool se::dynamic_bitset::get(std::size_t pos) const
{
	if (numbits <= pos)
		return false;

	auto& bits = bitArray[pos / bit_size];
	return bits[pos % bit_size];
}

std::string se::dynamic_bitset::to_string() const noexcept
{
	std::string s;
	auto iter = bitArray.crbegin();
	for (; iter != bitArray.crend(); ++iter)
	{
		s += iter->to_string() + " ";
	}

	return s;
}

bool se::dynamic_bitset::is_proper_subset_of(const dynamic_bitset& other) const
{
	if (is_same(other))
		return false;

	return is_subset_of(other);
}

bool se::dynamic_bitset::is_subset_of(const dynamic_bitset& other) const
{
	if (bitArray.size() > other.bitArray.size())
	{
		for (int i = other.bitArray.size(); i < bitArray.size(); i++)
		{
			if (bitArray[i].any())
			{
				return false;
			}
		}
	}
	else
	{
		for (int i = 0; i < bitArray.size(); i++)
		{
			if ((bitArray[i] & other.bitArray[i]) != bitArray[i])
			{
				return false;
			}
		}
	}

	return true;
}

std::size_t se::dynamic_bitset::find_first() const noexcept
{
	return find_from(0);
}

std::size_t se::dynamic_bitset::find_next(std::size_t pos) const noexcept
{
	return find_from(pos + 1);
}

std::size_t se::dynamic_bitset::find_from(std::size_t pos) const noexcept
{
	bool flag = true;
	for (int i = pos / bit_size; i < bitArray.size(); i++)
	{
		for (int j = 0; j < bitArray[i].size(); j++)
		{
			if (i == 0 && flag)
			{
				j = pos;
				flag = false;
			}
			if (bitArray[i][j])
			{
				return i * bit_size + j;
			}
		}
	}

	return npos;
}

bool se::dynamic_bitset::is_same(const dynamic_bitset& other) const
{
	auto& big = (bitArray.size() <= other.bitArray.size()) ? other.bitArray : bitArray;
	auto& small = (bitArray.size() > other.bitArray.size()) ? other.bitArray : bitArray;

	for (int i = small.size(); i < big.size(); i++)
	{
		if (big[i].any())
			return false;
	}

	if (std::equal(small.cbegin(), small.cend(), big.cbegin()))
	{
		return true;
	}
	else
	{
		return false;
	}
}
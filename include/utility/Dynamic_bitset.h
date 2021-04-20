#pragma once
#pragma once
#include <bitset>
#include <vector>

namespace se
{
	class Dynamic_bitset
	{
	public:
		static constexpr std::size_t bit_size = 8;
		static constexpr std::size_t npos = SIZE_MAX;

		using bitset = std::bitset<bit_size>;
		 
		Dynamic_bitset();
		~Dynamic_bitset() = default;
		Dynamic_bitset(const Dynamic_bitset&) = default;
		Dynamic_bitset& operator=(const Dynamic_bitset&) = default;
		bool operator==(const Dynamic_bitset&) const;

		void set(std::size_t pos);
		void reset(std::size_t pos);
		bool get(std::size_t pos) const;
		std::size_t size() const noexcept;
		void clear();
		void resize(std::size_t size);
		bool empty() const noexcept;
		std::string to_string() const noexcept;

		bool is_proper_subset_of(const Dynamic_bitset& bitset) const;
		std::size_t find_first() const noexcept;
		std::size_t find_next(std::size_t pos) const noexcept;

	private:
		std::size_t numbits;
		std::vector<bitset> bitArray;
		std::size_t find_from(std::size_t pos) const noexcept;
	};
}
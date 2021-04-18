#pragma once
#include <bitset>

namespace se
{
	class Dynamic_bitset
	{
	public:
		static constexpr size_t bit_size = 8;

		using size_t = std::size_t;
		using bitset = std::bitset<bit_size>;

		Dynamic_bitset();
		~Dynamic_bitset();
		Dynamic_bitset(const Dynamic_bitset&) = delete;
		Dynamic_bitset& operator=(const Dynamic_bitset&) = delete;

		void set(size_t pos);
		bool get(size_t pos) const;
		size_t size() const noexcept;
		void clear();
		void resize(size_t size);
		bool empty() const noexcept;
		std::string to_string() const noexcept;

		bool is_proper_subset_of(const Dynamic_bitset& bitset) const;

	private:
		size_t numbits;

		bitset _bitset;
		bitset* bitPtr;
	};
}
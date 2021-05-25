#include <type_traits>
#include "core/input/Key.h"

auto se::operator++(se::Key& x) -> se::Key
{
	return x = (Key)(std::underlying_type<Key>::type(x) + 1);
}

auto se::operator*(se::Key k) -> se::Key
{
	return k;
}

auto se::begin(se::Key) -> se::Key
{
	return Key::FIRST;
}

auto se::end(se::Key) -> se::Key
{
	Key l = Key::LAST;
	return ++l;
}
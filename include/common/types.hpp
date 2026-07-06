#pragma once 
#include <concepts>
#include "defs.hpp"

template <typename T>
concept AccessWidth = std::same_as<T, u8> || std::same_as<T, u16> || std::same_as<T, u32> || std::same_as<T, u64>;
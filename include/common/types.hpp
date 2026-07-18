#pragma once 
#include <concepts>
#include "defs.hpp"
#include <filesystem>

namespace fs = std::filesystem;

template <typename T>
concept AccessWidth = std::same_as<T, u8> || std::same_as<T, u16> || std::same_as<T, u32> || std::same_as<T, u64>;
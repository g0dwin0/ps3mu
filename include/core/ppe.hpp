#pragma once
#include <SDL3/SDL_audio.h>

#include <queue>

#include "common/defs.hpp"
#include "enums.hpp"

struct PPE {
  std::array<u64, 32> gpr;
  std::array<u64, 32> fpr;
  u64 lr;
  u64 ctr;
  u64 xer;
  u32 fpscr;
  u32 cr;
};
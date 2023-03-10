#pragma once

#include "pixel.hpp"

namespace yadl
{
   namespace Color
   {
      constexpr Pixel Black = Pixel(0, 0, 0, 255);
      constexpr Pixel White = Pixel(255, 255, 255, 255);
      constexpr Pixel Red = Pixel(255, 0, 0, 255);
      constexpr Pixel Green = Pixel(0, 255, 0, 255);
      constexpr Pixel Blue = Pixel(0, 0, 255, 255);
      constexpr Pixel Yellow = Pixel(255, 255, 0, 255);
      constexpr Pixel Cyan = Pixel(0, 255, 255, 255);
      constexpr Pixel Magenta = Pixel(255, 0, 255, 255);
      constexpr Pixel Transparent = Pixel(0, 0, 0, 0);
      constexpr Pixel Dark = Pixel(0x18, 0x18, 0x18, 255);
   }
}
#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include "canvas.hpp"
#include <optional>
#include "animation.hpp"
#include "pixel.hpp"

namespace yadl
{
    namespace io
    {
        std::optional<Canvas> Load(const std::string& filename);
        bool SaveAsPNG(const std::string& filename, const Canvas& canvas);
        bool SaveAsPPM6(const std::string& filename, const Canvas& canvas);
        bool SaveAsPPM3(const std::string& filename, const Canvas& canvas);
        bool SaveAsGIF(const std::string& filename, const Animation& animation, int32_t delay = 10);
    }
}
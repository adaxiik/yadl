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
        /**
         * @brief Load a canvas from a file.
         * 
         * @param filename 
         * @return std::optional<Canvas> 
         */
        std::optional<Canvas> Load(const std::string& filename);

        /**
         * @brief Save a canvas as PNG
         * 
         * @sa SaveAsPPM6
         * @sa SaveAsPPM3
         * @sa SaveAsGIF
         * 
         * @param filename 
         * @param canvas 
         * @return true if the file was saved successfully
         * @return false otherwise
         */
        bool SaveAsPNG(const std::string& filename, const Canvas& canvas);
        
        /**
         * @brief Save a canvas as PPM6
         * 
         * @sa SaveAsPNG
         * @sa SaveAsPPM3
         * @sa SaveAsGIF
         * 
         * @param filename 
         * @param canvas 
         * @return true if the file was saved successfully
         * @return false otherwise
         */
        bool SaveAsPPM6(const std::string& filename, const Canvas& canvas);
        
        /**
         * @brief Save a canvas as PPM3
         * 
         * @sa SaveAsPNG
         * @sa SaveAsPPM6
         * @sa SaveAsGIF
         * 
         * @param filename 
         * @param canvas 
         * @return true if the file was saved successfully
         * @return false otherwise
         */
        bool SaveAsPPM3(const std::string& filename, const Canvas& canvas);
        
        /**
         * @brief Save an animation as a GIF
         * 
         * @sa SaveAsPNG
         * @sa SaveAsPPM6
         * @sa SaveAsPPM3
         * 
         * @param filename 
         * @param animation 
         * @param delay 
         * @return true if the file was saved successfully
         * @return false otherwise
         */
        bool SaveAsGIF(const std::string& filename, const Animation& animation, int32_t delay = 10);


        /**
         * @brief Save an animation as a Y4M file
         * 
         * @param filename 
         * @param animation 
         * @param fps 
         * @return true if the file was saved successfully
         * @return false otherwise
         */
        bool SaveAsY4M(const std::string& filename, const Animation& animation, uint32_t fps = 30);
    }
}
#include "font.hpp"
#include <stdexcept>

#include "font_internal.hpp"

namespace yadl
{
    Font::Font(const std::string &path) : m_internal(std::make_shared<FontInternal>(path))
    {
    }

    Font::~Font()
    {
    }
}
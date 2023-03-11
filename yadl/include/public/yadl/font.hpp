#pragma once

#include <string>
#include <memory>
namespace yadl
{
    class FontInternal;
    
    class Font
    {
    private:
        friend class Text;
        std::shared_ptr<FontInternal> m_internal;

    public:
        Font() = default;
        Font(const std::string& path);
        Font(const Font& other) = default;
        Font& operator=(const Font& other) = default;
        ~Font();

    };

}

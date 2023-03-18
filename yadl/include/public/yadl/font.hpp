#pragma once

#include <string>
#include <memory>
namespace yadl
{
    class FontInternal;
    
    class Font
    {
    private:
        std::shared_ptr<FontInternal> m_internal;

    public:
        Font() = default;
        Font(const std::string& path);
        Font(const Font& other) = default;
        Font& operator=(const Font& other) = default;
        Font(Font&& other) noexcept = default;
        Font& operator=(Font&& other) noexcept = default;
        ~Font() = default;

        std::shared_ptr<FontInternal> GetInternal() const { return m_internal; };

    };

}

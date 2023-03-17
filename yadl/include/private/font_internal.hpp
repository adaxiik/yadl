#pragma once
#include <string>
#include <memory>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "canvas.hpp"

namespace yadl
{
    class FontInternal
    {
    private:
        constexpr static int m_fontSize{48};
        constexpr static int m_glyphMetricsCapacity{128};
        FT_Library m_library;
        FT_Face m_face;


        std::map<char, Canvas> m_glyphCanvasMap;

        FontInternal();

    public:
        FontInternal(const std::string &path);
        FontInternal(const FontInternal &other) = delete;
        FontInternal &operator=(const FontInternal &other) = delete;
        FontInternal(FontInternal &&other) = default;
        FontInternal &operator=(FontInternal &&other) = default;
        ~FontInternal();
        struct GlyphMetrics
        {
            FT_UInt width;
            FT_UInt height;
            FT_Int bearingX;
            FT_Int bearingY;
            FT_UInt advance;
        };

        const Canvas &GetCharCanvas(char c) const;
        const GlyphMetrics &GetGlyphMetrics(char c) const;

    private:
        struct GlyphAtlas
        {
            FT_UInt m_atlasWidth;
            FT_UInt m_atlasHeight;
            GlyphMetrics m_glyphMetrics[m_glyphMetricsCapacity] = {}; // todo: unify map or array usage
        };
        GlyphAtlas m_glyphAtlas{};

    };
}
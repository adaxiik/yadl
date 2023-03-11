#include "font_internal.hpp"

#include <stdexcept>
#include <iostream>
#include "canvas.hpp"
namespace yadl
{
    FontInternal::FontInternal()
    {
        FT_Error error = FT_Init_FreeType(&m_library);
        if (error)
            throw std::runtime_error("Failed to initialize FreeType library");
    }

    FontInternal::FontInternal(const std::string &path) : FontInternal()
    {
        FT_Error error = FT_New_Face(m_library, path.c_str(), 0, &m_face);

        if (error == FT_Err_Unknown_File_Format)
            throw std::runtime_error("Failed to load font: unknown file format");
        else if (error)
            throw std::runtime_error("Failed to load font");

        error = FT_Set_Pixel_Sizes(m_face, 0, m_fontSize);
        if (error)
            throw std::runtime_error("Failed to set font size");
        


        FT_Int32 flags = FT_LOAD_RENDER /*| FT_LOAD_TARGET_(FT_RENDER_MODE_SDF) */;
        
        for(int i = 32; i < 128; i++)
        {
            error = FT_Load_Char(m_face, i, flags);
            if (error)
                throw std::runtime_error("Failed to load glyph");

            m_glyphAtlas.m_atlasWidth += m_face->glyph->bitmap.width;
            if(m_glyphAtlas.m_atlasHeight < m_face->glyph->bitmap.rows)
                m_glyphAtlas.m_atlasHeight = m_face->glyph->bitmap.rows;
        }

        
        int atlasOffset = 0;
        for(int i = 32; i < 128; i++)
        {
            error = FT_Load_Char(m_face, i, flags);
            if (error)
                throw std::runtime_error("Failed to load glyph");

            FT_Bitmap *bitmap = &m_face->glyph->bitmap;
            m_glyphCanvasMap[i] = Canvas(bitmap->width, bitmap->rows);

            int glyphOffset = 0;
            for(size_t y = 0; y < bitmap->rows; y++)
            {
                for(size_t x = 0; x < bitmap->width; x++)
                {
                    uint8_t alpha = bitmap->buffer[glyphOffset + x];
                    m_glyphCanvasMap[i].SetPixel(x, y, Pixel(255,255,255, alpha));
                }

                atlasOffset += m_glyphAtlas.m_atlasWidth;
                glyphOffset += bitmap->pitch;
            }

            atlasOffset -= m_glyphAtlas.m_atlasWidth * bitmap->rows;
            atlasOffset += bitmap->width;

            m_glyphAtlas.m_glyphMetrics[i].m_width = bitmap->width;
            m_glyphAtlas.m_glyphMetrics[i].m_height = bitmap->rows;
            m_glyphAtlas.m_glyphMetrics[i].m_bearingX = m_face->glyph->bitmap_left;
            m_glyphAtlas.m_glyphMetrics[i].m_bearingY = m_face->glyph->bitmap_top;
            m_glyphAtlas.m_glyphMetrics[i].m_advance = m_face->glyph->advance.x / 64;

        }

    }

    FontInternal::~FontInternal()
    {
        FT_Done_Face(m_face);
        FT_Done_FreeType(m_library);
    }

    const Canvas& FontInternal::GetCharCanvas(char c) const
    {
        if(m_glyphCanvasMap.find(c) == m_glyphCanvasMap.end())
            return m_glyphCanvasMap.at('?');
        return m_glyphCanvasMap.at(c);
    }

    const FontInternal::GlyphMetrics& FontInternal::GetGlyphMetrics(char c) const
    {
        if(c < 0 || c >= m_glyphMetricsCapacity)
            return m_glyphAtlas.m_glyphMetrics['?'];
        return m_glyphAtlas.m_glyphMetrics[static_cast<int>(c)];
    }

}
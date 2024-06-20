#include "FontManager.h"
#include "Logger.h"
#include "Utility.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "3rdParty/json.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H  

FontManager::FontManager() {}

void FontManager::unloadFont(std::string identifier) {
    if (loadedFonts.find(identifier) == loadedFonts.end()) { return; }
    for (auto& it : loadedFonts[identifier]) {
        glDeleteTextures(1, &it.second.textureID);
    }
    loadedFonts.erase(identifier);
}

std::map<char, Character> FontManager::getFont(std::string identifier) {
    if (loadedFonts.find(identifier) == loadedFonts.end()) {
        LOG_ERROR("Font was requested that does not exist: " + identifier);
        std::map<char, Character> blank;
        return blank;
    }
    return loadedFonts[identifier];
}

int FontManager::loadFont(std::string filename, int fontsize, std::string identifier) {

    std::map<char, Character> Characters;

    FT_Library ft;
    FT_Face face;

    if (FT_Init_FreeType(&ft)) {
        LOG_ERROR("Could not initialize FreeType library.");
        return 1;
    }
    if (FT_New_Face(ft, filename.c_str(), 0, &face)) {
        LOG_ERROR("Could not load font: " + filename);
        FT_Done_FreeType(ft);
        return 1;
    }

    FT_Set_Pixel_Sizes(face, 0, fontsize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    glDisable(GL_BLEND);

    for (unsigned char c = 0; c < 128; c++) {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            LOG_ERROR("Failed to load glyph (" + std::to_string(c) + ") in font: " + filename);
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    unloadFont(identifier); // unload the existing font if any
    loadedFonts[identifier] = Characters;

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return 0;
}

// Call once to load a JSON file into the loadedFonts for use.
// The JSON object should be in the following format:
// {
//   "FONT_IDENTIFIER": {
//        "path": "assets/fonts/public_pixel.ttf",
//        "size" : 32
//    }
// }
void FontManager::buildFontList(const char* filepath) {
    std::string s = readFileIntoString(filepath);
    if (s.length() == 0) {
        LOG_ERROR("Could not build font library because JSON file could not be read.");
        return;
    }
    if (!nlohmann::json::accept(s)) {
        LOG_ERROR("Could not build font library because JSON is malformed.");
        return;
    }
    nlohmann::json jsondata = nlohmann::json::parse(s);
    for (auto it = jsondata.begin(); it != jsondata.end(); it++) {
        try {
            std::string newIdentifier = it.key();
            std::string newPath = it.value()["path"];
            int newSize = it.value()["size"];
            loadFont(newPath, newSize, newIdentifier);
        }
        catch (nlohmann::json::exception& e) {
            LOG_ERROR("Font data JSON for (", it.key(), ") contained inappropriate data types. Skipping loading.");
        }
    }
}
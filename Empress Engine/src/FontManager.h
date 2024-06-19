#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <map>
#include <string>

#include <glm/glm.hpp>

struct Character {
    unsigned int textureID;  // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};

class FontManager {
public:
    FontManager();
    int loadFont(std::string filename, int fontsize, std::string identifier);
    void buildFontList(const char* filepath);
    void unloadFont(std::string identifier);
    std::map<char, Character> getFont(std::string identifier);
private:
    std::map<std::string, std::map<char, Character>> loadedFonts;
};

#endif // FONT_MANAGER_H
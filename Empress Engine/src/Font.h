#ifndef FONT_H
#define FONT_H

#include <map>
#include <string>

#include <glm/glm.hpp>

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class FontManager {
public:
    FontManager();
    int loadFont(std::string filename);
private:
};

extern std::map<char, Character> Characters;
#endif // FONT_H
#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

inline const char* WINDOW_LABEL = "My Window";
inline const int DEFAULT_SCREEN_WIDTH = 1280;
inline const int DEFAULT_SCREEN_HEIGHT = 960;
inline const float SCREEN_CLEAR_COLOR[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
inline const int TARGET_FPS = 60;

inline const char* ANIMATION_MANIFEST_FILEPATH = "assets/animation_manifest.json";
inline const char* SHADER_MANIFEST_FILEPATH = "assets/shaders/shader_manifest.json";

inline const bool HOT_TEXTURE_SWAPPING_ENABLED = true;
inline const bool HOT_SHADER_SWAPPING_ENABLED = true;

#endif // GAME_CONFIG_H
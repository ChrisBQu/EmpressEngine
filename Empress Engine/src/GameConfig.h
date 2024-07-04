#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

inline constexpr const char* WINDOW_LABEL = "My Window";

inline constexpr int DEFAULT_SCREEN_WIDTH = 1280;
inline constexpr int DEFAULT_SCREEN_HEIGHT = 720;
inline constexpr int TARGET_FPS = 60;

inline constexpr int SOUND_FREQUENCY = 44100;
inline constexpr int SOUND_HARDWARE_CHANNELS = 2;
inline constexpr int SOUND_SAMPLE_SIZE = 4096;

inline constexpr float SCREEN_CLEAR_COLOR[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

inline constexpr const char* ANIMATION_MANIFEST_FILEPATH = "assets/animation_manifest.json";
inline constexpr const char* SHADER_MANIFEST_FILEPATH = "assets/shaders/shader_manifest.json";
inline constexpr const char* FONT_MANIFEST_FILEPATH = "assets/fonts/font_manifest.json";

inline constexpr bool HOT_TEXTURE_SWAPPING_ENABLED = true;
inline constexpr bool HOT_SHADER_SWAPPING_ENABLED = true;

inline constexpr bool PRINT_FPS_TO_CONSOLE = false;

inline constexpr bool DRAW_BOUNDING_BOXES = true;
inline constexpr int LINE_SEGMENTS_TO_APPROXIMATE_CIRCLE = 16;

struct GameConfiguration {
	int screenWidth;
	int screenHeight;
	int FPS;
};

extern GameConfiguration gameConfig;


#endif // GAME_CONFIG_H
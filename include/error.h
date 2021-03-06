#ifndef Error_H
#define Error_H

/// <summary>
/// Namespace to store all SDL errors
/// </summary>
namespace Error
{
  static const int SDL_INIT_ERROR = 1;
  static const int SDL_WINDOW_ERROR = 2;
  static const int SDL_WINDOW_SURFACE_ERROR = 3;
  static const int SDL_LOAD_BMP_ERROR = 4;
  static const int SDL_INIT_TTF_ERROR = 5;
  static const int SDL_LOAD_FONT_ERROR = 6;
}
#endif
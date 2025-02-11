/**
 * @file displayPbm.h
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief
 * @version 0.1
 * @date 18-12-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef DISPLAY_PBM_H
#define DISPLAY_PBM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "SDL2/SDL.h"
#include <stdint.h>

#include "../pbm.h"

pbm_return pbm_loadImage(const char *imagePath, pbm_image *imageHandler);

pbm_return pbm_saveImage(const char *imagePath, const pbm_image *imageHandler);

pbm_return pbm_displayImage(SDL_Renderer *screen, const pbm_image *image);

#ifdef __cplusplus
}
#endif

#endif /*DISPLAY_PBM_H*/
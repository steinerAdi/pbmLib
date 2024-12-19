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

#include "SDL/SDL.h"
#include <stdint.h>

#include "../pbm.h"

PBM_RETURN pbm_load(const char *imagePath, pbm_Image *imageHandler);

PBM_RETURN pbm_save(const char *imagePath, const pbm_Image *imageHandler);

PBM_RETURN pbm_display(SDL_Surface *screen, const pbm_Image *image);

#ifdef __cplusplus
}
#endif

#endif /*DISPLAY_PBM_H*/
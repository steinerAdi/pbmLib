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

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t *data; // Die Bilddaten (ein Byte pro Pixel)
} pbm_Image;

int pbm_load(const char *imagePath, pbm_Image *imageHandler);

void pbm_display(SDL_Surface *screen, pbm_Image *image);

#ifdef __cplusplus
}
#endif

#endif /*DISPLAY_PBM_H*/
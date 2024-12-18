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

typedef struct {
    int width;
    int height;
    unsigned char *data; // Die Bilddaten (ein Byte pro Pixel)
} pbm_Image;

pbm_Image pbm_load(const char *imagePath);

void pbm_display(SDL_Surface *screen, char * filename);

#ifdef __cplusplus
}
#endif

#endif /*DISPLAY_PBM_H*/
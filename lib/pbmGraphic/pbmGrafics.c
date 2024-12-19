/**
 * @file pbmGrafics.c
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief
 * @version 0.1
 * @date 19-12-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "pbmGrafics.h"
#include <stddef.h>

PBM_RETURN pbm_fill(pbm_Image *imageHandler, PBM_COLORS color){
    if(NULL == imageHandler){
        return PBM_ARGUMENTS;
    }

    uint32_t imageDataSize = (imageHandler->width * imageHandler->height + 7) / 8;

    uint8_t fillValue = UINT8_MAX * (uint8_t) color;

    for (uint32_t i = 0; i < imageDataSize; i++)
    {
        imageHandler->data[i] = color;
    }    
}

PBM_RETURN pbm_setPixel(pbm_Image *imageHandler, uint32_t x, uint32_t y, PBM_COLORS color);

PBM_RETURN pbm_writeChar(pbm_Image *imageHandler, uint32_t x, uint32_t y, PBM_COLORS color, char character);

PBM_RETURN pbm_writeString(pbm_Image *imageHandler, uint32_t x, uint32_t y, PBM_COLORS color, const char *msg);

PBM_RETURN pbm_drawLine(pbm_Image *imageHandler, uint32_t xStart, uint32_t yStart, uint32_t xEnd, uint32_t yEnd, PBM_COLORS color);
/**
 * @file pbmGraphics.h
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief
 * @version 0.1
 * @date 19-12-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef PBM_GRAPHICS_H
#define PBM_GRAPHICS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../pbm.h"

pbm_return pbm_fill(pbm_image *imageHandler, pbm_colors color);

pbm_return pbm_setPixel(pbm_image *imageHandler, uint32_t x, uint32_t y, pbm_colors color);

pbm_return pbm_writeChar(pbm_image *imageHandler, uint32_t x, uint32_t y, pbm_colors color, char character);

pbm_return pbm_writeString(pbm_image *imageHandler, uint32_t x, uint32_t y, pbm_colors color, const char *msg);

pbm_return pbm_drawLine(pbm_image *imageHandler, uint32_t xStart, uint32_t yStart, uint32_t xEnd, uint32_t yEnd,
                        pbm_colors color);

// PBM_RETURN pbm_drawCircle(pbm_Image *imageHandler)

#ifdef __cplusplus
}
#endif

#endif /*PBM_GRAPHICS_H*/
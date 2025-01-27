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

#include "pbmGraphics.h"
#include <stddef.h>

pbm_return pbm_fill(pbm_image *imageHandler, pbm_colors color) {
  if (NULL == imageHandler) {
    return PBM_ARGUMENTS;
  }

  uint32_t imageDataSize = (imageHandler->width * imageHandler->height + 7) / 8;

  uint8_t fillValue = UINT8_MAX * (uint8_t)color;

  for (uint32_t i = 0; i < imageDataSize; i++) {
    imageHandler->data[i] = fillValue;
  }
  return PBM_OK;
}

pbm_return pbm_setPixel(pbm_image *imageHandler, uint32_t x, uint32_t y, pbm_colors color) {
  if (imageHandler == NULL || x > imageHandler->width || y > imageHandler->height) {
    return PBM_ARGUMENTS;
  }
  uint32_t width = x / 8;
  uint8_t pattern = 0x80 >> (x % 8);
  uint32_t bytePosition = y * imageHandler->width / 8 + width;
  printf("Pattern: %d, position: %d\n", pattern, bytePosition);
  if (color) {
    imageHandler->data[bytePosition] |= pattern;
  } else {
    imageHandler->data[bytePosition] &= ~pattern;
  }
  return PBM_OK;
}

pbm_return pbm_writeChar(pbm_image *imageHandler, uint32_t x, uint32_t y, pbm_colors color, char character);

pbm_return pbm_writeString(pbm_image *imageHandler, uint32_t x, uint32_t y, pbm_colors color, const char *msg);

pbm_return pbm_drawLine(pbm_image *imageHandler, uint32_t xStart, uint32_t yStart, uint32_t xEnd, uint32_t yEnd,
                        pbm_colors color);
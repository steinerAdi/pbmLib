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
#include <stdlib.h>

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
                        pbm_colors color) {
  if (NULL == imageHandler || xStart > imageHandler->width || xEnd > imageHandler->width ||
      yStart > imageHandler->height || yEnd > imageHandler->height) {
    return PBM_ARGUMENTS;
  }
  int32_t xDiff = xEnd - xStart;
  int32_t yDiff = yEnd - yStart;
  uint32_t interpolationDuration;
  double xInterpolation;
  double yInterpolation;
  if (abs(xDiff) > abs(yDiff)) {
    interpolationDuration = abs(xDiff);
    if (xDiff > 0) {
      xInterpolation = 1;
    } else {
      xInterpolation = -1;
    }
    yInterpolation = (double)yDiff / abs(xDiff);
  } else {
    interpolationDuration = abs(yDiff);
    xInterpolation = (double)xDiff / abs(yDiff);
    if (yDiff > 0) {
      yInterpolation = 1;
    } else {
      yInterpolation = -1;
    }
  }
  for (uint32_t i = 0; i < interpolationDuration; i++) {
    pbm_setPixel(imageHandler, xStart + xInterpolation * i, yStart + yInterpolation * i, color);
  }
  return PBM_OK;
}
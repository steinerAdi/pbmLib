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

  uint32_t imageDataSize = (imageHandler->width * imageHandler->height) / 8;

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
  uint8_t pattern;
  uint32_t bytePosition;

  switch (imageHandler->alignment) {
  case PBM_DATA_HORIZONTAL_MSB:
    // uint32_t width = x / 8;
    pattern = 0x80 >> (x % 8);
    bytePosition = (y * imageHandler->width + x) / 8;
    break;
  case PBM_DATA_VERTICAL_LSB:
    uint32_t height = y / 8;
    pattern = (y % 8);
    bytePosition = height * imageHandler->width / 8 + x;
    break;
  default:
    return PBM_ERROR;
  }

  if (color) {
    imageHandler->data[bytePosition] |= pattern;
  } else {
    imageHandler->data[bytePosition] &= ~pattern;
  }
  return PBM_OK;
}

pbm_return pbm_drawLine(
  pbm_image *imageHandler,
  uint32_t xStart,
  uint32_t yStart,
  uint32_t xEnd,
  uint32_t yEnd,
  pbm_colors color) {
  if (
    NULL == imageHandler || xStart > imageHandler->width || xEnd > imageHandler->width ||
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

pbm_return
pbm_writeChar(pbm_image *imageHandler, uint32_t x, uint32_t y, pbm_colors color, pbm_font *font, char character) {
  if (NULL == imageHandler || NULL == font) {
    return PBM_ARGUMENTS;
  }
  // Check first the correct alignment

  // Set horizontal alignment
  // Set the correct size
  // uint32_t startBytePosition = (y * imageHandler->width + x) / 8;

  uint32_t startFontIndex = (uint32_t)character * font->height;
  for (uint32_t line = 0; line < font->height; line++) {
    for (uint32_t i = 0; i < font->width; i++) {
      if (font->fontData[startFontIndex + line] & (0x1 << (font->width - 1 - i) % 8)) {
        pbm_setPixel(imageHandler, x + i, y + line, color);
      } else {
        pbm_setPixel(imageHandler, x + i, y + line, !color);
      }
    }
  }
  return PBM_OK;
}

pbm_return
pbm_writeString(pbm_image *imageHandler, uint32_t x, uint32_t y, pbm_colors color, pbm_font *font, const char *msg) {
  if (NULL == imageHandler || NULL == font || NULL == msg) {
    return PBM_ARGUMENTS;
  }
  uint32_t currentX = x;
  while (*msg != '\0') {
    pbm_writeChar(imageHandler, currentX, y, color, font, *msg++);
    currentX += 8;
  }

  return PBM_OK;
}
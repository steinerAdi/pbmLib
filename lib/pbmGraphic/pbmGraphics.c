/**
 * @file pbmGraphics.c
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief Graphic library to set in an PBM image pixels, lines and writing to it.
 * @version 0.1
 * @date 19-12-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "pbmGraphics.h"
#include <stddef.h>
#include <stdlib.h>

#define CHARACTER_GAP (2) ///< Character gap for writing a string

/**
 * @brief function pointer to set offsets for bytes and bites
 *
 * @param uint32_t index the current line index
 * @param pbm_font font handler to get the width or height
 */
typedef uint32_t (*offsetCalculation)(uint32_t, pbm_font *);

/**
 * @brief byte offset calculation for horizontal MSB data
 *
 * @param index current horizontal data index
 * @param font the current used font
 * @return uint32_t the calculated byte offset
 */
uint32_t byteOffset_horizontalMSB(uint32_t index, pbm_font *font);

/**
 * @brief byte offset calculation for horizontal LSB data
 *
 * @param index current horizontal data index
 * @param font the current used font
 * @return uint32_t the calculated byte offset
 */
uint32_t byteOffset_horizontalLSB(uint32_t index, pbm_font *font);

/**
 * @brief bit offset calculation for horizontal MSB data
 *
 * @param index current horizontal data index
 * @param font the current used font
 * @return uint32_t the calculated byte offset
 */
uint32_t bitOffset_horizontalMSB(uint32_t index, pbm_font *font);

/**
 * @brief bit offset calculation for horizontal LSB data
 *
 * @param index current horizontal data index
 * @param font the current used font
 * @return uint32_t the calculated byte offset
 */
uint32_t bitOffset_horizontalLSB(uint32_t index, pbm_font *font);

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
  if (imageHandler == NULL) {
    return PBM_ARGUMENTS;
  }
  if (x >= imageHandler->width || y >= imageHandler->height) {
    return PBM_OUT_OF_RANGE;
  }
  uint8_t pattern;
  uint32_t bytePosition;
#define MSB_BIT (0x80)
#define LSB_BIT (0x01)
#define BYTE_SIZE (8)

  switch (imageHandler->alignment) {
  case PBM_DATA_HORIZONTAL_MSB:
    pattern = MSB_BIT >> (x % BYTE_SIZE);
    bytePosition = (y * imageHandler->width + x) / BYTE_SIZE;
    break;
  case PBM_DATA_HORIZONTAL_LSB:
    pattern = LSB_BIT << (x % BYTE_SIZE);
    bytePosition = (y * imageHandler->width + x) / BYTE_SIZE;
    break;
  case PBM_DATA_VERTICAL_MSB:
    pattern = MSB_BIT >> (y % BYTE_SIZE);
    bytePosition = y / BYTE_SIZE * imageHandler->width + x;
    break;
  case PBM_DATA_VERTICAL_LSB:
    pattern = LSB_BIT << (y % BYTE_SIZE);
    bytePosition = y / BYTE_SIZE * imageHandler->width + x;
    break;
  default:
    return PBM_ERROR;
  }

  if (PBM_BLACK == color) {
    imageHandler->data[bytePosition] |= pattern;
  } else {
    imageHandler->data[bytePosition] &= ~pattern;
  }
  return PBM_OK;
}

pbm_return pbm_drawLine(pbm_image *imageHandler, uint32_t xStart, uint32_t yStart, uint32_t xEnd, uint32_t yEnd, pbm_colors color) {
  if (NULL == imageHandler) {
    return PBM_ARGUMENTS;
  }
  if (xStart > imageHandler->width || xEnd > imageHandler->width || yStart > imageHandler->height ||
      yEnd > imageHandler->height) {
    return PBM_OUT_OF_RANGE;
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

pbm_return pbm_writeChar(pbm_image *imageHandler, uint32_t x, uint32_t y, pbm_colors color, pbm_font *font, uint8_t character) {
  if (NULL == imageHandler || NULL == font) {
    return PBM_ARGUMENTS;
  }
  // Check first the correct alignment
  offsetCalculation byteOffset;
  offsetCalculation bitOffset;
  switch (font->alignment) {
  case PBM_DATA_HORIZONTAL_LSB:
    byteOffset = byteOffset_horizontalLSB;
    bitOffset = bitOffset_horizontalLSB;
    break;
  case PBM_DATA_HORIZONTAL_MSB:
    byteOffset = byteOffset_horizontalMSB;
    bitOffset = bitOffset_horizontalMSB;
    break;
  default:
    return PBM_ARGUMENTS;
  }
  // Set horizontal alignment
  // Set the correct size
  uint32_t bytePerLine = (font->width - 1) / 8 + 1;
  uint32_t startFontIndex = (uint32_t)character * font->height * bytePerLine;
  for (uint32_t line = 0; line < font->height; line++) {
    for (uint32_t i = 0; i < font->width; i++) {
      uint32_t currentByte = startFontIndex + line * bytePerLine + byteOffset(i, font);
      uint32_t currentBit = bitOffset(i, font);
      if (font->fontData[currentByte] & currentBit) {
        pbm_setPixel(imageHandler, x + i, y + line, color);
      } else {
        pbm_setPixel(imageHandler, x + i, y + line, !color);
      }
    }
  }
  return PBM_OK;
}

pbm_return pbm_writeString(pbm_image *imageHandler, uint32_t x, uint32_t y, pbm_colors color, pbm_font *font, const char *msg) {
  if (NULL == imageHandler || NULL == font || NULL == msg) {
    return PBM_ARGUMENTS;
  }
  uint32_t currentX = x;
  while (*msg != '\0') {
    pbm_writeChar(imageHandler, currentX, y, color, font, *msg++);
    currentX += font->width + CHARACTER_GAP;
  }

  return PBM_OK;
}

uint32_t byteOffset_horizontalMSB(uint32_t index, pbm_font *font) {
  return (font->width - 1 - index) / 8;
}
uint32_t byteOffset_horizontalLSB(uint32_t index, pbm_font *font) {
  return ((index + 8 - (font->width % 8)) / 8);
}

uint32_t bitOffset_horizontalMSB(uint32_t index, pbm_font *font) {
  return (0x1 << (font->width - 1 - index) % 8);
}
uint32_t bitOffset_horizontalLSB(uint32_t index, pbm_font *font) {
  return (0x1 << (index + 8 - (font->width % 8)) % 8);
}
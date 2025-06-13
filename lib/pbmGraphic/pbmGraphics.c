/**
 * @file pbmGraphics.c
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief Graphic library to set in an PBM image (1 pixel depth image) pixels, lines and writing to it.
 *
 * This lib is used write graphical (pixels, lines) elements and write text in the image.
 *
 * @version 0.2
 * @date 19-12-2024
 * @date 13-06-2025
 *
 * @copyright (C) 2025 Adrian STEINER
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https: //www.gnu.org/licenses/>.
 *
 */

#include "pbmGraphics.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define CHARACTER_GAP (1)         ///< Character gap for writing a string
#define IMAGE_BUFFER_BIT_SIZE (8) ///< Image buffer bit size per element

/**
 * @brief function pointer to set offsets for bytes and bites
 *
 * @param uint32_t index the current line index
 * @param pbm_font font handler to get the width or height
 */
typedef uint32_t (*offsetCalculation)(uint32_t, const pbm_font *);

/**
 * @brief byte offset calculation for horizontal MSB data
 *
 * @param index current horizontal data index
 * @param font the current used font
 * @return uint32_t the calculated byte offset
 */
uint32_t byteOffset_horizontalMSB(uint32_t index, const pbm_font *font);

/**
 * @brief byte offset calculation for horizontal LSB data
 *
 * @param index current horizontal data index
 * @param font the current used font
 * @return uint32_t the calculated byte offset
 */
uint32_t byteOffset_horizontalLSB(uint32_t index, const pbm_font *font);

/**
 * @brief bit offset calculation for horizontal MSB data
 *
 * @param index current horizontal data index
 * @param font the current used font
 * @return uint32_t the calculated byte offset
 */
uint32_t bitOffset_horizontalMSB(uint32_t index, const pbm_font *font);

/**
 * @brief bit offset calculation for horizontal LSB data
 *
 * @param index current horizontal data index
 * @param font the current used font
 * @return uint32_t the calculated byte offset
 */
uint32_t bitOffset_horizontalLSB(uint32_t index, const pbm_font *font);

pbm_return pbm_fill(pbm_image *imageHandler, pbm_colors color) {
  if (NULL == imageHandler || color > PBM_BLACK) {
    return PBM_ARGUMENTS;
  }

  uint32_t imageDataSize = 0;

  switch (imageHandler->alignment) {
  case PBM_DATA_VERTICAL_LSB:
  case PBM_DATA_VERTICAL_MSB:
    imageDataSize = ((imageHandler->height - 1) / IMAGE_BUFFER_BIT_SIZE + 1) * imageHandler->width;
    break;
  case PBM_DATA_HORIZONTAL_LSB:
  case PBM_DATA_HORIZONTAL_MSB:
    imageDataSize = ((imageHandler->width - 1) / IMAGE_BUFFER_BIT_SIZE + 1) * imageHandler->height;
    break;
  default:
    break;
  }

  uint8_t fillValue = UINT8_MAX * (uint8_t)color;

  for (uint32_t i = 0; i < imageDataSize; i++) {
    imageHandler->data[i] = fillValue;
  }
  return PBM_OK;
}

pbm_return pbm_setPixel(pbm_image *imageHandler, uint32_t x, uint32_t y, pbm_colors color) {
#define MSB_BIT (0x80)
#define LSB_BIT (0x01)

  if (imageHandler == NULL) {
    return PBM_ARGUMENTS;
  }
  if (PBM_IMAGE_END == x) {
    x = imageHandler->width - 1;
  }
  if (PBM_IMAGE_END == y) {
    y = imageHandler->height - 1;
  }
  if (x >= imageHandler->width || y >= imageHandler->height) {
    return PBM_OUT_OF_RANGE;
  }
  uint8_t pattern;
  uint32_t bytePosition;

  switch (imageHandler->alignment) {
  case PBM_DATA_HORIZONTAL_MSB:
    pattern = MSB_BIT >> (x % IMAGE_BUFFER_BIT_SIZE);
    bytePosition = (y * imageHandler->width + x) / IMAGE_BUFFER_BIT_SIZE;
    break;
  case PBM_DATA_HORIZONTAL_LSB:
    pattern = LSB_BIT << (x % IMAGE_BUFFER_BIT_SIZE);
    bytePosition = (y * imageHandler->width + x) / IMAGE_BUFFER_BIT_SIZE;
    break;
  case PBM_DATA_VERTICAL_MSB:
    pattern = MSB_BIT >> (y % IMAGE_BUFFER_BIT_SIZE);
    bytePosition = y / IMAGE_BUFFER_BIT_SIZE * imageHandler->width + x;
    break;
  case PBM_DATA_VERTICAL_LSB:
    pattern = LSB_BIT << (y % IMAGE_BUFFER_BIT_SIZE);
    bytePosition = y / IMAGE_BUFFER_BIT_SIZE * imageHandler->width + x;
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

pbm_return pbm_drawLine(
    pbm_image *imageHandler,
    uint32_t xStart,
    uint32_t yStart,
    uint32_t xEnd,
    uint32_t yEnd,
    pbm_colors color) {

  if (NULL == imageHandler) {
    return PBM_ARGUMENTS;
  }
  if (PBM_IMAGE_END == xStart) {
    xStart = imageHandler->width - 1;
  }
  if (PBM_IMAGE_END == yStart) {
    yStart = imageHandler->height - 1;
  }

  if (PBM_IMAGE_END == xEnd) {
    xEnd = imageHandler->width - 1;
  }
  if (PBM_IMAGE_END == yEnd) {
    yEnd = imageHandler->height - 1;
  }

  if (xStart > imageHandler->width || xEnd > imageHandler->width || yStart > imageHandler->height ||
      yEnd > imageHandler->height) {
    return PBM_OUT_OF_RANGE;
  }

  int32_t xDiff = xEnd - xStart;
  int32_t yDiff = yEnd - yStart;
  uint32_t interpolationDuration;
  if (abs(xDiff) > abs(yDiff)) {
    interpolationDuration = abs(xDiff);
  } else {
    interpolationDuration = abs(yDiff);
  }
  for (uint32_t i = 0; i < interpolationDuration; i++) {
    int32_t xInterpolation = (xDiff * (int32_t)i) / (int32_t)interpolationDuration;
    int32_t yInterpolation = (yDiff * (int32_t)i) / (int32_t)interpolationDuration;
    pbm_setPixel(imageHandler, xStart + xInterpolation, yStart + yInterpolation, color);
  }
  return PBM_OK;
}

pbm_return pbm_writeChar(
    pbm_image *const imageHandler,
    const uint32_t x,
    const uint32_t y,
    pbm_colors color,
    const pbm_font *font,
    const uint8_t character) {

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
  uint32_t bytePerLine = (font->width - 1) / IMAGE_BUFFER_BIT_SIZE + 1;
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

pbm_return pbm_writeString(
    pbm_image *const imageHandler,
    const uint32_t x,
    const uint32_t y,
    pbm_colors color,
    const pbm_font *font,
    pbm_stringAlignment textAlignment,
    const char *msg) {
  if (NULL == imageHandler || NULL == font || NULL == msg) {
    return PBM_ARGUMENTS;
  }
  uint32_t stringLen = strlen(msg);
  if (0 == stringLen) {
    return PBM_ARGUMENTS;
  }
  uint32_t msgBitLen = (stringLen - 1) * (font->width + CHARACTER_GAP) + font->width;
  uint32_t xOffset;
  uint32_t yOffset;

  switch (textAlignment) {
  case PBM_STRING_LEFT_TOP:
  case PBM_STRING_LEFT_CENTER:
  case PBM_STRING_LEFT_BOTTOM:
    xOffset = 0;
    break;
  case PBM_STRING_CENTER_TOP:
  case PBM_STRING_CENTER_CENTER:
  case PBM_STRING_CENTER_BOTTOM:
    xOffset = msgBitLen / 2;
    break;
  case PBM_STRING_RIGHT_TOP:
  case PBM_STRING_RIGHT_CENTER:
  case PBM_STRING_RIGHT_BOTTOM:
    xOffset = msgBitLen;
    break;
  default:
    xOffset = 0;
    break;
  }

  switch (textAlignment) {
  case PBM_STRING_LEFT_TOP:
  case PBM_STRING_CENTER_TOP:
  case PBM_STRING_RIGHT_TOP:
    yOffset = 0;
    break;
  case PBM_STRING_LEFT_CENTER:
  case PBM_STRING_CENTER_CENTER:
  case PBM_STRING_RIGHT_CENTER:
    yOffset = font->height / 2;
    break;
  case PBM_STRING_LEFT_BOTTOM:
  case PBM_STRING_CENTER_BOTTOM:
  case PBM_STRING_RIGHT_BOTTOM:
    yOffset = font->height;
    break;
  default:
    yOffset = 0;
    break;
  }

  uint32_t currentX = ((x == PBM_IMAGE_END) ? imageHandler->width : x) - xOffset;
  uint32_t currentY = ((y == PBM_IMAGE_END) ? imageHandler->height : y) - yOffset;

  while (*msg != '\0') {
    pbm_writeChar(imageHandler, currentX, currentY, color, font, *msg++);
    currentX += font->width + CHARACTER_GAP;
  }

  return PBM_OK;
}

// Byte offsets calculations
uint32_t byteOffset_horizontalMSB(uint32_t index, const pbm_font *font) {
  return (font->width - 1 - index) / IMAGE_BUFFER_BIT_SIZE;
}
uint32_t byteOffset_horizontalLSB(uint32_t index, const pbm_font *font) {
  return ((index + IMAGE_BUFFER_BIT_SIZE - (font->width % IMAGE_BUFFER_BIT_SIZE)) / IMAGE_BUFFER_BIT_SIZE);
}

// Bit offset calculations
uint32_t bitOffset_horizontalMSB(uint32_t index, const pbm_font *font) {
  return (0x1 << (font->width - 1 - index) % IMAGE_BUFFER_BIT_SIZE);
}
uint32_t bitOffset_horizontalLSB(uint32_t index, const pbm_font *font) {
  return (0x1 << (index + IMAGE_BUFFER_BIT_SIZE - (font->width % IMAGE_BUFFER_BIT_SIZE)) % IMAGE_BUFFER_BIT_SIZE);
}
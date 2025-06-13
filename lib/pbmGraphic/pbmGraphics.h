/**
 * @file pbmGraphics.h
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief Graphic library to set in an PBM image (1 pixel depth image) pixels, lines and writing to it.
 *        This header file defines the function prototypes and ads the necessary pbmFontHandler
 *        for writing text to the image.
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

#ifndef PBM_GRAPHICS_H
#define PBM_GRAPHICS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../pbm.h"
#include "pbmFontHandler.h"

#include <stdint.h>

#define PBM_IMAGE_END UINT32_MAX

typedef enum {
  PBM_STRING_LEFT_TOP,
  PBM_STRING_CENTER_TOP,
  PBM_STRING_RIGHT_TOP,
  PBM_STRING_LEFT_CENTER,
  PBM_STRING_CENTER_CENTER,
  PBM_STRING_RIGHT_CENTER,
  PBM_STRING_LEFT_BOTTOM,
  PBM_STRING_CENTER_BOTTOM,
  PBM_STRING_RIGHT_BOTTOM
} pbm_stringAlignment;

/**
 * @brief Fill the full image to the desired color
 *
 * @param imageHandler the image to overwrite
 * @param color the desired color
 * @return pbm_return state
 */
pbm_return pbm_fill(pbm_image *imageHandler, pbm_colors color);

/**
 * @brief set a pixel in the image to the desired color
 *
 * @param imageHandler the image to set a pixel
 * @param x x position (horizontal)
 * @param y y position (vertical)
 * @param color the desired color
 * @return pbm_return state
 */
pbm_return pbm_setPixel(
    pbm_image *imageHandler,
    uint32_t x,
    uint32_t y,
    pbm_colors color);

/**
 * @brief draw a line in the image
 *
 * @param imageHandler the image to draw a line
 * @param xStart start position in x (horizontal)
 * @param yStart start position in y (vertical)
 * @param xEnd end position in x
 * @param yEnd end position in y
 * @param color the desired color
 * @return pbm_return state
 */
pbm_return pbm_drawLine(
    pbm_image *imageHandler,
    uint32_t xStart,
    uint32_t yStart,
    uint32_t xEnd,
    uint32_t yEnd,
    pbm_colors color);

/**
 * @brief Write a character with the given font into the image
 *
 * @param imageHandler the image to write a character
 * @param x start position on the top left corner in x
 * @param y start position on the top left corner in y
 * @param color the desired color
 * @param font the font handler with the bitmapped font, size and orientation
 * @param character the desired character from the font
 * @return pbm_return state
 */
pbm_return pbm_writeChar(
    pbm_image *const imageHandler,
    const uint32_t x,
    const uint32_t y,
    pbm_colors color,
    const pbm_font *font,
    const uint8_t character);

/**
 * @brief Write a string on one with the given font into the image
 *
 * The string must be a C character string and be mapped to the given font.
 * The gap between the character is fix with 2 pixels
 *
 * @param imageHandler the image to write a string
 * @param x start position on the top left corner in x
 * @param y start position on the top left corner in y
 * @param color the desired color
 * @param font the font handler with the bitmapped font, size and orientation
 * @param textAlignment
 * @param msg the C string to write to the image
 * @return pbm_return state
 */
pbm_return pbm_writeString(
    pbm_image *const imageHandler,
    const uint32_t x,
    const uint32_t y,
    pbm_colors color,
    const pbm_font *font,
    pbm_stringAlignment textAlignment,
    const char *msg);

#ifdef __cplusplus
}
#endif

#endif /*PBM_GRAPHICS_H*/
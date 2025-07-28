/**
 * @file pbmFontHandler.h
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief Handles a font to display it with the necessary data
 * @version 0.1
 * @date 28-01-2025
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
 */

#ifndef PBM_FONT_HANDLER_H
#define PBM_FONT_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "pbm_types.h"

/**
 * @brief PBM font handler
 *
 */
typedef struct {
  const unsigned char *fontData; ///< Font structure represented in bytes
  uint8_t width;                 ///< Font width
  uint8_t height;                ///< Font height
  pbm_data_alignment alignment;  ///< Data alignment of the the font structure
} pbm_font;

#ifdef __cplusplus
}
#endif

#endif
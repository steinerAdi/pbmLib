/**
 * @file pbmFontHandler.h
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief Handles a font to display it with the necessary data
 * @version 0.1
 * @date 28-01-2025
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef PBM_FONT_HANDLER_H
#define PBM_FONT_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../pbm.h"

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
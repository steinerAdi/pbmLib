/**
 * @file pbm.h
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief
 * @version 0.1
 * @date 19-12-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef PBM_H
#define PBM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
  PBM_DATA_HORIZONTAL_MSB = 0, ///< Data bytes are stored in horizontal direction (x direction) with MSB first
  PBM_DATA_HORIZONTAL_LSB,     ///< Data bytes are stored in horizontal direction (x direction) with LSB
  PBM_DATA_VERTICAL_MSB,       ///< Data bytes are stored in vertical direction (y direction) with MSB first
  PBM_DATA_VERTICAL_LSB        ///< Data bytes are stored in vertical direction (y direction) with LSB first
} pbm_data_alignment;

typedef enum {
  PBM_WHITE = 0, ///< White color index
  PBM_BLACK = 1  ///< Black color index
} pbm_colors;

typedef enum {
  PBM_OK = 0,    ///< No errors
  PBM_ERROR,     ///< Unkown error
  PBM_ARGUMENTS, ///< Function argument error
  PBM_SIZE       ///< Size out of range
} pbm_return;

typedef struct {
  uint32_t width;               ///< width of the image
  uint32_t height;              ///< height of the image
  pbm_data_alignment alignment; ///< Data bytes alignment
  uint8_t *data;                ///< Image buffer data with min size of (width*height+7)/8
} pbm_image;

#ifdef __cplusplus
}
#endif

#endif /*PBM_H*/
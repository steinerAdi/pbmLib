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

typedef struct {
  uint32_t width;  ///< width of the image
  uint32_t height; ///< height of the image
  uint8_t *data;   ///< Image buffer data with min size of (width*height+7)/8
} pbm_image;

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

#ifdef __cplusplus
}
#endif

#endif /*PBM_H*/
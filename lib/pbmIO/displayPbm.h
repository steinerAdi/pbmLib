/**
 * @file displayPbm.h
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief Load, save and render a PBM image
 * @version 0.1
 * @date 18-12-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef DISPLAY_PBM_H
#define DISPLAY_PBM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "SDL2/SDL.h"
#include <stdint.h>

#include "../pbm.h"

/**
 * @brief Load an PBM P4 image from the path
 * The data of the image will be created on the heap.
 * The user is responsible to free the data at the end of the image.
 *
 * @param imagePath The path to the image
 * @param imageHandler Image handler structure.
 * @return pbm_return state of the function
 */
pbm_return pbm_loadImage(const char *imagePath, pbm_image *imageHandler);

/**
 * @brief Saves or overwrites an image on the path.
 *
 * @param imagePath The path to the new image
 * @param imageHandler The image to save
 * @return pbm_return state of the function
 */
pbm_return pbm_saveImage(const char *imagePath, const pbm_image *imageHandler);

/**
 * @brief Render an image in the renderer.
 *
 * @param screen The renderer to display the image in a window
 * @param image The image to display
 * @return pbm_return state of the function
 */
pbm_return pbm_renderImage(SDL_Renderer *screen, const pbm_image *image);

#ifdef __cplusplus
}
#endif

#endif /*DISPLAY_PBM_H*/
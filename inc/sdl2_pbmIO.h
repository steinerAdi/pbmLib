/**
 * @file sdl2_pbmIO.h
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief Load, save and render a PBM image
 * @version 0.1
 * @date 18-12-2024
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

#ifndef SDL2_PBMIO_H
#define SDL2_PBMIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL2/SDL.h>
#include <stdint.h>

#include "pbm_types.h"

/**
 * @brief Load an PBM P4 image from the path
 * The data of the image will be created on the heap.
 * The user is responsible to free the data at the end of the application.
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

#endif /* SDL2_PBMIO_H */

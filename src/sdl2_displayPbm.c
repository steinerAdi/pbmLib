/**
 * @file displayPbm.c
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

#include "sdl2_displayPbm.h"

pbm_return pbm_loadImage(const char *imagePath, pbm_image *imageHandler) {
  FILE *file = fopen(imagePath, "rb");
  if (file == NULL) {
    printf("ERROR, could not load %s\n", imagePath);
    return PBM_ERROR;
  }

  char header[3];
  if (fscanf(file, "%2s", header) != 1) {
    printf("Invalid file format\n");
    fclose(file);
    return PBM_ERROR;
  }
  if ('P' != header[0] || '4' != header[1]) {
    printf("Invalid PBM file format, should be P4 but is %c%c\n", header[0],
           header[1]);
    fclose(file);
    return PBM_ERROR;
  }

  // Skip comments (lines starting with #)
  char c;
  while ((c = fgetc(file)) == '#') {
    while ((c = fgetc(file)) != '\n') {
      ;
    }
  }
  ungetc(c, file);

  if (fscanf(file, "%u %u", &(imageHandler->width), &(imageHandler->height)) !=
      2) {
    printf("Error reading image size\n");
    fclose(file);
    return PBM_ERROR;
  }

  // Skip the maximum grayscale value
  fscanf(file, "%*d");

  // Load image data
  // 1 bit per pixel => 1 byte for 8 pixels
  uint32_t imageDataSize = (imageHandler->width * imageHandler->height + 7) / 8;
  uint8_t *data = (uint8_t *)malloc(imageDataSize);
  if (data == NULL) {
    printf("Error allocating memory for image data\n");
    fclose(file);
    return PBM_ERROR;
  }

  fread(data, 1, imageDataSize, file);
  imageHandler->data = data;
  imageHandler->alignment = PBM_DATA_HORIZONTAL_MSB;
  fclose(file);
  return PBM_OK;
}

pbm_return pbm_saveImage(const char *imagePath, const pbm_image *imageHandler) {
  if (NULL == imagePath || NULL == imageHandler || NULL == imageHandler->data) {
    return PBM_ARGUMENTS;
  }

  FILE *file = fopen(imagePath, "wb");
  if (file == NULL) {
    printf("ERROR, could not open %s\n", imagePath);
    return PBM_ERROR;
  }

  if (0 == imageHandler->width || 0 == imageHandler->height) {
    fclose(file);
    return PBM_SIZE;
  }

  uint32_t imageDataSize = (imageHandler->width * imageHandler->height + 7) / 8;

  fprintf(file, "P4\n");
  fprintf(file, "%u %u\n", imageHandler->width, imageHandler->height);
  fwrite(imageHandler->data, 1, imageDataSize, file);
  fprintf(file, "\n");
  fclose(file);
  return PBM_OK;
}

pbm_return pbm_renderImage(SDL_Renderer *screen, const pbm_image *image) {
  if (NULL == screen || NULL == image) {
    return PBM_ARGUMENTS;
  }

  for (uint32_t y = 0; y < image->height; y++) {
    for (uint32_t x = 0; x < image->width; x++) {
      uint32_t byteIndex = (y * image->width + x) / 8;
      uint32_t bitIndex = (y * image->width + x) % 8;
      uint8_t byte = image->data[byteIndex];
      uint8_t pixelColor =
          (byte & (0x80 >> bitIndex)) ? 0 : 255; // 0 for black, 255 for white

      // Draw the pixel (black or white)
      SDL_SetRenderDrawColor(screen, pixelColor, pixelColor, pixelColor,
                             SDL_ALPHA_OPAQUE);
      SDL_Rect rect = {x, y, 1, 1}; // 1x1 pixel
      SDL_RenderFillRect(screen, &rect);
    }
  }

  return PBM_OK;
}
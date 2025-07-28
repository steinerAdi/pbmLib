/**
 * @file main.c
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief Test application for pbm library for graphic and IO functions
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

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "pbm_graphics.h"
#include "pbm_types.h"
#include "sdl2_displayPbm.h"

#include "fonts/12x20_horizontal_LSB.h"
#include "fonts/12x20_horizontal_MSB.h"
#include "fonts/32x53_horizontal_MSB.h"
#include "fonts/6x8_horizontal_LSB.h"
#include "fonts/6x8_horizontal_MSB.h"

#define IMAGE_PATH ("sample.pbm")

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

enum registeredImages { IMG_READ, IMG_FONT, IMG_ALIGNMENT, IMG_MAX };

enum fontNames {
  FONT6X8_LSB,
  FONT6X8_MSB,
  FONT12X20_LSB,
  FONT12X20_MSB,
  FONT32X53_MSB,
  MAX_FONTS
};

void drawFontImage(pbm_image *imageHandler, const pbm_font *usedFont);
void drawAlignmentImage(pbm_image *imageHandler, const pbm_font *usedFont);
const char *getAlignment(pbm_data_alignment alignment);

int main(int argc, char const *argv[]) {
  const char *filePath;
  if (argc > 1) {
    filePath = argv[1];
  } else {
    filePath = IMAGE_PATH;
  }

  pbm_image imageHandler[IMG_MAX];
  if (pbm_loadImage(filePath, &imageHandler[IMG_READ])) {
    printf("Error: Loading image %s\n", filePath);
    return EXIT_FAILURE;
  }

  printf("Load image with size: %d x %d\n", imageHandler[IMG_READ].width,
         imageHandler[IMG_READ].height);

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  atexit(SDL_Quit);

  SDL_Window *window =
      SDL_CreateWindow("Image viewer", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, imageHandler[IMG_READ].width,
                       imageHandler[IMG_READ].height, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("SDL_CreateWindow: %s\n", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("SDL_CreateRenderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  imageHandler[IMG_FONT].alignment = imageHandler[IMG_READ].alignment;
  imageHandler[IMG_FONT].width = imageHandler[IMG_READ].width;
  imageHandler[IMG_FONT].height = imageHandler[IMG_READ].height;
  imageHandler[IMG_FONT].data = (uint8_t *)malloc(
      imageHandler[IMG_FONT].width * imageHandler[IMG_FONT].height / 8);

  imageHandler[IMG_ALIGNMENT].alignment = imageHandler[IMG_READ].alignment;
  imageHandler[IMG_ALIGNMENT].width = imageHandler[IMG_READ].width;
  imageHandler[IMG_ALIGNMENT].height = imageHandler[IMG_READ].height;
  imageHandler[IMG_ALIGNMENT].data =
      (uint8_t *)malloc(imageHandler[IMG_ALIGNMENT].width *
                        imageHandler[IMG_ALIGNMENT].height / 8);

  pbm_image *fontImage = &imageHandler[IMG_FONT];
  pbm_image *alignmentImage = &imageHandler[IMG_ALIGNMENT];

  pbm_fill(fontImage, PBM_WHITE);

  pbm_font storedFonts[MAX_FONTS] = {
      [FONT6X8_LSB] = {.alignment = PBM_DATA_HORIZONTAL_LSB,
                       .fontData = &font_6x8H_LSB[0][0],
                       .width = 6,
                       .height = 8},
      [FONT6X8_MSB] = {.alignment = PBM_DATA_HORIZONTAL_MSB,
                       .fontData = &font_6x8H_MSB[0][0],
                       .width = 6,
                       .height = 8},
      [FONT12X20_LSB] = {.alignment = PBM_DATA_HORIZONTAL_LSB,
                         .fontData = &font_12x20H_LSB[0][0],
                         .width = 12,
                         .height = 20},
      [FONT12X20_MSB] = {.alignment = PBM_DATA_HORIZONTAL_MSB,
                         .fontData = &font_12x20H_MSB[0][0],
                         .width = 12,
                         .height = 20},
      [FONT32X53_MSB] = {.alignment = PBM_DATA_HORIZONTAL_MSB,
                         .fontData = &font_32x53H_MSB[0][0],
                         .width = 32,
                         .height = 53}};

  enum fontNames currentFont = FONT6X8_LSB;
  drawFontImage(fontImage, &storedFonts[currentFont]);
  drawAlignmentImage(alignmentImage, &storedFonts[currentFont]);
  bool running = true;
  SDL_Event event;
  enum registeredImages currentImage = IMG_FONT;
  pbm_renderImage(renderer, &imageHandler[currentImage]);
  SDL_RenderPresent(renderer);

  /* Super loop */
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        pbm_saveImage("build/saved.pbm",
                      (const pbm_image *)&imageHandler[currentImage]);
        running = false;
        break;
      case SDL_KEYUP: {
        switch (event.key.keysym.sym) {
        case SDLK_RIGHT:
          currentImage = (currentImage + 1) % IMG_MAX;
          pbm_renderImage(renderer, &imageHandler[currentImage]);
          SDL_RenderPresent(renderer);
          break;
        case SDLK_UP:
          currentFont = (currentFont + 1) % MAX_FONTS;
          pbm_invertColor(&imageHandler[IMG_READ]);
          drawFontImage(fontImage, &storedFonts[currentFont]);
          drawAlignmentImage(alignmentImage, &storedFonts[currentFont]);
          pbm_renderImage(renderer, &imageHandler[currentImage]);
          SDL_RenderPresent(renderer);
        default:
          break;
        }
      }
      default:
        break;
      }
    }
  }
  puts("Closing image viewer");
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  for (size_t i = 0; i < IMG_MAX; i++) {
    free(imageHandler[i].data);
  }
  return EXIT_SUCCESS;
}

void drawFontImage(pbm_image *imageHandler, const pbm_font *usedFont) {
  pbm_fill(imageHandler, PBM_WHITE);
  uint32_t xPos = 2;
  char buffer[100] = {0};
  snprintf(buffer, 100, "USED FONT: %u x %u %s", usedFont->width,
           usedFont->height, getAlignment(usedFont->alignment));
  pbm_writeString(imageHandler, xPos, 1, PBM_BLACK, usedFont,
                  PBM_STRING_LEFT_TOP, buffer);
  uint32_t yPos = usedFont->height + 1;
  for (uint16_t i = 0; i <= 255; i++) {
    // printf("Character %d = '%c'\n", i, i);
    pbm_writeChar(imageHandler, xPos, yPos, PBM_BLACK, usedFont, (char)i);
    yPos += usedFont->height + 1;
    if ((yPos + usedFont->height) > imageHandler->height) {
      yPos = usedFont->height + 1;
      xPos += usedFont->width + 2;
    }
  }
}

void drawAlignmentImage(pbm_image *imageHandler, const pbm_font *usedFont) {
  // Draw border
  pbm_fill(imageHandler, PBM_WHITE);
  pbm_drawLine(imageHandler, 0, 0, PBM_IMAGE_END, 0, PBM_BLACK);
  pbm_drawLine(imageHandler, 0, 0, PBM_IMAGE_END, PBM_IMAGE_END, PBM_BLACK);
  pbm_drawLine(imageHandler, PBM_IMAGE_END, 0, 0, PBM_IMAGE_END, PBM_BLACK);
  // printf("Retval %d", pbm_drawLine(imageHandler, 0, PBM_IMAGE_END,
  // PBM_IMAGE_END, 0, PBM_BLACK));
  char fontInfoBuf[100];
  snprintf(fontInfoBuf, sizeof(fontInfoBuf), "FONT %dx%d %s", usedFont->width,
           usedFont->height, getAlignment(usedFont->alignment));
  pbm_writeString(imageHandler, imageHandler->width / 2,
                  imageHandler->height / 3, PBM_BLACK, usedFont,
                  PBM_STRING_CENTER_CENTER, fontInfoBuf);
  pbm_writeString(imageHandler, 0, 0, PBM_BLACK, usedFont, PBM_STRING_LEFT_TOP,
                  "LEFT TOP");
  pbm_writeString(imageHandler, imageHandler->width / 2, 0, PBM_BLACK, usedFont,
                  PBM_STRING_CENTER_TOP, "CENTER TOP");
  pbm_writeString(imageHandler, PBM_IMAGE_END, 0, PBM_BLACK, usedFont,
                  PBM_STRING_RIGHT_TOP, "RIGHT TOP");
  pbm_writeString(imageHandler, 0, imageHandler->height / 2, PBM_BLACK,
                  usedFont, PBM_STRING_LEFT_CENTER, "LEFT CENTER");
  pbm_writeString(imageHandler, imageHandler->width / 2,
                  imageHandler->height / 2, PBM_BLACK, usedFont,
                  PBM_STRING_CENTER_CENTER, "CENTER CENTER");
  pbm_writeString(imageHandler, PBM_IMAGE_END, imageHandler->height / 2,
                  PBM_BLACK, usedFont, PBM_STRING_RIGHT_CENTER, "RIGHT CENTER");
  pbm_writeString(imageHandler, 0, PBM_IMAGE_END, PBM_BLACK, usedFont,
                  PBM_STRING_LEFT_BOTTOM, "LEFT BOTTOM");
  pbm_writeString(imageHandler, imageHandler->width / 2, PBM_IMAGE_END,
                  PBM_BLACK, usedFont, PBM_STRING_CENTER_BOTTOM,
                  "CENTER BOTTOM");
  pbm_writeString(imageHandler, PBM_IMAGE_END, PBM_IMAGE_END, PBM_BLACK,
                  usedFont, PBM_STRING_RIGHT_BOTTOM, "RIGHT BOTTOM");

  // Add circle in the image center
  pbm_drawCircle(imageHandler, imageHandler->width / 2,
                 imageHandler->height / 2, imageHandler->height / 3, PBM_BLACK);
}

const char *getAlignment(pbm_data_alignment alignment) {
  if (alignment >= PBM_DATA_MAX_ALIGNMENTS) {
    alignment = PBM_DATA_MAX_ALIGNMENTS;
  }

  static const char *textAlignment[PBM_DATA_MAX_ALIGNMENTS + 1] = {
      [PBM_DATA_HORIZONTAL_MSB] = "MSB-H",
      [PBM_DATA_HORIZONTAL_LSB] = "LSB-H",
      [PBM_DATA_VERTICAL_MSB] = "MSB-V",
      [PBM_DATA_VERTICAL_LSB] = "LSB-V",
      [PBM_DATA_MAX_ALIGNMENTS] = "NOT VALID"};
  return textAlignment[alignment];
}

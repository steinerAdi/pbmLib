/**
 * @file main.c
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief
 * @version 0.1
 * @date 18-12-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#undef PBM_USE_IO

#include "SDL2/SDL.h"
#include <stdbool.h>
#include <stdlib.h>

#include "displayPbm.h"
#include "pbm.h"
#include "pbmGraphics.h"

#include "12x20_horizontal_LSB_1.h"
#include "12x20_horizontal_MSB_1.h"
#include "32x53_horizontal_MSB_1.h"
#include "6x8_horizontal_LSB_1.h"
#include "6x8_horizontal_MSB_1.h"

#define IMAGE_PATH ("sample.pbm")

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

enum registeredImages {
  IMG_READ,
  IMG_FONT,
  IMG_ALIGNMENT,
  IMG_MAX
};

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

int main(int argc, char const *argv[]) {
  const char *filePath;
  if (argc > 1) {
    filePath = argv[1];
  } else {
    filePath = IMAGE_PATH;
  }

  pbm_image imageHandler[IMG_MAX];
  if (pbm_loadImage(filePath, &imageHandler[IMG_READ])) {
    return EXIT_FAILURE;
  }

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  atexit(SDL_Quit);

  SDL_Window *window = SDL_CreateWindow("Image viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      imageHandler[IMG_READ].width, imageHandler[IMG_READ].height, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("SDL_CreateWindow: %s\n", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("SDL_CreateRenderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  imageHandler[IMG_FONT].alignment = imageHandler[IMG_READ].alignment;
  imageHandler[IMG_FONT].width = imageHandler[IMG_READ].width;
  imageHandler[IMG_FONT].height = imageHandler[IMG_READ].height;
  imageHandler[IMG_FONT].data = (uint8_t *)malloc(imageHandler[IMG_FONT].width * imageHandler[IMG_FONT].height / 8);

  imageHandler[IMG_ALIGNMENT].alignment = imageHandler[IMG_READ].alignment;
  imageHandler[IMG_ALIGNMENT].width = imageHandler[IMG_READ].width;
  imageHandler[IMG_ALIGNMENT].height = imageHandler[IMG_READ].height;
  imageHandler[IMG_ALIGNMENT].data = (uint8_t *)malloc(imageHandler[IMG_ALIGNMENT].width * imageHandler[IMG_ALIGNMENT].height / 8);

  pbm_image *fontImage = &imageHandler[IMG_FONT];
  pbm_image *alignmentImage = &imageHandler[IMG_ALIGNMENT];

  pbm_fill(fontImage, PBM_BLACK);
  pbm_drawLine(fontImage, 0, 0, fontImage->width - 1, fontImage->height - 1, PBM_WHITE);
  pbm_drawLine(fontImage, 0, fontImage->height, fontImage->width, 0, PBM_WHITE);

  pbm_font storedFonts[MAX_FONTS] = {
      [FONT6X8_LSB] = {.alignment = PBM_DATA_HORIZONTAL_LSB, .fontData = &font_6x8H_LSB[0][0], .width = 6, .height = 8},
      [FONT6X8_MSB] = {.alignment = PBM_DATA_HORIZONTAL_MSB, .fontData = &font_6x8H_MSB[0][0], .width = 6, .height = 8},
      [FONT12X20_LSB] = {.alignment = PBM_DATA_HORIZONTAL_LSB, .fontData = &font_12x20H_LSB[0][0], .width = 12, .height = 20},
      [FONT12X20_MSB] = {.alignment = PBM_DATA_HORIZONTAL_MSB, .fontData = &font_12x20H_MSB[0][0], .width = 12, .height = 20},
      [FONT32X53_MSB] = {.alignment = PBM_DATA_HORIZONTAL_MSB, .fontData = &font_32x53H_MSB[0][0], .width = 32, .height = 53}};

  enum fontNames currentFont = FONT6X8_LSB;
  drawFontImage(fontImage, &storedFonts[currentFont]);
  drawAlignmentImage(alignmentImage, &storedFonts[currentFont]);
  bool running = true;
  SDL_Event event;
  enum registeredImages currentImage = IMG_FONT;
  pbm_renderImage(renderer, &imageHandler[currentImage]);
  SDL_RenderPresent(renderer);
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        pbm_saveImage("build/saved.pbm", (const pbm_image *)&imageHandler[IMG_FONT]);
        running = false;
        break;
      case SDL_KEYUP: {
        switch (event.key.keysym.sym) {
        case SDLK_RIGHT:
          currentImage = (currentImage + 1) % IMG_MAX;
          pbm_renderImage(renderer, &imageHandler[currentImage]);
          SDL_RenderPresent(renderer);
          break;
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
  uint32_t xPos = 40;
  char buffer[100] = {0};
  snprintf(buffer, 100, "USED FONT: %u x %u", usedFont->width, usedFont->height);
  pbm_writeString(imageHandler, xPos, 1, PBM_WHITE, usedFont, PBM_STRING_TOP_LEFT, buffer);
  uint32_t yPos = usedFont->height + 1;
  for (uint16_t i = 0; i <= 255; i++) {
    // printf("Character %d = '%c'\n", i, i);
    pbm_writeChar(imageHandler, xPos, yPos, PBM_WHITE, usedFont, (char)i);
    yPos += usedFont->height + 1;
    if ((yPos + usedFont->height) > imageHandler->height) {
      yPos = usedFont->height + 1;
      xPos += usedFont->width + 2;
    }
  }
}

void drawAlignmentImage(pbm_image *imageHandler, const pbm_font *usedFont) {
  pbm_writeString(imageHandler, imageHandler->width / 2, imageHandler->height, PBM_BLACK, usedFont, PBM_STRING_CENTER_CENTER, "CENTER");
}
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

#include "6x8_horizontal_MSB_1.h"

#define IMAGE_PATH ("sample.pbm")

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

enum registeredImages { IMG_READ, IMG_LIB, IMG_MAX };

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

  SDL_Window *window = SDL_CreateWindow(
    "Image VIEWER",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    imageHandler[IMG_READ].width,
    imageHandler[IMG_READ].height,
    SDL_WINDOW_SHOWN);
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

  imageHandler[IMG_LIB].alignment = imageHandler[IMG_READ].alignment;
  imageHandler[IMG_LIB].width = imageHandler[IMG_READ].width;
  imageHandler[IMG_LIB].height = imageHandler[IMG_READ].height;
  imageHandler[IMG_LIB].data = (uint8_t *)malloc(imageHandler[IMG_LIB].width * imageHandler[IMG_LIB].height / 8);

  pbm_image *libImage = &imageHandler[IMG_LIB];

  pbm_fill(libImage, PBM_BLACK);
  pbm_drawLine(libImage, 0, 0, libImage->width - 1, libImage->height - 1, PBM_WHITE);
  pbm_drawLine(libImage, 0, libImage->height, libImage->width, 0, PBM_WHITE);

  pbm_font font6x8 = {.alignment = libImage->alignment, .fontData = &font_6x8H_MSB[0][0], .width = 6, .height = 8};
  // for (char i = ' '; i < 'z'; i++) {
  //   pbm_writeChar(&imageHandler, 40, yPos, PBM_BLACK, &font6x8, i);
  //   yPos += 8;
  // }
  pbm_writeString(libImage, 4 * 8, 2 * 8, PBM_BLACK, &font6x8, "Hallo Welt");

  bool running = true;
  SDL_Event event;
  enum registeredImages currentImage = IMG_LIB;
  pbm_displayImage(renderer, &imageHandler[currentImage]);
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        // pbm_saveImage("saved.pbm", (const pbm_image *)&imageHandler);
        // free(imageHandler.data);
        running = false;
        break;
      case SDL_KEYUP: {
        switch (event.key.keysym.sym) {
        case SDLK_RIGHT:
          currentImage = (currentImage + 1) % IMG_MAX;
          pbm_displayImage(renderer, &imageHandler[currentImage]);
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
  free(imageHandler[IMG_READ].data);
  free(imageHandler[IMG_LIB].data);
  return EXIT_SUCCESS;
}

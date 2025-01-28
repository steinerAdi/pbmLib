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

#define PBM_USE_IO

#include "SDL/SDL.h"
#include <stdlib.h>

#include "displayPbm.h"
#include "pbm.h"
#include "pbmGraphics.h"

#include "6x8_horizontal_MSB_1.h"

#define IMAGE_PATH ("sample.pbm")

int main(int argc, char const *argv[]) {
  const char *filePath;
  if (argc > 1) {
    filePath = argv[1];
  } else {
    filePath = IMAGE_PATH;
  }

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {
    // Error in process
    printf("Cannot initialise SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  atexit(SDL_Quit);

  pbm_image imageHandler;
  if (pbm_loadImage(filePath, &imageHandler)) {
    return EXIT_FAILURE;
  }

  SDL_Surface *screen = SDL_SetVideoMode(imageHandler.width, imageHandler.height, 32, SDL_SWSURFACE);
  if (!screen) {
    printf("Cannot set video mode: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Event event;
  pbm_fill(&imageHandler, PBM_BLACK);
  pbm_drawLine(&imageHandler, 0, 0, imageHandler.width - 1, imageHandler.height - 1, PBM_WHITE);
  pbm_drawLine(&imageHandler, 0, imageHandler.height, imageHandler.width, 0, PBM_WHITE);

  pbm_font font6x8 = {.alignment = PBM_DATA_HORIZONTAL_MSB, .font = font_6x8H_MSB, .width = 6, .height = 8};

  printf("Start test: ");
  printf("%02x\n", font_6x8H_MSB[1][0]);
  // pbm_writeChar(&imageHandler, 5 * 8, 5 * 8, PBM_BLACK, &font6x8, 0x1);

  // pbm_drawLine(&imageHandler, 0, 0, imageHandler.width, imageHandler.height, PBM_BLACK);
  pbm_displayImage(screen, (const pbm_image *)&imageHandler);
  // displayPBM_displayWindow(screen, IMAGE_PATH);

  // Auf Events warten
  while (SDL_WaitEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      pbm_saveImage("saved.pbm", (const pbm_image *)&imageHandler);
      free(imageHandler.data);
      puts("Leave window. Bye Bye");
      return EXIT_SUCCESS;
    }
  }
  printf("Error in message handling\n");
  free(imageHandler.data);
  return EXIT_SUCCESS;
}

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

#include "SDL/SDL.h"
#include <stdlib.h>

#include "displayPbm.h"

#define IMAGE_PATH ("sample.pbm")

int main(int argc, char const *argv[])
{

  const char *filePath;
  if (argc > 1)
  {
    filePath = argv[1];
  }
  else
  {
    filePath = IMAGE_PATH;
  }

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
  {
    // Error in process
    printf("Cannot initialise SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  atexit(SDL_Quit);

  pbm_Image imageHandler;
  if (pbm_load(filePath, &imageHandler))
  {
    return EXIT_FAILURE;
  }

  SDL_Surface *screen = SDL_SetVideoMode(imageHandler.width, imageHandler.height, 32, SDL_SWSURFACE);
  if (!screen)
  {
    printf("Cannot set video mode: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Event event;

  pbm_display(screen, &imageHandler);
  // displayPBM_displayWindow(screen, IMAGE_PATH);

  // Auf Events warten
  while (SDL_WaitEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      free(imageHandler.data);
      puts("Leave window. Bye Bye");
      return EXIT_SUCCESS;
    }
  }
  printf("Error in message handling\n");
  free(imageHandler.data);
  return EXIT_SUCCESS;
}

/**
 * @file displayPbm.c
 * @author Adrian STEINER (adi.steiner@hotmail.ch)
 * @brief
 * @version 0.1
 * @date 18-12-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "displayPbm.h"

SDL_Surface *loadBMP(const char *file_name);

pbm_Image pbm_load(const char *imagePath){
    ;
}

void pbm_display(SDL_Surface *screen, char * filename);
{

    SDL_Surface *image = loadBMP(filename);
    SDL_Rect dest_rect;

    dest_rect.x = 350;
    dest_rect.y = 150;

    SDL_BlitSurface(image, 0, window, &dest_rect);

    SDL_Flip(window);

}

SDL_Surface *loadBMP(const char *file_name)
{
    SDL_Surface *tmp = SDL_LoadBMP(file_name);
    SDL_Surface *image = NULL;

    if (tmp)
    {
        image = SDL_DisplayFormat(tmp);
        SDL_FreeSurface(tmp);

        if (!image)
        {
            printf("ERROR in converting: %s\n", SDL_GetError());
        }
    }
    else
    {
        printf("ERROR: image(%s) could not load: %s",file_name,  SDL_GetError());
    }

    return image;
}
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

int pbm_load(const char *imagePath, pbm_Image *imageHandler)
{
    FILE *file = fopen(imagePath, "rb");
    if (file == NULL)
    {
        printf("ERROR, could not load %s\n", imagePath);
        return EXIT_FAILURE;
    }

    char header[3];
    if (fscanf(file, "%2s", header) != 1)
    {
        printf("Invalid file format\n");
        fclose(file);
        return EXIT_FAILURE;
    }
    if ('P' != header[0] || '4' != header[1])
    {
        printf("Invalid PBM file format, should be P4 but is %c%c\n", header[0], header[1]);
        fclose(file);
        return EXIT_FAILURE;
    }

    // Skip comments (lines starting with #)
    char c;
    while ((c = fgetc(file)) == '#')
    {
        while ((c = fgetc(file)) != '\n')
            ;
    }
    ungetc(c, file);

    if (fscanf(file, "%d %d", &(imageHandler->width), &(imageHandler->height)) != 2)
    {
        printf("Error reading image size\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Skip the maximum grayscale value
    fscanf(file, "%*d");

    // Load image data
    // 1 bit per pixel => 1 byte for 8 pixels
    uint32_t imageDataSize = (imageHandler->width * imageHandler->height + 7) / 8;
    uint8_t *data = (uint8_t *)malloc(imageDataSize);
    if (data == NULL)
    {
        printf("Error allocating memory for image data\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    fread(data, 1, imageDataSize, file);
    imageHandler->data = data;
    fclose(file);
    return EXIT_SUCCESS;
}

void pbm_display(SDL_Surface *screen, pbm_Image *image)
{
    if (NULL == screen || NULL == image)
    {
        return;
    }

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255)); // White background

    // SDL_Surface *image = loadBMP(filename);

    for (uint32_t y = 0; y < image->height; y++)
    {
        for (uint32_t x = 0; x < image->width; x++)
        {
            uint32_t byteIndex = (y * image->width + x) / 8;
            uint32_t bitIndex = (y * image->width + x) % 8;
            uint8_t byte = image->data[byteIndex];
            uint8_t pixelColor = (byte & (0x80 >> bitIndex)) ? 0 : 255; // 0 for black, 255 for white

            // Draw the pixel (black or white)
            SDL_Rect rect = {x, y, 1, 1}; // 1x1 pixel
            SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, pixelColor, pixelColor, pixelColor));
        }
    }

    SDL_Flip(screen);
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
        printf("ERROR: image(%s) could not load: %s", file_name, SDL_GetError());
    }

    return image;
}
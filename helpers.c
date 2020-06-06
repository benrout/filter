#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through each row
    for (int i = 0; i < height; i++)
    {
        // Loop through each pixel in each row
        for (int j = 0; j < width; j++)
        {
            // Get values for red, green and blue
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            int total = red + green + blue;

            // Calculate the average
            int average = round(total / (float)3);

            // Set the values to the average
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Calculate midpoint of image row
    int midpoint = floor(width / 2);

    // Loop through each row
    for (int i = 0; i < height; i++)
    {
        // Loop through the pixels in each row
        for (int j = 0; j < midpoint; j++)
        {
            // Get pointers to both pixels to swap and store in *a and *b
            RGBTRIPLE *a = &image[i][j];
            RGBTRIPLE *b = &image[i][width - 1 - j];

            // Swap a and b
            RGBTRIPLE temp = *a;
            *a = *b;
            *b = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of the original image
    RGBTRIPLE original[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original[i][j] = image[i][j];
        }
    }

    // Loop through each row
    for (int i = 0; i < height; i++)
    {
        // Loop through each pixel in each row
        for (int j = 0; j < width; j++)
        {
            int num_pixels = 0;
            int total_red = 0;
            int total_green = 0;
            int total_blue = 0;

            // Loop through the surrounding pixels of each pixel
            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {
                    // Check if surrounding pixel is out of bounds, then continue
                    if (k < 0 || k > height - 1 || l < 0 || l > width - 1)
                    {
                        continue;
                    }

                    // Otherwise add to totals
                    total_red += original[k][l].rgbtRed;
                    total_green += original[k][l].rgbtGreen;
                    total_blue += original[k][l].rgbtBlue;
                    num_pixels++;
                }
            }

            // calculate averages
            int average_red = round(total_red / (float)num_pixels);
            int average_green = round(total_green / (float)num_pixels);
            int average_blue = round(total_blue / (float)num_pixels);

            // set pixel to averages
            image[i][j].rgbtRed = average_red;
            image[i][j].rgbtGreen = average_green;
            image[i][j].rgbtBlue = average_blue;
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of the original image
    RGBTRIPLE original[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original[i][j] = image[i][j];
        }
    }

    // Initialise Gx and Gy matrices
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Loop through each row
    for (int i = 0; i < height; i++)
    {
        // Loop through each pixel in each row
        for (int j = 0; j < width; j++)
        {
            // Initialise gx values
            int gx_red = 0;
            int gx_green = 0;
            int gx_blue = 0;

            // Initialise gy values
            int gy_red = 0;
            int gy_green = 0;
            int gy_blue = 0;

            // Loop through the surrounding pixels of each pixel
            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {
                    // Check if surrounding pixel is out of bounds, then continue
                    if (k < 0 || k > height - 1 || l < 0 || l > width - 1)
                    {
                        continue;
                    }

                    // Calculate Gx values
                    gx_red += original[k][l].rgbtRed * Gx[k - i + 1][j - l + 1];
                    gx_green += original[k][l].rgbtGreen * Gx[k - i + 1][j - l + 1];
                    gx_blue += original[k][l].rgbtBlue * Gx[k - i + 1][j - l + 1];

                    // Calculate Gy values
                    gy_red += original[k][l].rgbtRed * Gy[k - i + 1][j - l + 1];
                    gy_green += original[k][l].rgbtGreen * Gy[k - i + 1][j - l + 1];
                    gy_blue += original[k][l].rgbtBlue * Gy[k - i + 1][j - l + 1];
                }
            }

            // calculate new colour values from gx and gy values
            int new_red = round(sqrt(pow(gx_red, 2) + pow(gy_red, 2)));
            int new_green = round(sqrt(pow(gx_green, 2) + pow(gy_green, 2)));
            int new_blue = round(sqrt(pow(gx_blue, 2) + pow(gy_blue, 2)));

            // // set pixel to new values
            image[i][j].rgbtRed = new_red > 255 ? 255 : new_red;
            image[i][j].rgbtGreen = new_green > 255 ? 255 : new_green;
            image[i][j].rgbtBlue = new_blue > 255 ? 255 : new_blue;
        }
    }
}

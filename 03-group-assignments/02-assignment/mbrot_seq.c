#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <complex.h>

#define REAL float
#define M 1000

int *mbrot_cpu(REAL complex c0, REAL complex c1, int h, int w, int threads, int *result) {
    REAL delta_x = (crealf(c1) - crealf(c0)) / w;
    REAL delta_y = (cimagf(c1) - cimagf(c0)) / h;

    REAL complex z;
    REAL complex c;
    int i, j, k;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            z = 0;
            c = (crealf(c0) + (i * delta_y)) + (cimagf(c0) + (j * delta_x))*I;
            for (k = 0; k < M && cabsf(z) <= 2.0; k++) {
                z = z*z + c;
            }
            result[i*w + j] = M - k;
        }
    }

    return result;
}


void write_image(int h, int w, int *matrix, char *filename) {
    FILE* fp = NULL;
    fp = fopen(filename, "w");

    png_infop info_ptr = NULL;
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_init_io(png_ptr, fp);
    info_ptr = png_create_info_struct(png_ptr);

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, info_ptr, w, h, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    png_bytep row = malloc(3 * w * sizeof(png_byte));

    // compose image
    int i, j;
    for (i = 0; i < h; i++) {
        for (j = 0; j < 3 * w; j++)
            row[j] = ((float)matrix[i*w + j/3] / M) * 255;

        // write to the png file
        png_write_row(png_ptr, row);
    }
    png_write_end(png_ptr, NULL);

    free(row);
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    fclose(fp);
}


int main(int argc, char* argv[]) {
    REAL complex c0 = atof(argv[1]) + atof(argv[2]) * I;
    REAL complex c1 = atof(argv[3]) + atof(argv[4]) * I;

    int w = atoi(argv[5]);
    int h = atoi(argv[6]);

    int gpu = atoi(argv[7]);
    int threads = atoi(argv[8]);

    char* filename = argv[9];

    int *brotset = malloc(sizeof(int) * w * h);
    brotset = mbrot_cpu(c0, c1, h, w, threads, brotset);

    write_image(h, w, brotset, filename);

    free(brotset);

    return 0;
}

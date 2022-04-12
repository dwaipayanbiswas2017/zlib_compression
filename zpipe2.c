/* zpipe.c: example of proper use of zlib's inflate() and deflate()
   Not copyrighted -- provided to the public domain
   Version 1.4  11 December 2005  Mark Adler */

/* Version history:
   1.0  30 Oct 2004  First version
   1.1   8 Nov 2004  Add void casting for unused return values
                     Use switch statement for inflate() return values
   1.2   9 Nov 2004  Add assertions to document zlib guarantees
   1.3   6 Apr 2005  Remove incorrect assertion in inf()
   1.4  11 Dec 2005  Add hack to avoid MSDOS end-of-line conversions
                     Avoid some compiler warnings for input and output buffers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "zlib.h"

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 16384
#define CHAR_CHUNK 1
#define MAX 100

unsigned long getFileSize(FILE* source)
{
    fseek(source, 0L, SEEK_END);
    unsigned long size = ftell(source);
    fseek(source, 0L, SEEK_SET);
    return size;
}

/* report a zlib or i/o error */
void zerr(int ret)
{
    fputs("zpipe: ", stderr);
    switch (ret) {
    case Z_ERRNO:
        if (ferror(stdin))
            fputs("error reading stdin\n", stderr);
        if (ferror(stdout))
            fputs("error writing stdout\n", stderr);
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    }
}

int compress_data(FILE* source, FILE* dest, int level)
{
    int ret, flush;
    clock_t t1, t2;
    unsigned char msg[MAX];

    unsigned long fileSize   = getFileSize(source);
    unsigned long source_len = fileSize;
    unsigned long dest_len   = fileSize;
    unsigned long long count = 0;

    snprintf(msg, MAX, "Source File Size: %lu\n", fileSize);
    fputs(msg, stderr);

    char* source_data = (char*)malloc(fileSize);
    char* dest_data   = (char*)malloc(fileSize);

    if (source_data == NULL)
    {
        fputs("Error Source allocation...\n", stderr);
    }
    fputs("Source_data allocation done...\n", stderr);

    if (dest_data == NULL)
    {
        fputs("Error Destination allocation...\n", stderr);
    }
    fputs("Destination_data allocation done...\n", stderr);
    

    do
    {
        fread(source_data + count, 1, CHAR_CHUNK, source);
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        count++;
    } while ((flush != Z_FINISH) && (count != fileSize));
    count--;
    

    t1 = clock();
    
    ret = compress2(dest_data, &dest_len, source_data, source_len, level);

    t2 = clock();
    double elapsed = ((double)(t2 - t1)) / CLOCKS_PER_SEC;

    snprintf(msg, MAX, "Time required: %lf Second\n", elapsed);
    fputs(msg, stderr);

    snprintf(msg, MAX, "Destinition File Size: %lu\n", dest_len);
    fputs(msg, stderr);

    if (ferror(dest))
    {
        fputs("Destnition ERROR !!!", stderr);
    }

    fputs("Destnition write start ...", stderr);
    if (fwrite(dest_data, 1, dest_len, dest) != dest_len)
    {
        fputs("Destnition write failed !!!", stderr);
        return Z_ERRNO;
    }
    fputs("Destnition write SUCCESS...", stderr);

    return ret;
}

int uncompress_data(FILE* source, FILE* dest)
{
    int ret, flush;
    clock_t t1, t2;
    unsigned char msg[MAX];

    unsigned long fileSize   = getFileSize(source);
    unsigned long source_len = fileSize;
    unsigned long dest_len   = 2097152;
    unsigned long long count = 0;

    snprintf(msg, MAX, "Source File Size: %lu\n", fileSize);
    fputs(msg, stderr);

    unsigned char* source_data = (char*)malloc(source_len);
    unsigned char* dest_data = (char*)malloc(dest_len);

    if (source_data == NULL)
    {
        fputs("Error Source allocation...\n", stderr);
        return Z_ERRNO;
    }
    fputs("Source_data allocation done...\n", stderr);

    if (dest_data == NULL)
    {
        fputs("Error Destination allocation...\n", stderr);
        return Z_ERRNO;
    }
    fputs("Destination_data allocation done...\n", stderr);

    do
    {
        fread(source_data + count, 1, CHAR_CHUNK, source);
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        count++;
    } while ((flush != Z_FINISH) && (count != fileSize));
    count--;


    t1 = clock(); // start time

    ret = uncompress2(dest_data, &dest_len, source_data, &source_len);

    t2 = clock(); // end time
    double elapsed = ((double)(t2 - t1)) / CLOCKS_PER_SEC; // decompression time

    snprintf(msg, MAX, "Time required: %lf Second\n", elapsed);
    fputs(msg, stderr);

    snprintf(msg, MAX, "Destinition File Size: %lu\n", dest_len);
    fputs(msg, stderr);

    if (ferror(dest))
    {
        fputs("Destnition ERROR !!!", stderr);
    }

    fputs("Destnition write start ...\n", stderr);
    if (fwrite(dest_data, 1, dest_len, dest) != dest_len)
    {
        fputs("Destnition write failed !!!", stderr);
        return Z_ERRNO;
    }
    fputs("Destnition write SUCCESS...\n", stderr);

    return ret;
}

int main(int argc, char **argv)
{
    /*
    ***************************************************************************************
    * COMPRESS COMMAND   : ./CompressionTest.exe source_file.dat destination_file.dat.z
    * DECOMPRESS COMMAND : ./CompressionTest.exe [-d] destination_file.dat.z source_file.dat
    ***************************************************************************************
    */
    
    int ret;
    char msg[MAX];
    snprintf(msg, MAX, "Argc: %d\nARGV[1] => %s\nARGV[2] => %s\n", argc, argv[1], argv[2]);
    fputs(msg, stderr);
    if (argc == 3)
    {
        FILE* source = fopen(argv[1], "rb+");
        FILE* dest   = fopen(argv[2], "wb+");

        ret = compress_data(source, dest, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK)
            zerr(ret);
        return ret;
    }
    else if ((argc == 4) && (strcmp(argv[1], "-d") == 0))
    {
        FILE *source = fopen(argv[2], "rb+");
        FILE *dest   = fopen(argv[3], "wb+");

        ret = uncompress_data(source, dest);
        if (ret != Z_OK)
            zerr(ret);
        return ret;
    }
    else
    {
        fputs("CompressionTest usage: ./CompressionTest.exe [-d] < source > dest\n", stderr);
        return 1;
    }
}
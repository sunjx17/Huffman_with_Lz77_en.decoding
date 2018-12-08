/***************************************************************************
 *          Lempel, Ziv Encoding and Decoding
 *
 *   File    : bitio.h
 *   Authors : David Costa and Pietro De Rosa
 *
 ***************************************************************************/
#pragma once
/***************************************************************************
 *                                CONSTANTS
 ***************************************************************************/
#define BIT_IO_W 0
#define BIT_IO_R 1
#define BIT_IO_WB 2
/***************************************************************************
 *                            TYPE DEFINITIONS
 ***************************************************************************/
struct bitFILE;

/***************************************************************************
 *                         FUNCTIONS DECLARATION
 ***************************************************************************/
#ifndef bitio_h
#define bitio_h
int bitof(int n);
int bitIO_feof(struct bitFILE *bitF);
int bitIO_ferror(struct bitFILE *bitF);
struct bitFILE* bitIO_open(const char *path, int mode);
int bitIO_close(struct bitFILE *bitF);
int bitIO_write(struct bitFILE *bitF, void *info, int nbit);
int bitIO_read(struct bitFILE *bitF, void *info, int info_s, int nbit);unsigned int bitio_getnowpos(bitFILE*);int bitIO_w_putchar(bitFILE *,unsigned int ,void*);
bool bitIO_readbit(struct bitFILE *bitF);
#endif

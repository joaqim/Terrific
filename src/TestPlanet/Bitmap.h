#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <memory>
#include <limits.h> // for INT_MAX


#define RGB(r,g,b)          ((DWORD)((BYTE)(r) | ((BYTE)(g) << 8) | ((BYTE)(b) << 16)))

#define GetRValue(rgb)      ((BYTE)  (rgb) )
#define GetGValue(rgb)      ((BYTE) ((rgb) >> 8))
#define GetBValue(rgb)      ((BYTE) ((rgb) >> 16))

//////////////////////////////////////////////////////
struct Point {
  Point() = default;
 Point(int const x_, int const y_) : x(x_), y(y_) {}
  int x, y;
};


// for Linux platform, plz make sure the size of data type is correct for BMP spec.
// if you use this on Windows or other platforms, plz pay attention to this.
typedef int LONG;
typedef unsigned char BYTE;
typedef unsigned int DWORD;
typedef unsigned short WORD;

// __attribute__((packed)) on non-Intel arch may cause some unexpected error, plz be informed.

typedef struct tagBITMAPFILEHEADER
{
  WORD    bfType; // 2  /* Magic identifier */
  DWORD   bfSize; // 4  /* File size in bytes */
  WORD    bfReserved1; // 2
  WORD    bfReserved2; // 2
  DWORD   bfOffBits; // 4 /* Offset to image data, bytes */ 
} __attribute__((packed)) BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
  DWORD    biSize; // 4 /* Header size in bytes */
  LONG     biWidth; // 4 /* Width of image */
  LONG     biHeight; // 4 /* Height of image */
  WORD     biPlanes; // 2 /* Number of colour planes */
  WORD     biBitCount; // 2 /* Bits per pixel */
  DWORD    biCompress; // 4 /* Compression type */
  DWORD    biSizeImage; // 4 /* Image size in bytes */
  LONG     biXPelsPerMeter; // 4
  LONG     biYPelsPerMeter; // 4 /* Pixels per meter */
  DWORD    biClrUsed; // 4 /* Number of colours */ 
  DWORD    biClrImportant; // 4 /* Important colours */ 
} __attribute__((packed)) BITMAPINFOHEADER;

/*
  typedef struct tagRGBQUAD
  {
  unsigned char    rgbBlue;   
  unsigned char    rgbGreen;
  unsigned char    rgbRed;  
  unsigned char    rgbReserved;
  } RGBQUAD;
  * for biBitCount is 16/24/32, it may be useless
  */

typedef struct tagBITMAPINFO {
  BITMAPINFOHEADER bmiHeader; 
}__attribute__((packed)) BITMAPINFO;

typedef struct
{
  BYTE    b;
  BYTE    g;
  BYTE    r;
} RGB_data; // RGB TYPE, plz also make sure the order

int bmp_generator(std::string const &filename, int width, int height, unsigned char *data);

class Bitmap {
public:
  Bitmap(int const width_, int const height_);
  bool GenerateBitmap(std::string const &filename);
  bool SaveBitmap(std::string const &filename);

  void SetPixel(int x, int y, BYTE r= 0x07, BYTE g=0xff, BYTE b=0x07);
  void SetPixel(int x, int y, float r, float g, float b);

  int const width() {return _width;}
  int const height() {return _height;}
 private:
  BITMAPINFO bitmapInfo;
  BITMAPFILEHEADER bmp_head;
  BITMAPINFOHEADER bmp_info;


  RGB_data buffer[512][512];

  int _width, _height;
  int _size; 


};





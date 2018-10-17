#include "Bitmap.h"

using std::cout;
using std::endl;
using std::cerr;
 
int bmp_generator(std::string const &filename, int width, int height, unsigned char *data) {
  BITMAPFILEHEADER bmp_head;
  BITMAPINFOHEADER bmp_info;
  int size = width * height * 3;

  bmp_head.bfType = 0x4D42; // 'BM'
  bmp_head.bfSize= size + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // 24 + head + info no quad    
  bmp_head.bfReserved1 = bmp_head.bfReserved2 = 0;
  bmp_head.bfOffBits = bmp_head.bfSize - size;
  // finish the initial of head

  bmp_info.biSize = 40;
  bmp_info.biWidth = width;
  bmp_info.biHeight = height;
  bmp_info.biPlanes = 1;
  bmp_info.biBitCount = 24; // bit(s) per pixel, 24 is true color
  bmp_info.biCompress = 0;
  bmp_info.biSizeImage = size;
  bmp_info.biXPelsPerMeter = 0;
  bmp_info.biYPelsPerMeter = 0;
  bmp_info.biClrUsed = 0 ;
  bmp_info.biClrImportant = 0;
  // finish the initial of infohead;

  // copy the data
  FILE *fp;
  if (!(fp = fopen(filename.c_str(),"wb"))) return EXIT_FAILURE;

  fwrite(&bmp_head, 1, sizeof(BITMAPFILEHEADER), fp);
  fwrite(&bmp_info, 1, sizeof(BITMAPINFOHEADER), fp);
  fwrite(data, 1, size, fp);
  fclose(fp);

  return EXIT_SUCCESS;
}

#if 0
int main(int argc, char **argv)
{
  int i,j;

  RGB_data buffer[512][512];

  memset(buffer, 0, sizeof(buffer));

  for (i = 0; i < 256; i++)
  {
    for (j = 0; j < 256; j++)
    {
      //buffer[i][j].g = buffer[i][j].b = 0x07;
      //buffer[i][j].r = 0xff;
         }
  }

  bmp_generator("./test.bmp", 512, 512, (BYTE*)buffer);    

  return EXIT_SUCCESS;
}
#endif



//////////////////////////////////////////////////////


Bitmap::Bitmap(int const width_, int const height_) :
    _width(width_), _height(height_), _size(_width * _height * 3)
{
  const std::string filename = "./test.bmp"; 

  memset(buffer, 0, sizeof(buffer)); // Clear memory 
#if 0
  //bmp_generator("./test.bmp", _width, _height, (BYTE*)buffer); 

  for (std::size_t i = 0; i < _width; i++) {
    for (std::size_t j = 0; j < _height; j++) {
      buffer[i][j].g = buffer[i][j].b = 0x07;
      buffer[i][j].r = 0xff;
      if(i > 100 && j > 100) {
            SetPixel(i, j, 0x07, 0x07, 0x07);
            }

    }
  }

  assert(SaveBitmap(filename) == true);
#endif
}

bool Bitmap::GenerateBitmap(std::string const &filename) {
  //_size = _width * _height * 3;

  bmp_head.bfType = 0x4D42; // 'BM'
  bmp_head.bfSize= _size + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); // 24 + head + info no quad    
  bmp_head.bfReserved1 = bmp_head.bfReserved2 = 0;
  bmp_head.bfOffBits = bmp_head.bfSize - _size;
  // finish the initial of head

  bmp_info.biSize = 40;
  bmp_info.biWidth = _width;
  bmp_info.biHeight = _height;
  bmp_info.biPlanes = 1;
  bmp_info.biBitCount = 24; // bit(s) per pixel, 24 is true color
  bmp_info.biCompress = 0;
  bmp_info.biSizeImage = _size;
  bmp_info.biXPelsPerMeter = 0;
  bmp_info.biYPelsPerMeter = 0;
  bmp_info.biClrUsed = 0 ;
  bmp_info.biClrImportant = 0;
  // finish the initial of infohead;

  return true;
}

bool Bitmap::SaveBitmap(std::string const &filename) {
  if(!GenerateBitmap(filename)) {
    std::cout << "Failed to generate Bitmap" << std::endl;
    return false;
  }

  // copy the data
  FILE *fp;
  if (!(fp = fopen(filename.c_str(),"wb"))) {
    std::cout << "Failed to open "  << filename  << " for writing\n";
    return false;
  }

  fwrite(&bmp_head, 1, sizeof(BITMAPFILEHEADER), fp);
  fwrite(&bmp_info, 1, sizeof(BITMAPINFOHEADER), fp);
  fwrite(buffer, 1, _size, fp);
  fclose(fp);
  return true;
};


void Bitmap::SetPixel(int x, int y, float r, float g, float b) {
  SetPixel(x, y, RGB(r, g, b));
}

void Bitmap::SetPixel(int x, int y, BYTE r, BYTE g, BYTE b) {
  buffer[x][y].r = r;
  buffer[x][y].g = g;
  buffer[x][y].b = b;
}

//////////////////////////////////////////////////////

static int DistanceSqrd(int const x1, int const y1, int const x2, int const y2) {
  int xd = x2 - x1;
  int yd = y2 - y1;
  return (xd * xd) + (yd * yd);
}

static int DistanceSqrd(const Point& point, int x, int y) {
  int xd = x - point.x;
  int yd = y - point.y;
  return (xd * xd) + (yd * yd);
}

//////////////////////////////////////////////////////
class Voronoi {
 public:
  void Make(Bitmap* bmp, int count) {
    bmp_ = bmp;
    CreatePoints(count);
    CreateColors();
    CreateSites();
    SetSitesPoints();
  }
 
 private:
  void CreateSites() {
    int w = bmp_->width(), h = bmp_->height(), d;
    for (int hh = 0; hh < h; hh++) {
      for (int ww = 0; ww < w; ww++) {
        int ind = -1, dist = INT_MAX;
        for (size_t it = 0; it < points_.size(); it++) {
          const Point& p = points_[it];
          d = DistanceSqrd(p, ww, hh);
          if (d < dist) {
            dist = d;
            ind = it;
          }
        }
 
        if (ind > -1)
          bmp_->SetPixel(ww, hh, colors_[ind]);
        //else
        //__asm nop // should never happen!
      } 
    }
  }
 
  void SetSitesPoints() {
    for (const auto& point : points_) {
      int x = point.x, y = point.y;
      for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++)
          bmp_->SetPixel(x + i, y + j);
    }
  }
 
  void CreatePoints(int count) {
    const int w = bmp_->width() - 20, h = bmp_->height() - 20;
    for (int i = 0; i < count; i++) {
      points_.push_back({ rand() % w + 10, rand() % h + 10 });
    }
  }
 
  void CreateColors() {
    for (size_t i = 0; i < points_.size(); i++) {
      DWORD c = RGB(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50);
      colors_.push_back(c);
    }
  }
 
  std::vector<Point> points_;
  std::vector<DWORD> colors_;
  Bitmap* bmp_;
};


#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <unordered_map>

class RGBToHex
{
  public:
  static std::string rgb(int r, int g, int b);
};

unsigned char sat(int x)
{
  return std::max(std::min(x,255),0);
}

constexpr char tab[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
void convert(char* out, unsigned char in)
{
    out[0] = tab[in >> 4];
    out[1] = tab[in & 0xf];
}

struct Dur
{
    std::chrono::high_resolution_clock::time_point start;
    std::string header;

    Dur(std::string header_="") : start(std::chrono::high_resolution_clock::now()), header(header_)
    {
    }

    ~Dur()
    {
        std::chrono::high_resolution_clock::time_point finish = std::chrono::high_resolution_clock::now();
        std::cout << header << " " << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << std::endl;
    }
};

std::string RGBToHex::rgb(int r, int g, int b)
{
    static char y[6];
    convert(y, sat(r));
    convert(y+2, sat(g));
    convert(y+4, sat(b));
    return std::string(y, 6);
}

int main()
{
    Dur a("convert");
    for (long i=0; i<50000000; i++)
    {
        auto x = RGBToHex::rgb(10,223,54);
        x += "s";
        if (x.empty())
            return 1;
    }
    return 0;
}

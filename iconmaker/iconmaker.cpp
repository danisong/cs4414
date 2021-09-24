#include "pngwriter.hpp"
using namespace std;

void horizontal(pngwriter& png, double red, double green, double blue) 
{
    png.filledsquare(1, 30, 64, 35, red, green, blue);
}

void vertical(pngwriter& png, double red, double green, double blue)
{
    png.filledsquare(30, 1, 35, 64, red, green, blue);
}

void leftdiag(pngwriter& png, double red, double green, double blue)
{
    int i;
    png.line(1, 64, 64, 1, red, green, blue);
    for (i = 1; i < 4; i++) {
        png.line(1, 64-i, 64-i, 1, red, green, blue);
        png.line(1+i, 64, 64, 1+i, red, green, blue);
    }
}

void rightdiag(pngwriter& png, double red, double green, double blue)
{
    int i;
    png.line(1, 1, 64, 64, red, green, blue);
    for (i = 1; i < 4; i++) {
        png.line(1+i, 1, 64, 64-i, red, green, blue);
        png.line(1, 1+i, 64-i, 64, red, green, blue);
    }
}

void addcolor(int c, string& fn, double& r, double& g, double& b) 
{
    if (c == 0) {fn += "g"; r = 0.0; g = 1.0; b = 0.0;}
    else if (c == 1) {fn += "r"; r = 1.0; g = 0.0; b = 0.0;}
    else {fn += "y"; r = 1.0; g = 1.0; b = 0.0;}
}

int main()
{  
    string filename;
    // colors: 0 = green, 1 = red, 2 = yellow
    int i, color1, color2, color3, color4;
    double r1, g1, b1, r2, g2, b2, r3, g3, b3, r4, g4, b4;

    // 2-street intersection
    for (i = 0; i < 9; i++) {
        filename = "files/i2";
        color1 = i/3;
        addcolor(color1, filename, r1, g1, b1);
        color2 = i%3;
        addcolor(color2, filename, r2, g2, b2);

        filename += ".png";
        pngwriter test(64, 64, 1.0, filename.c_str());
        horizontal(test, r1, g1, b1);
        vertical(test, r2, g2, b2);
        test.close();
    }

    // 3-street intersection
    for (i = 0; i < 27; i++) {
        filename = "files/i3";
        color1 = i/9;
        addcolor(color1, filename, r1, g1, b1);
        color2 = (i/3)%3;
        addcolor(color2, filename, r2, g2, b2);
        color3 = i%3;
        addcolor(color3, filename, r3, g3, b3);

        filename += ".png";
        pngwriter test(64, 64, 1.0, filename.c_str());
        horizontal(test, r1, g1, b1);
        leftdiag(test, r2, g2, b2);
        vertical(test, r3, g3, b3);
        test.close();
    }

    // 4-street intersection
    for (i = 0; i < 81; i++) {
        filename = "files/i4";
        color1 = i/27;
        addcolor(color1, filename, r1, g1, b1);
        color2 = (i/9)%3;
        addcolor(color2, filename, r2, g2, b2);
        color3 = (i/3)%3;
        addcolor(color3, filename, r3, g3, b3);
        color4 = i%3;
        addcolor(color4, filename, r4, g4, b4);
        
        filename += ".png";
        pngwriter test(64, 64, 1.0, filename.c_str());
        horizontal(test, r1, g1, b1);
        leftdiag(test, r2, g2, b2);
        vertical(test, r3, g3, b3);
        rightdiag(test, r4, g4, b4);
        test.close();
    }
    return 0;
}
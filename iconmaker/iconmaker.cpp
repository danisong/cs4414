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

int main()
{
    
    string filename;
    int i;
    double r1, g1, b1, r2, g2, b2, r3, g3, b3, r4, g4, b4;

    // 2-street intersection
    for (i = 0; i < 9; i++) {
        filename = "files/i2";
        if (i/3 == 0) {
            filename += "g";
            r1 = 0.0;
            g1 = 1.0;
            b1 = 0.0;
        }
        else if (i/3 == 1) {
            filename += "r";
            r1 = 1.0;
            g1 = 0.0;
            b1 = 0.0;            
        }
        else {
            filename += "y";
            r1 = 1.0;
            g1 = 1.0;
            b1 = 0.0;
        }
        if (i%3 == 0) {
            filename += "g";
            r2 = 0.0;
            g2 = 1.0;
            b2 = 0.0;
        }
        else if (i%3 == 1) {
            filename += "r";
            r2 = 1.0;
            g2 = 0.0;
            b2 = 0.0; 
        }
        else {
            filename += "y";
            r2 = 1.0;
            g2 = 1.0;
            b2 = 0.0;
        }

        filename += ".png";
        pngwriter test(64, 64, 1.0, filename.c_str());
        horizontal(test, r1, g1, b1);
        vertical(test, r2, g2, b2);
        test.close();
    }

    // 3-street intersection
    for (i = 0; i < 27; i++) {
        filename = "files/i3";
        if (i/9 == 0) {
            filename += "g";
            r1 = 0.0;
            g1 = 1.0;
            b1 = 0.0;
        }
        else if (i/9 == 1) {
            filename += "r";
            r1 = 1.0;
            g1 = 0.0;
            b1 = 0.0;            
        }
        else {
            filename += "y";
            r1 = 1.0;
            g1 = 1.0;
            b1 = 0.0;
        }
        if (i/3 == 0) {
            filename += "g";
            r2 = 0.0;
            g2 = 1.0;
            b2 = 0.0;
        }
        else if (i/3 == 1) {
            filename += "r";
            r2 = 1.0;
            g2 = 0.0;
            b2 = 0.0;            
        }
        else {
            filename += "y";
            r2 = 1.0;
            g2 = 1.0;
            b2 = 0.0;
        }
        if (i%3 == 0) {
            filename += "g";
            r3 = 0.0;
            g3 = 1.0;
            b3 = 0.0;
        }
        else if (i%3 == 1) {
            filename += "r";
            r3 = 1.0;
            g3 = 0.0;
            b3 = 0.0; 
        }
        else {
            filename += "y";
            r3 = 1.0;
            g3 = 1.0;
            b3 = 0.0;
        }

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
        if (i/27 == 0) {
            filename += "g";
            r1 = 0.0;
            g1 = 1.0;
            b1 = 0.0;
        }
        else if (i/27 == 1) {
            filename += "r";
            r1 = 1.0;
            g1 = 0.0;
            b1 = 0.0;            
        }
        else {
            filename += "y";
            r1 = 1.0;
            g1 = 1.0;
            b1 = 0.0;
        }
        if (i/9 == 0) {
            filename += "g";
            r2 = 0.0;
            g2 = 1.0;
            b2 = 0.0;
        }
        else if (i/9 == 1) {
            filename += "r";
            r2 = 1.0;
            g2 = 0.0;
            b2 = 0.0;            
        }
        else {
            filename += "y";
            r2 = 1.0;
            g2 = 1.0;
            b2 = 0.0;
        }
        if (i/3 == 0) {
            filename += "g";
            r3 = 0.0;
            g3 = 1.0;
            b3 = 0.0;
        }
        else if (i/3 == 1) {
            filename += "r";
            r3 = 1.0;
            g3 = 0.0;
            b3 = 0.0;            
        }
        else {
            filename += "y";
            r3 = 1.0;
            g3 = 1.0;
            b3 = 0.0;
        }
        if (i%3 == 0) {
            filename += "g";
            r4 = 0.0;
            g4 = 1.0;
            b4 = 0.0;
        }
        else if (i%3 == 1) {
            filename += "r";
            r4 = 1.0;
            g4 = 0.0;
            b4 = 0.0; 
        }
        else {
            filename += "y";
            r4 = 1.0;
            g4 = 1.0;
            b4 = 0.0;
        }

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


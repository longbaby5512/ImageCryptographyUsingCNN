#pragma once
#include "HopfieldNetwork.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <ctime>
#include <iostream>
#include <string>

class ImageCryptography
{
private:
    static void copy(int* src, int* dst, int len)
    {
        for (int i = 0; i < len; ++i)
            dst[i] = src[i];
    }
public:
    static void encrypt(const char*, int*, int = 30, int = 5, bool = true, bool = true, char* = nullptr);
    static void decrypt(const char*, int*, int = 30, int = 5, bool = true, bool = true, char* = nullptr);

    static void encrypt(const char*, std::vector<int*>, int = 30, int = 5, bool = true, bool = true, char* = nullptr);
    static void decrypt(const char*, std::vector<int*>, int = 30, int = 5, bool = true, bool = true, char* = nullptr);
};




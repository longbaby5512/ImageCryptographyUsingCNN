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
public:
    static void encrypt(const char*, int*, bool = true, bool = false, char* = nullptr);
    static void decrypt(const char*, int*, bool = true, bool = false, char* = nullptr);
};




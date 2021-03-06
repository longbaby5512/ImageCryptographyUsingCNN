#include "ImageCryptography.h"

void ImageCryptography::encrypt(const char* plainImageName, int * key, int lengthPattern, int blockPattern, bool showImage, bool saveImage, char* saveName)
{   
    HopfieldNetwork hopfieldNetwork(lengthPattern, blockPattern);
    int* pattern = new int[lengthPattern * blockPattern];
    copy(key, pattern, lengthPattern * blockPattern);
    clock_t startTime(clock());
    clock_t curentTime(startTime);

    cv::Mat image = cv::imread(plainImageName);
    if (image.empty())
        return;
    std::cout << "Start encrypting...";

    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            int pixel[3];
            pixel[0] = image.at<cv::Vec3b>(i, j)[0];
            pixel[1] = image.at<cv::Vec3b>(i, j)[1];
            pixel[2] = image.at<cv::Vec3b>(i, j)[2];

            hopfieldNetwork.encrypt(pixel, pattern);

            image.at<cv::Vec3b>(i, j)[0] = pixel[0];
            image.at<cv::Vec3b>(i, j)[1] = pixel[1];
            image.at<cv::Vec3b>(i, j)[2] = pixel[2];
            
            if (clock() - curentTime >= 2000)
            {
                curentTime = clock();
                std::cout << ".";
            }
        }
    }
    
    clock_t endTime = clock();
    if ((endTime - startTime) / 1000 < 60)
        std::cout << "\nEnding encrypting... Encrypting in " << (endTime - startTime) / 1000 << " seconds\n";
    else
    {
        int min = (endTime - startTime) / 1000 / 60;
        int sec = (endTime - startTime) / 1000 % 60;
        std::cout << "\nEnding encrypting... Encrypting in " << min << " minutes " << sec << " seconds\n";
    }

    delete[] pattern;
    pattern = nullptr;

    if (showImage)
    {
        cv::imshow("Image Encryption", image);
        cv::waitKey(0);
    }

    if (saveImage)
    {
        if (!saveName)
        {
            std::string name, ext;
            size_t pos(0);
            size_t len(strlen(plainImageName));
            for (size_t i = 0; i < len; ++i)
            {
                if (plainImageName[i] == '.')
                {
                    pos = i + 1;
                    break;
                }
                name.push_back(plainImageName[i]);
            }
            for (size_t i = pos; i < len; ++i)
                ext.push_back(plainImageName[i]);

            cv::imwrite(name + "_encryption." + ext, image);
        }
        else
            cv::imwrite(saveName, image);
    }
}

void ImageCryptography::decrypt(const char* cipherImageName, int* key, int lengthPattern, int blockPattern, bool showImage, bool saveImage, char* saveName)
{
    HopfieldNetwork hopfieldNetwork(lengthPattern, blockPattern);

    int* pattern = new int[lengthPattern * blockPattern];
    copy(key, pattern, lengthPattern * blockPattern);

    clock_t startTime(clock());
    clock_t curentTime(startTime);
    cv::Mat image = cv::imread(cipherImageName);
    if (image.empty())
        return;
    std::cout << "Start decrypting...";

    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
        {
            int pixel[3];
            pixel[0] = image.at<cv::Vec3b>(i, j)[0];
            pixel[1] = image.at<cv::Vec3b>(i, j)[1];
            pixel[2] = image.at<cv::Vec3b>(i, j)[2];

            hopfieldNetwork.decrypt(pixel, pattern);

            image.at<cv::Vec3b>(i, j)[0] = pixel[0];
            image.at<cv::Vec3b>(i, j)[1] = pixel[1];
            image.at<cv::Vec3b>(i, j)[2] = pixel[2];

            if (clock() - curentTime >= 2000)
            {
                curentTime = clock();
                std::cout << ".";
            }
        }
    }

    clock_t endTime = clock();
    if ((endTime - startTime) / 1000 < 60)
        std::cout << "\nEnding encrypting... Decrypting in " << (endTime - startTime) / 1000 << " seconds\n";
    else
    {
        int min = (endTime - startTime) / 1000 / 60;
        int sec = (endTime - startTime) / 1000 % 60;
        std::cout << "\nEnding encrypting... Decrypting in " << min << " minutes " << sec << " seconds\n";
    }

    delete[] pattern;
    pattern = nullptr;

    if (showImage)
    {
        cv::imshow("Image Decryption", image);
        cv::waitKey(0);
    }

    if (saveImage)
    {
        if (!saveName)
        {
            std::string name, ext;
            size_t pos(0);
            size_t len(strlen(cipherImageName));
            for (size_t i = 0; i < len; ++i)
            {
                if (cipherImageName[i] == '.')
                {
                    pos = i + 1;
                    break;
                }
                name.push_back(cipherImageName[i]);
            }
            for (int i = pos; i < len; ++i)
                ext.push_back(cipherImageName[i]);

            cv::imwrite(name + "_decryption." + ext, image);
        }
        else
            cv::imwrite(saveName, image);
    }
}

void ImageCryptography::encrypt(const char* plainImageName, std::vector<int*> key, int lengthPattern, int blockPattern, bool showImage, bool saveImage, char* saveName)
{
    clock_t startTime(clock());
    clock_t curentTime(startTime);

    cv::Mat image = cv::imread(plainImageName);
    if (image.empty())
        return;
    std::cout << "Start encrypting...";

    for (auto it = key.begin(); it != key.end(); ++it)
    {
        HopfieldNetwork hopfieldNetwork(lengthPattern, blockPattern);

        int* pattern = new int[lengthPattern * blockPattern];
        copy(*it, pattern, lengthPattern * blockPattern);

        for (int i = 0; i < image.rows; ++i)
        {
            for (int j = 0; j < image.cols; ++j)
            {
                int pixel[3];
                pixel[0] = image.at<cv::Vec3b>(i, j)[0];
                pixel[1] = image.at<cv::Vec3b>(i, j)[1];
                pixel[2] = image.at<cv::Vec3b>(i, j)[2];

                hopfieldNetwork.encrypt(pixel, pattern);

                image.at<cv::Vec3b>(i, j)[0] = pixel[0];
                image.at<cv::Vec3b>(i, j)[1] = pixel[1];
                image.at<cv::Vec3b>(i, j)[2] = pixel[2];

                if (clock() - curentTime >= 2000)
                {
                    curentTime = clock();
                    std::cout << ".";
                }
            }
        }
        delete[] pattern;
        pattern = nullptr;
    }

    clock_t endTime = clock();
    if ((endTime - startTime) / 1000 < 60)
        std::cout << "\nEnding encrypting... Encrypting in " << (endTime - startTime) / 1000 << " seconds\n";
    else
    {
        int min = (endTime - startTime) / 1000 / 60;
        int sec = (endTime - startTime) / 1000 % 60;
        std::cout << "\nEnding encrypting... Encrypting in " << min << " minutes " << sec << " seconds\n";
    }


    if (showImage)
    {
        cv::imshow("Image Encryption", image);
        cv::waitKey(0);
    }

    if (saveImage)
    {
        if (!saveName)
        {
            std::string name, ext;
            size_t pos(0);
            size_t len(strlen(plainImageName));
            for (size_t i = 0; i < len; ++i)
            {
                if (plainImageName[i] == '.')
                {
                    pos = i + 1;
                    break;
                }
                name.push_back(plainImageName[i]);
            }
            for (size_t i = pos; i < len; ++i)
                ext.push_back(plainImageName[i]);

            cv::imwrite(name + "_encryption." + ext, image);
        }
        else
            cv::imwrite(saveName, image);
    }
}

void ImageCryptography::decrypt(const char* cipherImageName, std::vector<int*> key, int lengthPattern, int blockPattern, bool showImage, bool saveImage, char* saveName)
{
    clock_t startTime(clock());
    clock_t curentTime(startTime);
    cv::Mat image = cv::imread(cipherImageName);
    if (image.empty())
        return;
    std::cout << "Start decrypting...";

    for (auto it = key.rbegin(); it != key.rend(); ++it)
    {
        HopfieldNetwork hopfieldNetwork(lengthPattern, blockPattern);

        int* pattern = new int[lengthPattern * blockPattern];
        copy(*it, pattern, lengthPattern * blockPattern);
        for (int i = 0; i < image.rows; ++i)
        {
            for (int j = 0; j < image.cols; ++j)
            {
                int pixel[3];
                pixel[0] = image.at<cv::Vec3b>(i, j)[0];
                pixel[1] = image.at<cv::Vec3b>(i, j)[1];
                pixel[2] = image.at<cv::Vec3b>(i, j)[2];

                hopfieldNetwork.decrypt(pixel, pattern);

                image.at<cv::Vec3b>(i, j)[0] = pixel[0];
                image.at<cv::Vec3b>(i, j)[1] = pixel[1];
                image.at<cv::Vec3b>(i, j)[2] = pixel[2];

                if (clock() - curentTime >= 2000)
                {
                    curentTime = clock();
                    std::cout << ".";
                }
            }
        }
        delete[] pattern;
        pattern = nullptr;
    }

    clock_t endTime = clock();
    if ((endTime - startTime) / 1000 < 60)
        std::cout << "\nEnding encrypting... Decrypting in " << (endTime - startTime) / 1000 << " seconds\n";
    else
    {
        int min = (endTime - startTime) / 1000 / 60;
        int sec = (endTime - startTime) / 1000 % 60;
        std::cout << "\nEnding encrypting... Decrypting in " << min << " minutes " << sec << " seconds\n";
    }


    if (showImage)
    {
        cv::imshow("Image Decryption", image);
        cv::waitKey(0);
    }

    if (saveImage)
    {
        if (!saveName)
        {
            std::string name, ext;
            size_t pos(0);
            size_t len(strlen(cipherImageName));
            for (size_t i = 0; i < len; ++i)
            {
                if (cipherImageName[i] == '.')
                {
                    pos = i + 1;
                    break;
                }
                name.push_back(cipherImageName[i]);
            }
            for (int i = pos; i < len; ++i)
                ext.push_back(cipherImageName[i]);

            cv::imwrite(name + "_decryption." + ext, image);
        }
        else
            cv::imwrite(saveName, image);
    }
}
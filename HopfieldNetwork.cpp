#include "HopfieldNetwork.h"

HopfieldNetwork::HopfieldNetwork(int lengthPattern, int blockPattern)
{
    this->lengthPattern = lengthPattern;
    this->blockPattern = blockPattern;
    weightMatrix = new int[lengthPattern * lengthPattern];
    chooseWeight = 0;
    for (int i = 0; i < 38; ++i) {
        chaoticNetworkArray[i] = 0;
    }
}

HopfieldNetwork::~HopfieldNetwork()
{
    delete[] weightMatrix;
    weightMatrix = nullptr;
}

void HopfieldNetwork::generateWeightMatrixBinary(int* pattern)
{
    for (int j = 0; j < lengthPattern; ++j)
    {
        for (int i = j; i < lengthPattern; ++i)
        {
            if (i == j)
                weightMatrix[j * lengthPattern + i] = 0;
            else
            {
                int sum(0);
                for (int k = 0; k < blockPattern; ++k)
                    sum += (pattern[k * lengthPattern + i] * 2 - 1) * (pattern[k * lengthPattern + j] * 2 - 1);

                weightMatrix[j * lengthPattern + i] = weightMatrix[i * lengthPattern + j] = sum;
            }
        }
    }
}

void HopfieldNetwork::generateWeightMatrixBipolar(int* pattern)
{
    for (int j = 0; j < lengthPattern; ++j)
    {
        for (int i = j; i < lengthPattern; ++i)
        {
            if (i == j)
                weightMatrix[j * lengthPattern + i] = 0;
            else
            {
                int sum(0);
                for (int k = 0; k < blockPattern; ++k)
                    sum += pattern[k * lengthPattern + i] * pattern[k * lengthPattern + j];

                weightMatrix[j * lengthPattern + i] = weightMatrix[i * lengthPattern + j] = sum;
            }
        }
    }
}

/**
 * @brief Build Hopfield Neural Network
 *
 * @param iter numerical order of iterator
 * @param pattern pattern value of neural network
 */
void HopfieldNetwork::buildChaoticNetwork(int iter, int* pattern)
{
    int selection(iter % blockPattern); // choose numerical order of selection pattern for building

    // If 37th of pattern is 0, choose weight updating binary
    if (!chooseWeight)
        generateWeightMatrixBinary(pattern); // Update weight matrix for binary input pattern

    // If 37th of pattern is 1, choose weight updating bipolar
    else
    {
        // Convert binary input pattern to bipolar input pattern
        for (int i = 0; i < lengthPattern * blockPattern; ++i)
        {
            if (!pattern[i])
                pattern[i] = -1;
        }
        generateWeightMatrixBipolar(pattern); // Update weight matrix for bipolar input pattern
    }

    // Calculate i .th element of chaostic network
    int y[38]{ 0 };     // Initialize y, y is hidden layer
    int threshold(0); // Initialize threshold value

    // Initialize hidden layer value
    for (int i = 0; i < lengthPattern; ++i)
        y[i] = pattern[i];

    // Update pattern by chaostic network
    for (int i = 0; i < lengthPattern; ++i)
    {
        for (int j = 0; j < lengthPattern; ++j)
            y[i] += y[j] * weightMatrix[i * lengthPattern + j]; // yi = yi + ∑(yj * wji)

        chaoticNetworkArray[i] = y[i] > threshold ? 1 : 0;

        // Update pattern again
        pattern[selection * lengthPattern + i] = chaoticNetworkArray[i];
    }

    // If use update bipolar generator function, convert bipolar input pattern to binary input pattern, else continue
    for (int i = 0; i < lengthPattern * blockPattern; ++i)
    {
        if (pattern[i] == -1)
            pattern[i] = 0;
    }
    chooseWeight = pattern[selection * lengthPattern + 37];
}

void HopfieldNetwork::dec2bin(int* decArr, int* binArr)
{
    int counter(0);
    for (int i = 0; i < 3; ++i)
    {
        int decVal = decArr[i];
        int tmp[8]{ 0 };
        int secondConter(7);
        while (decVal > 0)
        {
            tmp[secondConter--] = decVal % 2;
            decVal /= 2;
        }
        for (int j = 0; j < 8; j++)
        {
            binArr[counter++] = tmp[j];
        }
    }
}

void HopfieldNetwork::bin2dec(int* binArr, int* decArr)
{
    for (int i = 0; i < 3; ++i)
    {
        int val(binArr[i * 8] * 128 + binArr[i * 8 + 1] * 64 + binArr[i * 8 + 2] * 32 + binArr[i * 8 + 3] * 16 +
            binArr[i * 8 + 4] * 8 + binArr[i * 8 + 5] * 4 + binArr[i * 8 + 6] * 2 + binArr[i * 8 + 7]);
        decArr[i] = val;
    }
}

void HopfieldNetwork::encrypt(int* plaintext, int* pattern)
{
    int binArr[24];
    dec2bin(plaintext, binArr);
    // Iterate 24 times
    for (int iter = 0; iter < 24; ++iter)
    {
        buildChaoticNetwork(iter, pattern);
        int aj[24];           // Initialize aj
        int a1jp[5], a1ja[5]; // Initialize a1j = dj

        for (int i = 0; i < 24; i++)
            aj[i] = chaoticNetworkArray[i]; // aj by 24-bit first of chaostic network

        for (int i = 0; i < 5; i++)
        {
            a1jp[i] = a1ja[i] = chaoticNetworkArray[i + 24];
            a1jp[i] ^= binArr[i];  // a1j xor with 5-bit first of binary array
            a1ja[i] ^= aj[i + 19]; // a1j xor with 5 bit after of a1j
        }

        for (int i = 0; i < 24; i++)
        {
            if (i < 19)
            {
                binArr[i] = binArr[i + 5]; // Part 2 Pj
                aj[23 - i] = aj[18 - i];   // Part 1 Aj
            }
            else
            {
                binArr[i] = a1jp[i - 19];  // Part 1 pj
                aj[i - 19] = a1ja[i - 19]; // Part 2 Aj
            }
        }

        for (int i = 0; i < 24; i++)
            binArr[i] ^= aj[i];
    }
    bin2dec(binArr, plaintext);
}

void HopfieldNetwork::decrypt(int* ciphertext, int* pattern)
{
    int binArr[24];
    dec2bin(ciphertext, binArr);
    int outputchaosticnetwork[24][30];

    for (int ite = 0; ite < 24; ite++)
    {
        buildChaoticNetwork(ite, pattern);
        for (int i = 0; i < 30; i++)
            outputchaosticnetwork[ite][i] = chaoticNetworkArray[i];
    }

    // Iterate 24 times
    for (int ite = 23; ite >= 0; ite--)
    {
        int aj[24];           // Initialize aj
        int a1jp[5], a1ja[5]; // Initialize a1j = dj
        for (int i = 0; i < 24; i++)
            aj[i] = outputchaosticnetwork[ite][i]; // aj by 24-bit first of chaostic network
        for (int i = 0; i < 5; i++)
        {
            a1jp[i] = a1ja[i] = outputchaosticnetwork[ite][i + 24];
            a1ja[i] ^= aj[i + 19]; // a1j xor with 5-bit after of a1j
        }
        for (int i = 0; i < 24; i++)
        {
            if (i < 19)
                aj[23 - i] = aj[18 - i];
            else
                aj[i - 19] = a1ja[i - 19];
        }
        for (int i = 0; i < 24; i++)
            binArr[i] ^= aj[i];

        int a1jp1[5];
        for (int i = 0; i < 5; i++)
            a1jp1[i] = binArr[19 + i];

        for (int i = 0; i < 24; i++)
        {
            if (i < 19)
                binArr[23 - i] = binArr[18 - i];
            else
                binArr[i - 19] = a1jp1[i - 19];
        }

        for (int i = 0; i < 5; i++)
            binArr[i] ^= a1jp[i];
    }
    bin2dec(binArr, ciphertext);
}

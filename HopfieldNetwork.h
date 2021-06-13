#pragma once
class HopfieldNetwork
{
private:
    int lengthPattern;
    int blockPattern;
    int* weightMatrix;
    int chaoticNetworkArray[38]; // Initialize output of Hopfield neural network
    int chooseWeight;            // Choose the first weight update function

public:
    HopfieldNetwork(int, int);
    ~HopfieldNetwork();
    void generateWeightMatrixBinary(int*);
    void generateWeightMatrixBipolar(int*);
    void buildChaoticNetwork(int, int*);
    void bin2dec(int*, int*);
    void dec2bin(int*, int*);
    void encrypt(int*, int*);
    void decrypt(int* cyphertext, int* pattern);
};
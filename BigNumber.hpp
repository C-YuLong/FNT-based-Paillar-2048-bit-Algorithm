#ifndef BIGNUMBER
#define BIGNUMBER

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>

class BigNumber {
public:
    virtual ~BigNumber() {}

    virtual void fromHexString(const std::string& hex) = 0;
    virtual void fromUint8Array(const uint8_t* arr, size_t size) = 0;
    virtual void print() const = 0;
    virtual void printHexNumber(const std::string& debug) const = 0;

protected:
    // Convert hex string to uint8 array
    void hexStringToUint8Array(const std::string& hex, uint8_t* arr, size_t size) {
        std::stringstream ss;
        size_t len = hex.length();
        size_t arrIndex = 0;

        for (size_t i = 0; i < len; i += 2) {
            std::string part;
            if(len-i-2==-1) 
                part = hex.substr(0, 1);
            else
                part = hex.substr(len - i - 2, 2);
            ss.clear();
            ss << std::hex << part;
            uint16_t value;
            ss >> value;
            arr[arrIndex++] = value;
            // if(i == len-3){
            //     std::cout << "i: " << i << " len: " << len << " part: " << part << " value: " << value << std::endl;
            // }
            // if(i == len-1){
            //     std::cout << "i: " << i << " len: " << len << " part: " << part << " value: " << value << std::endl;
            // }
        }

        // Fill remaining part with 0
        for (size_t i = arrIndex; i < size; ++i) {
            arr[i] = 0;
        }
        // printHex(arr, size);
    }

    void printNumber(const uint8_t* arr, size_t size, const std::string& debug) const {
        std::cout << debug << ": " << std::endl;
        std::cout << "0x";
        for (int i=size-1; i>=0; --i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]);
            if(static_cast<int>(arr[i]) != 0){
                ;
            }
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // Print helper function to avoid leading zeros
    void printHex(const uint8_t* arr, size_t size) const {
        // bool leading = true;
        // for (int i = size - 1; i >= 0; --i) {
        for (size_t i = 0; i < size; ++i) {
            // if (leading && arr[i] == 0) {
            //     continue;  // skip leading zeros
            // }
            // leading = false;
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]) << ", ";
            // std::cout << std::hex << std::setw(2) << std::setfill('0') << "0x" << static_cast<int>(arr[i]) << ", ";
        }
        std::cout << std::endl;
    }

    void printHex(const uint32_t* arr, size_t size) const {
        // bool leading = true;
        // for (int i = size - 1; i >= 0; --i) {
        for (int i = 0; i < size; ++i) {
            // if (leading && arr[i] == 0) {
            //     continue;  // skip leading zeros
            // }
            // leading = false;
            // std::cout << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(arr[i]) << ", ";
            std::cout << std::setw(8) << std::setfill('0') << static_cast<int>(arr[i]) << ", ";
            // std::cout << std::hex << std::setw(2) << std::setfill('0') << "0x" << static_cast<int>(arr[i]) << ", ";
        }
        std::cout << std::endl;
    }

    void printHex(const uint64_t* arr, size_t size) const {
        // bool leading = true;
        // for (int i = size - 1; i >= 0; --i) {
        for (int i = 0; i < size; ++i) {
            // if (leading && arr[i] == 0) {
            //     continue;  // skip leading zeros
            // }
            // leading = false;
            // std::cout << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(arr[i]) << ", ";
            std::cout << std::setw(8) << std::setfill('0') << static_cast<int>(arr[i]) << ", ";
            // std::cout << std::hex << std::setw(2) << std::setfill('0') << "0x" << static_cast<int>(arr[i]) << ", ";
        }
        std::cout << std::endl;
    }

};


#endif
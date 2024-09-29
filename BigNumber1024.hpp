#ifndef BIGNUMBER_1024
#define BIGNUMBER_1024

#include <string>
#include <cstdint>
#include <bits/stdc++.h>
#include "BigNumber.hpp"
#include "BigNumber2048.hpp"


class BigNumber1024 : public BigNumber {

public:
    uint8_t data[128]; // 由0到128分别为由低到高的权重

    BigNumber1024() {
        for (size_t i = 0; i < 128; ++i) {
            data[i] = 0;
        }
    }

    BigNumber1024(const std::string& hex) {
        fromHexString(hex);
    }

    BigNumber1024(const uint8_t* arr, size_t size) {
        fromUint8Array(arr, size);
    }

    void fromHexString(const std::string& hex) override {
        hexStringToUint8Array(hex, data, 128);
    }

    void fromUint8Array(const uint8_t* arr, size_t size) override {
        for (size_t i = 0; i < 128; ++i) {
            if (i < size) {
                data[i] = arr[i];
            } else {
                data[i] = 0;
            }
        }
    }

    void print() const override {
        printHex(data, 128);
    }
    
    void printHexNumber(const std::string& debug) const override {
        printNumber(data, size, debug);
    }

    // 返回最高位
    int highestBit() const {
        for (int i = size-1; i >= 0; --i) {
            if (data[i] != 0) {
                for (int j = 7; j >= 0; --j) {
                    if ((data[i] & (1 << j)) != 0) {
                        return i * 8 + j;
                    }
                }
            }
        }
        return -1;  // If the number is zero
    }

    // 升级为2048位的数
    BigNumber2048 upgrade() const ;

    // 重载左移运算符
    BigNumber1024& operator<<(int shift) {
        if (shift < 0) return *this;  // Invalid shift
        int byteShift = shift / 8;
        int bitShift = shift % 8;

        // Shift by bytes first
        if (byteShift > 0) {
            for (int i = 127; i >= byteShift; --i) {
                data[i] = data[i - byteShift];
            }
            for (int i = 0; i < byteShift; ++i) {
                data[i] = 0;
            }
        }

        // Shift remaining bits
        if (bitShift > 0) {
            uint8_t carry = 0;
            for (int i = 0; i < 128; ++i) {
                uint8_t newCarry = (data[i] >> (8 - bitShift)) & 0xFF;
                data[i] = (data[i] << bitShift) | carry;
                carry = newCarry;
            }
        }
        return *this;
    }

    // 重载右移运算符
    BigNumber1024& operator>>(int shift) {
        if (shift < 0) return *this;  // Invalid shift
        int byteShift = shift / 8;
        int bitShift = shift % 8;

        // Shift by bytes first
        if (byteShift > 0) {
            for (int i = 0; i < 128 - byteShift; ++i) {
                data[i] = data[i + byteShift];
                // printf("put %d to %d\n", data[i + byteShift], data[i]);
            }
            for (int i = 128 - byteShift; i < 128; ++i) {
                data[i] = 0;
            }
        }

        // Shift remaining bits
        if (bitShift > 0) {
            uint8_t carry = 0;
            for (int i = 127; i >= 0; --i) {
                uint8_t newCarry = (data[i] << (8 - bitShift)) & 0xFF;
                data[i] = (data[i] >> bitShift) | carry;
                carry = newCarry;
            }
        }
        return *this;
    }

    // 重载加法运算符：没有考虑进位
    BigNumber1024 operator+(const BigNumber1024& other) const {
        BigNumber1024 result;
        uint16_t carry = 0;
        for (int i = 0; i < size; ++i) {
            uint16_t sum = data[i] + other.data[i] + carry;
            result.data[i] = sum & 0xFF;
            carry = (sum >> 8) & 0xFF;
        }
        return result;
    }

    // 重载减法运算符：没有考虑负数（也许有，但是没有仔细研究）
    BigNumber1024 operator-(const BigNumber1024& other) const {
        BigNumber1024 result;
        int16_t borrow = 0;
        for (int i = 0; i < size; ++i) {
            int16_t diff = static_cast<int16_t>(data[i]) - static_cast<int16_t>(other.data[i]) - borrow;
            if (diff < 0) {
                diff += 256;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.data[i] = diff & 0xFF;
        }
        return result;
    }

    // 重载大于符号
    bool operator>(const BigNumber1024& other) const {
        for (int i = size; i >= 0; --i) {
            if (data[i] > other.data[i]) return true;
            if (data[i] < other.data[i]) return false;
        }
        return false;  // they are equal
    }

    // 重载大于等于符号
    bool operator>=(const BigNumber1024& other) const {
        for (int i = size; i >= 0; --i) {
            if (data[i] > other.data[i]) return true;
            if (data[i] < other.data[i]) return false;
        }
        return true;  // they are equal
    }

    // 重载小于符号
    bool operator<(const BigNumber1024& other) const {
        for (int i = size; i >= 0; --i) {
            if (data[i] < other.data[i]) return true;
            if (data[i] > other.data[i]) return false;
        }
        return false;  // they are equal
    }

    // 重载小于等于符号
    bool operator<=(const BigNumber1024& other) const {
        for (int i = size; i >= 0; --i) {
            if (data[i] < other.data[i]) return true;
            if (data[i] > other.data[i]) return false;
        }
        return true;  // they are equal
    }

    // 重载赋值运算符
    BigNumber1024& operator=(const BigNumber1024& other) {
        if (this == &other) {
            return *this; // Handle self-assignment
        }

        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }

        return *this;
    }

private:
    const int size = 128;
    
    
};



#endif
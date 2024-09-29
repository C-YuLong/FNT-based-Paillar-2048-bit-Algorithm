#ifndef BIGNUMBER_8192
#define BIGNUMBER_8192

#include <string>
#include <cstdint>
#include <bits/stdc++.h>
#include "BigNumber.hpp"
#include "BigNumber4096.hpp"

class BigNumber8192 : public BigNumber {
public:
    uint8_t data[1024];
    uint8_t overflow = 0; // 不得已而为之，这里的溢出太严重了，添加一个byte来存储溢出的部分
    // 添加了包含overflow的初始化函数

    BigNumber8192() {
        for (size_t i = 0; i < 1024; ++i) {
            data[i] = 0;
        }
    }

    BigNumber8192(const std::string& hex) {
        fromHexString(hex);
    }

    BigNumber8192(const uint8_t* arr, size_t size) {
        fromUint8Array(arr, size);
    }

    BigNumber8192(const uint8_t* arr, size_t size, uint8_t overflow) {
        fromUint8Array(arr, size);
        this->overflow = overflow;
    }

    void fromHexString(const std::string& hex) override {
        hexStringToUint8Array(hex, data, 1024);
    }

    void fromUint8Array(const uint8_t* arr, size_t size) override {
        for (size_t i = 0; i < 1024; ++i) {
            if (i < size) {
                data[i] = arr[i];
            } else {
                data[i] = 0;
            }
        }
    }

    void print() const override {
        printHex(data, 1024);
    }

    void printHexNumber(const std::string& debug) const override {
        printNumber(data, size, debug);
    }

    // 降级为4096位的数，考虑为保留低位
    BigNumber4096 downgrade() const ;

    // Returns the highest bit
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

    // Overloaded assignment operator
    BigNumber8192& operator=(const BigNumber8192& other) {
        if (this == &other) {
            return *this; // Handle self-assignment
        }

        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }

        return *this;
    }

    // Overloaded left shift operator
    BigNumber8192& operator<<(int shift) {
        if (shift < 0) return *this;  // Invalid shift
        int byteShift = shift / 8;
        int bitShift = shift % 8;
        
        if (byteShift >= size) {
            // If shifting more than the size, it becomes zero
            for (int i = 0; i < size; ++i) {
                data[i] = 0;
            }
            return *this;
        }

        // Shift by bytes first
        if (byteShift > 0) {
            for (int i = size - 1; i >= byteShift; --i) {
                data[i] = data[i - byteShift];
            }
            for (int i = 0; i < byteShift; ++i) {
                data[i] = 0;
            }
        }

        // Shift the remaining bits
        if (bitShift > 0) {
            for (int i = size - 1; i > 0; --i) {
                data[i] = (data[i] << bitShift) | (data[i - 1] >> (8 - bitShift));
            }
            data[0] <<= bitShift;
        }

        return *this;
    }

    // 重载右移运算符（考虑了overflow）
    BigNumber8192& operator>>(int shift) {
        if (shift < 0) return *this;  // Invalid shift
        int byteShift = shift / 8;
        int bitShift = shift % 8;

        // Shift by bytes first
        if (byteShift > 0) {
            for (int i = 0; i < size - byteShift; ++i) {
                data[i] = data[i + byteShift];
                // printf("put %d to %d\n", data[i + byteShift], data[i]);
            }
            // for overflow byte
            data[size - byteShift] = overflow;
            for (int i = size - byteShift+1; i < size; ++i) {
                data[i] = 0;
            }
        }

        // Shift remaining bits
        if (bitShift > 0) {
            uint8_t carry = 0;
            for (int i = size-1; i >= 0; --i) {
                uint8_t newCarry = (data[i] << (8 - bitShift)) & 0xFF;
                data[i] = (data[i] >> bitShift) | carry;
                carry = newCarry;
            }
        }
        return *this;
    }

    // 重载加法运算符：没有考虑进位
    BigNumber8192 operator+(const BigNumber8192& other) const {
        BigNumber8192 result;
        uint16_t carry = 0;
        for (int i = 0; i < size; ++i) {
            uint16_t sum = data[i] + other.data[i] + carry;
            result.data[i] = sum & 0xFF;
            carry = (sum >> 8) & 0xFF;
        }
        return result;
    }

    // 重载减法运算符：没有考虑负数（也许有，但是没有仔细研究）
    BigNumber8192 operator-(const BigNumber8192& other) const {
        BigNumber8192 result;
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
    bool operator>(const BigNumber8192& other) const {
        for (int i = size; i >= 0; --i) {
            if (data[i] > other.data[i]) return true;
            if (data[i] < other.data[i]) return false;
        }
        return false;  // they are equal
    }

    // 重载大于等于符号
    bool operator>=(const BigNumber8192& other) const {
        for (int i = size; i >= 0; --i) {
            if (data[i] > other.data[i]) return true;
            if (data[i] < other.data[i]) return false;
        }
        return true;  // they are equal
    }

    // 重载小于符号
    bool operator<(const BigNumber8192& other) const {
        for (int i = size; i >= 0; --i) {
            if (data[i] < other.data[i]) return true;
            if (data[i] > other.data[i]) return false;
        }
        return false;  // they are equal
    }

    // 重载小于等于符号
    bool operator<=(const BigNumber8192& other) const {
        for (int i = size; i >= 0; --i) {
            if (data[i] < other.data[i]) return true;
            if (data[i] > other.data[i]) return false;
        }
        return true;  // they are equal
    }

private:
    const int size = 1024;
};

#endif // BIGNUMBER_8192

#ifndef PAILLAR_8
#define PAILLAR_8

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <bits/stdc++.h>
#include "BigNumber.hpp"
#include "BigNumber8192.hpp"
#include "BigNumber4096.hpp"
#include "BigNumber2048.hpp"
#include "BigNumber1024.hpp"

#define DEBUG_INFO 0


// 测试函数
void printHex(const uint8_t* arr, size_t size, std::string option) {
    // for (size_t i = size-1; i > 0 ; --i) {
    //     // 输出每个字节的十六进制表示，填充为两位数，高位补零
    //     if(option == "HEX")
    //         std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]);
    //     if(option == "DEC")
    //         std::cout << std::dec << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]) << " ";
    // }
    if(option == "HEX"){
        for (size_t i = size-1; i > 0 ; --i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]);
        }
    }
    if(option == "DEC")
        for (size_t i =0; i < size ; ++i) {
            std::cout << std::dec << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]) << " ";
        }
    std::cout << std::dec << std::endl; // 重置为十进制格式
}

void printHex(const uint32_t* arr, size_t size, std::string option) {
    if(option == "HEX"){
        for (size_t i = size-1; i > 0 ; --i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]);
        }
    }
    if(option == "DEC")
        for (size_t i =0; i < size ; ++i) {
            std::cout << std::dec << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]) << " ";
        }
    std::cout << std::dec << std::endl; // 重置为十进制格式
}

void printHex(const uint64_t* arr, size_t size, std::string option) {
    if(option == "HEX"){
        for (size_t i = size-1; i > 0 ; --i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]);
        }
    }
    if(option == "DEC")
        for (size_t i =0; i < size ; ++i) {
            std::cout << std::dec << std::setw(2) << std::setfill('0') << static_cast<int>(arr[i]) << " ";
        }
    std::cout << std::dec << std::endl; // 重置为十进制格式
}


class FNT {
    private:
        // static const long p = 104857601;         // 27 bits
        // static const int g = 3;

        // static const int omega_256 = 73653238;       // 27 bits
        // static const int omega_512 = 33690314;
        // static const int omega_1024 = 18773644;
        // static const int omega_2048 = 4354736;
        
        // static const int omega_inv_256 = 42994480;   // 27 bits
        // static const int omega_inv_512 = 83847972;
        // static const int omega_inv_1024 = 23338676;
        // static const int omega_inv_2048 = 18512281;

        // static const int N256_inv = 104448001;      // 27 bits
        // static const int N512_inv = 104652801;
        // static const int N1024_inv = 104755201;
        // static const int N2048_inv = 104806401;

        static const long p = 469762049;
        static const int g = 3;

        static const int omega_256 = 338628632;       // 27 bits
        static const int omega_512 = 25153357;
        static const int omega_1024 = 110059487;
        static const int omega_2048 = 165447688;
        
        static const int omega_inv_256 = 215855482;   // 27 bits
        static const int omega_inv_512 = 436579181;
        static const int omega_inv_1024 = 458753944;
        static const int omega_inv_2048 = 63413564;

        static const int N256_inv = 467927041;      // 27 bits
        static const int N512_inv = 468844545;
        static const int N1024_inv = 469303297;
        static const int N2048_inv = 469532673;
        
        static void RightShift(uint8_t* data, int size, int shift) {
            int byteShift = (shift / 8);
            int bitShift = shift % 8;

            // Shift by bytes first
            if (byteShift > 0) {
                for (int i = 0; i < size - byteShift; ++i) {
                    data[i] = data[i + byteShift];
                    // printf("put %d to %d\n", data[i + byteShift], data[i]);
                }
                for (int i = size - byteShift; i < size; ++i) {
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
        }

        // 由于输入的base至少是27bit，所以至少需要支持32位的乘法计算
        static uint64_t mod_exp(uint64_t base, uint64_t exp, uint64_t mod) {
            uint64_t result = 1;
            // printf("base: %d, exp: %d\n", base, exp);
            while (exp > 0) {
                if (exp % 2 == 1) {
                    result = ((result % mod) * (base % mod)) % mod;
                }
                base = ((base % mod) * (base % mod)) % mod;
                // assert(base < mod);
                // printf("base * base: %d\n", base);
                // printf("log_2_base: %d\n", std::log2(base));
                // printf("base: %d, exp: %d\n", base, exp);
                exp /= 2;
            }
            return result;
        }

        // FNT 中的辅助函数
        static int bit_reverse(int i, int log2n) {
            int rev_i = 0;
            for (int j = 0; j < log2n; ++j) {
                rev_i = (rev_i << 1) | (i & 1);
                i >>= 1;
            }
            return rev_i;
        }

        // FNT 中的辅助函数
        static void bit_reverse_copy(const uint64_t* a, uint64_t* result, int n) {
            int log2n = std::log2(n);
            for (int i = 0; i < n; ++i) {
                int rev_i = bit_reverse(i, log2n);
                result[rev_i] = a[i];
            }
        }

        // FNT 中的辅助函数的重载
        static void bit_reverse_copy(const uint8_t* a, uint32_t* result, int n) {
            int log2n = std::log2(n);
            for (int i = 0; i < n; ++i) {
                int rev_i = bit_reverse(i, log2n);
                result[rev_i] = a[i];
                // if(rev_i == 0)
                    // printf("a[%d]: %d, result[%d]: %d\n", i, a[i], rev_i, result[rev_i]);
            }
        }

        // 快速数论变换 (FNT)
        static void fnt_iterative(const uint8_t* a, uint32_t* re_a, int n, int p, int omega) {
            // uint8_t re_a[n];
            // printHex(a, n, "DEC");
            bit_reverse_copy(a, re_a, n);
            // printHex(re_a, n, "DEC");
            // int count = 0;

            for (int m = 1; m < n; m *= 2) {
                // check OK
                // omega: 27 bits, n: 5 bits, m: 5 bits
                uint64_t omega_m = mod_exp(omega, n / (2 * m), p);
                // printf("omega_m: %d\n", omega_m);
                for (int k = 0; k < n; k += 2 * m) {
                    uint64_t omega_mk = 1;
                    // printf("re_a[%d]: %d\n", k, re_a[k]);
                    for (int j = 0; j < m; ++j) {
                        // omega_mk 27 bits, re_a 8 bits, p 27 bits, t 55 bits
                        uint64_t t = (omega_mk * static_cast<uint64_t>(re_a[k + j + m])) % static_cast<uint64_t>(p);
                        // u 27 bits
                        uint64_t u = re_a[k + j];
                        re_a[k + j] = (u + t) % static_cast<uint64_t>(p);
                        re_a[k + j + m] = (u - t + p) % static_cast<uint64_t>(p);
                        // omega_mk 27 bits, omega_m 27 bits, p 27 bits
                        omega_mk = (omega_mk * omega_m) % static_cast<uint64_t>(p);
                        // debug: print u, t, j, m, (u + t) % p, (u - t) % p
                        // if (m==4){
                            // printf("u: %d, t: %d, j: %d, k: %d, m: %d, (u + t): %d, (u - t): %d\n", u, t, j, k, m, (u + t), (u - t));
                            // printf("omega_mk: %d, omega_m: %d, p: %d\n", omega_mk, omega_m, p);
                            // exit(1);
                            // count ++;
                            // if(count == 5){
                                // exit(1);
                            // }
                        // }

                    }
                }
                // debug: print re_a
                // printHex(re_a, n, "DEC");
            }
            // return re_a;
        }

        // 快速数论变换逆变换 (iFNT)
        static void ifnt_iterative(const uint64_t* A, uint64_t* re_A, int n, int p, int omega_inv, int N_inv) {
            // uint8_t* re_A = bit_reverse_copy(A, n);
            // uint8_t re_A[n];
            bit_reverse_copy(A, re_A, n);
            // printHex(re_A, n);

            for (int m = 1; m < n; m *= 2) {
                // omega_inv: 27 bits, n: 5 bits, m: 5 bits, p: 27 bits, omega_m: 27 bits
                uint64_t omega_m = mod_exp(omega_inv, n / (2 * m), p);
                for (int k = 0; k < n; k += 2 * m) {
                    uint64_t omega_mk = 1;
                    for (int j = 0; j < m; ++j) {
                        // omega_mk 27 bits, re_A 8 bits, p 27 bits, t 55 bits
                        uint64_t t = (static_cast<uint64_t>(omega_mk) * static_cast<uint64_t>(re_A[k + j + m])) % p;
                        // u 27 bits
                        uint64_t u = re_A[k + j];
                        re_A[k + j] = (u + t) % p;
                        re_A[k + j + m] = (u - t + p) % p;
                        // omega_mk 27 bits, omega_m 27 bits, p 27 bits
                        omega_mk = (omega_mk * omega_m) % p;
                    }
                }
            }

            // printHex(re_A, n);

            // Normalize the result
            // int n_inv = mod_exp(n, p - 2, p); // Inverse of n under modulo p
            for (int i = 0; i < n; ++i) {
                // re_A 8 bits, n_inv 27 bits, p 27 bits
                re_A[i] = (static_cast<uint64_t>(re_A[i]) * static_cast<uint64_t>(N_inv)) % p;
            }

            // return re_A;
        }
        
        // 进位函数，T是base
        static void processArray(uint64_t* arr, uint8_t *d_arr, int n, uint64_t T) {
            for (size_t i = 0; i < n; ++i) {
                if (arr[i] >= T) {
                    uint64_t carry = arr[i] / T;
                    arr[i] %= T;
                    if (i < n - 1) {
                        arr[i + 1] += carry; // 向高位进位
                    }
                }
                assert(arr[i] < T);
                d_arr[i] = arr[i];
            }
        }

    public:

        static BigNumber2048 multiply(const BigNumber1024 &A, const BigNumber1024 &B) {

            const int N = 1024 / 8 * 2;       // 10 bits

            // Perform FNT on A and B
            uint32_t A_fnt[N] = {0};
            uint32_t B_fnt[N] = {0};

            uint8_t A_tmp[N] = {0};
            uint8_t B_tmp[N] = {0};

            for(int i = 0; i < N/2; i++){
                A_tmp[i] = A.data[i];
                B_tmp[i] = B.data[i];
            }

            // printHex(A_tmp, N);

            fnt_iterative(A_tmp, A_fnt, N, p, omega_256);
            // printHex(A_fnt, N);
            fnt_iterative(B_tmp, B_fnt, N, p, omega_256);
            // printHex(B_fnt, N);

            // Perform pointwise multiplication
            uint64_t C_fnt[N];
            for (int i = 0; i < N; ++i) {
                // A_fnt 8 bits, B_fnt 8 bits, p 27 bits, C_fnt 8 bits
                C_fnt[i] = (static_cast<uint64_t>(A_fnt[i]) * static_cast<uint64_t>(B_fnt[i])) % p;
                // printf("");
                // printf("A_fnt[%d]: %d, B_fnt[%d]: %d, C_fnt[%d]: %d\n", i, A_fnt[i], i, B_fnt[i], i, C_fnt[i]);
            }

            // Perform inverse FNT on C
            // uint8_t *C = ifnt_iterative(C_fnt, N, p);
            uint64_t C[N] = {0};
            // printHex(C_fnt, N);
            ifnt_iterative(C_fnt, C, N, p, omega_inv_256, N256_inv);
            // printHex(C, N);

            uint64_t data[N] = {0};
            uint8_t data_tmp[N] = {0};
            processArray(C, data_tmp, N, 256);
            // printHex(data_tmp, N);

            // Return the result
            return BigNumber2048(data_tmp, 256);
        }

        static BigNumber4096 multiply(const BigNumber2048 &A, const BigNumber2048 &B) {

            const int N = 2048 / 8 * 2;       // 10 bits

            // Perform FNT on A and B
            uint32_t A_fnt[N] = {0};
            uint32_t B_fnt[N] = {0};

            uint8_t A_tmp[N] = {0};
            uint8_t B_tmp[N] = {0};

            for(int i = 0; i < N/2; i++){
                A_tmp[i] = A.data[i];
                B_tmp[i] = B.data[i];
            }

            // printHex(A_tmp, N);

            fnt_iterative(A_tmp, A_fnt, N, p, omega_512);
            // printHex(A_fnt, N);
            fnt_iterative(B_tmp, B_fnt, N, p, omega_512);
            // printHex(B_fnt, N);

            // Perform pointwise multiplication
            uint64_t C_fnt[N];
            for (int i = 0; i < N; ++i) {
                // A_fnt 8 bits, B_fnt 8 bits, p 27 bits, C_fnt 8 bits
                C_fnt[i] = (static_cast<uint64_t>(A_fnt[i]) * static_cast<uint64_t>(B_fnt[i])) % p;
                // printf("");
                // printf("A_fnt[%d]: %d, B_fnt[%d]: %d, C_fnt[%d]: %d\n", i, A_fnt[i], i, B_fnt[i], i, C_fnt[i]);
            }

            // Perform inverse FNT on C
            // uint8_t *C = ifnt_iterative(C_fnt, N, p);
            uint64_t C[N] = {0};
            // printHex(C_fnt, N);
            ifnt_iterative(C_fnt, C, N, p, omega_inv_512, N512_inv);
            // printHex(C, N);

            uint64_t data[N] = {0};
            uint8_t data_tmp[N] = {0};
            processArray(C, data_tmp, N, 256);
            // printHex(data_tmp, N);

            // Return the result
            return BigNumber4096(data_tmp, 512);
        }

        static BigNumber8192 multiply(const BigNumber4096 &A, const BigNumber4096 &B) {

            const int N = 4096 / 8 * 2;       // 10 bits

            // Perform FNT on A and B
            uint32_t A_fnt[N] = {0};
            uint32_t B_fnt[N] = {0};

            uint8_t A_tmp[N] = {0};
            uint8_t B_tmp[N] = {0};

            for(int i = 0; i < N/2; i++){
                A_tmp[i] = A.data[i];
                B_tmp[i] = B.data[i];
            }

            // printHex(A_tmp, N);

            fnt_iterative(A_tmp, A_fnt, N, p, omega_1024);
            // printHex(A_fnt, N);
            fnt_iterative(B_tmp, B_fnt, N, p, omega_1024);
            // printHex(B_fnt, N);

            // Perform pointwise multiplication
            uint64_t C_fnt[N];
            for (int i = 0; i < N; ++i) {
                // A_fnt 8 bits, B_fnt 8 bits, p 27 bits, C_fnt 8 bits
                C_fnt[i] = (static_cast<uint64_t>(A_fnt[i]) * static_cast<uint64_t>(B_fnt[i])) % p;
                // printf("");
                // printf("A_fnt[%d]: %d, B_fnt[%d]: %d, C_fnt[%d]: %d\n", i, A_fnt[i], i, B_fnt[i], i, C_fnt[i]);
            }

            // Perform inverse FNT on C
            // uint8_t *C = ifnt_iterative(C_fnt, N, p);
            uint64_t C[N] = {0};
            // printHex(C_fnt, N);
            ifnt_iterative(C_fnt, C, N, p, omega_inv_1024, N1024_inv);
            // printHex(C, N);

            uint64_t data[N] = {0};
            uint8_t data_tmp[N] = {0};
            processArray(C, data_tmp, N, 256);
            // printHex(data_tmp, N);

            // Return the result
            return BigNumber8192(data_tmp, 1024);
        }

        static BigNumber8192 multiply(const BigNumber8192 &A, const BigNumber8192 &B) {

            const int N = 8192 / 8 * 2;       // 10 bits

            // Perform FNT on A and B
            uint32_t A_fnt[N] = {0};
            uint32_t B_fnt[N] = {0};

            uint8_t A_tmp[N] = {0};
            uint8_t B_tmp[N] = {0};

            for(int i = 0; i < N/2; i++){
                A_tmp[i] = A.data[i];
                B_tmp[i] = B.data[i];
            }

            // printHex(A_tmp, N);

            fnt_iterative(A_tmp, A_fnt, N, p, omega_2048);
            // printHex(A_fnt, N, "DEC");
            fnt_iterative(B_tmp, B_fnt, N, p, omega_2048);
            // printHex(B_fnt, N);

            // Perform pointwise multiplication
            uint64_t C_fnt[N];
            for (int i = 0; i < N; ++i) {
                // A_fnt 8 bits, B_fnt 8 bits, p 27 bits, C_fnt 8 bits
                C_fnt[i] = (static_cast<uint64_t>(A_fnt[i]) * static_cast<uint64_t>(B_fnt[i])) % p;
                // printf("");
                // printf("A_fnt[%d]: %d, B_fnt[%d]: %d, C_fnt[%d]: %d\n", i, A_fnt[i], i, B_fnt[i], i, C_fnt[i]);
            }

            // Perform inverse FNT on C
            // uint8_t *C = ifnt_iterative(C_fnt, N, p);
            uint64_t C[N] = {0};
            // printHex(C_fnt, N);
            ifnt_iterative(C_fnt, C, N, p, omega_inv_2048, N2048_inv);
            // printHex(C, N);

            uint64_t data[N] = {0};
            uint8_t data_tmp[N] = {0};
            processArray(C, data_tmp, N, 256);
            // printHex(data_tmp, N, "HEX");
            // exit(1);

            // Return the result
            // 注意：这里返回的并不是完整的结果，而是截断后的结果
            // 注意：有出错的可能
            
            #if DEBUG_INFO
                if (data_tmp[1024] != 0) {
                    std::cout << "OverFlow Error in Paillar8.hpp: 8192 * 8192 = 8192 fucnction" << std::endl;
                    std::cout << "Error: data_tmp[1024] != 0" << std::endl;
                    std::cout << "Using OverFlow Byte to work out" << std::endl;
                    // exit(1);
                    // data_tmp[1024] = 0;
                }
            #endif
            if(data_tmp[1025] != 0){
                printf("data_tmp[1025]: %d\n", data_tmp[1025]);
                printf("data_tmp[1026]: %d\n", data_tmp[1026]);
            }
            
            assert(data_tmp[1025] == 0);

            return BigNumber8192(data_tmp, 1024, data_tmp[1024]);
        }

        static BigNumber8192 multiply(const BigNumber8192 &A, const BigNumber8192 &B, int shift) {

            const int N = 8192 / 8 * 2;       // 10 bits

            // Perform FNT on A and B
            uint32_t A_fnt[N] = {0};
            uint32_t B_fnt[N] = {0};

            uint8_t A_tmp[N] = {0};
            uint8_t B_tmp[N] = {0};

            for(int i = 0; i < N/2; i++){
                A_tmp[i] = A.data[i];
                B_tmp[i] = B.data[i];
            }

            // printHex(A_tmp, N);

            fnt_iterative(A_tmp, A_fnt, N, p, omega_2048);
            // printHex(A_fnt, N, "DEC");
            fnt_iterative(B_tmp, B_fnt, N, p, omega_2048);
            // printHex(B_fnt, N);

            // Perform pointwise multiplication
            uint64_t C_fnt[N];
            for (int i = 0; i < N; ++i) {
                // A_fnt 8 bits, B_fnt 8 bits, p 27 bits, C_fnt 8 bits
                C_fnt[i] = (static_cast<uint64_t>(A_fnt[i]) * static_cast<uint64_t>(B_fnt[i])) % p;
                // printf("");
                // printf("A_fnt[%d]: %d, B_fnt[%d]: %d, C_fnt[%d]: %d\n", i, A_fnt[i], i, B_fnt[i], i, C_fnt[i]);
            }

            // Perform inverse FNT on C
            // uint8_t *C = ifnt_iterative(C_fnt, N, p);
            uint64_t C[N] = {0};
            // printHex(C_fnt, N);
            ifnt_iterative(C_fnt, C, N, p, omega_inv_2048, N2048_inv);
            // printHex(C, N);

            uint64_t data[N] = {0};
            uint8_t data_tmp[N] = {0};
            processArray(C, data_tmp, N, 256);
            // printHex(data_tmp, N, "HEX");
            // exit(1);

            // Return the result
            // 注意：这里返回的并不是完整的结果，而是截断后的结果
            // 注意：有出错的可能
            
            #if DEBUG_INFO
                if (data_tmp[1024] != 0) {
                    std::cout << "OverFlow Error in Paillar8.hpp: 8192 * 8192 = 8192 fucnction" << std::endl;
                    std::cout << "Error: data_tmp[1024] != 0" << std::endl;
                    std::cout << "Using OverFlow Byte to work out" << std::endl;
                    // exit(1);
                    // data_tmp[1024] = 0;
                }
            #endif

            RightShift(data_tmp, N, shift);

            if(data_tmp[1025] != 0){
                printf("data_tmp[1025]: %d\n", data_tmp[1025]);
                printf("data_tmp[1026]: %d\n", data_tmp[1026]);
            }
            
            assert(data_tmp[1025] == 0);

            return BigNumber8192(data_tmp, 1024);
        }


};

// 处理交叉引用的问题：推迟实现函数

// 升级为2048位的数
BigNumber2048 BigNumber1024::upgrade() const {
    uint8_t data2048[2*size] = {0};
    for (int i = 0; i < size; ++i) {
        data2048[i] = data[i];
    }
    return BigNumber2048(data2048, 2*size);
}

// 升级为4096位的数
BigNumber4096 BigNumber2048::upgrade() const {
    uint8_t data4096[2*size] = {0};
    for (int i = 0; i < size; ++i) {
        data4096[i] = data[i];
    }
    return BigNumber4096(data4096, 2*size);
}

// 升级为8192位的数
BigNumber8192 BigNumber4096::upgrade() const {
    uint8_t data8192[2*size] = {0};
    for (int i = 0; i < size; ++i) {
        data8192[i] = data[i];
    }
    return BigNumber8192(data8192, 2*size);
}


// 降级为1024位的数
BigNumber1024 BigNumber2048::downgrade() const { 
    uint8_t data1024[size/2] = {0};
    for (int i = 0; i < size/2; ++i) {
        data1024[i] = data[i];
    }
    return BigNumber1024(data1024, size/2);
}

// 降级为2048位的数
BigNumber2048 BigNumber4096::downgrade() const { 
    uint8_t data2048[size/2] = {0};
    for (int i = 0; i < size/2; ++i) {
        data2048[i] = data[i];
    }
    return BigNumber2048(data2048, size/2);
}

// 降级为4096位的数，考虑为保留低位
BigNumber4096 BigNumber8192::downgrade() const { 
    uint8_t data4096[size/2] = {0};
    for (int i = 0; i < size/2; ++i) {
        data4096[i] = data[i];
    }
    return BigNumber4096(data4096, size/2);
}


#endif // PAILLAR_8
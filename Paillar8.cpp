#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>
#include <bits/stdc++.h>
#include "Paillar8.hpp"

#include "BigNumber.hpp"
#include "BigNumber8192.hpp"
#include "BigNumber4096.hpp"
#include "BigNumber2048.hpp"
#include "BigNumber1024.hpp"

#include "N2_ARRAY.hpp"
#include "P_ARRAY.hpp"
#include "P2_ARRAY.hpp"
#include "Q_ARRAY.hpp"
#include "Q2_ARRAY.hpp"

#include "para.hpp"

using namespace std;


// 用在FastExp中的第一步的结果
void BarrettReductionFastExp1(BigNumber4096 A, BigNumber4096 q, BigNumber4096& r){
    const int w_1 = A.highestBit()+1;
    // const int w_2 = q.highestBit();
    // H = (1 << (w_1 + 1)) // q
    // 考虑到这里输入的q是n**2，是一个很大的数
    // w_2 = 4096
    BigNumber4096 H;
    if(w_1 < 4096){
        r = A;
    }
    else{
        assert(w_1 == 4096);
        if (A < q) r = A;
        else r = A - q;
        assert(r < q);
    }
}

// 用在Fastexp中循环的函数
// 该函数仅能处理mod n^2(n^2=p^2*q^2)的情况，输入的数组是预计算的H数组
void BarrettReductionModn2(const BigNumber8192 A, BigNumber4096 q, BigNumber4096& r, string *n2_array){
    // 在这个函数中，获得的A就很大了，不能单独约简，只能考虑完整的Barrett算法实现了。
    const int w_1 = A.highestBit()+1;
    const int w_2 = q.highestBit()+1;
    // printf("w_1 = %d, w_2 = %d\n", w_1, w_2);

    // H = (1 << (w_1 + 1)) // q
    BigNumber8192 H;
    if(w_1 >= 4095)
        // 预计算了所有的H 
        H = BigNumber8192(n2_array[w_1 - 4095]);
    else
        H = BigNumber8192("0");
    // H.print(); // H是对的
    // exit(0);

    // q1 = A >> (w_2)
    BigNumber8192 A_tmp = A;
    BigNumber8192 q1 = (A_tmp >> w_2);
    // q1.print(); // q_1是对的
    // exit(0);

    // q2 = q1 * H
    // BigNumber8192 q2 = FNT::multiply(q1, H);
    // q2.print(); // q_2是对的
    // exit(0);

    // q3 = q2 >> (w_1 - w_2 + 1) 
    // BigNumber8192 q3 = q2 >> (w_1 - w_2 + 1);
    BigNumber8192 q3 = FNT::multiply(q1, H, w_1 - w_2 + 1);
    // printf("q2.overflow = %d\n", q2.overflow);
    // q_3 可能是4097位，这里能不能降级要画个问号
    // BigNumber4096 q3_4096 = q3.downgrade();
    // q3.print(); // q_3是对的s
    // exit(0);

    // r = A - q3 * q
    // BigNumber8192 r_8192 = A - FNT::multiply(q3_4096, q);
    BigNumber8192 r_8192 = A - FNT::multiply(q3, q.upgrade());
    // BigNumber8192 prod = FNT::multiply(q3, q.upgrade());
    // q.upgrade().print();
    // prod.print();
    // r_8192.print(); // r_8192是对的
    // A.print();
    // exit(0);

    // 理论上，此时的r应该在0到3q之间
    if(r_8192 > (q.upgrade() << 1)){
        r = (r_8192 - (q << 1).upgrade()).downgrade();
    }
    else if(r_8192 > q.upgrade()){
        r = (r_8192 - q.upgrade()).downgrade();
    }
    else
        r = r_8192.downgrade();

}

// 该函数仅能处理mod p或者mod q的情况，输入的数组是预计算的H数组
void BarretReductionModp(BigNumber2048 A, BigNumber1024 p, BigNumber1024& r, string* p_array){
    // 在这个函数中，获得的A就很大了，不能单独约简，只能考虑完整的Barrett算法实现了。
    const int w_1 = A.highestBit()+1;
    const int w_2 = p.highestBit()+1;
    // printf("w_1 = %d, w_2 = %d\n", w_1, w_2);

    // H = (1 << (w_1 + 1)) // q
    BigNumber2048 H;
    if(w_1 >= 1023)
        // 预计算了所有的H，最大的H有1026位
        H = BigNumber2048(p_array[w_1 - 1023]);
    else
        H = BigNumber2048("0");
    // H.print(); // H是对的
    // exit(0);

    // q1 = A >> (w_2)
    BigNumber2048 A_tmp = A;
    BigNumber2048 q1 = (A_tmp >> w_2);
    // q1.print(); // q_1是对的
    // exit(0);

    // q2 = q1 * H
    BigNumber4096 q2 = FNT::multiply(q1, H);
    // q2.print(); // q_2是对的
    // exit(0);

    // q3 = q2 >> (w_1 - w_2 + 1) 
    BigNumber4096 q3 = q2 >> (w_1 - w_2 + 1);
    // printf("q2.overflow = %d\n", q2.overflow);
    // q_3 可能是4097位，这里能不能降级要画个问号
    // BigNumber4096 q3_4096 = q3.downgrade();
    // q3.print(); // q_3是对的s
    // exit(0);

    // r = A - q3 * q
    // BigNumber8192 r_8192 = A - FNT::multiply(q3_4096, q);
    BigNumber2048 r_2048 = A - FNT::multiply(q3, p.upgrade().upgrade()).downgrade().downgrade();
    // BigNumber8192 prod = FNT::multiply(q3, q.upgrade());
    // q.upgrade().print();
    // prod.print();
    // r_8192.print(); // r_8192是对的
    // A.print();
    // exit(0);

    // 理论上，此时的r应该在0到3q之间
    if(r_2048 > (p.upgrade() << 1)){
        r = (r_2048 - (p << 1).upgrade()).downgrade();
    }
    else if(r_2048 > p.upgrade()){
        r = (r_2048 - p.upgrade()).downgrade();
    }
    else
        r = r_2048.downgrade();

}

// 重载上述函数
void BarretReductionModp(BigNumber4096 A, BigNumber1024 p, BigNumber1024& r, string* p_array){
    // 在这个函数中，获得的A就很大了，不能单独约简，只能考虑完整的Barrett算法实现了。
    const int w_1 = A.highestBit()+1;
    const int w_2 = p.highestBit()+1;
    // printf("w_1 = %d, w_2 = %d\n", w_1, w_2);

    // H = (1 << (w_1 + 1)) // q
    BigNumber2048 H;
    if(w_1 >= 1023)
        // 预计算了所有的H，最大的H有1026位
        H = BigNumber2048(p_array[w_1 - 1023]);
    else
        H = BigNumber2048("0");
    // H.print(); // H是对的
    // exit(0);

    // q1 = A >> (w_2)
    BigNumber4096 A_tmp = A;
    BigNumber4096 q1 = (A_tmp >> w_2);
    // q1.print(); // q_1是对的
    // exit(0);

    // q2 = q1 * H
    BigNumber8192 q2 = FNT::multiply(q1, H.upgrade());
    // q2.print(); // q_2是对的
    // exit(0);

    // q3 = q2 >> (w_1 - w_2 + 1) 
    BigNumber8192 q3 = q2 >> (w_1 - w_2 + 1);
    // printf("q2.overflow = %d\n", q2.overflow);
    // q_3 可能是4097位，这里能不能降级要画个问号
    // BigNumber4096 q3_4096 = q3.downgrade();
    // q3.print(); // q_3是对的s
    // exit(0);

    // r = A - q3 * q
    // BigNumber8192 r_8192 = A - FNT::multiply(q3_4096, q);
    BigNumber4096 r_4096 = A - FNT::multiply(q3, p.upgrade().upgrade().upgrade()).downgrade();
    // BigNumber8192 prod = FNT::multiply(q3, q.upgrade());
    // q.upgrade().print();
    // prod.print();
    // r_8192.print(); // r_8192是对的
    // A.print();
    // exit(0);

    // 理论上，此时的r应该在0到3q之间
    if(r_4096 > (p.upgrade().upgrade() << 1)){
        r = (r_4096 - (p << 1).upgrade().upgrade()).downgrade().downgrade();
    }
    else if(r_4096 > p.upgrade().upgrade()){
        r = (r_4096 - p.upgrade().upgrade()).downgrade().downgrade();
    }
    else
        r = r_4096.downgrade().downgrade();

}

// 该函数仅能处理mod p^2或者mod q^2的情况，输入的数组是预计算的H数组
void BarretReductionModp2(BigNumber8192 A, BigNumber2048 p2, BigNumber2048& r, string* p2_array){
     // 在这个函数中，获得的A就很大了，不能单独约简，只能考虑完整的Barrett算法实现了。
    const int w_1 = A.highestBit()+1;
    const int w_2 = p2.highestBit()+1;
    // printf("w_1 = %d, w_2 = %d\n", w_1, w_2);

    // H = (1 << (w_1 + 1)) // q
    BigNumber8192 H;
    if(w_1 >= 2047)
        // 预计算了所有的H H最大bit为2050
        H = BigNumber8192(p2_array[w_1 - 2047]);
    else
        H = BigNumber8192("0");
    // printf("%s", p2_array[w_1 - 4095].c_str());
    // H.printHexNumber("H"); // H是对的
    // exit(0);

    // q1 = A >> (w_2)
    BigNumber8192 A_tmp = A;
    BigNumber8192 q1 = (A_tmp >> w_2);
    // q1.printHexNumber("q1"); // q_1是对的
    // exit(0);

    // q2 = q1 * H
    // BigNumber8192 q2 = FNT::multiply(q1, H);
    // q2.print(); // q_2是对的
    // exit(0);

    // q3 = q2 >> (w_1 - w_2 + 1) 
    // BigNumber8192 q3 = q2 >> (w_1 - w_2 + 1);
    // BigNumber8192 q3 = FNT::multiply(q1, H, w_1 - w_2 + 1);
    BigNumber8192 q3 = FNT::multiply(q1, H, w_1 - w_2 + 1);
    // printf("q2.overflow = %d\n", q2.overflow);
    // q_3 可能是4097位，这里能不能降级要画个问号
    // BigNumber4096 q3_4096 = q3.downgrade();
    // q3.printHexNumber("q3"); // q_3是对的s
    // exit(0);

    // r = A - q3 * q
    // BigNumber8192 r_8192 = A - FNT::multiply(q3_4096, q);
    BigNumber8192 r_8192 = A - FNT::multiply(q3, p2.upgrade().upgrade());
    // BigNumber8192 prod = FNT::multiply(q3, q.upgrade());
    // q.upgrade().print();
    // prod.print();
    // r_8192.print(); // r_8192是对的
    // A.print();
    // exit(0);

    // 理论上，此时的r应该在0到3q之间
    if(r_8192 > (p2.upgrade() << 1).upgrade()){
        r = (r_8192 - (p2 << 1).upgrade().upgrade()).downgrade().downgrade();
    }
    else if(r_8192 > p2.upgrade().upgrade()){
        r = (r_8192 - p2.upgrade().upgrade()).downgrade().downgrade();
    }
    else
        r = r_8192.downgrade().downgrade();
}

// 基于Barrett约简的返回商
// Result = A // p
void BarretBasedDivide(const BigNumber2048 A, const BigNumber1024 p, BigNumber2048 &Result, string* p_array){
    // 在这个函数中，获得的A就很大了，不能单独约简，只能考虑完整的Barrett算法实现了。
    const int w_1 = A.highestBit()+1;
    const int w_2 = p.highestBit()+1;
    // printf("w_1 = %d, w_2 = %d\n", w_1, w_2);
    // A.printHexNumber("A");
    // p.printHexNumber("p");

    // H = (1 << (w_1 + 1)) // q
    BigNumber2048 H;
    if(w_1 >= 1023)
        // 预计算了所有的H，最大的H有1026位
        H = BigNumber2048(p_array[w_1 - 1023]);
    else
        H = BigNumber2048("0");
    // H.printHexNumber("H"); // H是对的
    // exit(0);

    // q1 = A >> (w_2)
    BigNumber2048 A_tmp = A;
    BigNumber2048 q1 = (A_tmp >> w_2);
    // q1.print(); // q_1是对的
    // exit(0);

    // q2 = q1 * H
    BigNumber4096 q2 = FNT::multiply(q1, H);
    // q2.print(); // q_2是对的
    // exit(0);

    // q3 = q2 >> (w_1 - w_2 + 1) 
    BigNumber4096 q3 = q2 >> (w_1 - w_2 + 1);
    // printf("q2.overflow = %d\n", q2.overflow);
    // q_3 可能是4097位，这里能不能降级要画个问号
    // BigNumber4096 q3_4096 = q3.downgrade();
    // q3.printHexNumber("q3"); // q_3是对的
    // exit(0);

    // r = A - q3 * q
    // BigNumber8192 r_8192 = A - FNT::multiply(q3_4096, q);
    BigNumber8192 q_8192 = FNT::multiply(q3, p.upgrade().upgrade());
    // q_8192.printHexNumber("q_8192");
    // BigNumber2048 q_2048 = ;
    BigNumber2048 r_2048 = A - q_8192.downgrade().downgrade();
    // r_2048.printHexNumber("r_2048");
    // BigNumber8192 prod = FNT::multiply(q3, q.upgrade());
    // q.upgrade().print();
    // prod.print();
    // r_8192.print(); // r_8192是对的
    // A.print();
    // exit(0);

    // 理论上，此时的r应该在0到3q之间
    if(r_2048 >= (p.upgrade() << 1)){
        // r = (r_2048 - (p << 1).upgrade()).downgrade();
        Result = q3.downgrade() + BigNumber2048("2");
    }
    else if(r_2048 >= p.upgrade()){
        // r = (r_2048 - p.upgrade()).downgrade();
        Result = q3.downgrade() + BigNumber2048("1");
    }
    else
        // r = r_2048.downgrade();
        Result = q3.downgrade();

}



// 计算 Result = x^n mod p
// 仅能处理mod p= mod (n^2)的情况 不能改变
void FastExp(const BigNumber2048 x, const BigNumber2048 n, const BigNumber4096 p, BigNumber4096 &Result){
    // x = BarrettReduction(x, p);

    // 这里的p是n^2，是一个很大的数，十分接近4096的极限，那么这里可以默认x<p
    Result = BigNumber4096("1");
    BigNumber4096 x_tmp = x.upgrade();
    BarrettReductionFastExp1(x_tmp, p, x_tmp);
    BigNumber2048 n_tmp = n;
    BigNumber8192 result_tmp = Result.upgrade();
    // int count = 0;

    while(n_tmp.highestBit()+1 > 0){
        // printf("n_tmp.highestBit() = %d\n", n_tmp.highestBit());
        // printf("n_tmp.data[0] = %d\n", n_tmp.data[0]);
        if(n_tmp.data[0] & 1){
            result_tmp = FNT::multiply(result_tmp.downgrade(), x_tmp);
            BarrettReductionModn2(result_tmp, p, Result, N2_ARRAY);
            result_tmp = Result.upgrade();
        }
        n_tmp = n_tmp >> 1;
        BigNumber8192 x_tmp_2 = FNT::multiply(x_tmp, x_tmp);
        BarrettReductionModn2(x_tmp_2, p, x_tmp, N2_ARRAY);
        // count++;
        // if(count > 2041){
        //     printf("count = %d\n", count);
        //     // x_tmp.print();
        //     Result.print();
        //     cout << endl;
        // }
    }

    // Result = result_tmp.downgrade();

}

// 仅能处理mod p= mod (n^2)的情况 不能改变
void FastExp(const BigNumber4096 x, const BigNumber2048 n, const BigNumber4096 p, BigNumber4096 &Result){
    // x = BarrettReduction(x, p);

    // 这里的p是n^2，是一个很大的数，十分接近4096的极限，那么这里可以默认x<p
    Result = BigNumber4096("1");
    BigNumber4096 x_tmp = x;
    BarrettReductionFastExp1(x_tmp, p, x_tmp);
    BigNumber2048 n_tmp = n;
    BigNumber8192 result_tmp = Result.upgrade();
    // int count = 0;

    while(n_tmp.highestBit()+1 > 0){
        // printf("n_tmp.highestBit() = %d\n", n_tmp.highestBit());
        // printf("n_tmp.data[0] = %d\n", n_tmp.data[0]);
        if(n_tmp.data[0] & 1){
            result_tmp = FNT::multiply(result_tmp.downgrade(), x_tmp);
            BarrettReductionModn2(result_tmp, p, Result, N2_ARRAY);
            result_tmp = Result.upgrade();
        }
        n_tmp = n_tmp >> 1;
        BigNumber8192 x_tmp_2 = FNT::multiply(x_tmp, x_tmp);
        BarrettReductionModn2(x_tmp_2, p, x_tmp, N2_ARRAY);
        // count++;
        // if(count > 2041){
        //     printf("count = %d\n", count);
        //     // x_tmp.print();
        //     Result.print();
        //     cout << endl;
        // }
    }
}

// 在这里我们预计，p和q都会占满1024bit
// 这里的p可以改为题目中的p或者q
// 仅能处理mod p 或者 mod q的情况 不能改变
void FastExp(const BigNumber1024 x, const BigNumber1024 n, const BigNumber1024 p, BigNumber1024 &Result, string* p_array){
    // x = BarrettReduction(x, p);

    Result = BigNumber1024("1");
    BigNumber1024 x_tmp = x;
    // x对p求余
    // q mod (p-2)
    // 为了防止使用孪生质数，我还是
    if(x_tmp >= p){
        x_tmp = x_tmp - p;
    }
    BigNumber1024 n_tmp = n;
    BigNumber2048 result_tmp = Result.upgrade();
    // int count = 0;

    while(n_tmp.highestBit()+1 > 0){
        // printf("n_tmp.highestBit() = %d\n", n_tmp.highestBit());
        // printf("n_tmp.data[0] = %d\n", n_tmp.data[0]);
        if(n_tmp.data[0] & 1){
            result_tmp = FNT::multiply(result_tmp.downgrade(), x_tmp);
            // Result = result_tmp mod p
            BarretReductionModp(result_tmp, p, Result, p_array);
            result_tmp = Result.upgrade();
        }
        n_tmp = n_tmp >> 1;
        BigNumber2048 x_tmp_2 = FNT::multiply(x_tmp, x_tmp);
        BarretReductionModp(x_tmp_2, p, x_tmp, p_array);
        // count++;
        // if(count > 2041){
        //     printf("count = %d\n", count);
        //     // x_tmp.print();
        //     Result.print();
        //     cout << endl;
        // }
    }
}

// p=p^2 不能改变
void FastExp(const BigNumber4096 x, const BigNumber1024 n, const BigNumber2048 p, BigNumber2048 &Result, string *p2_array){
    // x = BarrettReduction(x, p);

    // x.printHexNumber();
    // n.printHexNumber();

    Result = BigNumber2048("1");
    BigNumber4096 x_tmp = x;
    // BarrettReductionFastExp1(x_tmp, p.upgrade(), x_tmp);
    BigNumber2048 x_tmp_2048;
    BarretReductionModp2(x_tmp.upgrade(), p, x_tmp_2048, p2_array);
    x_tmp = x_tmp_2048.upgrade();
    // x_tmp.printHexNumber();
    BigNumber1024 n_tmp = n;
    BigNumber8192 result_tmp = Result.upgrade().upgrade();
    // int count = 0;

    while(n_tmp.highestBit()+1 > 0){
        // printf("n_tmp.highestBit() = %d\n", n_tmp.highestBit());
        // printf("n_tmp.data[0] = %d\n", n_tmp.data[0]);
        if(n_tmp.data[0] & 1){
            result_tmp = FNT::multiply(result_tmp.downgrade(), x_tmp);
            BarretReductionModp2(result_tmp, p, Result, p2_array);
            result_tmp = Result.upgrade().upgrade();
        }
        n_tmp = n_tmp >> 1;
        BigNumber8192 x_tmp_2 = FNT::multiply(x_tmp, x_tmp);
        BigNumber2048 x_tmp_2048;
        // x_tmp_2.printHexNumber();
        BarretReductionModp2(x_tmp_2, p, x_tmp_2048, p2_array);
        x_tmp = x_tmp_2048.upgrade();
        // count++;
        // if(count > 2041){
        //     printf("count = %d\n", count);
            // x_tmp.printHexNumber("x_tmp");
        //     Result.print();
        //     cout << endl;
        // }
    }
}



// 计算 h = -x^2 mod n
void calculate_h(BigNumber1024 x, BigNumber2048 n, BigNumber2048& h){
    // BigNumber2048 x_square = FNT::multiply(x, x);
    // BarrettReduction(x_square, n, h);
    // h = h + n;

    // 在考虑选取的模数，即n(2048位)，如果此时的n足够大，使得2n都会超过4096位。
    BigNumber2048 x_square = FNT::multiply(x, x);
    // 如果此时的x_square < n，那么h = n - x_square即可
    if(x_square < n){
        h = n - x_square;
    }
    // 如果此时的x_square >= n，那么h = 2n - x_square即可
    else{
        x_square = x_square - n;
        h = n - x_square;
    }
}

// 计算 h_s = h ^ n (mod n^2)
// 注意，由于在BarrettReductionModn2中预计算了所有的H，导致这里的n是固定的不可随便改变的。
void calculate_hs(const BigNumber2048 h, const BigNumber2048 n, BigNumber4096& h_s){
    BigNumber4096 n_square = FNT::multiply(n, n);
    FastExp(h, n, n_square, h_s);
}

// 计算 c = (mn+1)h_s^\alpha mod n^2
// c 要赋初值1
void calculate_c(const BigNumber2048 m, const BigNumber2048 n, const BigNumber4096 h_s, const BigNumber1024 alpha, BigNumber4096& c){
    BigNumber4096 mn = FNT::multiply(m, n);
    BigNumber4096 n_square = FNT::multiply(n, n);
    BarrettReductionFastExp1(mn, n_square, mn);
    BigNumber4096 mn_1 = mn + BigNumber4096("1");
    // n_square.print();
    // exit(0);
    
    // BigNumber8192 mn_1_h_s = FNT::multiply(mn_1, h_s);
    BigNumber4096 mn_1_4096;
    // BarrettReductionModn2(mn_1.upgrade(), n_square, mn_1);
    BarrettReductionFastExp1(mn_1, n_square, mn_1_4096);
    FastExp(h_s, alpha.upgrade(), n_square, c);
    BigNumber8192 c_tmp = FNT::multiply(c, mn_1_4096);
    BarrettReductionModn2(c_tmp, n_square, c, N2_ARRAY);
}

// m：明文 n：公钥 alpha：私钥 x：随机数 c：密文
void encrypt(const BigNumber2048 m, const BigNumber2048 n, const BigNumber1024 alpha, const BigNumber1024 x, BigNumber4096& c){
    BigNumber2048 h;
    calculate_h(x, n, h);
    BigNumber4096 h_s;
    calculate_hs(h, n, h_s);
    calculate_c(m, n, h_s, alpha, c);
}




// 返回L(x,p)= (x-1) // p
void L(const BigNumber2048 x, const BigNumber1024 p, BigNumber2048& result, string* p_array){
    BigNumber2048 x_1 = x - BigNumber2048("1");
    // BigNumber2048 result;
    BarretBasedDivide(x_1, p, result, p_array);
    // result.printHexNumber("result");
}

// hp = (p-1)*q^-1 mod p
// mp = L(c^(p-1) mod p^2, p)*hp mod p
void calculate_mp(const BigNumber4096 c, const BigNumber1024 p, const BigNumber1024 q_inv, BigNumber1024& mp, string* p_array, string *p2_array){
    // BigNumber1024 q_inv = BigNumber1024(q_inv_string);
    // p.printHexNumber("p");
    BigNumber2048 h_p = FNT::multiply(p - BigNumber1024("1"), q_inv);
    // q_inv.printHexNumber("q_inv");
    // h_p.printHexNumber("h_q");
    BigNumber1024 h_p_modp;
    BarretReductionModp(h_p, p, h_p_modp, p_array);
    // h_p_modp.printHexNumber("h_p_modp");
    
    // mp = L(c^(p-1) mod p^2, p)*hp mod p

    // 计算c^(p-1) mod p^2
    BigNumber2048 p_2 = FNT::multiply(p, p);
    BigNumber2048 c_p_1;
    // 注意这里是mod p^2
    FastExp(c, p - BigNumber1024("1"), p_2, c_p_1, p2_array);
    // c_p_1.printHexNumber("c_p_1");

    // 计算L(c^(p-1) mod p^2, p)
    BigNumber2048 L_result;
    // 注意这里是mod p
    L(c_p_1, p, L_result, p_array);
    // L_result.printHexNumber("L_result");

    // 计算最后一步
    BigNumber4096 mp_4096 = FNT::multiply(L_result, h_p_modp.upgrade());
    BarretReductionModp(mp_4096, p, mp, p_array);
    // mp_4096.printHexNumber("mp_4096");
}

// 计算u：u = (mq-mp)*p^-1 mod q
void calculate_u(const BigNumber1024 mp, const BigNumber1024 mq, const BigNumber1024 p, const BigNumber1024 q, BigNumber1024& u){
    // u = (mq-mp)*p^-1 mod q
    BigNumber1024 p_inv = BigNumber1024(p_inv_string);
    BigNumber1024 mq_mp;
    // 为了防止负数
    if(mq >= mp){
        mq_mp = mq - mp;
    }
    else{
        mq_mp = mq + q - mp;
    
    }
    // mq_mp.printHexNumber("mq_mp");
    BigNumber2048 u_2048 = FNT::multiply(mq_mp, p_inv);
    BarretReductionModp(u_2048, q, u, Q_ARRAY);
}

void decrypt(const BigNumber4096 c, const BigNumber1024 p, const BigNumber1024 q, BigNumber2048& m){

    // 计算mp
    BigNumber1024 mp;
    BigNumber1024 q_inv = BigNumber1024(q_inv_string);
    calculate_mp(c, p, q_inv, mp, P_ARRAY, P2_ARRAY);
    // mp.printHexNumber("mp");

    // 计算mq
    BigNumber1024 mq;
    BigNumber1024 p_inv = BigNumber1024(p_inv_string);
    calculate_mp(c, q, p_inv, mq, Q_ARRAY, Q2_ARRAY);
    // mq.printHexNumber("mq");

    // 计算u：u = (mq-mp)*p^-1 mod q
    BigNumber1024 u;
    calculate_u(mp, mq, p, q, u);
    // u.printHexNumber("u");

    // 解密：m = mp + u * p
    m = mp.upgrade() + FNT::multiply(u, p);

}




void test_crt(){
    // n = p * q
    BigNumber2048 n = BigNumber2048 ("eeabfbeef45ea5a122be4ea1b5b362bd3f2a3cb2963ecd606ac37be8d6fcc3ae693cd857016e638b9cfb7b386c96739bbaf2dfc1e41eb89cb62df87f16d585965276233f9ed862703ccf0126c51ba9e492644519159d174385f839ec87ff09ee5594e71744f737cffdd6bce67651c5361973c47281cd5b13c64bc285f0113da9e7952a556c0b6ee1fd99b7a04f1fc59b67d42530a9227ce9ab65fddb9f55ea672dbd53be9554ff2f8926e8ca15c3c6c9dc480893a916b5b52a3ca734a6ee0f59ba51ff68cbe2776bf9a473cc360954866fd024ef836f06038438ecf3798e05fcd438e4bb1b3b043258799e042406d61ffeb76ebd44b8bba3b058f9d44772f34d");

    // 生成随机数 x
    // BigNumber1024 x = BigNumber1024("BCCBB0B25867B220B49753D28AA8BE768B294EA0371255EDD33625758DC7BF83132635F6AF1C7325659FBAFE0DAEA2AF96874176FA907E88964FD72BC7550A3D333034E364A63A2998D9847DE9869BB1C2E5B7DAFE311BD63191453A38CC58EF9606D11E458248427B047FEB8588E1D1C362E4E603DF4941C1D7DDEEF0CAB983");
    BigNumber1024 x = BigNumber1024("DD39D0C647FE578D1BEAE7DC5521BD57C8CA123AA59A0367ED933C986F7E33BBEC12C5493EA3815872C96DA98EF774A7BF9494E42D0F0EC07E795314E83AEEFC56D3A8C7B0479CD9C5C5019D1D3C7834A37C0F0FC66582446401ADFD41ACC8DFE5735C777D740028415E2B195E8522389A35DFF4DB685D1C1B28E180A90EA38D");
    

    // 生成随机数 alpha
    BigNumber1024 alpha = BigNumber1024("8b0ce44007393957baba64f3fd3f797a112cb7e22df1b6826e95a5ab4b27fa566cbd0c8d3b6ce7d0e0c8cd85a1c588117cb042eb5c3245e1e0f78aa6180c50fb");

    // 明文m
    // BigNumber2048 m = BigNumber2048("1A8D62D4369C8BF1586CC2FE8F89CFD5D9D650E950E1D06CEDB748E638247C7740548A8455A8F4DBF2E64232429150A5A1D77881A3A53C48B60C02E65251AA89E991FDD0FEFDF9AEFD01A46E7AE4318907AE723C5300ED75B63F2C4617E9ADE73FBB7A8D65895E7DCE39D4F74DB08F6E5FD28BFE06DF834B5A03EFA5DE926360023B794FF776B9B2C87A73B659CDE80E0E2DD6337161CE629ABE6F0DBD522093ED06CD6494A39FD89A1220814D1B2B762F6F2894E7CCA08DC68FF23C2838EABD5D899B9C26AB5C85324A0547AC33BD5B7A56C46AD1F6D14F3B577FD9C86AA0170FDBE5C385E1C09BDC491A9D7EFE41E6308E803D49205F0E0CFC51B615E535E6");
    BigNumber2048 m = BigNumber2048("7F23187568DCF8110DF3F22DCAA767A928B990262E4C8CAE232A0300CEBACFF2E967255D76BD14392D36F4B304928302A1F430F00931B20F621DFEAFCFA188819340CBEA4823AAD26BA601235DCB75BF965B2A092A36151491B966ECB7956AD35E291BCD394701F051D5C0637A9519A56604BE8FBB30B15AAFB68568D61B9AA4E070683105F0B47E2C8E3337A02CD9A1169B808C0036C7C8E1E177FA160D9B41C8A5540545387D9587AA76CF7A7F31A43B00C04DC9E0F82CB20A69D8A2EB641956C510C6F74A65D20EBBDBA8BC67B3FCB80DBA3D3AB7A691A799166CF844D56DC40E24F28FCD96BB10B304AD6B99E23D16843091BFE8A7E2FD62A55BB56D2288");
    

    // 密文c
    BigNumber4096 c = BigNumber4096("1");

    // 开始加密
    BigNumber2048 h;
    calculate_h(x, n, h);
    // h.print();
    // exit(0);

    BigNumber4096 h_s = BigNumber4096("1");
    calculate_hs(h, n, h_s);
    // h_s.print();
    // exit(0);

    calculate_c(m, n, h_s, alpha, c);
    // c.print();
    c.printHexNumber("c");
}

void test_decrt(){
    // BigNumber4096 c = BigNumber4096("91782944f645cf148573941f733ed5b6bc4c994c2bea1d938f4cb9868d54e4741a496f9fa31027d57802549bbe43a9b008bdd55732edd2c3b79769ca5255fc5fa943e047195b07c2f392dddeb184a07a5eee64cd8951c998c074c7c6b8b23075f17c6c9e2cbf3a9c0c3e99290db0d7fe28d83ec00c719a81d1413801950d9d4c24b5f71633c4dc5d8fdcd6742285bace256c641a17aa2aaa3bc12e972c945778a1c56cd45951f8f5dd9677e12e7d8ae42726e5788cb17ea9bcc5bf28751b64dfb7ff61c9ed2d0bc0e8a8c9b1e6c0479e7d95cbeebe7781e5eef1eadba409afdbde2c9ef4afe900d4323b16c57c32ab74c9fe1085781706bf0c526f1e277467a78cec02ae56df65dc9bee2d685524148625e9f11b34a08bea432815a437648a03bc5550f5990f5cee2ae139372bdaab85cda765739d8a74bb20b20426dc50afc48f2bd12ef5dc6668cf13dcf90417dfc6f65ab918950b441ee594299c58a717606986ee7aa466a5aca5118ead3b90bebe52c5e3381b5e50baaeff3193d46049d0b296809132b03b3f17c6741ae12c28771aa7c4a7a60281528b8308046a80380aef91a9323c6737183dd60fc7ddc7ee08aa580318573498e992391d5f84e0978dc33dff7726ee0b88dfe812d69022c8e136671255a1c504c94c87320c7ca05c27635e1d2b3d06a5d62961b81ee642b169e0253eba216e1c88d94ac30790ccbc36");
    // BigNumber4096 c = BigNumber4096("53f63f043f28d09d35e463d68c2983424df972ba49e59700e86fb34db11bd322ee1f09f9b8453142d589b9dcd4e5d3c84bf8a94f27fe3f38f2d3d61cdffd158aed91c85a6747ac3595f578355846708ef4a567e6348037818affef8b0b3f4eff28666d59e66d337f0657d41a9813fb5f2cd5f894a701d8861c056fa29c2be2d043d882d0439ca88914a0c4032dfe311e36a76085c3a587e194614d4abca4de17cad363a93a2e67a93805c25d9e66a7105ee67cf4b11688e339a78d4f8db218af778a6b53140b4e01b0da1ba29eea2c22f29250f67f5d8fd4d43dca1c2b368009e73a1c7e2b18991b2945778eacddd43e48aa76fd813d01488b96c432bdd0123cb928201f4060175fb5af472e01f25fd0f01c17bbc0cb639139138b8e6dbc57c22c460271452f2bcd3a1d655ba90ed93ee313e6f92919a20f43ceb556e6ef7581cf2faa5740847d932b91eab05e3b152e202aecd2af24a122644cd83763a8d23e5b03581ad89515eb13b6b2a1ac8e4801140ea3edc5e012726ad2f43f09c9f18d8311d4e79fcd845b47a606608cc252f82be04cc3e3110a2839cbbf7a35703c96c2afd64460e56ff3f22516f7e13d027f874eda022d2b19cdcb8647d8d70a92ed2d5476c9aca7c74f9501a4f0785f35a3462043490ac2bcaf2a14b61276c059650f3a623a6e6a818416850703c512fd19a8e13681b542fbb4cb1f8e731e09f216");
    BigNumber4096 c = BigNumber4096("6d817600ff8e728784cc38130f2fe2f82955fd5f444ab04e0d4ee463e11dadf93706a241ff9dd5fa130dd53f1d8aa181952058a352916f0152a91a194ece88f87633afcdf027d3f3e73fee61a815899ce7aae6c3bb8da64d7197495286dab5db5795a1fb43e86ec22c5dec18365956df8ed72c0886dd7490255ee35432669d5f16edef1fb35587e1959707bfaef0ba6e5192fd562f6ad615989eaa285695def3bade64e2e077ecf76df848fc589d5564db2082784eef750d904a456690cdcfda0660c15323cc49b6c3f0ec50bf87f59a8a6c8fadc42635445b3f1037ee3f3af087667f31679b27d8a6825ed607382d6558ea8fab01106520a2c32e78169ec0970b2f0fc8c67f0d5e9a88a83f0d44b9220c3b4f021b19cec91b4ea17c283e3564c8b5d80990ecb6fe533b363587f87dc556cbb2e53928726b1b43eba86ed7a8e216ac31d418d9bee0714be3f7849202310afea20100042ffdb8bee7c6c1dfbdeb96e970178dadb08effa62038cf4e014ed745ba1bbe685bcad5488aaf6f339a500657ac0422f8261e955086e620e14150c6ecce4b14585f31cca18d58993e5315dca847b9854709099a81dad2de058295fe069fba0f8c1e07f5ede0cbdc33cbbd62171383523e10c18f5d90a2d3d1a9cbaf5d8d0344646c296e3199d45373d03e10895795462369957a155528c312a64bad326bbadb9d2bc1bbe121575d70f33a");
    

    BigNumber1024 p = BigNumber1024("fc2ffe76bf35218c3cf386fd77d3fd46d760db0e5135dfa0c59932e9b4fbb0f3918363690616084105e6d7994d29916ddc1b841366cef4bc9d90bc9075d86e577bb6a34bbb7aec8813d5b78e124b8e452d811b2ed461c741055e59ad1c27f4d08f55ab1197604a01589fb1e094b4fd7d61c68fffbdd709721c7ab559abc1cc01");

    BigNumber1024 q = BigNumber1024("f247aead159e22ff932390894c285843fba88a59879218b0365f5f5d212196ae4f6539f174a8c14d6887dc554d2efe5b9472afaac18ef56b5b0e800539216bbc3b4b7f03a6b79750501c05f5294825d4290ba7249106428433e0bb554306b63d199add5a2409f96c6b2592d701b630dfc939828bc64bb005a7df10275ed4974d");

    BigNumber2048 m = BigNumber2048("1");

    decrypt(c, p, q, m);
    // m.print();
    m.printHexNumber("m");
}

int main(){

    // test_Barret_2();
    // test_crt();
    test_decrt();
    // testModp2();

    return 0;
}
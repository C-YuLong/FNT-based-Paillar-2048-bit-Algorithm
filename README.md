# C++ Implementation of Paillier Encryption Based on Custom Big Number Class

v0.3





## Preliminary Explanation

This project is based on fixed values of $p$ and $q$ for computation.
$$
\begin{aligned}
p &= 177092068480735400854085936532566857106120280764577509765803339901544135759472066666117863886710233937613992438912318775026915931500403241175346661418233082352571559981771026111674521592470172566750802301187337513462870505971987298390405825403438064218796956493360970377443003039167540306288241695473000369153 \\
(p &= 0xfc2ffe76bf35218c3cf386fd77d3fd46d760db0e5135dfa0c59932e9b4fbb0f3918363690616084105e6d7994d29916ddc1b841366cef4bc9d90bc9075d86e577bb6a34bbb7aec8813d5b78e124b8e452d811b2ed461c741055e59ad1c27f4d08f55ab1197604a01589fb1e094b4fd7d61c68fffbdd709721c7ab559abc1cc01) \\
q &= 170134808228960721208095350458107390109573733329021377280204452375403114047025850581240057108821671754959765367486404349420445330219950136615430934439135100382845778661809332290874928032539319787311155164043376696534166912622062275544869888915196051820520420546846996578231633452059952729007671253273167566669 \\
(q &= 0xf247aead159e22ff932390894c285843fba88a59879218b0365f5f5d212196ae4f6539f174a8c14d6887dc554d2efe5b9472afaac18ef56b5b0e800539216bbc3b4b7f03a6b79750501c05f5294825d4290ba7249106428433e0bb554306b63d199add5a2409f96c6b2592d701b630dfc939828bc64bb005a7df10275ed4974d) \\
n &= p \times q
\end{aligned}
$$





## File Structure:

1. Definition files for big number classes:

`BigNumber.hpp`, `BigNumber1024.hpp`, `BigNumber2048.hpp`, `BigNumber4096.hpp`, `BigNumber8192.hpp`

2. Precomputed arrays based on fixed $n$:

`N2_array.hpp`

3. Main file header (Fast Number Theoretic Transform):

`Paillar8.hpp`

4. Main file:

`Paillar8.cpp`





## Architecture Explanation:

1. **Big Number Class**

   (Note: Please refer to the code and comments in the code)

   The big number class implements large integers with bit widths of 1024, 2048, 4096, and 8192 using an array of `uint8_t`.

   The classes are named `BigNumber1024`, `BigNumber2048`, `BigNumber4096`, and `BigNumber8192`, all based on a common parent class `BigNumber`.

   Basic operations are implemented within the class.

   Note: To avoid implementing larger numbers, an `overflow` bit is added to the 8192-bit class to prevent overflow. Overflow beyond a single byte is not considered (treated as failure).

   Below is the explanation of **member functions**. Note: Not all classes implement all the member functions. This list contains a union of all the functions.

   | Member Function Name | Purpose                | Input                      | Output                 | Remarks                    |
   | -------------------- | ---------------------- | -------------------------- | ---------------------- | -------------------------- |
   | highestBit           | The highest bit -1     | None                       | int                    |                            |
   | upgrade              | Enlarge the number     | None                       | Bigger type of Number  | not with 8192              |
   | downgrade            | Lower the number       | None                       | Smaller type of number | not with 1024              |
   | <<                   | Left Shift             | int                        | same type of numbers   |                            |
   | >>                   | Right Shift            | int                        | same type of numbers   |                            |
   | +                    | addition               | same type of numbers       | same type of numbers   | overflow is not considered |
   | -                    | minus                  | same type of numbers       | same type of numbers   | No negative numbers        |
   | <                    | less than              | same type of numbers       | same type of numbers   |                            |
   | <=                   | less than or equal     | same type of numbers       | same type of numbers   |                            |
   | >                    | greater than           | same type of numbers       | same type of numbers   |                            |
   | >=                   | greater than or equal  | same type of numbers       | same type of numbers   |                            |
   | =                    | assign value           | same type of numbers       | same type of numbers   |                            |
   | print                | print as string        | None                       | terminal out           |                            |
   | printHexNumber       | print as string in HEX | debug info                 | terminal out           |                            |
   | BigNumberXXXX        | construction function  | HEX string                 |                        | **No "0x" contained**      |
   |                      | construction function  | uint8_t  array and size    |                        | **size should be precise** |
   | BigNumber8192        | construction function  | uint8_t array and overflow |                        | Only 8192 has              |

   Below are the information of the **members**:

   | Name     | Functions                           | type      |
   | -------- | ----------------------------------- | --------- |
   | data     | The storing array of the BigNumbers | uint8_t   |
   | size     | The size of the mentioned array     | const int |
   | overflow | The overflow in 8192 calculations   | uint8_t   |

   

2. **Precomputed Array `N2_array` Based on Fixed $n$:**

   This precomputed array is used for the Barrett reduction, where the value of H is calculated as follows:
   $$
   H = (1 << (w_1 + 1)) // n^2)
   $$


   Here, `w_1` is enumerated from $4095$ to $8192$ (inclusive), and all corresponding values of H are obtained.

   To access the value of `H` corresponding to `w_1`, use `N2_array[w_1 - 4095]`.

   

3. **Fast Number Theoretic Transform (NTT):**

   The specific implementation is based on the static class `FNT` (no need to construct members).

   The basic concept of the Fast Number Theoretic Transform is not introduced here; the fixed parameters used are given below.

   ```c++
   static const long p = 469762049;
   static const int g = 3;
   
   static const int omega_256 = 338628632;
   static const int omega_512 = 25153357;
   static const int omega_1024 = 110059487;
   static const int omega_2048 = 165447688;
   
   static const int omega_inv_256 = 215855482;
   static const int omega_inv_512 = 436579181;
   static const int omega_inv_1024 = 458753944;
   static const int omega_inv_2048 = 63413564;
   
   static const int N256_inv = 467927041;
   static const int N512_inv = 468844545;
   static const int N1024_inv = 469303297;
   static const int N2048_inv = 469532673;
   ```

   Reason for choosing this prime:

   When considering multiplying 8192-bit by 8192-bit, it essentially involves converting to a polynomial with base 256 and then performing polynomial convolution. The maximum value during the convolution process should be $(2^7 - 1)^2 * 2048 \approx 2^{27}$. Therefore, the chosen $p$ should be larger than this value to prevent coefficient changes when performing $mod \ p$ operations. Additionally, the maximum value of $p$ should not exceed $2^{31}$, as this could lead to the need for larger variable types and potential overflow issues.

   Next, we explain the implementation principle:

   For multiplying large numbers of length N, a $2*N$-point NTT is used, with zero-padding at the end.

   The table of member functions is provided below:

   | Member Function  | Description                             | Input                 | Output                | Remarks                          |
   | ---------------- | --------------------------------------- | --------------------- | --------------------- | -------------------------------- |
   | mod_exp          | Fast exponentiation for small numbers   |                       |                       | Internal call                    |
   | bit_reverse      | Bit reversal                            |                       |                       | Internal call                    |
   | bit_reverse_copy | Butterfly transform position lookup     |                       |                       | Internal call                    |
   | bit_reverse_copy | Overload of the above function          |                       |                       | Internal call                    |
   | fnt_iterative    | Fast Number Theoretic Transform         |                       |                       | Internal call                    |
   | ifnt_iterative   | Inverse Fast Number Theoretic Transform |                       |                       | Internal call                    |
   | processArray     | Carry function with T as the carry      |                       |                       | Internal call                    |
   | multiply         | 2048 = 1024 * 1024                      | 1024 big number class | 2048 big number class |                                  |
   | multiply         | 2048 * 2048 = 2048                      | 2048 big number class | 2048 big number class |                                  |
   | multiply         | 4096 * 4096 = 8192                      | 2048 big number class | 8192 big number class |                                  |
   | multiply         | 8192 * 8192 = 8192                      | 8192 big number class | 8192 big number class | Allows overflow of only one byte |
   | multiply         | 8192 * 8192 = 8192, with shifts         | 8192 big number class | 8192 big number class | Shifts allowed after computation |



4. **Main File:**

   The main file implements the encryption process based on a fixed number. Below is an explanation of the implemented functions:

   | Function Name            | Functionality                                                | Remarks                                                      |
   | ------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
   | BarrettReductionFastExp1 | r = A % q                                                    | Optimization for Barrett reduction: considering very large $q$ (where $2q$ would overflow), only a single subtraction is performed after a check. Please refer to the code directly for further details. |
   | BarrettReductionModn2    | $r = A \% n^2$                                               | Modulus calculation specifically for $n^2$                   |
   | FastExp                  | $Result = x^n \mod p$                                        | Fast exponentiation algorithm. $n$ cannot be modified since it calls the above Barrett reduction algorithm. |
   | FastExp                  | $Result = x^n \mod p$                                        | Overloaded version for different bit-widths                  |
   | calculate_h              | $h = -x^2 \mod n$                                            | First step in the encryption process                         |
   | calculate_hs             | $h_s = h^n \mod n^2$                                         | Second step in the encryption process                        |
   | calculate_c              | $c = (1 + mn)h_s^{lpha} \mod n^2$                            | Third step in the encryption process                         |
   | BarrettReductionModp     | $r = A \mod p$                                               | Modulus calculation specifically for $p$                     |
   | BarretReductionModp2     | $r = A \mod p^2$                                             | Modulus calculation specifically for $p^2$                   |
   | BarretBasedDivide        | $Result = A // p$                                            | Division operation using Barrett reduction                   |
   | caculate_mp              | $hp = (p-1)*q^{-1} \mod p$ \ $mp = L(c^{p-1} \mod p^2, p)*hp \mod p$ | Calculation of $mp$ in the decryption process                |
   | calculate_u              | $u = (mq - mp)*p^{-1} \mod q$                                | Calculation of $u$ in the decryption process                 |
   | encrypt                  | Computes ciphertext $c$ from plaintext $m$                   | Encryption function                                          |
   | decrypt                  | Decrypts ciphertext $c$ to retrieve plaintext $m$            | Decryption function                                          |





## Explanation of Variables in the Algorithm

| Variable Name | Meaning                            | Bit Width |
| ------------- | ---------------------------------- | --------- |
| n             | $n=pq$                             | 2048      |
| p             | Prime number                       | 1024      |
| q             | Prime number                       | 1024      |
| n_square      | $n^2$                              | 4096      |
| x             | $x = randint(2, min(p, q))$        | 1024      |
| h             | $h = -x^2 \mod n$                  | 2048      |
| $h_s$         | $h_s = h^n \mod n^2$               | 4096      |
| c             | Ciphertext                         | 4096      |
| $\alpha$      | $randint(2, 2^{ceil(key_size/2)})$ | 1024      |
| m             | Plaintext                          | 2048      |





# Python Tool: Generate Header Files

Given prime numbers $p$ and $q$, calculate precomputed header files in Barrett reduction.

`P_ARRAY.hpp`, `Q_ARRAY.hpp`, `P2_ARRAY.hpp`, `Q2_ARRAY.hpp`, `N2_ARRAY.hpp`

```python
def cal_all_H(title, array_name, option, q, low, high):
```

- title: file name
- array_name: C array name
- option: output structure or string
- q: number
- low, high: array range

```python
def generateHpp(p, q):
```

Generate header files based on prime numbers $p$ and $q$.

```python
def generatePara(p, q):
```

Calculate the inverse of $p$ and $q$ header files.























# 基于自定义大数类Paillar加密的C++实现

v0.3

## 前提说明

本项目是基于固定的$p,q$计算的。
$$
\begin{aligned}
p &= 177092068480735400854085936532566857106120280764577509765803339901544135759472066666117863886710233937613992438912318775026915931500403241175346661418233082352571559981771026111674521592470172566750802301187337513462870505971987298390405825403438064218796956493360970377443003039167540306288241695473000369153 \\
(p &= 0xfc2ffe76bf35218c3cf386fd77d3fd46d760db0e5135dfa0c59932e9b4fbb0f3918363690616084105e6d7994d29916ddc1b841366cef4bc9d90bc9075d86e577bb6a34bbb7aec8813d5b78e124b8e452d811b2ed461c741055e59ad1c27f4d08f55ab1197604a01589fb1e094b4fd7d61c68fffbdd709721c7ab559abc1cc01) \\
q &= 170134808228960721208095350458107390109573733329021377280204452375403114047025850581240057108821671754959765367486404349420445330219950136615430934439135100382845778661809332290874928032539319787311155164043376696534166912622062275544869888915196051820520420546846996578231633452059952729007671253273167566669 \\
(q &= 0xf247aead159e22ff932390894c285843fba88a59879218b0365f5f5d212196ae4f6539f174a8c14d6887dc554d2efe5b9472afaac18ef56b5b0e800539216bbc3b4b7f03a6b79750501c05f5294825d4290ba7249106428433e0bb554306b63d199add5a2409f96c6b2592d701b630dfc939828bc64bb005a7df10275ed4974d) \\
n &= p \times q
\end{aligned}
$$




## 文件架构：

1. 大数类的定义文件：

`BigNumber.hpp`，`BigNumber1024.hpp`,`BigNumber2048.hpp`,`BigNumber4096.hpp`,`BigNumber8192.hpp`

2. 基于固定$n$的预计算数组：

`N2_array.hpp`

3. 主文件头文件（快速数论变换）：

`Paillar8.hpp`

4. 主文件：

`Paillar8.cpp`



## 架构说明：

1. **大数类**

   （注意：请以代码以及代码中的注释为准）

   在大数类中，实现了位宽为1024,2048,4096,8192的大数的实现，采用的是`uint8_t`的数组实现。

   名字分别为`BigNumber1024`,`BigNumber2048`,`BigNumber4096`,`BigNumber8192`，基于共同的父类`BigNumber`

   在类的内部实现了基本的运算。

   注意：为了不再实现更高位的数，在8192位中加入了`overflow`位来防止溢出，超过一个byte的溢出暂且没有考虑（认为失败）。

   以下是**成员函数**的说明，注意：并不是所有的类都实现了所有的成员函数，这里列出全部函数的并集。

   | 成员函数名称   | 作用                   | 输入                      | 输出         | 备注           |
   | -------------- | ---------------------- | ------------------------- | ------------ | -------------- |
   | highestBit     | 返回最高位-1           | 无                        | int类型      |                |
   | upgrade        | 将该数升级为高一级的数 | 无                        | 更大类型的数 | 8192没有       |
   | downgrade      | 将该数升级为低一级的数 | 无                        | 更小类型的数 | 1024没有       |
   | <<             | 左移                   | int                       | 同类型数     |                |
   | >>             | 右移                   | int                       | 同类型数     |                |
   | +              | 加法                   | 同类型数                  | 同类型数     | 没有考虑溢出   |
   | -              | 减法                   | 同类型数                  | 同类型数     | 没有考虑负数   |
   | <              | 小于判断               | 同类型数                  | 同类型数     |                |
   | <=             | 小于等于判断           | 同类型数                  | 同类型数     |                |
   | >              | 大于判断               | 同类型数                  | 同类型数     |                |
   | >=             | 大于等于判断           | 同类型数                  | 同类型数     |                |
   | =              | 赋值                   | 同类型数                  | 同类型数     |                |
   | print          | 输出字符串结果         | 无                        | 终端输出     |                |
   | printHexNumber | 输出字符串结果         | debug信息                 | 终端输出     |                |
   | BigNumberXXXX  | 构造函数               | 十六进制字符串            |              | **不能含有0x** |
   |                | 构造函数               | uint8_t的数组和大小       |              | **大小不能超** |
   | BigNumber8192  | 构造函数               | uint8_t的数组和大小和溢出 |              | 仅仅为8192拥有 |

   以下是成员变量的说明

   | 名称     | 作用                       | 类型      |
   | -------- | -------------------------- | --------- |
   | data     | 大数的存放数组             | uint8_t   |
   | size     | 上述数组的大小             | const int |
   | overflow | 在8192位中防止计算结果溢出 | uint8_t   |

   

2. **基于固定$n$的预计算数组`N2_array`：**

   这里的预计算数组是为了在Barrett约简中使用的数组，计算的是H的值，其中
   $$
   H = (1 << (w_1 + 1)) // n^2)
   $$
   对这里的`w_1`从$4095 - 8192$（两边包含）进行枚举，得到所有的H。

   那么访问`w_1`对应的`H`应该是`N2_array[w_1-4095]`

   

3. **快速数论变换（NTT）：**

   具体的实现是基于静态类（无需构造成员）`FNT`

   不再介绍快速数论变换的基本概念，这里给出采用的固定的参数。

   ```c++
   static const long p = 469762049;
   static const int g = 3;    
   
   static const int omega_256 = 338628632;
   static const int omega_512 = 25153357;
   static const int omega_1024 = 110059487;
   static const int omega_2048 = 165447688;
       
   static const int omega_inv_256 = 215855482;
   static const int omega_inv_512 = 436579181;
   static const int omega_inv_1024 = 458753944;
   static const int omega_inv_2048 = 63413564;
   
   static const int N256_inv = 467927041;
   static const int N512_inv = 468844545;
   static const int N1024_inv = 469303297;
   static const int N2048_inv = 469532673;
   ```

   给出选取这个质数的原因：

   在考虑8192位乘以8192位时，本质上是采取了转为base为256的多项式，然后做多项式的卷积，那么卷积的过程中的最大值应该是$(2^7-1)^2*2048 \approx 2^{27}$，那么选取的$p$应该要大于这个数，防止$mod \quad p$时导致系数改变；其次，$p$的最大值最好不要超过$2^{31}$，不然导致需要更大的变量类型，可能导致溢出问题。

   接下来说明实现原理：

   对于长度为N的大数相乘，使用的是$2*N$点的NTT。需要在后段补0。

   下面给出成员函数表格：

   | 成员函数         | 含义                     | 输入       | 输出       | 备注                   |
   | ---------------- | ------------------------ | ---------- | ---------- | ---------------------- |
   | mod_exp          | 小数的快速幂             |            |            | 内部调用               |
   | bit_reverse      | 位反转                   |            |            | 内部调用               |
   | bit_reverse_copy | 蝴蝶变换位置寻找         |            |            | 内部调用               |
   | bit_reverse_copy | 重载上述函数             |            |            | 内部调用               |
   | fnt_iterative    | 快速数论变换             |            |            | 内部调用               |
   | ifnt_iterative   | 快速数论逆变换           |            |            | 内部调用               |
   | processArray     | 以T为进位的进位函数      |            |            | 内部调用               |
   | multiply         | 2048=1024*1024           | 1024大数类 | 2048大数类 |                        |
   | multiply         | 2048*2048=2048           | 2048大数类 | 2048大数类 |                        |
   | multiply         | 4096*4096=8192           | 2048大数类 | 8192大数类 |                        |
   | multiply         | 8192*8192=8192           | 8192大数类 | 8192大数类 | 仅仅允许一个byte的溢出 |
   | multiply         | 8192*8192=8192，包含移位 | 8192大数类 | 8192大数类 | 允许在计算之后移位     |

   

4. 主文件：

   在主文件中实现了基于固定数的加密过程，以下对实现的函数进行说明

   | 函数名称                 | 功能                                                         | 备注                                                         |
   | ------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
   | BarrettReductionFastExp1 | r = A % q                                                    | 对于Barrett约简的优化：考虑$q$很大的情况(2q都会溢出)，仅做判断做一次减法，建议直接查看代码 |
   | BarrettReductionModn2    | $r = A % n^2$                                                | 专属$n^2$的求模计算                                          |
   | FastExp                  | $Result = x^n\quad modp$                                     | 快速幂算法，n还是不能改，因为调用了上述的Barrett约简算法     |
   | FastExp                  | $Result = x^n\quad modp$                                     | 位宽不一样的重载                                             |
   | calculate_h              | $h = -x^2 \quad modn$                                        | 加密第一步                                                   |
   | calculate_hs             | $h_s = h^n \quad mod n^2$                                    | 加密第二步                                                   |
   | calculate_c              | $c=(1+mn)h_s^{\alpha}\quad mod n^2$                          | 加密第三步                                                   |
   | BarrettReductionModp     | $r=A\quad modp$                                              | 专属$p$的求模计算                                            |
   | BarretReductionModp2     | $r=A\quad modp^2$                                            | 专属$p^2$的求模计算                                          |
   | BarretBasedDivide        | $Result=A//p$                                                | 利用Barrett约简做出的除法                                    |
   | caculate_mp              | $hp = (p-1)*q^{-1} \quad mod p \\mp = L(c^{p-1} mod p^2, p)*hp \quad mod p$ | 解密过程中的mp计算                                           |
   | calculate_u              | $u = (mq-mp)*p^{-1} \quad mod q$                             | 解密过程中的u计算                                            |
   | encrypt                  | 计算明文$m$的密文$c$                                         | 加密函数                                                     |
   | decrypt                  | 对密文$c$的解密得到明文$m$                                   | 解密函数                                                     |





## 算法中变量含义说明

| 变量名   | 意义                           | 位宽 |
| -------- | ------------------------------ | ---- |
| n        | $n=pq$                         | 2048 |
| p        | 质数                           | 1024 |
| q        | 质数                           | 1024 |
| n_square | $n^2$                          | 4096 |
| x        | $x = randint(2, min(p, q))$    | 1024 |
| h        | $h = -x^2 \quad mod n$         | 2048 |
| $h_s$    | $h_s = h^n \quad mod n^2$      | 4096 |
| c        | 密文                           | 4096 |
| $\alpha$ | randint(2, 2^ceil(key_size/2)) | 1024 |
| m        | 明文                           | 2048 |









# python-tool：生成头文件

给定质数$p$和$q$，在Barrett约简中，计算预计算的头文件

`P_ARRAY.hpp`，`Q_ARRAY.hpp`，`P2_ARRAY.hpp`，`Q2_ARRAY.hpp`，`N2_ARRAY.hpp`



```python
def cal_all_H(title, array_name, option, q, low, high):
```

title：文件名

array_name：C数组名

option：输出结构体或者字符串

q：数

low，high：数组起止



```python
def generateHpp(p, q):
```

根据质数p和q



```python
def generatePara(p, q):
```

计算p和q的逆的头文件


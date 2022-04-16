/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  int all = ~(~x & ~y);
  int all1 = x & y;
  int all0 = ~x & ~y;
  return all & ~all1 & ~all0;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1 << 31;
}

//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  return !((x + 1) + x + 1) & !!(~x); // x + x + 1 + 1 = 0 => x = -1 or x = Tmax
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {  
  return !(~((x & 0xFF) & (x >> 8) & (x >> 16) & (x >> 24)) & 0xAA);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return (~x) + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  int left =  x + (~0x30) + 1;  // x - 0x30
  int right = 0x39 + (~x) + 1;  // 0x39 - x
  int l = !(left & (1 << 31));  // left <= 0
  int r = !(right & (1 << 31)); // right <= 0
  return l & r;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  int all1 = !!x + (~0);  // x != 0 => all1=0 ; x = 0 => all1 = ~0
  return (~all1 & y) | (all1 & z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int delta = x + ((~y) + 1);
  int xisNeg = x & (1 << 31);
  int yisPos = ~y & (1 << 31);
  int xisPos = !xisNeg;
  int yisNeg = !yisPos;
  return !(xisPos & yisNeg) & (!!(xisNeg & yisPos) | !!(delta & (1 << 31)) | !delta);
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  int a = (x | (x >> 16));
  int b = (a | (a >> 8));
  int c = (b | (b >> 4));
  int d = (c | (c >> 2));
  int e = (d | (d >> 1));
  return (e & 1) ^ 1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int y = (~(((x >> 31) & 1) + ~0)) ^ x; // 从统计x的最长0/1前缀，转化为求y的最长0前缀。
  int delta0 = ~(!(y >> 16) + ~0) & 16;  // delta0=16如果y的前16位都是0，否则delta0=0.
  int y0 = y >> (~(!delta0 + ~0) & 16);  // 如果delta0=0，则y右移16位，否则y不用动。
  int delta1 = ~(!((y0 >> 8) & 0xff) + ~0) & 8;
  int y1 = y0 >> (~(!delta1 + ~0) & 8);
  int delta2 = ~(!((y1 >> 4) & 0xf) + ~0) & 4;
  int y2 = y1 >> (~(!delta2 + ~0) & 4);
  int delta3 = ~(!((y2 >> 2) & 0x3) + ~0) & 2;
  int y3 = y2 >> (~(!delta3 + ~0) & 2);
  int delta4 = ~(!((y3 >> 1) & 0x1) + ~0) & 1;
  int y4 = y3 >> (~(!delta4 + ~0) & 1);
  int delta5 = ~(!(y4 & 1) + ~0) & 1;

  int max0 = delta0 + delta1 + delta2 + delta3 + delta4 + delta5;
  return 32 + (~max0) + 2;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) { // 爽卡极限 op=30
  int s0 = uf >> 31;
  int e0 = (uf >> 23) & 0xff;
  int w0 = ((uf << 9) >> 9);
  int w = w0 + w0;
  int eisNotZero = !!e0;
  int all1 = ~0;
  int jw = (~(!e0 + all1) & (w >> 23)) | eisNotZero; // 如果e不是0，则一定要进位，因为1.XXX * 2肯定会进位
  int eisFF = !!(e0 ^ 0xff) + all1;
  w = w >> (jw & eisNotZero);  // e = 0不需要进位，因为e从0变到1时，会自然多一个1.xx代替0.xx
  return  (eisFF & uf) | (~eisFF & ((s0 << 31) | ((e0 + jw) << 23) | w)); 
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) { 
  int s0 = uf >> 31;
  int e0 = (uf >> 23) & 0xff;
  int w0 = (uf << 9) >> 9;
  int all1 = ~0;                             // 这部分我刚开始没发现可以用if，导致怎么也优化不进30个op
  int w = (((!e0) ^ 1) << 23) | w0;          // 把四行注释的取消注释，再把两个if注释掉，一样是对的。
  int e = e0 + (~127) + 1;
  int res = w >> (24 + (~e));   // 23 - e
  //int isNeg = ~(s0 + all1);
  //int eisNotNeg = ~(!(e >> 31) + all1);
  int notOverflow = (!(e0 ^ 0xff) + all1) & (!((e + (~32) + 1) >> 31) + all1);
  // res = (isNeg & (~res + 1)) | (~isNeg & res);
  if (s0) res = ~res + 1;
  if (notOverflow){
    if (e >> 31) res = 0;   // 指数为负，结果直接是0.
  } else res = 1 << 31;
  //return (~notOverflow & (1 << 31)) | (notOverflow & (eisNotNeg & res));
  return res;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    if (x >= 0) {
      if (x > 127) return 0xff << 23;
      else return (x + 127) << 23;
    } else {
      if (x >= -23) return (1 << 23) >> (-x);
      else return 0;
    }
}

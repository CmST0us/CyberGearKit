//
//  float16.c
//  test_bit
//
//  Created by Eric Wu on 2023/9/5.
//

#include "float16.h"
#include <stdio.h>
#include <stdint.h>
#define SIGN_MASK 0x8000
#define EXP_MASK 0x7C00
#define NAN_VALUE 0x7FFF
#define IS_ZERO(x) (((x) & 0x7FFF) == 0)
#define IS_INVALID(x) (((x) & EXP_MASK) == EXP_MASK)
#define IS_NAN(x) (((x) & 0x7FFF) > 0x7C00)
#define IS_INF(x) ( ((x) & 0x7FFF) == 0x7C00)
#define MANTISSA(x) (((x) & 1023) | (((x) & 0x7C00) == 0 ? 0 : 1024))
#define EXPONENT(x) (((x) & 0x7C00) >> 10)
#define SIGNED_INF_VALUE(x)  ((x & SIGN_MASK) | 0x7C00)

short f16_sub(short ain,short bin)
{
    unsigned short a=ain;
    unsigned short b=bin;
    if(((a ^ b) & 0x8000) != 0)
        return f16_add(a,b ^ 0x8000);
    unsigned short sign = a & 0x8000;
    a = a << 1;
    b = b << 1;
    if(a < b) {
        unsigned short x=a;
        a=b;
        b=x;
        sign ^= 0x8000;
    }
    unsigned short ax = a & 0xF800;
    unsigned short bx = b & 0xF800;
    if(a >=0xf800 || b>=0xf800) {
        if(a > 0xF800 || b > 0xF800 || a==b)
            return 0x7FFF;
        unsigned short res = sign | 0x7C00;
        if(a == 0xf800)
            return res;
        else
            return res ^ 0x8000;
    }
    int exp_diff = ax - bx;
    unsigned short exp_part  = ax;
    if(exp_diff != 0) {
        int shift = exp_diff >> 11;
        if(bx != 0)
            b = ((b & 2047) | 2048) >> shift;
        else
            b >>= (shift - 1);
    }
    else {
        if(bx == 0) {
            unsigned short res = (a-b) >> 1;
            if(res == 0)
                return res;
            return res | sign;
        }
        else {
            b=(b & 2047) | 2048;
        }
    }
    unsigned short r=a - b;
    if((r & 0xF800) == exp_part) {
        return (r>>1) | sign;
    }
    unsigned short am = (a & 2047) | 2048;
    unsigned short new_m = am - b;
    if(new_m == 0)
        return 0;
    while(exp_part !=0 && !(new_m & (2048))) {
        exp_part-=0x800;
        if(exp_part!=0)
            new_m<<=1;
    }
    return (((new_m & 2047) | exp_part) >> 1) | sign;
}

short f16_add(short a,short b)
{
    if (((a ^ b) & 0x8000) != 0)
        return f16_sub(a,b ^ 0x8000);
    short sign = a & 0x8000;
    a &= 0x7FFF;
    b &= 0x7FFF;
    if(a<b) {
        short x=a;
        a=b;
        b=x;
    }
    if(a >= 0x7C00 || b>=0x7C00) {
        if(a>0x7C00 || b>0x7C00)
            return 0x7FFF;
        return 0x7C00 | sign;
    }
    short ax = (a & 0x7C00);
    short bx = (b & 0x7C00);
    short exp_diff = ax - bx;
    short exp_part = ax;
    if(exp_diff != 0) {
        int shift = exp_diff >> 10;
        if(bx != 0)
            b = ((b & 1023) | 1024) >> shift;
        else
            b >>= (shift - 1);
    }
    else {
        if(bx == 0) {
            return (a + b) | sign;
        }
        else {
            b=(b & 1023) | 1024;
        }
    }
    short r=a+b;
    if ((r & 0x7C00) != exp_part) {
        unsigned short am = (a & 1023) | 1024;
        unsigned short new_m = (am + b) >> 1;
        r =( exp_part + 0x400) | (1023 & new_m);
    }
    if((unsigned short)r >= 0x7C00u) {
        return sign | 0x7C00;
    }
    return r | sign;
}


short f16_mul(short a,short b)
{
    int sign = (a ^ b) & SIGN_MASK;

    if(IS_INVALID(a) || IS_INVALID(b)) {
        if(IS_NAN(a) || IS_NAN(b) || IS_ZERO(a) || IS_ZERO(b))
            return NAN_VALUE;
        return sign | 0x7C00;
    }

    if(IS_ZERO(a) || IS_ZERO(b))
        return 0;
    unsigned short m1 = MANTISSA(a);
    unsigned short m2 = MANTISSA(b);

    uint32_t v=m1;
    v*=m2;
    int ax = EXPONENT(a);
    int bx = EXPONENT(b);
    ax += (ax==0);
    bx += (bx==0);
    int new_exp = ax + bx - 15;
    
    if(v & ((uint32_t)1<<21)) {
        v >>= 11;
        new_exp++;
    }
    else if(v & ((uint32_t)1<<20)) {
        v >>= 10;
    }
    else { // denormal
        new_exp -= 10;
        while(v >= 2048) {
            v>>=1;
            new_exp++;
        }
    }
    if(new_exp <= 0) {
        v>>=(-new_exp + 1);
        new_exp = 0;
    }
    else if(new_exp >= 31) {
        return SIGNED_INF_VALUE(sign);
    }
    return (sign) | (new_exp << 10) | (v & 1023);
}

short f16_div(short a,short b)
{
    short sign = (a ^ b) & SIGN_MASK;
    if(IS_NAN(a) || IS_NAN(b) || (IS_INVALID(a) && IS_INVALID(b)) || (IS_ZERO(a) && IS_ZERO(b)))
        return 0x7FFF;
    if(IS_INVALID(a) || IS_ZERO(b))
        return sign | 0x7C00;
    if(IS_INVALID(b))
        return 0;
    if(IS_ZERO(a))
        return 0;

    unsigned short m1 = MANTISSA(a);
    unsigned short m2 = MANTISSA(b);
    uint32_t m1_shifted = m1;
    m1_shifted <<= 10;
    uint32_t v= m1_shifted / m2;
    unsigned short rem = m1_shifted % m2;
    
    int ax = EXPONENT(a);
    int bx = EXPONENT(b);
    ax += (ax==0);
    bx += (bx==0);
    int new_exp = ax - bx + 15 ;

    if(v == 0 && rem==0)
        return 0;

    while(v < 1024 && new_exp > 0) {
        v<<=1;
        rem<<=1;
        if(rem >= m2) {
            v++;
            rem -= m2;
        }
        new_exp--;
    }
    while(v >= 2048) {
        v>>=1;
        new_exp++;
    }
    
    if(new_exp <= 0) {
        v>>=(-new_exp + 1);
        new_exp = 0;
    }
    else if(new_exp >= 31) {
        return SIGNED_INF_VALUE(sign);
    }
    return sign | (v & 1023) | (new_exp << 10);
}

short f16_neg(short v)
{
    return SIGN_MASK ^ v;
}
short f16_from_int(int32_t sv)
{
    uint32_t v;
    int sig = 0;
    if(sv < 0) {
        v=-sv;
        sig=1;
    }
    else
        v=sv;
    if(v==0)
        return 0;
    int e=25;
    while(v >= 2048) {
        v>>=1;
        e++;
    }
    while(v<1024) {
        v<<=1;
        e--;
    }
    if(e>=31)
        return SIGNED_INF_VALUE(sig << 15);
    return (sig << 15) | (e << 10) | (v & 1023);
}
int32_t f16_int(short a)
{
    unsigned short value = MANTISSA(a);
    short shift = EXPONENT(a) - 25;
    if(shift > 0)
        value <<= shift;
    else if(shift < 0)
        value >>= -shift;
    if(a & SIGN_MASK)
        return -(int32_t)(value);
    return value;
}

int f16_gte(short a,short b)
{
    if(IS_ZERO(a) && IS_ZERO(b))
        return 1;
    if(IS_NAN(a) || IS_NAN(b))
        return 0;
    if((a & SIGN_MASK) == 0) {
        if((b & SIGN_MASK) == SIGN_MASK)
            return 1;
        return a >= b;
    }
    else {
        if((b & SIGN_MASK) == 0)
            return 0;
        return (a & 0x7FFF) <= (b & 0x7FFF);
    }
}

int f16_gt(short a,short b)
{
    if(IS_NAN(a) || IS_NAN(b))
        return 0;
    if(IS_ZERO(a) && IS_ZERO(b))
        return 0;
    if((a & SIGN_MASK) == 0) {
        if((b & SIGN_MASK) == SIGN_MASK)
            return 1;
        return a > b;
    }
    else {
        if((b & SIGN_MASK) == 0)
            return 0;
        return (a & 0x7FFF) < (b & 0x7FFF);
    }
    
}
int f16_eq(short a,short b)
{
    if(IS_NAN(a) || IS_NAN(b))
        return 0;
    if(IS_ZERO(a) && IS_ZERO(b))
        return 1;
    return a==b;
}

int f16_lte(short a,short b)
{
    if(IS_NAN(a) || IS_NAN(b))
        return 0;
    return f16_gte(b,a);
}

int f16_lt(short a,short b)
{
    if(IS_NAN(a) || IS_NAN(b))
        return 0;
    return f16_gt(b,a);
}
int f16_neq(short a,short b)
{
    return !f16_eq(a,b);
}


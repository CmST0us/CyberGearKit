//
//  float16.h
//  test_bit
//
//  Created by Eric Wu on 2023/9/5.
//

#pragma once

#include <stdint.h>

#define f16_tenth   11878
#define f16_fifth   12902
#define f16_third   13653
#define f16_half    14336
#define f16_one     15360
#define f16_two     16384
#define f16_three   16896
#define f16_five    17664
#define f16_ten     18688
#define f16_pi      16968
#define f16_half_pi 15944

short f16_add(short a,short b);
short f16_sub(short a,short b);
short f16_mul(short a,short b);
short f16_div(short a,short b);
short f16_neg(short a);
short f16_from_int(int32_t v);
int32_t f16_int(short v);

int f16_gte(short a,short b);
int f16_gt(short a,short b);
int f16_eq(short a,short b);
int f16_lte(short a,short b);
int f16_lt(short a,short b);
int f16_neq(short a,short b);


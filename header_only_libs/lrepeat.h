#ifndef REPEAT_FUNC
#error "No repeat function"
#endif

#ifndef REPEAT_COUNT
#error "No repeat count"
#endif
//Powers of 2.
#define REPEAT1_FUNC(a) REPEAT_FUNC(a) REPEAT_FUNC(a+1)
#define REPEAT2_FUNC(a) REPEAT1_FUNC(a) REPEAT1_FUNC(a+2)
#define REPEAT3_FUNC(a) REPEAT2_FUNC(a) REPEAT2_FUNC(a+4)
#define REPEAT4_FUNC(a) REPEAT3_FUNC(a) REPEAT3_FUNC(a+8)
#define REPEAT5_FUNC(a) REPEAT4_FUNC(a) REPEAT4_FUNC(a+16)
#define REPEAT6_FUNC(a) REPEAT5_FUNC(a) REPEAT5_FUNC(a+32)
#define REPEAT7_FUNC(a) REPEAT6_FUNC(a) REPEAT6_FUNC(a+64)
#define REPEAT8_FUNC(a) REPEAT7_FUNC(a) REPEAT7_FUNC(a+(1<<7))

#define REPEAT9_FUNC(a) REPEAT8_FUNC(a) REPEAT8_FUNC(a+(1<<8))
#define REPEAT10_FUNC(a) REPEAT9_FUNC(a) REPEAT9_FUNC(a+(1<<9))
#define REPEAT11_FUNC(a) REPEAT10_FUNC(a) REPEAT10_FUNC(a+(1<<10))
#define REPEAT12_FUNC(a) REPEAT11_FUNC(a) REPEAT11_FUNC(a+(1<<11))
#define REPEAT13_FUNC(a) REPEAT12_FUNC(a) REPEAT12_FUNC(a+(1<<12))
#define REPEAT14_FUNC(a) REPEAT13_FUNC(a) REPEAT13_FUNC(a+(1<<13))
#define REPEAT15_FUNC(a) REPEAT14_FUNC(a) REPEAT14_FUNC(a+(1<<14))
#define REPEAT16_FUNC(a) REPEAT15_FUNC(a) REPEAT15_FUNC(a+(1<<15))
#define REPEAT17_FUNC(a) REPEAT16_FUNC(a) REPEAT16_FUNC(a+(1<<16))
#define REPEAT18_FUNC(a) REPEAT17_FUNC(a) REPEAT17_FUNC(a+(1<<17))
#define REPEAT19_FUNC(a) REPEAT18_FUNC(a) REPEAT18_FUNC(a+(1<<18))
#define REPEAT20_FUNC(a) REPEAT19_FUNC(a) REPEAT19_FUNC(a+(1<<19))
#define REPEAT21_FUNC(a) REPEAT20_FUNC(a) REPEAT20_FUNC(a+(1<<20))
#define REPEAT22_FUNC(a) REPEAT21_FUNC(a) REPEAT21_FUNC(a+(1<<21))
#define REPEAT23_FUNC(a) REPEAT22_FUNC(a) REPEAT22_FUNC(a+(1<<22))
#define REPEAT24_FUNC(a) REPEAT23_FUNC(a) REPEAT23_FUNC(a+(1<<23))
#define REPEAT25_FUNC(a) REPEAT24_FUNC(a) REPEAT24_FUNC(a+(1<<24))
#define REPEAT26_FUNC(a) REPEAT25_FUNC(a) REPEAT25_FUNC(a+(1<<25))
#define REPEAT27_FUNC(a) REPEAT26_FUNC(a) REPEAT26_FUNC(a+(1<<26))
#define REPEAT28_FUNC(a) REPEAT27_FUNC(a) REPEAT27_FUNC(a+(1<<27))
#define REPEAT29_FUNC(a) REPEAT28_FUNC(a) REPEAT28_FUNC(a+(1<<28))
#define REPEAT30_FUNC(a) REPEAT29_FUNC(a) REPEAT29_FUNC(a+(1<<29))

#define RCBIT(n) ((REPEAT_COUNT & (1<<n))!=0)
//count offsets when processing bit N
#define CN29 (RCBIT(30)?(1<<30):0) /* + CN30*/
#define CN28 (RCBIT(29)?(1<<29):0) + CN29
#define CN27 (RCBIT(28)?(1<<28):0) + CN28
#define CN26 (RCBIT(27)?(1<<27):0) + CN27

#define CN25 (RCBIT(26)?(1<<26):0) + CN26
#define CN24 (RCBIT(25)?(1<<25):0) + CN25
#define CN23 (RCBIT(24)?(1<<24):0) + CN24
#define CN22 (RCBIT(23)?(1<<23):0) + CN23
#define CN21 (RCBIT(22)?(1<<22):0) + CN22
#define CN20 (RCBIT(21)?(1<<21):0) + CN21

#define CN19 (RCBIT(20)?(1<<20):0) + CN20
#define CN18 (RCBIT(19)?(1<<19):0) + CN19
#define CN17 (RCBIT(18)?(1<<18):0) + CN18
#define CN16 (RCBIT(17)?(1<<17):0) + CN17
#define CN15 (RCBIT(16)?(1<<16):0) + CN16
#define CN14 (RCBIT(15)?(1<<15):0) + CN15
#define CN13 (RCBIT(14)?(1<<14):0) + CN14
#define CN12 (RCBIT(13)?(1<<13):0) + CN13
#define CN11 (RCBIT(12)?(1<<12):0) + CN12
#define CN10 (RCBIT(11)?(1<<11):0) + CN11
#define CN9 (RCBIT(10)?(1<<10):0) + CN10
#define CN8 (RCBIT(9)?(1<<9):0) + CN9
#define CN7 (RCBIT(8)?(1<<8):0) + CN8
#define CN6 (RCBIT(7)?(1<<7):0) + CN7
#define CN5 (RCBIT(6)?(1<<6):0) + CN6
#define CN4 (RCBIT(5)?(1<<5):0) + CN5
#define CN3 (RCBIT(4)?(1<<4):0) + CN4
#define CN2 (RCBIT(3)?(1<<3):0) + CN3
#define CN1 (RCBIT(2)?(1<<2):0) + CN2
#define CN0 (RCBIT(1)?(1<<1):0) + CN1

#if REPEAT_COUT < (1<<8)
#undef CN8
#define CN8 0
#endif

#if REPEAT_COUT < (1<<10)
#undef CN10
#define CN10 0
#endif

#if REPEAT_COUT < (1<<12)
#undef CN12
#define CN12 0
#endif

#if REPEAT_COUT < (1<<14)
#undef CN14
#define CN14 0
#endif

#if REPEAT_COUT < (1<<16)
#undef CN16
#define CN16 0
#endif

#if REPEAT_COUT < (1<<18)
#undef CN18
#define CN18 0
#endif

#if REPEAT_COUT < (1<<20)
#undef CN20
#define CN20 0
#endif

#if REPEAT_COUT < (1<<22)
#undef CN22
#define CN22 0
#endif

#if REPEAT_COUT < (1<<24)
#undef CN24
#define CN24 0
#endif

#if REPEAT_COUT < (1<<26)
#undef CN26
#define CN26 0
#endif

#if REPEAT_COUT < (1<<28)
#undef CN28
#define CN28 0
#endif

#if REPEAT_COUNT & (1<<30)
REPEAT30_FUNC(0)
#endif

#if REPEAT_COUNT & (1<<29)
REPEAT29_FUNC(CN29)
#endif

#if REPEAT_COUNT & (1<<28)
REPEAT28_FUNC(CN28)
#endif

#if REPEAT_COUNT & (1<<27)
REPEAT27_FUNC(CN27)
#endif

#if REPEAT_COUNT & (1<<26)
REPEAT26_FUNC(CN26)
#endif

#if REPEAT_COUNT & (1<<25)
REPEAT25_FUNC(CN25)
#endif

#if REPEAT_COUNT & (1<<24)
REPEAT24_FUNC(CN24)
#endif

#if REPEAT_COUNT & (1<<23)
REPEAT23_FUNC(CN23)
#endif

#if REPEAT_COUNT & (1<<22)
REPEAT22_FUNC(CN22)
#endif

#if REPEAT_COUNT & (1<<21)
REPEAT21_FUNC(CN21)
#endif

#if REPEAT_COUNT & (1<<20)
REPEAT20_FUNC(CN20)
#endif

#if REPEAT_COUNT & (1<<19)
REPEAT19_FUNC(CN19)
#endif

#if REPEAT_COUNT & (1<<18)
REPEAT18_FUNC(CN18)
#endif

#if REPEAT_COUNT & (1<<17)
REPEAT17_FUNC(CN17)
#endif

#if REPEAT_COUNT & (1<<16)
REPEAT16_FUNC(CN16)
#endif

#if REPEAT_COUNT & (1<<15)
REPEAT15_FUNC(CN15)
#endif

#if REPEAT_COUNT & (1<<14)
REPEAT14_FUNC(CN14)
#endif

#if REPEAT_COUNT & (1<<13)
REPEAT13_FUNC(CN13)
#endif


#if REPEAT_COUNT & (1<<12)
REPEAT12_FUNC(CN12)
#endif

#if REPEAT_COUNT & (1<<11)
REPEAT11_FUNC(CN11)
#endif

#if REPEAT_COUNT & (1<<10)
REPEAT10_FUNC(CN10)
#endif

#if REPEAT_COUNT & (1<<9)
REPEAT9_FUNC(CN9)
#endif

#if REPEAT_COUNT & (1<<8)
REPEAT8_FUNC(CN8)
#endif

#if REPEAT_COUNT & 128
REPEAT7_FUNC(CN7)
#endif
#if REPEAT_COUNT & 64
REPEAT6_FUNC(CN6)
#endif
#if REPEAT_COUNT & 32
REPEAT5_FUNC(CN5)
#endif
#if REPEAT_COUNT & 16
REPEAT4_FUNC(CN4)
#endif
#if REPEAT_COUNT & 8
REPEAT3_FUNC(CN3)
#endif
#if REPEAT_COUNT & 4
REPEAT2_FUNC(CN2)
#endif
#if REPEAT_COUNT & 2
REPEAT1_FUNC(CN1)
#endif
#if REPEAT_COUNT & 1
REPEAT_FUNC(CN0)
#endif



#undef REPEAT_COUNT
#undef REPEAT_FUNC

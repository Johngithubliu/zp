#ifndef COMMON_H
#define COMMON_H


//数据类型
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned long u32;
typedef signed long s32;
typedef float fp32;
typedef double fp64;
typedef u8 bl;

typedef u8 uint8_t;
typedef s8 int8_t;
typedef u16 uint16_t;
typedef s16 int16_t;
typedef u32 uint32_t;
typedef s32 int32_t;


/*****************************************************************************/
#ifndef FALSE
	#define FALSE (0)
#endif

#ifndef TRUE
	#define TRUE (!FALSE)
#endif


#define BIT(n) (1 << (n))

#define BIT_GET(d, n) (((d) & BIT(n)) ? (1) : (0))
#define BIT_SET(d, n) ((d) |= BIT(n))
#define BIT_CLR(d, n) ((d) &= ~BIT(n))
#define BIT_XOR(d, n) ((d) ^= BIT(n))

#define BIT_ISSET(d, n) BIT_GET(d, n)
#define BIT_ISCLR(d, n) !(BIT_ISSET(d, n))



#define TO_U8(d) ((u8) (d))
#define TO_U16(d) ((u16) (d))
#define TO_U32(d) ((u32) (d))

#define TO_S8(d) ((s8) (d))
#define TO_S16(d) ((s16) (d))
#define TO_S32(d) ((s32) (d))


#define TO(type, d) ((type) (d))


#define U8_HI(d) (TO(u8,(d))>>4)
#define U8_LO(d) (TO(u8,(d))&0xF)

#define U16_HI(d)(TO(u16,(d))>>8)
#define U16_LO(d)(TO(u16,(d))&0x00FF)

/* macro for input binary */
#define LONG_TO_BIN(n) \
(                      \
((n >> 21) & 0x80) |   \
((n >> 18) & 0x40) |   \
((n >> 15) & 0x20) |   \
((n >> 12) & 0x10) |   \
((n >>  9) & 0x08) |   \
((n >>  6) & 0x04) |   \
((n >>  3) & 0x02) |   \
((n      ) & 0x01)     \
)
#define BIN(n) LONG_TO_BIN(0x##n##L)
#define _0b(n) BIN(n)

#define ARRAY_SIZE(array) ( sizeof(array) / sizeof(array[0]) )

#define ATOM(code) do {\
	EA = 0;\
	code;\
	EA = 1;\
}while(0);

///////////////////////////////////////////////////////////////////////////////
void ByteToHexAscii(u8 chr, char *hi, char *lo);
void ByteToDecAscii(u8 chr, char *hi, char *mi, char *lo);
u8 HexAsciiToByte(u8 asc);

s32 LinearInterpolation(s32 x, s32 x0, s32 y0, s32 x1, s32 y1);
s32 ParabolaInterpolation(s32 x, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2);

void ScientificNotationToFloat(fp64 *pFp, s32 mantissa, s32 exponent, u8 num);
void FloatToScientificNotation(fp64 fp, s32 *pMantissa, s32 *pExponent, u8 num);


#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h> // for uint64_t
#include <inttypes.h>

#define uint128_t unsigned __int128
#define MAX_64 "0xFFFFFFFFFFFFFFFF"

#define P10_UINT64 10000000000000000000ULL /* 19 zeroes */
#define E10_UINT64 19
#define STRINGIZER(x) #x
#define TO_STRING(x) STRINGIZER(x)

struct BigInt
{
    unsigned long long number[4];
    int sign;
};

struct BigIntMul
{
    unsigned long long number[8];
    int sign;
};

typedef struct BigInt BigInt;
typedef struct BigIntMul BigIntMul;

static int print_u128_u(uint128_t u128);
void printBigInt(unsigned long long number[], int len, int s);
BigInt sumBigInt(BigInt x, BigInt y);
BigInt resBigInt(BigInt x, BigInt y);
BigIntMul mulBigInt(BigInt x, BigInt y);
BigInt enterN256(char a0[], char a1[], char a2[], char a3[], int sign);
unsigned long long hexaToULL(char number[]);

int main(int argc, char *argv[])
{
    printf("Inicio del programa\n\n");
    struct BigInt x, y;

    //MAX VALUE 0xFFFFFFFFFFFFFFFF
    x = enterN256("0x0", "0xFFFFFFFFFFFFFFF", "0x0", "0xFFFFFFFFFFFFFFFF", 0);
    y = enterN256("0x0", "0x0", "0x0", "0x20", 0);
    printf("\n");

    printf("    X = ");
    printBigInt(x.number, 4, x.sign);
    printf("    Y = ");
    printBigInt(y.number, 4, y.sign);

    BigInt rSum = sumBigInt(x, y);
    BigInt rRes = resBigInt(x, y);
    BigIntMul rMul = mulBigInt(x, y);

    printf("X + Y = ");
    printBigInt(rSum.number, 4, rSum.sign);
    printf("X - Y = ");
    printBigInt(rRes.number, 4, rRes.sign);
    printf("X * Y = ");
    printBigInt(rMul.number, 8, rMul.sign);

    printf("\nFin del programa\n");
}
BigInt enterN256(char a0[], char a1[], char a2[], char a3[], int sign)
{
    struct BigInt n256;
    n256.number[3] = hexaToULL(a0);
    n256.number[2] = hexaToULL(a1);
    n256.number[1] = hexaToULL(a2);
    n256.number[0] = hexaToULL(a3);
    n256.sign = sign;

    return n256;
}

unsigned long long hexaToULL(char hexa[])
{
    char value[64];
    char *eptr;
    unsigned long long result;

    strcpy(value, hexa);

    result = strtoul(value, &eptr, 16);

    if (result == 0)
    {
        if (errno == EINVAL)
        {
            printf("Conversion error occurred: %d\n", errno);
            exit(0);
        }
        if (errno == ERANGE)
            printf("The value provided was out of range\n");
    }
    return result;
}
void printBigInt(unsigned long long number[], int len, int s)
{
    int i = 0;
    char sign;
    sign = s == 1 ? '-' : '+';
    //printf("%c0x", sign);
    printf("0x");

    for (i = len - 1; i >= 0; i--)
    {
        printf("%016llx ", number[i]);
    }
    printf("\n");
    return;
}

BigInt sumBigInt(BigInt x, BigInt y)
{
    struct BigInt out;
    out.number[0] = 0;
    out.number[1] = 0;
    out.number[2] = 0;
    out.number[3] = 0;
    out.sign = 0;

    unsigned long long c = 0;

    int i = 0;
    for (i = 0; i < 4; i++)
    {
        unsigned long long xc = x.number[i] + c;
        if (xc < c)
        {
            out.number[i] = y.number[i];
        }
        else
        {
            xc += y.number[i];
            c = y.number[i] > xc ? 1 : 0;
            out.number[i] = xc;
        }
    }
    return out;
}

BigInt resBigInt(BigInt x, BigInt y)
{
    struct BigInt out;
    out.number[0] = 0;
    out.number[1] = 0;
    out.number[2] = 0;
    out.number[3] = 0;
    out.sign = 0;

    unsigned long long c = 0;

    int i = 0;
    for (i = 0; i < 4; i++)
    {
        unsigned long long yc = y.number[i] + c;
        if (yc < c)
        {
            out.number[i] = x.number[i];
        }
        else
        {
            c = x.number[i] < yc ? 1 : 0;
            out.number[i] = x.number[i] - yc;
        }
    }

    return out;
}

BigIntMul mulBigInt(BigInt x, BigInt y)
{
    struct BigIntMul out;
    out.number[0] = 0;
    out.number[1] = 0;
    out.number[2] = 0;
    out.number[3] = 0;
    out.number[4] = 0;
    out.number[5] = 0;
    out.number[6] = 0;
    out.number[7] = 0;
    out.sign = 0;

    int i, j = 0;
    uint128_t s = 0;

    uint128_t multi = 0;
    unsigned long long c = 0;

   for (i = 0; i < 4; i++)
    {
        c = 0;
        for (j = 0; j < 4; j++)
        {
            multi = (uint128_t)x.number[j] * (uint128_t)y.number[i];

            printf("Multiplicacion: ");
            print_u128_u(multi);
            printf("\n");

            printf("i: %d j:%d x: %llu j: %llu\n", i, j, x.number[j], y.number[i]);
            s = out.number[i + j] + multi + c;
            printf("S res: ");
            print_u128_u(s);

            if (s > hexaToULL("0xFFFFFFFFFFFFFFFF"))
            {

                c = (s / hexaToULL("0xFFFFFFFFFFFFFFFF") -1 );
                s = c * hexaToULL("0xFFFFFFFFFFFFFFFF") -1;
                printf("\nDentro S: ");
                print_u128_u(s);
                printf(" Dentro c: %llu \n", c);
            }
            else
            {
                printf("\nmenor\n");
                c = 0;
            }
            print_u128_u(s);
            out.number[i + j] = (uint64_t)(s);
            printf("\nOUT %llu\n",out.number[i + j] );
        }
        out.number[i + j + 1] = c;
    }
    return out;
}

static int print_u128_u(uint128_t u128)
{
    int rc;
    if (u128 > UINT64_MAX)
    {
        uint128_t leading = u128 / P10_UINT64;
        uint64_t trailing = u128 % P10_UINT64;
        rc = print_u128_u(leading);
        rc += printf("%." TO_STRING(E10_UINT64) PRIu64, trailing);
    }
    else
    {
        uint64_t u64 = u128;
        rc = printf("%" PRIu64, u64);
    }
    return rc;
}
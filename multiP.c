#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

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

void printBigInt(unsigned long long number[], int len, int s);
BigInt sumBigInt(BigInt x, BigInt y);
BigInt resBigInt(BigInt x, BigInt y);
BigIntMul mulBigInt(BigInt x, BigInt y);

unsigned long long hexaToULL(char number[]);

int main(int argc, char *argv[])
{
    printf("Inicio del programa\n\n");
    struct BigInt x, y;

    x.number[0] = hexaToULL("0xFFFFFFFFFFFFFFFF");
    x.number[1] = hexaToULL("0xFFFFFFFFFFFFFFFF");
    x.number[2] = hexaToULL("0xFFFFFFFFFFFFFFFF");
    x.number[3] = hexaToULL("0xFFFFFFFFFFFFFFFF");
    x.sign = 0;

    y.number[0] = hexaToULL("1ABBB0");
    y.number[1] = hexaToULL("0");
    y.number[2] = hexaToULL("0xFFFFFFFFFFFFFFFF");
    y.number[3] = hexaToULL("0xFFFFFFFFFFFFFFFF");
    y.sign = 0;

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

    for (i = 0; i < len; i++)
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
    unsigned long long s = 0;

    for (i = 0; i < 4; i++)
    {
        unsigned long long c = 0;
        for (j = 0; j < 4; j++)
        {

            s = out.number[i + j] + (x.number[j] * y.number[i]) + c;
            out.number[i + j] = s;
        }
        out.number[i + j + 1] = c;
    }
    return out;
}

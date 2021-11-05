#define bias ( 1 << 14 )
#define INT_MAX 2147483647
#define INT_MIN -2149483647

int int_to_fp(int n);
int fp_to_int_round(int x);
int fp_to_int(int x);
int add_fp(int x, int y);
int add_mixed(int x, int n);
int sub_fp(int x, int y);
int sub_mixed(int x, int n);
int mult_fp(int x, int y);
int mult_mixed(int x, int n);
int div_fp(int x, int y);
int div_mixed(int x, int n);

/*

    fixed point arithmetic (represented as 32bit integer)
    MSB(bit 31) = sign bit
    bit 14~30 = integer bit
    bit 0 ~13 = fraction bits
    example : 12.50 = 1*2^3 + 1*2^2 + 1*2^(-1)

*/


/* converts integer to fixed point */
int
int_to_fp(int n)
{
    return n * bias;
}

/* converts integer to fixed point rounding to nearest integer */
int
fp_to_int_round(int x)
{
    if(x >= 0) return (x + bias / 2) / bias;
    else return (x - bias /2 ) / bias;
}

/* converts integer to fixed point rounding to zero */
int
fp_to_int(int x)
{ 
    return x / bias; 
}

/* returns float x + float y */
int
add_fp(int x,int y)
{
    return x + y;
}

/* returns float x + int n */
int
add_mixed(int x, int n)
{
    return x + bias * n;
}

/* returns float x - float y */
int
sub_fp(int x, int y)
{
    return x - y;
}

/* returns float x - int n */
int
sub_mixed(int x, int n)
{
    return x - bias * n;
}

int
mult_fp(int x, int y)
{
    return ((int64_t)x) * y / bias;
}

int
mult_mixed(int x, int n)
{
    return x * n;
}

int
div_fp(int x,int y)
{
    return ((int64_t)x) * bias / y;
}

int
div_mixed(int x, int n)
{
    return x / n;
    
}

__kernel void operator(
    __global int * y,
    const int a,
    const int b
    )
{
    y[0] = a / b;
    y[1] = a % b;
    y[2] = (int)((float)a/(float)b);
    y[3] = (int)remainder((float)a,(float)b);
    y[4] = a * b;
    y[5] = a / 5;
    y[6] = a % 5;
    y[7] = a + b;
}

#include <limits.h>
#include <stdio.h>

unsigned int bits(unsigned int value)
{
unsigned int result = 0;

unsigned int mask = 1;

while(mask)
{
result += (value & mask) != 0;
mask *= 2;
}

return result;
}

unsigned int even(unsigned int value)
{
return !(bits(value) % 2);
}

unsigned int odd(unsigned int value)
{
return !even(value);
}

int main()
{
const char *parity[] = {"ODD", "EVEN"};
unsigned int i = 0;

puts("Value Bits Parity");

for(; i < 20; ++i)
printf("%5u %4u %s\n", i, bits(i), parity[even(i)]);

return 0;
}
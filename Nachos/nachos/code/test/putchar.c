#include "syscall.h"
void print(char c, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        PutChar(c + i);
    }
    PutChar('\n');

}
int main()
{
    int i = 12;
    print('a',4);
    return i;
}
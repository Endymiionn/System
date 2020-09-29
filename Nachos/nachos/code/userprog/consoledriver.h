#ifdef CHANGED
#ifndef CONSOLEDRIVER_H
#define CONSOLEDRIVER_H
#include "copyright.h"
#include "utility.h"
#include "console.h"

class ConsoleDriver:dontcopythis
{
    public:
    ConsoleDriver(const char *readFile, const char *writeFile);
    // initialize the hardware console device
    ~ConsoleDriver(); // clean up
    void PutChar(int ch); // Unix putchar(3S)
    int GetChar(); // Unix getchar(3S)
    void PutString(const char *s); // Unix fputs(3S)
    void GetString(char *s, int n); // Unix fgets(3S)
    private:
    Console *console;
};

#endif // CONSOLEDRIVER_H
#endif // CHANGED
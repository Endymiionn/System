#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"
static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }
ConsoleDriver::ConsoleDriver(const char *in, const char *out)
{
readAvail = new Semaphore("read avail", 0);
writeDone = new Semaphore("write done", 0);
console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}
ConsoleDriver::~ConsoleDriver()
{
delete console;
delete writeDone;
delete readAvail;
}
void ConsoleDriver::PutChar(int ch)
{
console->TX(ch);
writeDone->P();
}
int ConsoleDriver::GetChar()
{
readAvail->P();
int ch = console->RX();
return ch;
}
void ConsoleDriver::PutString(const char s[])
{
    int i=0;
    while(s[i] != '\0')
    {
        PutChar(s[i]);
        i++;
    }
}
void ConsoleDriver::GetString(char *s, int n)
{
    int i=0;
    int ch = GetChar();
    while( i<n && i<MAX_STRING_SIZE)
    {
        if(ch != EOF && ch != '\n' && ch != '\0'){

        
            s[i] = ch;
            i++;
            ch = GetChar();}
        else
            break;
        
    }
    s[i] = '\0';
}
#endif // CHANGED

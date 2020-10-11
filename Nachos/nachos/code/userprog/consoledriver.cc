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
console->TX(ch); //ecriture du char
writeDone->P(); //signal de fin d'écriture
}
int ConsoleDriver::GetChar()
{
readAvail->P(); //attente de qque chose a lire
int ch = console->RX(); //copie du char 
return ch;
}
void ConsoleDriver::PutString(const char s[])
{
    int i=0;
    while(s[i] != '\0') //tant que la chaine n'est pas vide
    {
        PutChar(s[i]); //ecriture char par char
        i++;
    }
}
void ConsoleDriver::GetString(char *s, int n)
{
    int i=0;
    int ch;
    while( i<n && i<MAX_STRING_SIZE) //tant qu'on ne depasse pas la taille
    {
        if((ch=(char)GetChar()) != EOF && ch != '\n' && ch != '\0') //si il y a un caractère à copier
        {
            s[i] = ch;
            i++;
        }
    }
    s[i] = '\0'; // fin de chaine
}

// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "consoledriver.h"
#include "userthread.h"

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}

int copyStringFromMachine(int from, char *to, unsigned size)
{
    unsigned i=0; //compteur allant jusqu'a size-1
    int result; //variable temporaire de l'écriture de from+i
    while((i<size)&&(machine->ReadMem(from+i, 1, &result))) //tant qu'on ne depasse pas la taille et qu'on lit un char
    {
        *(to+i) = (char)result; //écriture du resultat dans l'adresse machine
        i++;
    }
    *(to+i) = '\0'; //écriture du symbole de fin de chaine
    return i;
}

int copyStringToMachine(char* from, int to, unsigned size)
{
	unsigned i=0; //compteur allant jusqu'a size-1
	while(i<size && from[i] != '\0') //tant que la chaine n'est pas fini ou qu'on ne passe pas la taille
	{
		machine->WriteMem(to+i, 1, from[i]); //ecriture dans la memoire de la machine
		i++;
	}
	machine->WriteMem(to+i, 1, '\0'); //fin de chaine
    return i;
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);
    int address = machine->registers[BadVAddrReg];

    switch (which)
      {
	case SyscallException:
          {
	    switch (type)
	      {
		case SC_Halt:
		  {
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
		    interrupt->Halt ();
		    break;

		  }
		case SC_PutChar:
		  {
			int ch = machine->ReadRegister(4); //recuperation du registre 4 = arg1
			DEBUG('s',"PutChar\n");
			consoledriver->PutChar(ch);
			break;
		  }

		case SC_PutString:
		{
			char *to = new char[MAX_STRING_SIZE+1]; // +1 pour le \0
			int ch = machine->ReadRegister(4); //recuperation string
			int res = 0;
			while(res != ch)
			{
				//copie du string entierement
				res = res + copyStringFromMachine(ch+res, to, MAX_STRING_SIZE); //copie string mips -> linux
				DEBUG('s', "PutString\n");
				consoledriver->PutString(to); //stockage de la string dans to;
			}
			delete [] to;
			break;
		}
		case SC_GetChar:
		{
			machine->WriteRegister(2, consoledriver->GetChar()); //stockage du char dans le registre 2
			int res = machine->ReadRegister(2); //recuperation du char dans le registre 2

			if(res!=-1)
				printf("%c\n", res);
			break;
		}
		case SC_GetString:
		{
			char *buff = new char[MAX_STRING_SIZE+1]; // +1 pour le \0
			int to = machine->ReadRegister(4); //recuperation de l'adresse mips (1er argument)
			int n = machine->ReadRegister(5); //recuperation de la taille (2eme argument)
			consoledriver->GetString(buff, n); //stockage du string dans le buffer
			copyStringToMachine(buff, to, n); //ecriture en memoire du buffer à l'adresse to
			delete [] buff;
			break;
		}
		case SC_Exit:
		{
			//on rentre dans ce case quand on sort du programme
			printf("Fin programme");
			interrupt->Halt (); //halt par defaut maintenant
			break;
		}
		case SC_ThreadCreate:
		{
			int addrFct = machine->ReadRegister(4);
			int addrArg = machine->ReadRegister(5);
			int res = do_ThreadCreate(addrFct, addrArg);
			machine->WriteRegister(2, res);
			break;
		}
		case SC_ThreadExit:
		{
			do_ThreadExit();
			break;
		}
		default:
		  {
		    printf("Unimplemented system call %d\n", type);
		    ASSERT(FALSE); //partie 6 assertion failed
			//pas d'appel system halt ducoup on va dans le case default du switch
		  }
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	  }

	case PageFaultException:
	  if (!address) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }
	  break;

	case ReadOnlyException:
	  printf ("Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case BusErrorException:
	  printf ("Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case AddressErrorException:
	  printf ("Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case OverflowException:
	  printf ("Overflow at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case IllegalInstrException:
	  printf ("Illegal instruction at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	default:
	  printf ("Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
	  ASSERT (FALSE);
	  break;
      }
}

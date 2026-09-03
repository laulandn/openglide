//**************************************************************
//*            OpenGLide - Glide to OpenGL Wrapper
//*             http://openglide.sourceforge.net
//*
//*            Linux specific clock functions
//*
//*         OpenGLide is OpenSource under LGPL license
//*              Originaly made by Fabio Barros
//*      Modified by Paul for Glidos (http://www.glidos.net)
//*               Linux version by Simon White
//**************************************************************
#ifndef WIN32

#include <stdio.h>
#include <string.h>
#include "platform/clock.h"

#include <Dialogs.h>


//#ifndef SetDialogTimeout
#ifdef __cplusplus
extern "C" {
#endif

// Define the missing link symbol as an inline macro mapping directly to the trap selector
OSStatus SetDialogTimeout(DialogRef theDialog, SInt16 itemToClick, UInt32 secondsToWait)
{
#ifdef __POWERPC__
    return (OSStatus)CallUniversalProc((UniversalProcPtr)
#else
    return
    // TODO: This is totally wrong on m68k...
#endif
    GetToolTrapAddress(0xAAA5),
        kPascalStackBased
        | RESULT_SIZE(SIZE_CODE(sizeof(short)))
        | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(DialogRef)))
        | STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(DialogItemIndex)))
        | STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(UInt32))),
        (theDialog), (itemToClick), (secondsToWait)
#ifdef __POWERPC__
        );
#else
        ;
#endif
}

#ifdef __cplusplus
}
#endif
//#endif


float ClockFrequency( void )
{
    char  str[35];
    float freq = 0.0;
    /*
    FILE *f = fopen ("/proc/cpuinfo", "r");
    // Be nice and don't crash
    if ( f == NULL ) return freq;
    while (fgets (str, sizeof (str), f))
    {
        if (!strncmp ("cpu MHz", str, 7))
        {
            sscanf (strchr(str, ':')+1, "%f", &freq);
            break;
        }
    }
    fclose (f);
    */
    freq=25.0;
    return freq * 1000000.0;
}


#endif // WIN32

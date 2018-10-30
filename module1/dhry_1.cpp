/**
\file dhry_1.cpp

University of Colorado Boulder <br>
ECEN 5803 Mastering Embedded System Architecture <br>
Project 2 Module 1 <br>

Code used to conduct Dhrystone benchmarking

Modified for use in this project by Chris Morroni <br> 
Version: 1.0 <br>
Date of current revision:  11/3/18 <br>
*/

// Source obtained from https://fossies.org/dox/dhrystone-2.1/dhry__1_8c_source.html
/*
 ****************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *
 *  Version:    C, Version 2.1
 *
 *  File:       dhry_1.c (part 2 of 3)
 *
 *  Date:       May 25, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 ****************************************************************************
 */
#include "dhry.h"
#include <stdlib.h>
#include <string.h>
#include "mbed.h"

/* Global Variables: */

Rec_Pointer Ptr_Glob, Next_Ptr_Glob;
int Int_Glob;
Boolean Bool_Glob;
char Ch_1_Glob, Ch_2_Glob;
int Arr_1_Glob[50];
int Arr_2_Glob[50][50];

/* variables for time measurement: */

long Begin_Time, End_Time, User_Time;
float Microseconds, Dhrystones_Per_Second;

/* end of variables for time measurement */

extern Serial pc;
Timer dhry_timer;

#define TOO_SMALL_TIME_US (2000000)

void dhry_main(int Number_Of_Runs)
{
    One_Fifty Int_1_Loc;
    One_Fifty Int_2_Loc;
    One_Fifty Int_3_Loc;
    char Ch_Index;
    dhry_e Enum_Loc;
    Str_30 Str_1_Loc;
    Str_30 Str_2_Loc;
    int Run_Index;

    /* Initializations */

    Next_Ptr_Glob = (Rec_Pointer) malloc(sizeof(Rec_Type));

    Ptr_Glob = (Rec_Pointer) malloc(sizeof(Rec_Type));

    Ptr_Glob->Ptr_Comp = Next_Ptr_Glob;
    Ptr_Glob->Discr = Ident_1;
    Ptr_Glob->variant.var_1.Enum_Comp = Ident_3;
    Ptr_Glob->variant.var_1.Int_Comp = 40;
    strcpy(Ptr_Glob->variant.var_1.Str_Comp, "DHRYSTONE PROGRAM, SOME STRING");
    strcpy(Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

    Arr_2_Glob[8][7] = 10;
    /* Was missing in published program. Without this statement,    */
    /* Arr_2_Glob [8][7] would have an undefined value.             */
    /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
    /* overflow may occur for this array element.                   */
    pc.printf("\r\n");
    pc.printf("Dhrystone Benchmark, Version 2.1 (Language: C)\r\n");
    pc.printf("\r\n");
    pc.printf("Execution starts, %d runs through Dhrystone\r\n", Number_Of_Runs);

    /***************/
    /* Start timer */
    /***************/

    dhry_timer.start();
    dhry_timer.reset();

    for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
    {

        Proc_5();
        Proc_4();
        /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
        Int_1_Loc = 2;
        Int_2_Loc = 3;
        strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
        Enum_Loc = Ident_2;
        Bool_Glob = !Func_2(Str_1_Loc, Str_2_Loc);
        /* Bool_Glob == 1 */
        while (Int_1_Loc < Int_2_Loc) /* loop body executed once */
        {
            Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
            /* Int_3_Loc == 7 */
            Proc_7(Int_1_Loc, Int_2_Loc, &Int_3_Loc);
            /* Int_3_Loc == 7 */
            Int_1_Loc += 1;
        } /* while */
        /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
        Proc_8(Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
        /* Int_Glob == 5 */
        Proc_1(Ptr_Glob);
        for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
        /* loop body executed twice */
        {
            if (Enum_Loc == Func_1(Ch_Index, 'C'))
            /* then, not executed */
            {
                Proc_6(Ident_1, &Enum_Loc);
                strcpy(Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
                Int_2_Loc = Run_Index;
                Int_Glob = Run_Index;
            }
        }
        /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
        Int_2_Loc = Int_2_Loc * Int_1_Loc;
        Int_1_Loc = Int_2_Loc / Int_3_Loc;
        Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
        /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
        Proc_2(&Int_1_Loc);
        /* Int_1_Loc == 5 */

    } /* loop "for Run_Index" */

    /**************/
    /* Stop timer */
    /**************/

    User_Time = dhry_timer.read_us();

    pc.printf("Execution ends\r\n");
    pc.printf("\r\n");
    pc.printf("Final values of the variables used in the benchmark:\r\n");
    pc.printf("\r\n");
    pc.printf("Int_Glob:            %d\r\n", Int_Glob);
    pc.printf("        should be:   %d\r\n", 5);
    pc.printf("Bool_Glob:           %d\r\n", Bool_Glob);
    pc.printf("        should be:   %d\r\n", 1);
    pc.printf("Ch_1_Glob:           %c\r\n", Ch_1_Glob);
    pc.printf("        should be:   %c\r\n", 'A');
    pc.printf("Ch_2_Glob:           %c\r\n", Ch_2_Glob);
    pc.printf("        should be:   %c\r\n", 'B');
    pc.printf("Arr_1_Glob[8]:       %d\r\n", Arr_1_Glob[8]);
    pc.printf("        should be:   %d\r\n", 7);
    pc.printf("Arr_2_Glob[8][7]:    %d\r\n", Arr_2_Glob[8][7]);
    pc.printf("        should be:   Number_Of_Runs + 10\r\n");
    pc.printf("Ptr_Glob->\r\n");
    pc.printf("  Ptr_Comp:          %d\n", (int) Ptr_Glob->Ptr_Comp);
    pc.printf("        should be:   (implementation-dependent)\r\n");
    pc.printf("  Discr:             %d\r\n", Ptr_Glob->Discr);
    pc.printf("        should be:   %d\r\n", 0);
    pc.printf("  Enum_Comp:         %d\r\n", Ptr_Glob->variant.var_1.Enum_Comp);
    pc.printf("        should be:   %d\r\n", 2);
    pc.printf("  Int_Comp:          %d\r\n", Ptr_Glob->variant.var_1.Int_Comp);
    pc.printf("        should be:   %d\r\n", 17);
    pc.printf("  Str_Comp:          %s\r\n", Ptr_Glob->variant.var_1.Str_Comp);
    pc.printf("        should be:   DHRYSTONE PROGRAM, SOME STRING\r\n");
    pc.printf("Next_Ptr_Glob->\r\n");
    pc.printf("  Ptr_Comp:          %d\r\n", (int) Next_Ptr_Glob->Ptr_Comp);
    pc.printf("        should be:   (implementation-dependent), same as above\r\n");
    pc.printf("  Discr:             %d\r\n", Next_Ptr_Glob->Discr);
    pc.printf("        should be:   %d\r\n", 0);
    pc.printf("  Enum_Comp:         %d\r\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
    pc.printf("        should be:   %d\r\n", 1);
    pc.printf("  Int_Comp:          %d\r\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
    pc.printf("        should be:   %d\r\n", 18);
    pc.printf("  Str_Comp:          %s\r\n", Next_Ptr_Glob->variant.var_1.Str_Comp);
    pc.printf("        should be:   DHRYSTONE PROGRAM, SOME STRING\r\n");
    pc.printf("Int_1_Loc:           %d\r\n", Int_1_Loc);
    pc.printf("        should be:   %d\r\n", 5);
    pc.printf("Int_2_Loc:           %d\r\n", Int_2_Loc);
    pc.printf("        should be:   %d\r\n", 13);
    pc.printf("Int_3_Loc:           %d\r\n", Int_3_Loc);
    pc.printf("        should be:   %d\r\n", 7);
    pc.printf("Enum_Loc:            %d\r\n", Enum_Loc);
    pc.printf("        should be:   %d\r\n", 1);
    pc.printf("Str_1_Loc:           %s\r\n", Str_1_Loc);
    pc.printf("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\r\n");
    pc.printf("Str_2_Loc:           %s\r\n", Str_2_Loc);
    pc.printf("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\r\n");
    pc.printf("\r\n");

    if (User_Time < TOO_SMALL_TIME_US)
    {
        pc.printf("Time too small, increase iterations\r\n");
    }
    else
    {
        Microseconds = (float) User_Time / (float) Number_Of_Runs;
        Dhrystones_Per_Second = (float) Number_Of_Runs / ((float) User_Time / (float) USEC_PER_SEC);
        pc.printf("Microseconds for one run through Dhrystone: ");
        pc.printf("%6.1f \r\n", Microseconds);
        pc.printf("Dhrystones per Second:                      ");
        pc.printf("%6.1f \r\n", Dhrystones_Per_Second);
        pc.printf("DMIPS:                      ");
        pc.printf("%6.1f \r\n", Dhrystones_Per_Second / 1757);
        pc.printf("\r\n");
    }
}

void Proc_1(Rec_Pointer Ptr_Val_Par)
{
    Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;
    /* == Ptr_Glob_Next */
    /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
    /* corresponds to "rename" in Ada, "with" in Pascal           */

    structassign(*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
    Ptr_Val_Par->variant.var_1.Int_Comp = 5;
    Next_Record->variant.var_1.Int_Comp = Ptr_Val_Par->variant.var_1.Int_Comp;
    Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
    Proc_3(&Next_Record->Ptr_Comp);
    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp
     == Ptr_Glob->Ptr_Comp */
    if (Next_Record->Discr == Ident_1)
    /* then, executed */
    {
        Next_Record->variant.var_1.Int_Comp = 6;
        Proc_6(Ptr_Val_Par->variant.var_1.Enum_Comp, &Next_Record->variant.var_1.Enum_Comp);
        Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
        Proc_7(Next_Record->variant.var_1.Int_Comp, 10, &Next_Record->variant.var_1.Int_Comp);
    }
    else
        /* not executed */
        structassign(*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */

/* executed once */
/* *Int_Par_Ref == 1, becomes 4 */
void Proc_2(One_Fifty * Int_Par_Ref)
{
    One_Fifty Int_Loc;
    dhry_e Enum_Loc;

    Int_Loc = *Int_Par_Ref + 10;
    do /* executed once */
        if (Ch_1_Glob == 'A')
        /* then, executed */
        {
            Int_Loc -= 1;
            *Int_Par_Ref = Int_Loc - Int_Glob;
            Enum_Loc = Ident_1;
        } /* if */
    while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */

/* executed once */
/* Ptr_Ref_Par becomes Ptr_Glob */
void Proc_3(Rec_Pointer * Ptr_Ref_Par)
{
    if (Ptr_Glob != NULL)
        /* then, executed */
        *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
    Proc_7(10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */

void Proc_4()
{
    Boolean Bool_Loc;

    Bool_Loc = Ch_1_Glob == 'A';
    Bool_Glob = Bool_Loc | Bool_Glob;
    Ch_2_Glob = 'B';
} /* Proc_4 */

void Proc_5()
{
    Ch_1_Glob = 'A';
    Bool_Glob = false;
} /* Proc_5 */

/* Procedure for the assignment of structures,          */
/* if the C compiler doesn't support this feature       */
#ifdef  NOSTRUCTASSIGN
memcpy (d, s, l)
register char *d;
register char *s;
register int l;
{
    while (l--) *d++ = *s++;
}
#endif

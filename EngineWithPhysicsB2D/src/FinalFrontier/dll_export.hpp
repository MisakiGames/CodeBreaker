//This code was made for the Multimedia Project 2a, 
//in the Multimedia Technology class at the FH Salzburg, 
//by Christopher Kastner and Tim Paul
#pragma once
////////////////////////////////////////////////////////////
// simplified version of dll export that only handles the
// windows case. For Linux/Mac case, the FINALFRONTIER_API
// need not be set to anything.
////////////////////////////////////////////////////////////

#ifdef FINALFRONTIER_DLL_EXPORTS
#define FINALFRONTIER_API __declspec(dllexport)
#else
#define FINALFRONTIER_API __declspec(dllimport)
#endif

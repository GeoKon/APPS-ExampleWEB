#pragma once
/* ---------------------------------------------------------------------------------
 *          GLOBAL Classes and Structures
 * ----------------------------------------------------------------------------------
 */
#include "externIO.h"       // global externals of cpu,...,eep            

// ---------------------- Definition of the Global class ----------------------------

enum wifi_state { TRYING_WIFI=0, STA_MODE=2, AP_MODE=1 };

class Globals
{
public:                                                 // ======= A1. Add here all volatile parameters 
    wifi_state wifiOK;          
    void initVolatile()                                 // ======= A2. Initialize here the volatile parameters
    {
        wifiOK = TRYING_WIFI;
    }    
    void printVolatile( char *prompt="", BUF *bp=NULL ) // ======= A3. Add to buffer (or print) all volatile parms
    {
        ;
    }
    struct gp_t                                         // ======= B1. Add here all non-volatile parameters into a structure
    {                           
        int   itest;                
    } gp;
    
    void initMyEEParms()                                // ======= B2. Initialize here the non-volatile parameters
    {
        gp.itest = 10;
    } 
    void registerMyEEParms()                            // ======= B3. Register parameters by name
    {   
        ;
    }
    void printMyEEParms( char *prompt="", BUF *bp=NULL ) // ======= B4. Add to buffer (or print) all volatile parms
    {
        PF( "%s\r\n", prompt );
    }    
    #include <GLOBAL.hpp>                               // Common code for all Global implementations
    
//    void initAllParms( int myMagic  )       
//    void fetchMyEEParms()
//    void saveMyEEParms()
};

// Exported class
extern Globals myp;                                     // allocated by Globals.cpp      

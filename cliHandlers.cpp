// --------------------------------- CLI HANDLERS -------------------------------------------------

    #include "externIO.h"               // global externals of cpu,...,server
    #include "SimpleSRV.h"
    #include "Globals.h"                // in this local directory
    
// --------------- allocation of classes used only by this module -------------------
  
    
// ------ References to classes allocated elsewhere but needed here -----------------
  
    void help( int n, char *args[] )
    {
        exe.help( n, args );
    }
    
    void files( int n, char *args[] )
    {
        BUF *bp = (BUF *)args[0];
        if( bp )
            bp->add( fileList().c_str() );
        else
            PRN( fileList().c_str() );
    }
    
    CMDTABLE mypTable[]= // must be external to be able to used by the cliSupport
    {
        {"h",       "Help! List of all commands",             help  },
        {"files",   "SPIFS List of files",                 files    },
        {NULL, NULL, NULL}
    };

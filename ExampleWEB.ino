/* ---------------------------------------------------------------------------
 *  Example of a simple WEB application. Should be used as a template
 *  to create other applications.
 *  --------------------------------------------------------------------------
 */
    #include <FS.h>
    
    #include "cliHandlers.h"    // in this local directory
    #include "SimpleAP.h"       // in GKE-Lw Library
    #include "SimpleSTA.h"      // in GKE-Lw Library
    #include "SimpleSRV.h"      // in GKE-Lw Library
    #include "Globals.h"        // in this local directory

    #include "eepTable.h"       // Table of eepClass CLI commands
    
// ----------- allocation of classes used globally ----------------------------
    
    CPU cpu;                    // instances of the externIO.h
    CLI cli;
    EXE exe;
    EEP eep;
    ESP8266WebServer server(80);    

    BUF buffer(1024);           // cli buffer used by cliCallbacks()

// ----------------------------- Main Setup -----------------------------------

#define CLI_WAITSEC 10      // how long to wait before RETURN is sensed for CLI
void setup() 
{
    cpu.init(); 
    ASSERT( SPIFFS.begin() );

    myp.initAllParms( 0x6789/*Magic number*/);

    exe.registerTable( mypTable );               // register CLI tables
    exe.registerTable( eepTable ); 
    exe.printTables( "See all tables" );        // instead of PF, you can optionally specify a Buf

    cli.init( ECHO_ON, "cmd: " );               // prepare CLI

    PF( "Push BUTTON for AP Setup (192.168.4.1)\r\n", CLI_WAITSEC );

    if( startCLIAfter( CLI_WAITSEC ) )
        myp.wifiOK = AP_MODE;
    else
        myp.wifiOK = STA_MODE;

    if( myp.wifiOK == STA_MODE )
    {
        setupSTA();
        srvCallbacks( server, Landing_STA_Page );             // standard WEB callbacks. "staLanding" is /. HTML page
    }
    if( myp.wifiOK == AP_MODE )
    {
        setupAP();
        srvCallbacks( server, Landing_STA_Page );             // standard WEB callbacks
    }
    cliCallbacks( server, buffer );                    // enable WEB CLI with buffer specified
    
    setTrace( T_REQUEST | T_JSON );             // default trace    
    server.begin( 80 );                         // start the server
    PRN("HTTP server started.");
    
    cli.init( ECHO_ON, "cmd: " );
    cli.prompt();
}
void loop()
{
    if( myp.wifiOK==AP_MODE )
        loopAP();
    if( myp.wifiOK==STA_MODE )
        loopSTA();

    server.handleClient();  
}

/* ---------------------------------------------------------------------------
 *  Example of a simple WEB application. Should be used as a template
 *  to create other applications.
 *  --------------------------------------------------------------------------
 */
    #include <FS.h>
    
    #include "cliHandlers.h"    // in this local directory
    #include "SimpleAP.h"       // in this local directory
    #include "SimpleSTA.h"      // in this local directory
    #include "SimpleSRV.h"      // in GKE-Lw Library
    #include "Globals.h"        // in this local directory

    #include "eepTable.h"       // Table of eepClass CLI commands
    
// ----- References to classes allocated elsewhere but needed here ------------

    #include "externIO.h"       // global externals of cpu,...,eep

// ----------- allocation of classes used globally ----------------------------
    
    CPU cpu;                    // instances of the externIO.h
    CLI cli;
    EXE exe;
    EEP eep;
    ESP8266WebServer server(80);    

    BUF buffer(1024);           // cli buffer used by cliCallbacks()

// ------ Forward References (located in this module --------------------------

    void initEEParms();

    
    
// ----------------------------- Main Setup -----------------------------------

#define CLI_WAITSEC 10      // how long to wait before RETURN is sensed for CLI
void setup() 
{
    cpu.init(); 
    ASSERT( SPIFFS.begin() );

    initEEParms();

    exe.registerTable( mypTable );               // register CLI tables
    exe.registerTable( eepTable ); 
    exe.printTables( "See all tables" );        // instead of PF, you can optionally specify a Buf

    cli.init( ECHO_ON, "cmd: " );               // prepare CLI

    PF( "----- Within %dsec: press CR for CLI, or push BUTTON for AP Setup (192.168.4.1)\r\n", CLI_WAITSEC );

    myp.wifiOK = STA_MODE;
    
    for( int i=0; i<CLI_WAITSEC*100; i++ )
    {
        if( (i%100)==0 ) PR(".");
        if( Serial.read() == 0x0D )
            interactForever();                  // located in SimpleSTA. Assumes Tables and cli.init() done.
        if( cpu.button() )
            {myp.wifiOK=AP_MODE; break;}
        delay(10);
    }
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

// ------------------- initialize eeprom parms --------------------------------------
#define myMAGIC 0xBABE
void initEEParms()
{
    if( !eep.checkEEParms( myMAGIC, myp.bsize ) )      // fetches parameters and returns TRUE or FALSE
    {
        PF("=== Initializing parms!\r\n" );
        eep.initHeadParms( myMAGIC, myp.bsize );        // initialize header parameters AND save them in eeprom
        eep.initWiFiParms();                            // initialize with default WiFi AND save them in eeprom
        
        myp.initMyParms( true );                        // initialize named parameters AND save them in EEPROM
        PF("AFTER INITIALIZATION\r\n");
    }
    eep.incrBootCount();
    myp.fetchMyParms();                                 // retrieve parameters from EEPROM
    eep.printHeadParms("--- Current Head Parms");       // print current parms
    eep.printWiFiParms("--- Current WiFi Parms");                 
}

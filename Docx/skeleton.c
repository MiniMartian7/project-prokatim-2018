/***********************************************************
*  skeleton.c
*  Example for ping-pong processing
*  Caution: It is intended, that this file ist not runnable. 
*  The file contains mistakes and omissions, which shall be
*  corrected and completed by the students.
*
*   F. Quint, HsKA
************************************************************/


#include <csl.h>
#include <csl_mcbsp.h>
#include <csl_irq.h>
#include <csl_edma.h>
#include <dsk6713_led.h>
#include "config_AIC23.h"
#include "skeleton.h"

#define BUFFER_LEN 500
/* Ping-Pong buffers. Place them in the compiler section .datenpuffer */
/* How do you place the compiler section in the memory?     */
#pragma DATA_SECTION(Buffer_in_ping, ".datenpuffer");
short Buffer_in_ping[BUFFER_LEN];
#pragma DATA_SECTION(Buffer_in_pong, ".datenpuffer");
short Buffer_in_pong[BUFFER_LEN];
#pragma DATA_SECTION(Buffer_out_ping, ".datenpuffer");
short Buffer_out_ping[BUFFER_LEN];
#pragma DATA_SECTION(Buffer_out_pong, ".datenpuffer");
short Buffer_out_pong[BUFFER_LEN];

//Configuration for McBSP1 (data-interface)
MCBSP_Config datainterface_config = {
		/* McBSP Control Register */
        MCBSP_FMKS(SPCR, FREE, NO)              |	//  Freilauf
        MCBSP_FMKS(SPCR, SOFT, xxxxxx)          |
        MCBSP_FMKS(SPCR, FRST, YES)             |	// Framesync ist ein
        MCBSP_FMKS(SPCR, GRST, YES)             |	// Reset aus, damit läuft der Samplerate- Generator
        MCBSP_FMKS(SPCR, XINTM, xxxxx)          |
        MCBSP_FMKS(SPCR, XSYNCERR, NO)          |	// empfängerseitig keine Überwachung der Synchronisation
        MCBSP_FMKS(SPCR, XRST, YES)             |	// Sender läuft (kein Reset- Status)	
        MCBSP_FMKS(SPCR, DLB, OFF)              |	// Loopback (Kurschluss) nicht aktiv
        MCBSP_FMKS(SPCR, RJUST, RZF)            |	// rechtsbündige Ausrichtung der Daten im Puffer
        MCBSP_FMKS(SPCR, CLKSTP, DISABLE)       |	// Clock startet ohne Verzögerung auf fallenden Flanke (siehe auch PCR-Register)
        MCBSP_FMKS(SPCR, DXENA, OFF)            |	// DX- Enabler wird nicht verwendet
        MCBSP_FMKS(SPCR, RINTM, RRDY)           |	// Sender Interrupt wird durch "RRDY-Bit" ausgelöst
        MCBSP_FMKS(SPCR, RSYNCERR, NO)          |	// senderseitig keine Überwachung der Synchronisation
        MCBSP_FMKS(SPCR, RRST, YES),			// Empfänger läuft (kein Reset- Status)
		/* Empfangs-Control Register */
        MCBSP_FMKS(RCR, RPHASE, SINGLE)         |	// Nur eine Phase pro Frame
        MCBSP_FMKS(RCR, RFRLEN2, DEFAULT)       |	// Länge in Phase 2, unrelevant
        MCBSP_FMKS(RCR, RWDLEN2, DEFAULT)       |	// Wortlänge in Phase 2, unrelevant
        MCBSP_FMKS(RCR, RCOMPAND, MSB)          |	// kein Compandierung der Daten (MSB first)
        MCBSP_FMKS(RCR, RFIG, NO)               |	// Rahmensynchronisationspulse (nach dem ersten Puls)) startet die Übertragung neu
        MCBSP_FMKS(RCR, RDATDLY, 0BIT)          |	// keine Verzögerung (delay) der Daten
        MCBSP_FMKS(RCR, RFRLEN1, OF(1))         |	// Länge der Phase 1 --> 1 Wort
        MCBSP_FMKS(RCR, RWDLEN1, xxxxx)         |	//
        MCBSP_FMKS(RCR, RWDREVRS, DISABLE),		// 32-bit Reversal nicht genutzt
		/* Sende-Control Register */
        MCBSP_FMKS(XCR, XPHASE, xxxxxx)         |	//
        MCBSP_FMKS(XCR, XFRLEN2, DEFAULT)       |	// Länge in Phase 2, unrelevant
        MCBSP_FMKS(XCR, XWDLEN2, DEFAULT)       |	// Wortlänge in Phase 2, unrelevant
        MCBSP_FMKS(XCR, XCOMPAND, MSB)          |	// kein Compandierung der Daten (MSB first)
        MCBSP_FMKS(XCR, XFIG, NO)               |	// Rahmensynchronisationspulse (nach dem ersten Puls)) startet die Übertragung neu
        MCBSP_FMKS(XCR, XDATDLY, 0BIT)          |	// keine Verzögerung (delay) der Daten
        MCBSP_FMKS(XCR, XFRLEN1, OF(1))         |	// Länge der Phase 1 --> 1 Wort
        MCBSP_FMKS(XCR, XWDLEN1, 16BIT)         |	// Wortlänge in Phase 1 --> 16 bit
        MCBSP_FMKS(XCR, XWDREVRS, DISABLE),		// 32-bit Reversal nicht genutzt
		/* Sample Rate Generator Register */
        MCBSP_FMKS(SRGR, GSYNC, DEFAULT)        |	// Einstellungen nicht relevant da
        MCBSP_FMKS(SRGR, CLKSP, DEFAULT)        |	// der McBSP1 als Slave läuft
        MCBSP_FMKS(SRGR, CLKSM, DEFAULT)        |	// und den Takt von aussen 
        MCBSP_FMKS(SRGR, FSGM, xxxxxxx)         |	// vorgegeben bekommt.
        MCBSP_FMKS(SRGR, FPER, DEFAULT)         |	// --
        MCBSP_FMKS(SRGR, FWID, DEFAULT)         |	// --
        MCBSP_FMKS(SRGR, CLKGDV, DEFAULT),		// --
		/* Mehrkanal */
        MCBSP_MCR_DEFAULT,				// Mehrkanal wird nicht verwendet
        MCBSP_RCER_DEFAULT,				// dito
        MCBSP_XCER_DEFAULT,				// dito
		/* Pinout Control Register */
        MCBSP_FMKS(PCR, XIOEN, SP)              |	// Pin wird für serielle Schnittstelle verwendet (alternativ GPIO)
        MCBSP_FMKS(PCR, RIOEN, SP)              |	// Pin wird für serielle Schnittstelle verwendet (alternativ GPIO)
        MCBSP_FMKS(PCR, FSXM, EXTERNAL)         |	// Framesync- Signal für Sender kommt von extern (Slave)
        MCBSP_FMKS(PCR, FSRM, EXTERNAL)         |	// Framesync- Signal für Empfänger kommt von extern (Slave)
        MCBSP_FMKS(PCR, CLKXM, INPUT)           |	// Takt für Sender kommt von extern (Slave)
        MCBSP_FMKS(PCR, CLKRM, INPUT)           |	// Takt für Empfänger kommt von extern (Slave)
        MCBSP_FMKS(PCR, CLKSSTAT, DEFAULT)      |	// unrelevant da PINS keine GPIOs
        MCBSP_FMKS(PCR, DXSTAT, DEFAULT)        |	// unrelevant da PINS keine GPIOs
        MCBSP_FMKS(PCR, FSXP, ACTIVEHIGH)       |	// Framesync senderseitig ist "activehigh"
        MCBSP_FMKS(PCR, FSRP, ACTIVEHIGH)       |	// Framesync empfängerseitig ist "activehigh"
        MCBSP_FMKS(PCR, CLKXP, FALLING)         |	// Datum wird bei fallender Flanke gesendet
        MCBSP_FMKS(PCR, CLKRP, RISING)			// Datum wird bei steigender Flanke übernommen
};

/* template for a EDMA configuration */
EDMA_Config configEDMARcv = {
    EDMA_FMKS(OPT, PRI, LOW)          |  // auf beide Queues verteilen
    EDMA_FMKS(OPT, ESIZE, xxxxx)       |  // Element size
    EDMA_FMKS(OPT, 2DS, NO)            |  // kein 2D-Transfer
    EDMA_FMKS(OPT, SUM, NONE)          |  // Quell-update mode -> FEST (McBSP)!!!
    EDMA_FMKS(OPT, 2DD, NO)            |  // 2kein 2D-Transfer
    EDMA_FMKS(OPT, DUM, xxx)           |  // Ziel-update mode
    EDMA_FMKS(OPT, TCINT,xxxx)         |  // EDMA interrupt erzeugen?
    EDMA_FMKS(OPT, TCC, OF(0))         |  // Transfer complete code (TCC)
    EDMA_FMKS(OPT, LINK, YES)          |  // Link Parameter nutzen?
    EDMA_FMKS(OPT, FS, NO),               // Frame Sync nutzen?

    EDMA_FMKS(SRC, SRC, OF(0)),           // Quell-Adresse

    EDMA_FMK (CNT, FRMCNT, NULL)       |  // Anzahl Frames
    EDMA_FMK (CNT, ELECNT, BUFFER_LEN),   // Anzahl Elemente

    (Uint32)Buffer_in_ping,       		  // Ziel-Adresse

    EDMA_FMKS(IDX, FRMIDX, DEFAULT)    |  // Frame index Wert
    EDMA_FMKS(IDX, ELEIDX, DEFAULT),      // Element index Wert

    EDMA_FMK (RLD, ELERLD, NULL)       |  // Reload Element
    EDMA_FMK (RLD, LINK, NULL)            // Reload Link
};

/* Transfer-Complete-Codes for EDMA-Jobs */
// 4 of em
int tccRcvPing;

/* EDMA-Handles; are these really all? */
// we need 3 for the transmit, and 3 for the receive
// we need both ping and pong in the reload
EDMA_Handle hEdmaRcv;  
EDMA_Handle hEdmaReload; 
						
								
								
								
main()
{
	MCBSP_Handle hMcbsp=0;
	
	
	CSL_init();  
	
	/* Configure McBSP0 and AIC23 */
	// when we want to parametrize this, we need to parametrize SP0 (for sending out data)
	// then we need to parametrize the ADC
	// this function does it
	Config_DSK6713_AIC23();
	

	// we read the samples from SP1
	// we parametrize
	/* Configure McBSP1*/
	hMcbsp = MCBSP_open(MCBSP_DEV1, MCBSP_OPEN_RESET);
    MCBSP_config(hMcbsp, &datainterface_config);
    
	/* configure EDMA */
    // we now want the EDMA to read the data for us from SP1, so:
    config_EDMA();

    /* finally the interrupts */
    // remember, interrupts need to be enabled
    config_interrupts();

    // we tell him to start, with xxxxxx telling us what to start
    MCBSP_start(hMcbsp, xxxxxx, 0xffffffff);
    MCBSP_write(hMcbsp, 0x0); 	/* one shot */
} /* finished*/



void config_EDMA(void)
{
	/* Konfiguration der EDMA zum Lesen*/
	hEdmaRcv = EDMA_open(EDMA_CHA_REVT1, EDMA_OPEN_RESET);  // EDMA Channel for REVT1
	hEdmaReload = EDMA_allocTable(-1);               // Reload-Parameters


	configEDMARcv.src = MCBSP_getRcvAddr(hMcbsp);          // delivers the address of the data receive register, which
														   // we write in the source addr

	tccRcvPing = EDMA_intAlloc(-1);                        // next available TCC (interrupt)
	configEDMARcv.opt |= EDMA_FMK(OPT,TCC,tccRcvPing);     // set it
	
	/* configure */
	EDMA_config(hEdmaRcv, &configEDMARcv);
	EDMA_config(hEdmaReload, &configEDMARcv);
	/* could we need also some other EDMA read job?*/
	// we do, we need configs for both ping and pong
	// so we need an additional config



	
	/* link transfers ping -> pong -> ping */
	EDMA_link(hEdmaRcv,xxxxxxx);  /* is that all? */
	// 3 calls of this function, and replace xxxxxx with corresponding handles


	/* do you want to hear music? */


	/* enable EDMA TCC */
	EDMA_intClear(tccRcvPing);
	EDMA_intEnable(tccRcvPing); // we do this for all 4 channels (setting the Tc INTERRUPT)
	/* some more? */

	/* which EDMAs do we have to enable? */
	EDMA_enableChannel(hEdmaRcv);
}


void config_interrupts(void)
{
	IRQ_map(IRQ_EVT_xxxx, 123456); // we map events (which int to which int number)
	IRQ_clear(xxxxx);
	IRQ_enable(xxxxx);
	IRQ_globalEnable();
}
// on our board, the ADC and DAC are on the same chip, and share the clock (there is only one clock and they both follow it)

void EDMA_interrupt_service(void)
{
	xxx int rcvPingDone=0; // replace with static to keep em from one function to the other
	xxx int rcvPongDone=0;
	xxx int xmtPingDone=0;
	xxx int xmtPongDone=0;
	
	if(EDMA_intTest(tccRcvPing)) {
		EDMA_intClear(tccRcvPing); /* clear is mandatory */
		rcvPingDone=1;
	}
	else if(xxxxxxxxx) {
		EDMA_intClear(yyyyyyyyyy);
		rcvPongDone=1;
	}
	
	........... // we do the same for transmit
	
	if(rcvPingDone && xmtPingDone) {
		rcvPingDone=0;
		xmtPingDone=0;
		// processing in SWI
		SWI_post(&SWI_process_ping);  // if it checks, ping buffers are filled, so we use the SWI for ping
	}
	else if(rcvPongDone && xmtPongDone) {
		rcvPongDone=0;
		xmtPongDone=0;
		// processing in SWI
		SWI_post(&SWI_process_pong);  // same shit for pong
	}
}

void process_ping_SWI(void)
{
	int i;
	for(i=0; i<BUFFER_LEN; i++)
		*(Buffer_out_ping+i) = *(Buffer_in_ping+i); 
}

void process_pong_SWI(void)
{
	int i;
	for(i=0; i<BUFFER_LEN; i++)
		*(Buffer_out_pong+i) = *(Buffer_in_pong+i); // GOLDEN WIRE
													// just copying the data from IN to OUT, to check if stuff works
	// THIS IS WHERE WE MIGHT ADD AMPLIFICATION, FOR THE DATA AT THE OUTPUT (ex: 2*)
}


// THat's it, what's below is experimentation with tasks
void SWI_LEDToggle(void)
{
	SEM_postBinary(&SEM_LEDToggle);	
}

void tsk_led_toggle(void)
{
	/* initializatoin of the task */
	/* nothing to do */
	
	/* process */
	while(1) {
		SEM_pendBinary(&SEM_LEDToggle, SYS_FOREVER); // calls LEDToggle, changes the LED, then goes back to sleep on the semaphore
		
		DSK6713_LED_toggle(1);
	}
}

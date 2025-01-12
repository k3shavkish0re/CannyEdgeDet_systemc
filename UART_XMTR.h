//===========================================
// Function : UART Transmitter 
//===========================================
#include "systemc.h"

// Define Variables
#define SIZE_BIT_COUNTER	3 
#define WORD_SIZE		8

#define STATE_IDLE		0
#define STATE_WAITING		1
#define STATE_SENDING		2

//#define _DEBUG_

// Module Definition
SC_MODULE (UART_XMTR){
	// Input/Output Signals
  	sc_in       < bool >		Load_XMT_datareg;
  	sc_in       < bool >		Byte_ready;
  	sc_in       < bool >		T_byte;
  	sc_in       < bool >		rst_b;
    sc_inout_rv <WORD_SIZE> Data_Bus;
  	sc_out      < bool >		Serial_out;
  	sc_in       < bool >		clk;

	// Internal Variables
  	sc_uint < SIZE_BIT_COUNTER >    IntState, NextIntState;
  	sc_uint < WORD_SIZE >			XMT_datareg;
  	sc_uint < WORD_SIZE+1 >			XMT_shftreg;
  	sc_uint < SIZE_BIT_COUNTER+1 >  bit_count;

  	// Functions Declaration
  	void Send_bit();
  	void Initialize();

  	// Constructor for the SC_MODULE
  	// sensitivity list
  	SC_CTOR(UART_XMTR) {
			// ...
			// Insert your code here
			// ...
      IntState = STATE_IDLE;
      NextIntState = STATE_IDLE;
      XMT_datareg = 0x0;
      XMT_shftreg = 0x0;
      bit_count = 0x0;
      SC_METHOD(Initialize);
      sensitive<<clk.pos()<<rst_b.neg();
      SC_METHOD(Send_bit);
      //sensitive<<clk.pos()<<Load_XMT_datareg<<Byte_ready<<T_byte<<rst_b<<Data_Bus;
      sensitive<<clk.pos()<<rst_b.neg();
  	}
};


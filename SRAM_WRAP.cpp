//===========================================
// Function : SRAM_WRAP.cpp
//===========================================
#include "SRAM_WRAP.h"

// ----- Code Starts Here ----- 
void SRAM_WRAP::Bus_Control() {
	sc_uint <4> Adr = AddressBus.read().to_uint() >> 28;
	sc_uint <1> bWRITE = (AddressBus.read().to_uint() >> 18) & 0x1;
 
	uint data = OutData.read().to_uint();    
	uint adr = AddressBus.read().to_uint();
	
	if(IntEnable){
		ControlBus.write(0);
	}
	else {
		ControlBus.write("Z");
	}

	if(IntEnable){
	 	if(Adr == 0x1 && bWRITE ==1)
			DataBus.write(data);
		else
			DataBus.write("ZZZZZZZZ");
	}
	else {
		DataBus.write("ZZZZZZZZ");
	}
}

void SRAM_WRAP::Function_SRAM_WRAP() {
	AddrDecoded = AddressBus.read().to_uint() >> 28;

	// Insert your code here.
	// ...
	// ...
	// ...
  //Reading the control signals values
  we_b_val = (AddressBus.read().to_uint() >> 18) & 0x1;
  ce_b_val = (AddressBus.read().to_uint() >> 19) & 0x1;
  Addr_val = (AddressBus.read().to_uint()) & 0x3FFFF;
  InData_val = (DataBus.read().to_uint());

	if(!bReset.read()){
		IntEnable = 0;
		Breq.write(0);
	}
	else if(IntEnable){
		if(AddrDecoded == 0x1) {// Address Decoding Matching
        	// Insert your code here.
	        // ...
	        // ...
	        // ...
        //assigning control signals values
        bCE.write(ce_b_val);
        bWE.write(we_b_val);
        InData.write(InData_val);
        Addr.write(Addr_val);
		}
		else {
			IntEnable = 0;
			Breq.write(0);
		}
	}
	else {	// !IntEnable
		if(Bgnt) {
			IntEnable = 1;
			Breq.write(0);
		}
		else if(AddrDecoded == 0x1){
			IntEnable = 0;
			Breq.write(1);
		}
		else {
			IntEnable = 0;
			Breq.write(0);
		}
	}
 }

//===========================================
// Function : UART_XMTR_WRAP.cpp
//===========================================
#include "UART_XMTR_WRAP.h"

// ----- Code Starts Here ----- 
void UART_XMTR_WRAP::Bus_Control() {
	uint bControl = ControlBus.read().to_uint();
	if(IntEnable){
		ControlBus.write(0);
	}
	else {
		ControlBus.write("Z");
	}
}

void UART_XMTR_WRAP::Function_UART_XMTR_WRAP() {
	AddrDecoded = AddressBus.read().to_uint() >> 28;

	// Insert your code here.
	// ...
	// ...
	// ...
  //Reading control signals values
  Data_Bus_val = (DataBus.read().to_uint());
  Load_XMT_datareg_val = (AddressBus.read().to_uint()) & 0x1;
  Byte_ready_val = (AddressBus.read().to_uint() >> 1) & 0x1;
  T_byte_val = (AddressBus.read().to_uint() >> 2) & 0x1;
 
	if(!bReset.read()){
		IntEnable = 0;
		Breq.write(0);
	}
  else if(IntEnable){
		if(AddrDecoded == 0x2) {// Address Decoding Matching
			// Insert your code here.
			// ...
			// ...
			// ...
      //assigning control signal values
      Data_Bus.write(Data_Bus_val);
      Load_XMT_datareg.write(Load_XMT_datareg_val);
      Byte_ready.write(Byte_ready_val);
      T_byte.write(T_byte_val);
      Breq.write(0);
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
		}else if(AddrDecoded == 0x2){
			IntEnable = 0;
			Breq.write(1);
		}else {
			IntEnable = 0;
			Breq.write(0);
		}
	}
 }

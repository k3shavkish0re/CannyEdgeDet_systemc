//===========================================
// Function : UART Transmitter
//===========================================
#include "UART_XMTR.h"

  // ----- Code Starts Here ----- 

void UART_XMTR::Send_bit() {
    switch(IntState) {
        case STATE_IDLE:
            if (rst_b == 0) {
                NextIntState = STATE_IDLE;
                bit_count = 0; // Reset bit_count on reset
            } else {
                if (Load_XMT_datareg == 1) {
                    XMT_datareg = Data_Bus;
                }
                if (Byte_ready == 1) {
                    sc_uint<9> one = 0x1;
                    XMT_shftreg = (XMT_datareg << 1) | one; // Concatenate data with start bit
                    NextIntState = STATE_WAITING;
                }
            }
            break;
        
        case STATE_WAITING:
            if (rst_b == 0) {
                NextIntState = STATE_IDLE;
                bit_count = 0; // Reset bit_count on reset
            } else {
                if (T_byte == 1) {
                    XMT_shftreg[0] = 0x0; // Add stop bit
                    NextIntState = STATE_SENDING;
                } else {
                    NextIntState = STATE_WAITING;
                }
            }
            break;
        
        case STATE_SENDING:
            if (rst_b == 0) {
                NextIntState = STATE_IDLE;
                bit_count = 0; // Reset bit_count on reset
            } else {
                if (bit_count >= 9) {
                    NextIntState = STATE_IDLE;
                    bit_count = 0; // Reset bit_count after sending
                } else {
                    // Shift right and insert bit in the MSB
                    sc_uint<9> temp = 0x100;
                    XMT_shftreg = (XMT_shftreg >> 1) | temp;
                    bit_count = bit_count + 1;
                }
            }
            break;
        
        default:
            NextIntState = STATE_IDLE;
    }
    Serial_out.write(XMT_shftreg[0]);
}


  void UART_XMTR::Initialize() {
	if(!rst_b.read()){
		IntState = STATE_IDLE;
		XMT_shftreg = 0x1ff;
		bit_count = 0;
	}
	else {
		IntState = NextIntState;

    //cout<<"posedge at: "<<sc_time_stamp()<<" rst_b: "<<rst_b<<" current state: "<<IntState<<" Load_XMT_datareg: "<<Load_XMT_datareg<<" Byte_ready: "<<Byte_ready<<" T_byte: "<<T_byte<<" Serial_out:"<<Serial_out<<" XMT_datareg: "<<XMT_datareg<<" XMT_shftreg: "<<XMT_shftreg<<" bit_count: "<<bit_count<<endl;
	}
  }


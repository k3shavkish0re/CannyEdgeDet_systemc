//===========================================
// Function : Canny Edge Detector
//===========================================
#include "Canny_Edge.h"

  // ----- Code Starts Here ----- 

void Canny_Edge::Write_Data() {
	if (!bCE.read() && !bWE.read()) {
		if(dWriteReg.read() == WRITE_REGX){
			regX[AddrRegRow.read()][AddrRegCol.read()] = InData.read();
      }
		// Insert Your Code here //
		// ...
		// ...
		// ...
    else if(dWriteReg.read() == WRITE_REGY)  //write regy option
      regY[AddrRegRow.read()][AddrRegCol.read()] = InData.read();
    else if(dWriteReg.read() == WRITE_REGZ)  //write regz option
      regZ[AddrRegRow.read()][AddrRegCol.read()] = InData.read();
		
		// For debug	
		#if defined (_DEBUG_)
		cout << "@" << sc_time_stamp() << ":: Write: " << InData.read() << endl;
		#endif
	}
}

void Canny_Edge::Read_Data() {
	if (!bCE.read() && bWE.read()) {
		unsigned int dData;
		if(dReadReg.read() ==  REG_GAUSSIAN) 		dData = Out_gf;
		// Insert Your Code here //
		// ...
		// ...
		// ...
    else if(dReadReg.read() == REG_GRADIENT){  //gradient operation
            dData = Out_gradient;
    }
    else if(dReadReg.read() == REG_DIRECTION) {  //direction operation
            dData = Out_direction;
    }
    else if(dReadReg.read() == REG_NMS){  //Non-maximum suppression operation
            dData = regX[AddrRegRow.read()][AddrRegCol.read()];
    }
    else if(dReadReg.read() == REG_HYSTERESIS){  //hysteresis operation
            dData = Out_bThres;
    }

		OutData.write(dData);
		
		// For debug	
		#if defined (_DEBUG_)
		cout << "@" << sc_time_stamp() << ":: Read[" << AddrRegRow.read() << "]";
		cout << "[" << AddrRegCol.read() << "]: " << dData << endl;
		#endif
	}
}

void Canny_Edge::Initialize() {
	if(!rst_b.read()){
		// Initialize 5x5 matrix
		short tRow, tCol;
		for(tRow=0; tRow<5; tRow++){
			for(tCol=0; tCol<5; tCol++){
				regX[tRow][tCol] = 0;
			}
		}
		// Initialize Gaussian matrix
		gf[0][0]=2;	gf[0][1]=4;	gf[0][2]=5;	gf[0][3]=4;	gf[0][4]=2;
		gf[1][0]=4;	gf[1][1]=9;	gf[1][2]=12;	gf[1][3]=9;	gf[1][4]=4;
		gf[2][0]=5;	gf[2][1]=12;	gf[2][2]=15;	gf[2][3]=12;	gf[2][4]=5;
		gf[3][0]=4;	gf[3][1]=9;	gf[3][2]=12;	gf[3][3]=9;	gf[3][4]=4;
		gf[4][0]=2;	gf[4][1]=4;	gf[4][2]=5;	gf[4][3]=4;	gf[4][4]=2;

		// Sobel dx
		Sobeldx[0][0]=-1;	Sobeldx[0][1]=0;	Sobeldx[0][2]=1;	
		Sobeldx[1][0]=-2;	Sobeldx[1][1]=0;	Sobeldx[1][2]=2;
		Sobeldx[2][0]=-1;	Sobeldx[2][1]=0;	Sobeldx[2][2]=1;
		// Sobel dy
		Sobeldy[0][0]=1;	Sobeldy[0][1]=2;	Sobeldy[0][2]=1;	
		Sobeldy[1][0]=0;	Sobeldy[1][1]=0;	Sobeldy[1][2]=0;
		Sobeldy[2][0]=-1;	Sobeldy[2][1]=-2;	Sobeldy[2][2]=-1;
	}
}

void Canny_Edge::Apply_Operation(){
	if(!bOPEnable.read()){
		if (OPMode.read() == MODE_GAUSSIAN){
			int c,d;	
			float sum = 0;
			// CONVOLUTION
			for(c=-2; c<=2; c++){
				for(d=-2; d<=2; d++){
					sum = sum + (float) regX[c+2][d+2] * gf[c+2][d+2];
				}
       		}
			Out_gf = char(sum/159);
	    	}
		else if(OPMode.read() == MODE_SOBEL){
			int c,d;
			short Gx=0;		// X direction Component
			short Gy=0;		// Y direction Component
	
			// 1. input : Sobeldx, Sobeldy, regX(Gaussian Filtered Image)
			// 2. Output : Out_gradient(0~255), Out_direction(0, 45, 90, 135)
			// Insert Your Code here //
			// ...
			// ...
			// ...
       for(c=-1; c<=1; c++) {
                for(d=-1; d<=1; d++) {
                    Gx += regX[c+1][d+1] * Sobeldx[c+1][d+1];    //convolution with sobel operator
                    Gy += regX[c+1][d+1] * Sobeldy[c+1][d+1];
                }
            }
       
       Out_gradient = (abs(Gx) + abs(Gy)) / 2;       //alpha = 2
       if (Out_gradient > 255) {
        Out_gradient = 255;
       }

       
       //approximation to calculate direction
       //first step
       if(Gy<0){
         Gx = -1 * Gx;
         Gy = -1 * Gy;
       }
       else{
         Gx = Gx;
         Gy = Gy;
       } 
       
       //second step
       if(Gx >= 0){
         if(Gy <= 0.4*Gx){
           Out_direction = 0;
         }
         else if(Gy > 0.4*Gx && Gy <= 2.4*Gx){
           Out_direction = 45;
         }
         else if(Gy > 2.4*Gx){
           Out_direction = 90;
         }
       }
       else{
         if(Gy <= -0.4*Gx){
           Out_direction = 0;
         }
         else if(Gy > -0.4*Gx && Gy <= -2.4*Gx){
           Out_direction = 135;
         }
         else if(Gy > -2.4*Gx){
           Out_direction = 90;
         }
       }


		}
		else if(OPMode.read() == MODE_NMS){
			// 1. input : regX(Gradient Image), regY(Direction Image)
			// 2. Output : regX(Gradient Image)
			// Insert Your Code here //
			// ...
			// ...
			// ...
      
      for (int i = 1; i < 4; i++) {
        for (int j = 1; j < 4; j++) {
            // Only process valid gradient values
            if (regX[i][j] >= 0) {
                int C = regX[i][j]; // Current pixel value
                bool keepC = true; // Flag to determine if C should be kept

                // Determine neighbors based on direction
                if (regY[i][j] == 0) { // Horizontal
                    int A = regX[i][j - 1];
                    int B = regX[i][j + 1];
                    if (C >= A && C >= B) {
                        // Suppress neighbors A and B
                        regX[i][j - 1] = 0;
                        regX[i][j + 1] = 0;
                    } else {
                        // C is not a local maximum
                        keepC = false;
                    }
                } else if (regY[i][j] == 45) { // Diagonal
                    int A = regX[i - 1][j + 1];
                    int B = regX[i + 1][j - 1];
                    if (C >= A && C >= B) {
                        regX[i - 1][j + 1] = 0;
                        regX[i + 1][j - 1] = 0;
                    } else {
                        keepC = false;
                    }
                } else if (regY[i][j] == 90) { // Vertical
                    int A = regX[i - 1][j];
                    int B = regX[i + 1][j];
                    if (C >= A && C >= B) {
                        regX[i - 1][j] = 0;
                        regX[i + 1][j] = 0;
                    } else {
                        keepC = false;
                    }
                } else if (regY[i][j] == 135) { // Diagonal
                    int A = regX[i - 1][j - 1];
                    int B = regX[i + 1][j + 1];
                    if (C >= A && C >= B) {
                        regX[i - 1][j - 1] = 0;
                        regX[i + 1][j + 1] = 0;
                    } else {
                        keepC = false;
                    }
                }

                // Suppress pixel C if it's not a local maximum
                if (!keepC) {
                    regX[i][j] = 0;
                }
            }
        }
      }

		}
		else if(OPMode.read() == MODE_HYSTERESIS){
			// You should use these two threshold values.
			unsigned short dThresHigh = 20;
			unsigned short dThresLow = 5;

			// 1. input : regX(Gradient Image), regY(Direction Image), regZ(On/Off Image)
			//            regZ[][]==1: On / regZ[][]==0: Off
			// 2. Output : Out_bThres (0(Off) or 1(On))
			// Insert Your Code here //
			// ...
			// ...
			// ...
      for (int i = 1; i < 4; i++) {
        for (int j = 1; j < 4; j++) {
        if (regX[i][j] >= dThresHigh) {
            regZ[i][j] = 1; // Strong pixel
        } 
        else if (regX[i][j] <= dThresLow) {
            regZ[i][j] = 0; // Weak pixel, suppress
        }
        else { // Candidate pixel
            bool connectedToStrong = false;

            // Check neighbors based on direction in regY
            if (regY[i][j] == 0) { // Horizontal direction
                if (regX[i][j-1] >= dThresHigh || regX[i][j+1] >= dThresHigh) {
                    connectedToStrong = true;
                }
            } else if (regY[i][j] == 45) { // Diagonal
                if (regX[i-1][j+1] >= dThresHigh || regX[i+1][j-1] >= dThresHigh) {
                    connectedToStrong = true;
                }
            } else if (regY[i][j] == 90) { // Vertical direction
                if (regX[i-1][j] >= dThresHigh || regX[i+1][j] >= dThresHigh) {
                    connectedToStrong = true;
                }
            } else if (regY[i][j] == 135) { // Diagonal
                if (regX[i-1][j-1] >= dThresHigh || regX[i+1][j+1] >= dThresHigh) {
                    connectedToStrong = true;
                }
            }

            if (connectedToStrong) {
                regZ[i][j] = 1; // Keep candidate pixel
            } else {
                regZ[i][j] = 0; // Discard candidate pixel
            }
        }
        }
      }

      // Output for Hysteresis
      Out_bThres = regZ[AddrRegRow.read()][AddrRegCol.read()];

		}
	}
}

/*  Finger Print Scanner Library na Nabibili sa eGizmo  Copyright (c) 2010 Andrew E. Bunac   This Library is Created Just for Lazy student of the Philippines*///*********************************************************************// INCLUDES//*****************************************de`****************************#include "Arduino.h" // Arduino 1.0 -- 2011 12 15 # Ben Schueler#include "FPS.h"//*********************************************************************// DEFINED//*************************fpScannerReceived********************************************//*****************************************e***************************		void FPS::setup(HardwareSerial *fp,int baudRate){		fpsSerial = fp;	}	void FPS::doReadMode(){		//this for loop will clear byte array sendDAta wala lang to pang linis lang		mode = READ_MODE;		for(int x = 0;x<24;x++){			fpScannerSend[x] = 0;		}		//if you going to read fPM you can see that 0xAA55  is code fore comand which will be send to fps        //and x0102 is command for Identify mode or scan mode ' ask fps manual about this		fpScannerSend[0] = 0x55;		fpScannerSend[1] = 0xAA;		fpScannerSend[2] = 0x2;		fpScannerSend[3] = 0x1;		checkSum();		//Serial.print(fpScannerSend[0]);		//Serial.write(fpScannerSend,24);		fpsSerial->write(fpScannerSend,24);	}	void FPS::checkSum(){		CKS=0;		//in check sum it is needed to get the summation of all data' in the case of fps only first 20 byte is needed to add		for(int x = 0; x < 22;x++){			CKS+=fpScannerSend[x];			}		// We will now append cpk to the last 2 bytes of our data since the cks is more than 256 we need to bytes         // to contain them we need to devide dem in to higher byte and lower byte        // first, lower byte of the  cpk it something like getting the mod of 255(hFF) here or just get the remainder 		fpScannerSend[22] = CKS &  0xFF;		//second, higher byte of the cpk  in this part you just         //count how many  byte are there in cks (devide it by 256 then you can get it)		fpScannerSend[23] = (CKS - (CKS &0xFF))/256;	}	byte FPS::monitorFPS(){		byte returnValue = NULL;				if(fpsSerial->available()>0){			byte index = 0;			int timer = 0;			while(timer<100){				while(fpsSerial->available()){					fpScannerReceived[index] = fpsSerial->read();					index++;					timer=0;				}				timer++;			}			fpsSerial->flush();//cleanup  serial buffer						if(fpScannerReceived[6] == 0 && index>=24){				if(fpScannerReceived[8] == 244 || fpScannerReceived[9]==0xFF){					returnValue =  NULL;//JUST RETURN NOTHING				}else{									}					//Get the Detected User fpScannerReceived[9]*256 +					returnValue= fpScannerReceived[8];							}else{				//user not match				returnValue =  FPS_ERROR;			}						//clean buffer			for(int x = 0; x<24;x++){				fpScannerReceived[x] = '\0';			}		}		if( returnValue >200) return NULL; // THERE IS NO INDEX NO THAT IS GREATER THAN 100 		return returnValue;			}					
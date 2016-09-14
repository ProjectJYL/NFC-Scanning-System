//
//  opCodes.h
//  MagicWandInteraction
//
//  Created by RIPL on 12/12/14.
//  Copyright (c) 2014 rangam.com. All rights reserved.
//

#ifndef MagicWandInteraction_opCodes_h
#define MagicWandInteraction_opCodes_h

typedef enum {
    
    
//===============================For App to Hardware OpCodes   =======================================
    
    kTurn_Reader_On_For_Registration = 0x31,
    kTurn_Reader_Off = 0x32,
    kGood_Job = 0x33,
    kTry_Again = 0x34,
    
    kTurn_Reader_On_For_Reading = 0x35,   /// This will work similar to 0x31
	kWrite_On_Reader = 0x36,
	kWrite_Device_Name = 0x37,
	kShow_Image = 0x38,
	kStart_Img_Transfer = 0x3A,
    
    //=============================== Seprator for opCode and Data   =======================================
    
    kopCode_Data_Separator = 0x26,  // &

    //=============================== Seprator for opCode and Data   =======================================


//===============================For Hardware to App OpCodes   =======================================
    kReader_Turned_On_For_Registration = 0x41,
    kReader_Turned_Off = 0x42,
	kAuthentication_Failed = 0x43,
	kReader_Turned_On_For_Reading = 0x45,
    kWrite_On_Reader_Successful = 0x46,
    kWrite_On_Reader_Fail = 0x47,
	kImage_Transfer_Ack = 0x48,
    kWrite_Device_Name_Ok = 0x49,
    kWrite_Device_Name_Failed = 0x50
    
    
    // For Hardware to App OpCodes

} opCodes;


#endif

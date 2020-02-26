/*
    ModbusSerial.cpp - Source for Modbus Serial Library
    Copyright (C) 2014 André Sarmento Barbosa
*/
#include "ModbusSerial.h"

//自己加的
unsigned char G_modbusPacket[20] = { 0x00 }; 
int G_modbusPacket_Length = 0;

ModbusSerial::ModbusSerial() {

}

bool ModbusSerial::setSlaveId(byte slaveId){
    _slaveId = slaveId;
    return true;
}

byte ModbusSerial::getSlaveId() {
    return _slaveId;
}

bool ModbusSerial::config(HardwareSerial* port, long baud, u_int format, int txPin) {
    this->_port = port;
    this->_txPin = txPin;
    (*port).begin(baud, format);

    delay(2000);

    if (txPin >= 0) {
        pinMode(txPin, OUTPUT);
        digitalWrite(txPin, LOW);
    }

    if (baud > 19200) {
        _t15 = 750;
        _t35 = 1750;
    } else {
        _t15 = 15000000/baud; // 1T * 1.5 = T1.5
        _t35 = 35000000/baud; // 1T * 3.5 = T3.5
    }

    return true;
}

#ifdef USE_SOFTWARE_SERIAL
bool ModbusSerial::config(SoftwareSerial* port, long baud, int txPin) {
    this->_port = port;
    this->_txPin = txPin;
    (*port).begin(baud);

    delay(2000);

    if (txPin >= 0) {
        pinMode(txPin, OUTPUT);
        digitalWrite(txPin, LOW);
    }

    if (baud > 19200) {
        _t15 = 750;
        _t35 = 1750;
    } else {
        _t15 = 15000000/baud; // 1T * 1.5 = T1.5
        _t35 = 35000000/baud; // 1T * 3.5 = T3.5
    }

    return true;
}
#endif

#ifdef __AVR_ATmega32U4__
bool ModbusSerial::config(Serial_* port, long baud, u_int format, int txPin) {
    this->_port = port;
    this->_txPin = txPin;
    (*port).begin(baud, format);
    while (!(*port));

    if (txPin >= 0) {
        pinMode(txPin, OUTPUT);
        digitalWrite(txPin, LOW);
    }

    if (baud > 19200) {
        _t15 = 750;
        _t35 = 1750;
    } else {
        _t15 = 15000000/baud; // 1T * 1.5 = T1.5
        _t35 = 35000000/baud; // 1T * 3.5 = T3.5
    }

    return true;
}
#endif

bool ModbusSerial::receive(byte* frame) {
    //first byte of frame = address
    byte address = frame[0];
    //Last two bytes = crc
    u_int crc = ((frame[_len - 2] << 8) | frame[_len - 1]);

    //Slave Check
    if (address != 0xFF && address != this->getSlaveId()) {
		Serial.println("Modbus address error!!! <ModbusSerial::receive>");
		return false;
	}

    //CRC Check
    if (crc != this->calcCrc(_frame[0], _frame+1, _len-3)) {
		Serial.println("Modbus crc error!!! <ModbusSerial::receive>");
		return false;
    }

    //PDU starts after first byte
    //framesize PDU = framesize - address(1) - crc(2)
    this->receivePDU(frame+1);
    //No reply to Broadcasts
    if (address == 0xFF) _reply = MB_REPLY_OFF;
    return true;
}

bool ModbusSerial::send(byte* frame) {
    byte i;

    if (this->_txPin >= 0) {
        digitalWrite(this->_txPin, HIGH);
        delay(1);
    }

	//这里是正常的回执，当_reply == MB_REPLY_ECHO，发的指令是什么就回执什么，应该从这里截取出来添加到回执的协议帧
    /*for (i = 0 ; i < _len ; i++) {
        (*_port).write(frame[i]);
    }*/
	//Get_Modbus_Send_frame(byte* frame);//
	//Get_Modbus_Send_len(byte _len);//
	for (i = 0; i < _len; i++)
	{
		G_modbusPacket[G_modbusPacket_Length++] = *(frame+i);
		Serial.print(String(G_modbusPacket[i], HEX) + " ");
	}
	Serial.println("ModbusSerial::send <ModbusSerial::send>");
	//Serial.write(G_modbusPacket, G_modbusPacket_Length);

    (*_port).flush();
    delayMicroseconds(_t35);

    if (this->_txPin >= 0) {
        digitalWrite(this->_txPin, LOW);
    }
}

////自己添加的函数，传入数组，返回数组
//unsigned char * ModbusSerial::Get_Modbus_Send_frame(unsigned char * frame)
//{
//	return nullptr;
//}
//
////自己添加的函数，传入数组的长度，返回数组的长度
//unsigned char ModbusSerial::Get_Modbus_Send_len(unsigned char len)
//{
//	return 0;
//}

bool ModbusSerial::sendPDU(byte* pduframe) {
    if (this->_txPin >= 0) {
        digitalWrite(this->_txPin, HIGH);
        delay(1);
    }
	Serial.println("ModbusSerial::sendPDU <sendPDU>");
	//G_modbusPacket[G_modbusPacket_Length++] = *(frame + i);

    //Send slaveId
    //(*_port).write(_slaveId);
	G_modbusPacket[G_modbusPacket_Length++] = _slaveId;

    ////Send PDU
    byte i;
    //for (i = 0 ; i < _len ; i++) {
    //    (*_port).write(pduframe[i]);
    //}
	for (i = 0; i < _len; i++)
	{
		G_modbusPacket[G_modbusPacket_Length++] = *(pduframe + i);
	}

    ////Send CRC
    word crc = calcCrc(_slaveId, _frame, _len);
    //(*_port).write(crc >> 8);
	G_modbusPacket[G_modbusPacket_Length++] = crc >> 8;
    //(*_port).write(crc & 0xFF);
	G_modbusPacket[G_modbusPacket_Length++] = crc & 0xFF;


    //(*_port).flush();
    delayMicroseconds(_t35);

    if (this->_txPin >= 0) {
        digitalWrite(this->_txPin, LOW);
    }
}

//void ModbusSerial::task() {
//    _len = 0;
//
//    while ((*_port).available() > _len)	{
//        _len = (*_port).available();
//        delayMicroseconds(_t15);
//    }
//
//    if (_len == 0) return;
//
//    byte i;
//    _frame = (byte*) malloc(_len);
//    for (i=0 ; i < _len ; i++) _frame[i] = (*_port).read();
//
//    if (this->receive(_frame)) {
//        if (_reply == MB_REPLY_NORMAL)
//            this->sendPDU(_frame);
//        else
//        if (_reply == MB_REPLY_ECHO)
//            this->send(_frame);
//    }
//
//    free(_frame);
//    _len = 0;
//}

void ModbusSerial::task(unsigned char* modbusPacket ,int Length) {
	_len = Length;
	//_len = 0;
	//while ((*_port).available() > _len) {
	//	_len = (*_port).available();
	//	delayMicroseconds(_t15);
	//}

	if (_len == 0) return;

	Serial.println("Test <task>");
	byte i;
	_frame = (byte*)malloc(_len);
	//for (i = 0; i < _len; i++) _frame[i] = (*_port).read();
	for (i = 0; i < _len; i++) _frame[i] = modbusPacket[i];

	Serial.println("--------");
	for (size_t i = 0; i < _len; i++)
	{
		Serial.println(_frame[i], HEX);
	}
	Serial.println("--------");


	if (this->receive(_frame)) {
		if (_reply == MB_REPLY_NORMAL)
			this->sendPDU(_frame);
		else
			if (_reply == MB_REPLY_ECHO)
				this->send(_frame);
	}

	free(_frame);
	_len = 0;
}

word ModbusSerial::calcCrc(byte address, byte* pduFrame, byte pduLen) {
	byte CRCHi = 0xFF, CRCLo = 0x0FF, Index;

    Index = CRCHi ^ address;
    CRCHi = CRCLo ^ _auchCRCHi[Index];
    CRCLo = _auchCRCLo[Index];

    while (pduLen--) {
        Index = CRCHi ^ *pduFrame++;
        CRCHi = CRCLo ^ _auchCRCHi[Index];
        CRCLo = _auchCRCLo[Index];
    }

    return (CRCHi << 8) | CRCLo;
}






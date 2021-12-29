#ifndef _MAX7219DMD_H
#define _MAX7219DMD_H

#include "bytecon.h"

#define CLK(x) _SET(CLK_PORT,CLK_Bit,x)
#define DIN(x) _SET(DIN_PORT,DIN_Bit,x)

class MAX7219DMD{
	public:
		MAX7219DMD(uint8_t,volatile uint8_t*,volatile uint8_t*,uint8_t);
		void Send(uint8_t,uint8_t);
		void Send(uint8_t,uint8_t,uint8_t);
		void Init(void);
		void Clear(void);
		void Brightness(uint8_t,uint8_t);	
		void Cursor(long int,int);
		void WriteStr(const char*);
	private:
		uint8_t _casc_len;
		volatile uint8_t *_cs_ddr;
		volatile uint8_t *_cs_port;
		uint8_t _cs_bit;
		long int _cursor_x;
		int _cursor_y;
};
MAX7219DMD::MAX7219DMD(uint8_t cascade_len,volatile uint8_t *CS_DDR,volatile uint8_t *CS_PORT,uint8_t CS_Bit){
	_casc_len=cascade_len;
	_cs_ddr=CS_DDR;
	_cs_port=CS_PORT;
	_cs_bit=CS_Bit;
}
void MAX7219DMD::Send(uint8_t addr, uint8_t data){
	for(uint8_t sel=8;sel>0;sel--){
		DIN(_READ(&addr,sel-1));
		CLK(1);
		CLK(0);}
	for(uint8_t sel=8;sel>0;sel--){
		DIN(_READ(&data,sel-1));
		CLK(1);
		CLK(0);}
}
void MAX7219DMD::Send(uint8_t casc,uint8_t addr,uint8_t data){
	for(uint8_t lp=0;lp<_casc_len-(casc-1);lp++){Send(0,0);}
	Send(addr,data);
	for(uint8_t lp=0;lp<(casc-1);lp++){Send(0,1);}
}
void MAX7219DMD::Clear(){
	for(uint8_t l=0;l<8;l++){
		_SET(_cs_port,_cs_bit,0);
		for(uint8_t s=0;s<_casc_len;s++){Send(1+l,0);}
	_SET(_cs_port,_cs_bit,1);}
}
void MAX7219DMD::Init(){
	_SET(_cs_ddr,_cs_bit,1);
	_SET(_cs_port,_cs_bit,1);
	_SET(CLK_DDR,CLK_Bit,1);
	_SET(CLK_PORT,CLK_Bit,0);
	_SET(DIN_DDR,DIN_Bit,1);
	_SET(DIN_PORT,DIN_Bit,0);
	_SET(_cs_port,_cs_bit,0);
	for(uint8_t s=0;s<_casc_len;s++){Send(0x0C,0);}
	_SET(_cs_port,_cs_bit,1);
	_SET(_cs_port,_cs_bit,0);
	for(uint8_t s=0;s<_casc_len;s++){Send(0x0F,0);}
	_SET(_cs_port,_cs_bit,1);
	_SET(_cs_port,_cs_bit,0);
	for(uint8_t s=0;s<_casc_len;s++){Send(0x09,0);}
	_SET(_cs_port,_cs_bit,1);
	_SET(_cs_port,_cs_bit,0);
	for(uint8_t s=0;s<_casc_len;s++){Send(0x0B,7);}
	_SET(_cs_port,_cs_bit,1);
	_SET(_cs_port,_cs_bit,0);
	for(uint8_t s=0;s<_casc_len;s++){Send(0x0A,0);}
	_SET(_cs_port,_cs_bit,1);
	Clear();
	_SET(_cs_port,_cs_bit,0);
	for(uint8_t s=0;s<_casc_len;s++){Send(0x0C,1);}
	_SET(_cs_port,_cs_bit,1);
	_cursor_x=0;
	_cursor_y=0;
}
void MAX7219DMD::Brightness(uint8_t Cascade_n,uint8_t Level){
	_SET(_cs_port,_cs_bit,0);
	Send(Cascade_n,0x0A,Level);
	_SET(_cs_port,_cs_bit,1);
}
void MAX7219DMD::Cursor(long int x,int y){
	_cursor_x=x;
	_cursor_y=y;
}
void MAX7219DMD::WriteStr(const char* data){
	for(uint8_t b3=(_cursor_y>0?_cursor_y:0);b3<8-(_cursor_y<0?_cursor_y*-1:0);b3++){
		uint8_t dlen=0;
		while(data[dlen]!=0){dlen++;}
		uint16_t cr_x=(_cursor_x>0?_cursor_x:0);
		long int ptr_x=(_cursor_x<0?_cursor_x*-1:0);
		uint8_t dig=0;
		uint8_t buff=0;
		uint8_t ptr=0;
		int ln=pgm_read_byte(&_font1_ascii1[data[ptr]-' '][5])+1;
		uint8_t pos=0;
		bool rlp=1;
		if(_cursor_x<0){
			if(ln-ptr_x<=0){
				ptr++;
				ptr_x-=ln;
				pos=ptr_x;
				ln=(pgm_read_byte(&_font1_ascii1[data[ptr]-' '][5])+1);
				ptr_x-=ln;
				ln-=pos;
				while(ln-ptr_x<=0){
					ptr++;
					pos=ptr_x;
					ln=(pgm_read_byte(&_font1_ascii1[data[ptr]-' '][5])+1);
					ptr_x-=ln;
					ln-=pos;}}
			else{
				pos=ptr_x;
				ln-=pos;
				ptr=0;}}
		_SET(_cs_port,_cs_bit,0);
		while(ptr<=dlen&&dig<_casc_len){
			if(rlp==1){
				if(_cursor_x>7){Send(0,0);cr_x=_cursor_x-8;dig++;}
				if(_cursor_x>15){Send(0,0);cr_x=_cursor_x-16;dig++;}
				if(_cursor_x>23){Send(0,0);cr_x=_cursor_x-24;dig++;}}
			for(uint8_t b1=(rlp==1?cr_x:0);b1<8;b1++){
				rlp=0;
				if(ln==0){
					ptr++;
					pos=0;
					if(data[ptr]==0){
						ln=-1;}
					else{ln=pgm_read_byte(&_font1_ascii1[data[ptr]-' '][5])+1;}}
				_SET(&buff,7-b1,(ln>1?((pgm_read_byte(&_font1_ascii1[data[ptr]-' '][pos])>>b3)&1):0));
				if(ln>0){ln--;}
				pos++;}
			Send(b3+1-_cursor_y,buff);
			dig++;
		}
		if(dig<4){Send(0,0);}
		if(dig<3){Send(0,0);}
		if(dig<2){Send(0,0);}
		_SET(_cs_port,_cs_bit,1);
		if(_cursor_y<0&&b3<1){
			_SET(_cs_port,_cs_bit,0);
			for(uint8_t lp=0;lp<_casc_len;lp++){
				Send(b3-_cursor_y,0);}
			_SET(_cs_port,_cs_bit,1);}
	}
	if(_cursor_y<-7){
			_SET(_cs_port,_cs_bit,0);
			for(uint8_t lp=0;lp<_casc_len;lp++){
				Send(8,0);}
			_SET(_cs_port,_cs_bit,1);
	}
}
#endif
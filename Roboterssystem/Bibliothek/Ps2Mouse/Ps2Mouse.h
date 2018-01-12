#ifndef __Ps2Mouse__
#define __Ps2Mouse__

#include <Arduino.h>


class Ps2Mouse {

	private: 
		    int MDATA, MCLK;
	public:
          Ps2Mouse (int MDATA, int MCLK);
		  void gohi(int pin);
		  void golo(int pin);
		  void write(char data);
		  char read(void);
		  void init();
		  
};

#endif
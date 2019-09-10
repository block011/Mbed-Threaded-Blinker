#ifndef BLINKER
#define BLINKER

#include "mbed.h"
#include "rtos.h"

Thread myBlinkerThread;
/*
   A Blinker class used to blink DigitalOut in specific patterns. {{ USES mbed.h AND rtos.h }}
	 Primarily created for led lights to mark progress or warn of an error.
	 
	 Created for CS435 Embedded Systems
	 Created by Alex Block
	 
	 MODES:
		NORMAL : a slow constant blink
		ERROR : a very fast constant blink
		PROCESSING : a fast blink followed by a slow blink
		OFF : no blink at all. Used if you want to keep the process running but don't want a signal to be passed through
	 
			* This program is free software: you can redistribute it and/or modify
			* it under the terms of the GNU General Public License as published by
			* the Free Software Foundation, either version 3 of the License, or
			* (at your option) any later version.
			*
			* This program is distributed in the hope that it will be useful,
			* but WITHOUT ANY WARRANTY; without even the implied warranty of
			* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			* GNU General Public License for more details.
			*
			* You should have received a copy of the GNU General Public License
			* along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/
class Blinker{
	public:
		//List of modes we currently have
		enum modes {NORMAL, ERROR, OFF, PROCESSING};
	
		//constructor and destructor 
		Blinker();
		Blinker(Blinker::modes myMode, DigitalOut led);
		~Blinker();


		//User can select mode
		void mode(Blinker::modes myMode);
		
		//User can select led
		void led(DigitalOut led);

		//Starts blinking thread for a specific led
		void start();

		//Stops blinking thread
		void stop();
		
	private:
		modes curMode;
		bool isOn;
		DigitalOut* myled;
		
		void blink(Blinker::modes myMode);
		
		//functions for different blinking patterns
		void normalBlink();
		void offBlink();
		void errorBlink();
		void processingBlink();
		
		//throws blinks
		void blinkTime(double time, double offtime = 0.1);
		void startThread();
};

//CONSTRUCTORS / DESTRUCTORS ######################
inline Blinker::Blinker(){
	DigitalOut myLed(LED1);
	Blinker(Blinker::NORMAL, myLed);
}

inline Blinker::Blinker(Blinker::modes myMode, DigitalOut myLed){
	mode(myMode);
	led(myLed);
}

inline Blinker::~Blinker(){
	myBlinkerThread.terminate();
}
//#################################################


//SETTERS #########################################
inline void Blinker::mode(Blinker::modes myMode){
	curMode = myMode;
}
inline void Blinker::led(DigitalOut led) {
	myled = &led;
}
//#################################################


inline void Blinker::start(){
	isOn = true;
	myBlinkerThread.start(callback(this, &Blinker::startThread));
}


inline void Blinker::stop(){
	isOn = false;
}



inline void Blinker::startThread(){
	while(true){
		blink(curMode);
		if(!isOn){
			myBlinkerThread.terminate();
		}
	}
}

inline void Blinker::blink(Blinker::modes myMode){
	switch(myMode){
		case NORMAL:
			normalBlink();
			break;
		case OFF:
			offBlink();
			break;
		case ERROR:
			errorBlink();
			break;
		case PROCESSING:
			processingBlink();
			break;
	}
}


inline void Blinker::normalBlink(){
	blinkTime(1.0,1.0);
	return;
}
inline void Blinker::offBlink(){
	wait(1);
	return;
}
inline void Blinker::errorBlink(){
	blinkTime(.15,.15);
	return;
}
inline void Blinker::processingBlink(){
	blinkTime(.5,.25);
	blinkTime(.15,.15);
	return;
}

inline void Blinker::blinkTime(double time, double offtime) {
	//offtime default to 0.1
	*myled = 1;
	wait(time);
	*myled = 0;
	wait(offtime);
	return;
}
#endif

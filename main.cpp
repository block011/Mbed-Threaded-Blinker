
#include "mbed.h"
#include "Blinker.h"

DigitalOut myled(LED1);
int main() {
	Blinker* loader = new Blinker(Blinker::NORMAL, myled);
	loader->start();
	wait(3);
	loader->mode(Blinker::ERROR);
  wait(3);
	loader->mode(Blinker::PROCESSING);
	wait(3);
	loader->stop();
	wait(3);
	
	
	loader->mode(Blinker::NORMAL);
	loader->start();
	wait(3);
	loader->mode(Blinker::ERROR);
  wait(3);
	loader->mode(Blinker::PROCESSING);
	wait(3);
	loader->mode(Blinker::OFF);
	wait(3);
	loader->stop();
}

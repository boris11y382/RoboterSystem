#include "UltraschallSensor.h"

UltraschallSensor::UltraschallSensor (int trigger, int echo) {
          this->trigger = trigger;
		  this->echo = echo;
}

void UltraschallSensor::setMode() {
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
}

double UltraschallSensor::getEntfernung() {
	digitalWrite(trigger, LOW); //Hier nimmt man die Spannung für kurze Zeit vom Trigger-Pin, damit man später beim senden des Trigger-Signals ein rauschfreies Signal hat.
	delay(5); //Dauer: 5 Millisekunden
	digitalWrite(trigger, HIGH); //Jetzt sendet man eine Ultraschallwelle los.
	delay(5); //Dieser „Ton“ erklingt für 5 Millisekunden.
	digitalWrite(trigger, LOW);//Dann wird der „Ton“ abgeschaltet.
	dauer = pulseIn(echo, HIGH); //Mit dem Befehl „pulseIn“ zählt der Mikrokontroller die Zeit in Mikrosekunden, bis der Schall zum Ultraschallsensor zurückkehrt.
	entfernung = (dauer/2) * 0.03432; //Nun berechnet man die Entfernung in Zentimetern. Man teilt zunächst die Zeit durch zwei (Weil man ja nur eine Strecke berechnen möchte und nicht die Strecke hin- und zurück). Den Wert multipliziert man mit der Schallgeschwindigkeit in der Einheit Zentimeter/Mikrosekunde und erhält dann den Wert in Zentimetern.
    return entfernung;
}
#include <Ps2Mouse.h>


#define MDATA 11
#define MCLK 10

Ps2Mouse mouse(MDATA, MCLK);

void setup()
{
  Serial.begin(9600);
  Serial.println("Mouse is getting initialized");
  mouse.init();
  Serial.println("Mouse initialized");
}

/*
 * get a reading from the mouse and report it back to the
 * host via the serial line.
 */
void loop()
{
  char mstat;
  char mx;
  char my;

  /* get a reading from the mouse */
  mouse.write(0xeb);  /* give me data! */
  mouse.read();      /* ignore ack */
  mstat = mouse.read();
  mx = mouse.read();
  my = mouse.read();

  /* send the data back up */
  Serial.print(mstat, BIN);
  Serial.print("\tX=");
  Serial.print(mx, DEC);
  Serial.print("\tY=");
  Serial.print(my, DEC);
  Serial.println();
  delay(20);  /* twiddle */
}

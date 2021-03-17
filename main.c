#include <stdbool.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "timer.h"
#include "servo.h"

int main()
{
	servo_init();

	while(1)
	{
	    move_servo(60);
	    timer_waitMillis(1000);
	    move_servo(120);
        timer_waitMillis(1000);
	}
}

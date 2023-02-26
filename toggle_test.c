#include <stdio.h>
#include <signal.h>

#include <pigpio.h>

// PI GPIO's we are using for the buttons
#define GPIO1 6
#define GPIO2 22

// Handles Ctrl-C exits
volatile int run = 0;
static void stop(int signum) {
  run = 0;
}

int main(int argc, char *argv[]) {
  if (gpioInitialise() < 0) {
    printf("init failed\n");
    return -1;
  }

  gpioSetMode(GPIO1, PI_OUTPUT);
  gpioSetMode(GPIO2, PI_OUTPUT);

  gpioSetSignalFunc(SIGINT, stop);
  printf("ctrl-c to stop\n");

  run = 1;
  while (run) {
    gpioWrite(GPIO1, PI_HIGH);
    time_sleep(1);
    gpioWrite(GPIO2, PI_HIGH);
    time_sleep(1);
    gpioWrite(GPIO1, PI_LOW);
    time_sleep(1);
    gpioWrite(GPIO2, PI_LOW);
    time_sleep(1);
  }

  printf("\ntidying up\n");
  gpioSetMode(GPIO1, PI_INPUT);
  gpioSetMode(GPIO2, PI_INPUT);
  gpioTerminate();
  return 0;
}

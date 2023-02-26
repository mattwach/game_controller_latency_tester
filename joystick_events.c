#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>
#include <linux/joystick.h>

#define DEVICE "/dev/input/js0"

static uint64_t get_ms(void) {
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);

  return (uint64_t)(t.tv_sec) * 1000 + (uint64_t)(t.tv_nsec) / 1000000;
}

int main(int argc, char *argv[]) {
  const char *device;
  struct js_event event;

  const int jsfd = open(DEVICE, O_RDONLY);
  if (jsfd == -1)
    perror("Could not open joystick");

  uint32_t start_ms = get_ms();

  while (1) {
    const ssize_t bytes = read(jsfd, event, sizeof(*event));
    if (bytes != sizeof(*event)) {
      break;
    }
    uint32_t delta_ms = get_ms() - start_ms;

    const char* name = "OTHER";
    switch (event.type) {
      case JS_EVENT_BUTTON:
        name = "BUTTON";
        break;
      case JS_EVENT_AXIS:
        name = "AXIS";
        break;
      default:
        break;
    }
    printf(
        "%u ms: %s %u %u\n",
        delta_ms,
        name,
        event.number,
        event.value);

    fflush(stdout);
  }

  close(jsfd);
  return 0;
}

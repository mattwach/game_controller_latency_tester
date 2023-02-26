#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <pigpio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <math.h>
#include <linux/joystick.h>

// which raspberry pi GPIO will trigger the joystick
//#define TRIGGER_GPIO 6
#define TRIGGER_GPIO 22

// how long to wait between toggles
#define TOGGLE_PERIOD_MS 5

// how many samples to collect
#define SAMPLES_TO_COLLECT 5000

// approx how often to report
#define REPORT_MS 1000

// joystick device
#define JOYSTICK_DEVICE "/dev/input/js0"

// Statistics to track
struct Stats {
  uint32_t max_latency_us;
  uint32_t min_latency_us;
  uint64_t total_latency_us;
  uint32_t sample_count;
  uint32_t sample_us[SAMPLES_TO_COLLECT];
} stats;


// Used to support ctrl-C
volatile int run = 0;

// Reads in a single joystick event
static int read_js_event(int fd, struct js_event *event) {
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}

// signal handler callback, when ctrl-c is pressed
static void stop(int signum) {
  run = 0;
}

// stops using GPIO
static void stopGPIO(void) {
  printf("\ntidying up\n");
  gpioSetMode(TRIGGER_GPIO, PI_INPUT);
  gpioTerminate();
}

// initialize GPIO and joystick.  Returns 0 if successful.
static int init(int* js_device) {
  if (gpioInitialise() < 0) {
    printf("init failed\n");
    return -1;
  }
  gpioSetMode(TRIGGER_GPIO, PI_OUTPUT);

  *js_device = open(JOYSTICK_DEVICE, O_RDONLY);
  if (*js_device == -1) {
    perror("Could not open JOYSTICK_DEVICE");
    stopGPIO();
    return -1;
  }

  gpioSetSignalFunc(SIGINT, stop);
  printf("ctrl-c to stop, or wait %d ms\n", TOGGLE_PERIOD_MS * SAMPLES_TO_COLLECT);

  return 0;
}

// Returns UNIX epoch as a millisecond value
static uint64_t get_ms(void) {
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);

  return (uint64_t)(t.tv_sec) * 1000 + (uint64_t)(t.tv_nsec) / 1000000;
}

// Returns UNIX epoch as a microsecond value
static uint64_t get_us(void) {
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);

  return (uint64_t)(t.tv_sec) * 1000000 + (uint64_t)(t.tv_nsec) / 1000;
}

// Sleeps for the given number of milliseconds.  Uses select
// to avoid a busy-wait penalty from the scheduler.  Accuracy
// will not be perfect but "good enough" for this usecase.
static void sleep_ms(uint32_t ms) {
  struct timeval tv;
  tv.tv_sec = ms / 1000;
  tv.tv_usec = (ms % 1000) * 1000;
  select(0, NULL, NULL, NULL, &tv);
}

// Toggles the GPIO, waits for a joystick event to occur, records the
// timng stats.
static int collect_sample(int js_device, int toggle_led) {
  struct js_event event;

  gpioWrite(TRIGGER_GPIO, toggle_led ? PI_HIGH : PI_LOW);

  const uint64_t start_us = get_us(); 
  while (1) {
    const int err = read_js_event(js_device, &event);
    if ((err == 0) && (event.type == JS_EVENT_BUTTON)) {
      break;
    }
    if (err < 0) {
      printf("read_js_event  error\n");
    }
  }
  const uint64_t end_us = get_us(); 


  const uint32_t sample_us = end_us - start_us;
  stats.sample_us[stats.sample_count] = sample_us;
  stats.total_latency_us += sample_us;
  if (sample_us < stats.min_latency_us) {
    stats.min_latency_us = sample_us;
  }
  if (sample_us > stats.max_latency_us) {
    stats.max_latency_us = sample_us;
  }
  ++stats.sample_count;

  return 1 - toggle_led;
}

// Prints the current stats to stdout
static void report_running_stats(void) {
  printf("samples: %u  min_us: %u  max_us: %u  last_us: %u  avg_us: %u\n",
      stats.sample_count,
      stats.min_latency_us,
      stats.max_latency_us,
      stats.sample_count > 0 ? stats.sample_us[stats.sample_count - 1] : 0,
      (uint32_t)(stats.total_latency_us / stats.sample_count));
}

// calculate standard deviation
static uint32_t calculate_std_dev(void) {
  const int32_t average_latency = (int32_t)(stats.total_latency_us / stats.sample_count);
  double squared_sum = 0;
  for (int i=0; i<stats.sample_count; ++i) {
    const int32_t delta = (int32_t)(stats.sample_us[i]) - average_latency;
    squared_sum += delta * delta;
  }
  const double std_dev = sqrt(squared_sum / stats.sample_count);
  return (uint32_t)std_dev;
}

int qsort_compare_function(const void *a, const void *b) {
  const uint32_t *x = (uint32_t *) a;
  const uint32_t *y = (uint32_t *) b;
  return *x - *y;
}

// prints stats after all samples are collected
static void report_final_stats(void) {
  report_running_stats();
  if (stats.sample_count == 0) {
    return;
  }

  const uint32_t std_dev = calculate_std_dev();

  // calculate median and 90%
  qsort(
      stats.sample_us,
      stats.sample_count,
      sizeof(stats.sample_us[0]),
      qsort_compare_function);
  const uint32_t percent50 = stats.sample_us[stats.sample_count / 2];
  const uint32_t percent90 = stats.sample_us[stats.sample_count * 9 / 10];
  printf(
      "stddev: %u us  50%%: %d us  90%%  %d us\n",
      std_dev,
      percent50,
      percent90);
}

// program entry point
int main(int argc, char *argv[]) {
  int js_device = 0;
  if (init(&js_device)) {
    return 1;
  }

  uint64_t next_report_ms = get_ms() + REPORT_MS;
  int toggle_led = 1;
  memset(&stats, 0, sizeof(stats));
  stats.min_latency_us = 10000000;  // just some large value
  run = 1;
  for (int i=0; i<SAMPLES_TO_COLLECT; ++i) {
    if (run == 0) {
      // ctrl-c check
      break;
    }
    toggle_led = collect_sample(js_device, toggle_led);
    sleep_ms(TOGGLE_PERIOD_MS);

    // check for a report
    const uint64_t time_ms = get_ms();
    if (time_ms >= next_report_ms) {
      report_running_stats();
      next_report_ms = time_ms + REPORT_MS;
    }
  } 

  report_final_stats();

  // finish up
  stopGPIO();
  close(js_device);

  return 0;
}

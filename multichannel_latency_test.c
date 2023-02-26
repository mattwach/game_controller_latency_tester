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

// how many samples to collect
#define SAMPLES_TO_COLLECT 1000

// approx how often to report
#define REPORT_MS 1000

// joystick device
#define JOYSTICK_DEVICE "/dev/input/js0"

// number of buttons (or axis)
#define BUTTON_COUNT 2

// how long to give unwanted joystick events to flush through
#define JS_FLUSH_TIME_MS 100

// trackig for a GPIO
struct GPIO {
  uint8_t pin;  // GPIO pin as defined by the raspberry pi
  uint8_t js_type;  // the associated type (button or axis)
  uint8_t js_number;  // button or axis number
  uint8_t toggle_value; // 1->high, 0->low
  uint32_t toggle_period_us;  // how often to toggle
  uint64_t last_toggle_us;  // timestamp for the last gpio toggle
  uint8_t responded;  // set to 0 when last_toggle_us is updated, 1 when an event happens   
} gpio[BUTTON_COUNT];

// Statistics to track
struct Stats {
  uint32_t max_latency_us;
  uint32_t min_latency_us;
  uint64_t total_latency_us;
  uint32_t sample_count;
  uint32_t sample_us[SAMPLES_TO_COLLECT];
  uint32_t events_lost;
} stats;

// Used to support ctrl-C
volatile int run = 0;

// joystick device file number
int js_fd = -1;

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

// Reads in a single joystick event with the given timeout
static int read_js_event(struct js_event *event, uint32_t ms) {
    struct timeval tv;
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    fd_set rd;
    FD_ZERO(&rd);
    FD_SET(js_fd, &rd);
    const int err = select(FD_SETSIZE, &rd, NULL, NULL, &tv);

    switch (err) {
      case 0:
        // timeout
        return err;
      case -1:
        // error
        return err;
      default:
        // have data
        break;
    }

    const ssize_t bytes = read(js_fd, event, sizeof(*event));
    if (bytes == sizeof(*event))
        return 1;

    // error reading event
    return -1;
}


// initialized a GPIO. returns 1 if successful, 0 otherwise
static int init_gpio(struct GPIO* g, uint8_t pin, uint32_t toggle_ms) {
  memset(g, 0, sizeof(*g));
  g->pin = pin;
  g->toggle_period_us = toggle_ms * 1000;

  gpioSetMode(pin, PI_OUTPUT);

  // first, a throw-away event becuase we don't know if the
  // pre-initialized gpio was in a pullup or pull-down configuration
  gpioWrite(pin, PI_HIGH);
  struct js_event event;
  int status = 1;
  for (int i=0; (status == 1) && (i<1000); ++i) {
    status = read_js_event(&event, JS_FLUSH_TIME_MS);
  }

  if (status != 0) {
    printf("gave up trying to flush joystick events: %d.\n", status);
    return 0;
  }

  // now send the "real" event from which we will determine the type and number
  gpioWrite(pin, PI_LOW);
  status = read_js_event(&event, JS_FLUSH_TIME_MS);
  if (status == 0) {
    // try again
    status = read_js_event(&event, JS_FLUSH_TIME_MS);
  }
  if (status == 0) {
    printf(
        "Did not receive js event after toggling gpio %u.  Consider running "
        "jstest-gtk and rechecking connections\n", pin);
    return 0;
  }
  if (status == -1) {
    printf("error while waiting for event from gpio %u\n", pin);
    return 0;
  }
  char* type_str = "unknown";
  if (event.type == JS_EVENT_BUTTON) {
    type_str = "button";
  } else if (event.type == JS_EVENT_AXIS) {
    type_str = "axis";
  } else {
    printf("Unexpected event type from gpio %u: %u\n", pin, event.type);
    return 0;
  }

  g->js_type = event.type;
  g->js_number = event.number;
  printf(
      "Initialized GPIO %u:  type: %s  number: %u\n", pin, type_str, g->js_number);
  return 1;
}

// signal handler callback, when ctrl-c is pressed
static void stop(int signum) {
  run = 0;
}

// stops using GPIO
static void stopGPIO(void) {
  printf("\ntidying up\n");
  for (int i=0; i<BUTTON_COUNT; ++i) {
    gpioSetMode(gpio[i].pin, PI_INPUT);
  }
  gpioTerminate();
}

// initialize GPIO and joystick.  Returns 0 if successful.
static int init() {
  memset(&stats, 0, sizeof(stats));
  stats.min_latency_us = 10000000;  // just some large value

  js_fd = open(JOYSTICK_DEVICE, O_RDONLY);
  if (js_fd == -1) {
    perror("Could not open JOYSTICK_DEVICE");
    return -1;
  }

  if (gpioInitialise() < 0) {
    printf("gpio init failed\n");
    close(js_fd);
    return -1;
  }

  gpioSetSignalFunc(SIGINT, stop);
  if (!init_gpio(gpio + 0, 22, 98)) {
    return -1;
  }
  if (!init_gpio(gpio + 1, 6, 100)) {
    return -1;
  }
  printf("ctrl-c to stop, or wait for %d samples\n", SAMPLES_TO_COLLECT);
  return 0;
}

// Prints the current stats to stdout
static void report_running_stats(void) {
  printf("samples: %u  min_us: %u  max_us: %u  last_us: %u  avg_us: %u  lost_events: %u\n",
      stats.sample_count,
      stats.min_latency_us,
      stats.max_latency_us,
      stats.sample_count > 0 ? stats.sample_us[stats.sample_count - 1] : 0,
      (uint32_t)(stats.total_latency_us / stats.sample_count),
      stats.events_lost);
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

static int qsort_compare_function(const void *a, const void *b) {
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

static void check_for_toggle(struct GPIO* g, uint64_t time_us) {
  if (time_us < (g->last_toggle_us + g->toggle_period_us)) {
    // not yet
    return;
  }

  if (!g->responded && (g->last_toggle_us > 0)) {
    // event was never received
    ++stats.events_lost;
  }

  g->toggle_value = 1 - g->toggle_value;
  gpioWrite(g->pin, g->toggle_value ? PI_HIGH : PI_LOW);
  g->last_toggle_us = get_us();
  g->responded = 0;
}

static void handle_js_event(const struct js_event* event) {
  struct GPIO* g = NULL;
  for (int i=0; (g == NULL) && (i<BUTTON_COUNT); ++i) {
    if ((gpio[i].js_type == event->type) &&
        (gpio[i].js_number == event->number)) {
      g = gpio + i;
    }
  }

  if (g == NULL) {
    return;
  }

  g->responded = 1;
  const uint64_t end_us = get_us(); 
  const uint32_t sample_us = end_us - g->last_toggle_us;
  stats.sample_us[stats.sample_count] = sample_us;
  stats.total_latency_us += sample_us;
  if (sample_us < stats.min_latency_us) {
    stats.min_latency_us = sample_us;
  }
  if (sample_us > stats.max_latency_us) {
    stats.max_latency_us = sample_us;
  }
  if (stats.sample_count < SAMPLES_TO_COLLECT) {
    ++stats.sample_count;
  }
}

static void loop(void) {
  const uint64_t time_us = get_us();
  int i;

  for (i = 0; i < BUTTON_COUNT; ++i) {
    check_for_toggle(gpio + i, time_us);
  }

  // wait for a js event until 1ms with no events has passed
  int state = 1;
  struct js_event event;
  while (state == 1) {
    state = read_js_event(&event, 1);
    if (state == -1) {
      printf("error reading js_event\n");
      run = 0;
    }
    if (state == 1) {
      handle_js_event(&event);
    }
  }
}

int main(int argc, char *argv[]) {
  if (init()) {
    return 1;
  }

  run = 1;
  uint64_t next_report_ms = get_ms() + REPORT_MS;
  while (run && (stats.sample_count < SAMPLES_TO_COLLECT)) {
    loop();
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
  close(js_fd);

  return 0;
}

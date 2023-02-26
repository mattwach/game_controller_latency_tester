TOOLS:=toggle_test simple_latency_test multichannel_latency_test joystick_events

all: $(TOOLS)

toggle_test: toggle_test.c
	gcc -Wall -o $@ $? -lpigpio

multichannel_latency_test: multichannel_latency_test.c
	gcc -Wall -o $@ $? -lpigpio

simple_latency_test: simple_latency_test.c
	gcc -Wall -o $@ $? -lpigpio

joystick_events: joystick_events.c
	gcc -Wall -o $@ $? -lpigpio

clean:
	rm $(TOOLS)


# game_controller_latency_tester

This project provides a (couple of) hardware designs for interfacing a game
controller to a rasperry pi and automatically measuring the input latency.

## What is input latency?

Input latency is the amount of time that passes between a button being pressed
on the controller and application (game) receiving the message.

This is the sum of all of the following:

   1. The game controller hardware signaling that the button is pressed
      (very fast)
   2. The game controller firmnware processig this signal into a USB or
      bluetooth message (varies)
   3. The host USB/bluetooth hardware receiving the message and notifying the
      host operating system (usually fast)
   4. The host operating system handling the message and passing it forward to
      the application (usually fast)

> "button" could also mean deflecting the sitck.

## PI verses Microcontroller verses PC host

Some setups, such as [this one](https://inputlag.science/controller/methodology)
use microcontroller hardware to handle #1, #3 and #4.  This project uses a
Raspberry PI instead.

One reason to use a microcontroller is more control over the environment.  A
Raspberry PI running Linux could get interrupted in the middle of a measurement
which would unfairly penalize the results.  That said, the PI has many
advantages as well:

   1. Even though the Raspberry PI could get interrupted, it's fairly simple
      to create a environment where this is not likely to happen.  For example
      you could run without a graphical environment or any services.  My own
      tests didn't even bother being careful with this and still got similar
      results as the [microcontroller version](https://inputlag.science/controller/methodology)
      testing the [GP2040](https://github.com/FeralAI/GP2040) firmware.  This
      is also due to the PI running at 4 core 1.5Ghz+ compared to just 16Mhz
      of the microcontroller solution.
   2. The Raspberry PI could be argued a more accurate test in some respects
      as it's using the same path that RetroPI would use.
   3. Supporting multiple different Game controller protocols, USB and Bluetooth,
      is already built in.
   4. Just one device to support everything verses a set of hardware needed
      in the microcontroller case.

If raspberry PI's are still hard to find, some clone is also fine.  The
needed requirements are GPIO support and USB.  You could also use a generic
PC with a USB to GPIO converter to send the needed trigger signals.  Something
like [this](https://github.com/FeralAI/GP2040) or roll-your-own with PI Pico or
similar.

> Not use a raspberry pi will mostly certainly require some tweaks to the
> provided source code.

## Basic Process

   1. Host computer toggles a GPIO and notes the current time in microsecond
      resolution.
   2. The GPIO causes the controller to think a button was pressed (or a stick
      was deflected)
   3. The controller sends a corresponding USB/Bluetooth message back to the
      computer
   4. Upon receiving the USB/Bluetooth messag, the computer notes how much
      time has passed since sending the GPIO

1-4 repeats many time (1000 or more) to build up statistics.

## Test Pattern

One test pattern would be to toggle the button very fast, like 1000 times per
second.  But since no human player could ever do this, it could be argued that
the test is for something that does not happen in real use thus poor results
might not be relevant to real-world experience.  With such a high rate, the
game controller input might even run low on memory and be forced to drop inputs
which again is something it would not need to do in a real world situation.

This tests tries to set up a more "real world" situation but firing two GPIO
channels at 10Hz (or 20 inputs per second).  The channels overlap one another
in varying ways - sometimes the GPIOs toggle at exactly the same time and
sometimes not.

The way this is implemented to to toggle GPIO1 every 100ms and GPIO2 every 98ms.
This creates a pattern that looks like this:

## Interpreting the results

The reported statistics will be as follows:

   * Median Latency: The "middle" latency value where half of the samples are
     larger and half of them are smaller.
   * Standard Deviation: A measure of latency variance.  A high number here
     (e.g. 5+ ms) can be bad news.  Even if the median latency is bad (say 20+ ms),
     a low standard deviation means that the human player can naturally adapt to it
     and also that patterns (such as dragon punch) will not be affected, just delayed.
     A high standard deviation will degrade both of these situations.
   * 90% latency: A pessimistic-but-reasonable way to think about the latency
     It basically says that 90% of the time, you'll see less latency than the
     reported number.

### What is "bad" latency?

I like to think of it as the odds that you will miss a frame.  At 60 Hz, a frame
is (around 16ms).  So if your latency is >16ms, then you will miss a frame every
time.  If your latency is 8ms, you will miss a frame about half the time, 4ms
means missing a frame 25% of the time. 100ms and you'll miss 6-7 frames (!).

How bad is it to miss a frame?  It depends on your skill level and the demands
of the game.  For most people, missing a single frame is unlikely to change the
outcome.  On the other hand, missing 5+ frames could easily make the game feel
laggy/unresponsive.

Another thing to think about is that latency will also come from other sources,
like the game engine itself.  A "good" game latency is thought to be about 3
frames - controller latency will add onto that.

### Human reaction time is measured at 100-250 ms.  Thus does 20ms of latency even matter?

This time is measured as a reaction to a stimulus.  For example, a light turns
on and a human hits a button as quickly as they can.  This is sometiimes the
case in a game, but there is another case that frequency comes up: rhythm
accuracy.

Rythm accuracy is when a human knows that a certain amout of time has to pass
before executing a move.  Drumming is a clear example, but there are many
example in games as well.  Playing a shmup can be though of as being similar to
playing a song where enemies appear at the same place at the same times and
this is even coordinated with the music.  In fighting games, the timing of
special move inputs is also a form of rythm - a human played does not "react"
to each input but executes then to a rythm that is trained to work.  Rythmn
accuracy in humans is measured to be as good as 10ms, much better than the
reasction time figure.

Again, even if the game controller latency is high (say 40ms), if it's
consistent (low standard deviation), then rythm might not be as adversely
affected.

## Hardware

You could directly connect a PI GPIO to a controller and a lot of the time it
would even work, but there are risks:

   1. The Raspberry PI is 3.3V, the controller might be something different, such
      as 5V.
   2. Connecting the PI to some unknown input could easily lead to situations
      where the PI is trying to power a ground connection to 3.3V or
      "current sink" a powered connection to ground.  These situations can
      lead to the controller and/or pi getting damaged.

Thus I suggest using some protection circuitry to keep the PI and controller
isolated from each other and prevent damage in the case of a wiring error.

> Wiring errors will not be able to execute the test but will likely not damage
> anything either.

I present two designs: optocoupler based, and NFET based.

The NFET design has the advantage of easier hookup and faster operation.  The
disadvantage is that it will only work with "pull up" style buttons, that look
like this:

[image]

another possible design is a "pull down" configuration that looks like this:

[image]

NFET will only work with the pull up configuration.  If you hook it to a pull
down button by mistake, no damage will occur but the test will not be able to
execute.  Pullup configurations are more common because it's usually preferable
to have a grounded and external high-impedance pin over a pin connected to
power.  Many microcontrollers that act as the "brains" of the game controller
only offer internal pull ups as an option as well, thus it's also often cheaper
to go with a pull up design.

Another advantage of the FET, assumed pull up configuration is that the USB
ground (on USB controllers) can be used as the ground connection for the
test too, thus only one test wire is needed per button.

The optocoupler based design will work with either pull up or pull down
configuratons, but you need to known which way the current flows through the
button and connect two wires per button (4 total). Getting any of this wrong
should not do any damage, but the test will fail to execute.  Optocouplers
are also quite a but slower than FETs.  The one I had on-hand took around 2us
to activate and 100+us to turn itself off.  You can get faster ones, but need
to look at the datasheet when you pick one.  In comparison, FET's just take
a handful of ns to turn on/off.  

Going forward, I'll assume we are going with the NFET design.  If you need
the optocoupler one, then your hardware build will be different but the
softare you run will be the same.


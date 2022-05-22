## Background
Due to the nature of my project, I feel like I should give a background on what a guitar pedal is, and how it works. A guitar pedal is basically a small circuit (sometimes with a computer) that sits between your guitar and the amplifier to add effects to the signal such as distortion, delay, and compression. I will only be talking about pedals with microcomputers as that’s the type of pedal I’m working with.  Basically, the pedal works by taking an input signal from the guitar using an ADC (analog to digital converter), then manipulates the signal in different ways to get different effects.  After the signal is changed it is sent back out to the amp using PWM gpio pins. (a way to regulate the output voltage of a gpio pin).

https://en.wikipedia.org/wiki/Effects_unit/

## Disclaimer
I am not submitting the hardware of the guitar pedal as that was done long before the hackathon, I am submitting the software that runs it (cppedal). Before this hackathon, no code was written for the pedal besides a throwaway test program (verified with hackathon organizers beforehand that this was allowed) to ensure the hardware was functional. None of the test code was used in the final product and was only for hardware testing purposes.

## Inspiration
I have been really getting into system and hardware programming lately.  I also love to play music so I wanted to merge these interests in a fun way!  A while ago I created a guitar pedal based on ElectroSmash PiPedal. Unfortunately, each effect was it’s own stand-alone application, meaning you had to compile and rerun the whole program from source every time you wanted to change the effect. 
Also, all of the PiPedal example effects are written using the bcm2835 chipset which is great if you have a Raspberry Pi zero or Rpi3.  However, due to the chip shortage, I have been unable to buy one because of the insane prices.  The example programs don't work with my Rpi4, due to it using a different chipset, so I wanted to create a program to solve all of these problems, as well as add some cool new features, such as an LCD and stackable effects.
https://www.electrosmash.com/pedal-pi 

## What it does
cppedal (pronounced cpp pedal) is a modular guitar pedal framework that is able to load in different components without having to recompile the main component "Framer". This is done by using a JSON config file (shown below) that can be edited to switch in and out components, and modify effects and input mappings without having to recompile.

It is able to:
- Swap between different effects defined in the config
- Create and map inputs (push-button and rotary-encoder) to gpio pins
- Map inputs to different effects (for example, distortion level)
    - with min, max, and start values.
- Stack effects on top of each other so multiple effects get processed one after another
- Display the current effect on the LCD
- Display changes to mapped inputs on the LCD


Components that can be swapped in and out are:
- Effect libraries
- Ingestor (What grabs data from ADC)
- pwm_output (How the signal is output to the guitar amp)
- input_library (How the input is handled. There are 2 support inputs: [push-button, rotary encoder])
- LCD library (Displays info to user)

This means that this project can work with any components (ADC, LCD, input), library, and/or platform. All you need to do is write shared libraries for the new components you wish to use and map them in the config.

## How I built it
### Why c++ was used
This project is built with C++ to get the most out of the hardware.  This allowed me to get as low to the hardware as I wanted and have some components abstracted away, such as the LCD Input and PwmOutput classes.

### Components
There are 5 main components of the application:
- Framer (builds the project from a bunch of small pieces (shared objects), much like how a framer builds a house)
- Ingestor (input audio)
- PwmOutput (output audio)
- Input
- LCD 

### Shared libs
This project also heavily makes use of shared libraries to load components.  Everything except for the "Framer" is a shared object.  An example of how a shared object was created can be found here:
https://github.com/qawse3dr/cppedal/blob/master/ingestors/include/cppedal/ingestor/ingestor.hpp

#### How to use a shared library
Basically, there is an Interface that Framer will use to interact with the shared library as well as a function to create the interface inside of Framer (in the example linked above, the function is called makeIngestor). 
A shared library inherits from this interface and overrides all of the virtual functions as well as creates a makeIngestor function that returns itself as a std::unique_ptr<Ingestor> so that Framer can interact with it.


## Challenges I ran into
The biggest challenge that I ran into, was the limitations of the hardware and libraries I was using. The biggest problem was performance. For a program like this, *SPEED* means everything.  Even a looping print statement will slow down the application and start affecting the quality of the signal making the sound choppy. This made me have to get creative with certain parts.  For example, the LCD I was using was super slow, meaning I had to create a basic producer/consumer model for the LCD to queue up print requests and execute them in another thread. Another problem was the gpio interrupts of the library I was using only allowed a C-style function pointer, without any arguments meaning that I couldn't pass any context of what gpio was pressed.  To resolve this problem, I added a threading model with polling instead, as I didn't have time to write my own interrupt handler.

## Accomplishments that I am proud of
I am proud of how efficient the program is. When I started this project, I was worried that all of the shared objects and different inputs would affect the sound.  There were times that this did occur, but I was able to resolve them, and the final product has no noticeable effect on the sound.  I am also proud of how modular I was able to make the software.  With the way I designed it, anyone using the software can create and add new effects and inputs with ease.  The JSON config file turned out better than I ever could have imagined and is very clean and easy to modify.

## What I learned
-Never trust libraries and always check the source code if you can. The one wiringPi library I was using had a bug for my ADC where it was reading the number off of it wrong (did a bit shift of 8 instead of 7).  This led to me thinking there was a problem in my program which I resolved by reimplementing their source in my own program.
- A good product takes time. While I am happy with the overall product, I know if I had 3 more months to work on this, I could make it 10 times better and cleaner. This just shows me that while you can get something functional fast, it’s not sustainable as corners need to be cut, creating lots of tech debt.  For example, due to the time constraints, I had to cut a lot of corners on some error handling during the latter parts of the hackathon and start assuming some of the input would be correct.

## What's next for ccpedal
- More Effects! The effects I used in this are mostly just adapted examples from ElectroSmash, but I didn't have time to port a lot of them and only did the basics. I would really like to port the rest of the effects they have, as well as play around and create my own.
- Rebuild the pedal to use 2 PWM pins instead of 1 to get a better bit resolution for the output.  Due to the design of the project, nothing else will need to be changed except for the target pwm-output library.

## Thank Yous
Thanks to the creators of the PiPedal (ElectroSmash). If you are interested in audio and hardware programming I highly suggest you check this out!

Link: https://www.electrosmash.com/pedal-pi

Thanks to the creators of WiringPi. I know it is no longer maintained by the creator but it is still a great library.

Thank you to the hackathon organizers, judges, and guests for making this event happen!
## CONFIG
```json
{
  // INPUT
  "ingestor": {
    "name": "mcp3002",
    "path": "lib/ingestors/libcppedal-ingestor-mcp3002.so"
  },
  // OUTPUT
  "pwm_output": {
    "name": "rpi",
    "path": "lib/pwm-output/libcppedal-pwm-output-rpi-single.so"
  },

  // LCD
  "lcd": {
    "name": "1602_LCD",
    "path": "lib/lcd/libcppedal-lcd-1602.so"
  },

  // INPUT LIB
  "input_library": {
    "name": "rpi",
    "path": "lib/input/libcppedal-input-rpi.so"
  },

  // EFFECTS
  "effect_libraries": [
    {
      "name": "clean",
      "path": "lib/effects/libcppedal-effect-clean.so"
    },
    {
      "name": "dist",
      "path": "lib/effects/libcppedal-effect-dist.so"
    },
    {
      "name" : "boost",
      "path" : "lib/effects/libcppedal-effect-boost.so"
    },
    {
      "name" : "fuzz",
      "path" : "lib/effects/libcppedal-effect-fuzz.so"
    }
    
  ],
  "inputs" : [
    // PUSH BUTTONS
    {
      "name": "push_left",
      "type": "push_button",
      "type_data": {
        "input_gpio": 23,
        "pull_up": true
      }
    },
    {
      "name": "push_middle",
      "type": "push_button",
      "type_data": {
        "input_gpio": 29,
        "pull_up": true
      }
    },
    {
      "name": "push_right",
      "type": "push_button",
      "type_data": {
        "input_gpio": 6,
        "pull_up": true
      }
    },

    // ROTARY ENCODERS
    {
      "name": "rotary_left",
      "type": "rotary_encoder",
      "type_data": {
        "data_gpio": 24,
        "clk_gpio": 25,
        "pull_up": true
      }
    },
    {
      "name": "rotary_middle",
      "type": "rotary_encoder",
      "type_data": {
        "data_gpio": 28,
        "clk_gpio": 27,
        "pull_up": true
      }
    },
    {
      "name": "rotary_right",
      "type": "rotary_encoder",
      "type_data": {
        "data_gpio": 5,
        "clk_gpio": 4,
        "pull_up": true
      }
    }
  ],

  "effects": [
    {
      "name": "Clean",
      "effects": ["clean"],
      "inputs": [] // No inputs for clean
    },
    {
      "name": "Distortion",
      "effects": ["dist"],
      "inputs": [
        {
          "name": "dist_level",
          "input": "rotary_right",
          "min": -30,
          "max": 30,
          "init": 5
        }
      ]
    },
    {
      "name": "Boost & Fuzz",
      "effects": ["fuzz", "boost"],
      "inputs": [
        {
          "name": "fuzz_level",
          "input": "rotary_right",
          "min": -30,
          "max": 30,
          "init": 5
        },
        {
          "name": "boost_level",
          "input": "rotary_middle",
          "min": -20,
          "max": 20,
          "init": 0
        },
        {
          "name": "boost_on", 
          "input": "push_middle"
        }
      ]
    },
    {
      "name": "Clean Boost",
      "effects": ["clean", "boost"],
      "inputs": [
        {
          "name": "boost_level",
          "input": "rotary_middle",
          "min": -20,
          "max": 20,
          "init": -5
        },
        {
          "name": "boost_on", 
          "input": "push_middle"
        }
      ]
    }

  ],

  // SUSSY BUTTONS
  "prev-effect-button": "push_left",
  "next-effect-button": "push_right"
}
```

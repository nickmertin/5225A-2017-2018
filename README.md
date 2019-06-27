# 5225A-2017-2018

This is the code used by team 5225A during the 2017-2018 VEX Robotics Competition: In The Zone season.

There are a few things that I'd like to make clear about this code. In particular, there are a lot of things not great about the way this code is structured. Some of these are workarounds for ROBOTC's
limitations, some are due to us not realizing there was a better way, and some are me not commenting anything for no apparent reason. Feel free to take inspiration from our code but don't take this
release as us endorsing bad code practices :)

Here are descriptions for the useful files in the repository:
 - `dev/tools/async.py`: Python script to generate `src/async.h`
 - `dev/tools/state.py`: Python script to generate `src/state.h`
 - `dev/tools/gen.bat`: Batch script to generate both files with our prefered configuration
 - `src/main.c`: Main file for ROBOTC; includes the core logic for most subsystems, input logic for driver control, automated stacking, port config, timeout logic, `#include`s for all other files, etc.
 - `src/Vex_Competition_Includes_Custom.c`: Custom modified version of ROBOTC's `Vex_Competition_Includes.c` file, to better work with our task and async wrappers
 - `src/async.{h,c}`: Library of macros to generate async wrappers for functions
 - `src/cycle.{h,c}`: Simple library for maintaining consistent timing in cycles
 - `src/joysticks.{h,c}`: Wrapper library for ROBOTC joystick API (i.e. `vexRT`)
 - `src/motors.{h,c}`: Wrapper library for ROBOTC motor API (i.e. `motor`)
 - `src/sensors.{h,c}`: Wrapper library for ROBOTC sensor API (i.e. `SensorValue`)
 - `src/state.{h,c}`: Library of macros to generate types and functions for state machines
 - `src/task.{h,c}`: Library for to manage ROBOTC tasks (introduces notion of parent/child tasks and fixes some issues with the built-in API)
 - `src/timeout.{h,c}`: Library for waiting for certain events with a timeout
 - `src/utilities.{h,c}`: Miscellaneous math and utility macros and functions
 - `src/auto.{h,c}`: Core tracking and autonomous motion algorithms logic
 - `src/auto_runs.{h,c}`: Auto selection logic, auto routines, programming skills routine, and support functions only used in programming skills
 - `src/auto_simple.{h,c}`: Autonomous motion algorithms
 - `src/custom_drive.{h,c}`: Input mapping curve for drive throttle
 - `src/custom_turning.{h,c}`: Input mapping curve for drive turn
 - `src/diagnostics.{h,c}`: LCD screen controller
 - `src/controls.h`: Joystick controls mapping

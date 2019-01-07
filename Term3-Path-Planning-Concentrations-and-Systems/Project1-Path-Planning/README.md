# CarND-Path-Planning-Project
Self-Driving Car Engineer Nanodegree Program
   
## Introduction
The goal of this project is to build a path planner that creates smooth, safe trajectories for the car to follow. The highway track has other vehicles, all going different speeds, but approximately obeying the 50 MPH speed limit. The car transmits its location, along with its sensor fusion data, which estimates the location of all the vehicles on the same side of the road.


## Rubric Points
Each of the rubric points given in project specifications are discussed below.

#### Compilation
The code compiles without any errors.

#### Valid Trajectories
* The car is able to drive at least 4.32 miles without incident.
* The car drives according to the speed limit.
* Max Acceleration and Jerk are not Exceeded.
* Car does not have collisions.
* The car stays in its lane, except for the time between changing lanes.
* The car is able to change lanes.


#### Reflection
The trajectory generation in this project has been performed by:
* Analysing other vehicles (a.k.a targets) on the highway around (front, left, right) ego vehicle.
* Planning appropriate safe behavior (keep lane, lane change left, lane change right).
* Generating waypoints and fitting a spline through those waypoints for a jerk free and smooth trajectory.
## Self Driving Car NanoDegree - PID Controller

A PID controller continuously calculates the error between a set point (SP) and desired variable (DV) to apply a correction based on proportional, derivative and integral errors. The correction is applied after multiplying the respective errors with some factors called gains. To achieve optimal system performance, however, these gains usually need to be tuned. Since the inception of PID control in 1911 by Elmer Sperry, the tuning process has remained elusive although some techniques for tuning have emerged over the last couple of decades. These include the popular Ziegler–Nichols method, Coordinate Ascent algorithm (TWIDDLE) or manual tuning. It can be shown that manual tuning of a PID controller without any system knowledge is a lost cause since a random process in three dimensions has negligible chances of converging to starting position. 

The tuning strategy adopted for this project was the one by Ziegler–Nichols. Although, it is difficult to get exact ultimate gain and period of oscillation required for Ziegler–Nichols tuning for the test track, a reasonable estimate was made to get starting gains. These gains were further tuned manually as per the tuning table given below:


| Response | Rise Time    | Overshoot | Settling Time | Steady-State Error | 
| -------- | ------------ | --------- | ------------- | ------------------ | 
| Kp       | Decrease     | Increase  | Minor Change  | Decrease           |
| Ki       | Decrease     | Increase  | Increase      | Eliminate          |
| Kd       | Minor Change | Decrease  | Decrease      | No Influence       |

The starting and final gains are:

| Method   |   Kp   |   Ki   |   Kd   |
| -------- | ------ | ------ | ------ |
| Starting | 0.2	| 0.002	 | 13.33  |
| Final    | 0.2	| 0.002	 |	7	  |


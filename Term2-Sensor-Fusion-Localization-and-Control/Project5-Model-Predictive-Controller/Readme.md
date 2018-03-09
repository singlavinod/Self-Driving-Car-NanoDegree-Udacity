# **Project 5: Model Predictive Control** 

## Introduction
Model Predictive Control is a control methodology which reframes the task of tracking a reference state into an optimization problem. This involves simulating different trajectories over a finite time horizon, T, with different control inputs and selecting the one that results in an optimal trajectory. The concept of optimality can be different depending upon application. For example, in the case of a self driving car, it is generally true that we want to follow a reference path with minimum jerks and maximum speed in a shortest possible time with minimum control inputs. As you can imagine this creates tradeoffs which require a certain amount of trial and error to prioritize the aspects that make most sense for current application. In this project, a program is written in C++ to predict an optimal trajectory using model predictive control for a set of waypoints received from the planning system and utilize the first set of control inputs for simulation at each timestep.

## Model
The model is a way to mathematically represent a physical phenomenon. Generally, we want the model to emulate the physical phenomenon as closely as possible. However, it so happens that models which are more accurate are harder to evaluate in terms of computations. Broadly speaking, there are two classes of models called Kinematic and Dynamic Models. Kinematic models do not explicitly take into account the effect of forces and are usually less accurate than Dynamic Models. However, in many cases, Kinematic models deliver a reasonable estimate at a fraction of computational cost as compared with Dynamic Models. In case of self driving vehicles, where real time control is a requirement, Kinematic models make sense since the uncertainty in the model is usually handled through feedback. Naturally, a Kinematic model is utilized in this project. The state vector chosen for this model has the form:
```
x = [px, py, v, psi, cte, epsi]
```
where:
px is the x-position of the car
py is the y-position of the car
v is the car velocity
psi is the heading/orientation of the car
cte is the cross-track error
epsi is the heading/orientation error

The state update equations for this model follow:

![Kinematic Model](https://s9.postimg.org/l13zq59zz/Selection_001.png)

where:
f(x_t) is the value of fitted polynomial at x_t
a is the acceleration/throttle input
delta is the steering input
L_f is the distance of the front axle from the center of mass of the vehicle
dt is the timestep duration and is equal to T divided by N (discussed in hyperparameters below)

## Hyperparameters
Once a mathematical model is selected, a decision regarding the time horizon, T, and number of timesteps, N, of the trajectory is to be made. Since the environment can change drastically even after a second, the value of time-horizon, T is chosen to be 1 sec. On the other hand, increasing the number of timesteps results in increased computational cost but a more responsive control scheme since, the timestep duration is shorter. Different combinations of N and T were tried and are listed below. Finally the combination which resulted in best performance (N = 10 and T = 1 sec) was selected. Note that the timestep in this combination is 100 msec which is exactly same as the latency.

|       T (sec) |       N       |
|:-------------:|:-------------:|
|       1       |       15      |
|       1       |       5       |
|       1       |       10      |

## Cost Function
The cost function takes into account the crosstrack error, heading error, reference velocity, magnitude of control inputs (steering and throttle) and the changes in control inputs from one time step to the next. It has a form:

```
cost = w1*cte^2 + w2*epsi^2 + w3*(v - v_ref)^2 + w4*delta^2 + w5*a^2 + w6*(delta1 - delta0)^2 + w7*(a1 - a0)^2
```
where:
v_ref is the reference velocity
w1, w2, ... w7 are weights representing the relative importance given to each of the individual parameters

The weights are tuned with the intuition that cross track and heading error should be the ones given most importance with difference between subsequent actuations the next, followed by control inputs and reference velocity. The combination of weights which give satisfactory performance for a reference velocity of 80 mph is selected.

## MPC Preprocessing
Since the simulator returns the waypoints in global coordinate system instead of car's local coordinate system, they need to be transformed before a polynomial fit is performed. The equations for this tranformation are given below:

![Selection_002.png](https://s9.postimg.org/hdstxq40f/Selection_002.png)

## Addressing Latency
The starter code provided for this project has an artificial latency of 100ms incorporated into it to simulate a real life scenario. This is addressed in the main.cpp by running the kinematic model for the duration of latency on the initial state and passing the state so obtained to the model predictive control Solve method. The code for this is shown below:
```
// Handle latency by passing initial state through the kinematic model for the duration
// of latency and use the resulting state as the new initial state ********************/
double delay = 0.1;
double Lf = 2.67;
int n_states = 6;
Eigen::VectorXd new_init_state(n_states);

new_init_state[0] = v * delay;
new_init_state[1] = 0.;
new_init_state[2] = v * (-steer_value) * delay / Lf; 
new_init_state[3] = v + throttle_value * delay;
new_init_state[4] = cte + v * sin(epsi) * delay;
new_init_state[5] = epsi + v * (-steer_value) * delay / Lf;
```

## Results

A screen grab of the video can be viewed on YouTube:

[![Results](http://img.youtube.com/vi/EcQxsd9pH_M/0.jpg)](http://www.youtube.com/watch?v=EcQxsd9pH_M)

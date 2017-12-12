#include "kalman_filter.h"
#include <iostream>
#include <math.h>

using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;
// Please note that the Eigen library does not initialize 
// VectorXd or MatrixXd objects with zeros upon creation.

KalmanFilter::KalmanFilter() {
  //cout<<"Kalman Filter constructor running...\n"<<endl;
}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Predict() {
  // predict the state
  x_ = F_ * x_;
  MatrixXd Ft = F_.transpose();
  P_ = F_ * P_ * Ft + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  //update the state by using Kalman Filter equations
  VectorXd z_pred = H_ * x_;
  VectorXd y = z - z_pred;
  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd PHt = P_ * Ht;
  MatrixXd K = PHt * Si;

  //new estimate
  x_ = x_ + (K * y);
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  //update the state by using Extended Kalman Filter equations
  VectorXd z_pred(3);

  double rho = sqrt(x_(0)*x_(0) + x_(1)*x_(1));
  double phi = atan2(x_(1), x_(0)); 
  double rho_dot;
  
  if(fabs(rho) > 0.0001){
    rho_dot = (x_(0)*x_(2) + x_(1)*x_(3))/rho;
  }
  else{
    rho_dot = 0; 
  }

  z_pred << rho, phi, rho_dot;
  
  VectorXd y = z - z_pred;
  
  //Normalize residual phi to be between -pi to pi
  y(1) = atan2(sin(y(1)), cos(y(1)));
  
  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd PHt = P_ * Ht;
  MatrixXd K = PHt * Si;

  //new estimate
  x_ = x_ + (K * y);
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_; 
}

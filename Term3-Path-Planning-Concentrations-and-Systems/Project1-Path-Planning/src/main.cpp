#include <fstream>
#include <math.h>
#include <uWS/uWS.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "json.hpp"
#include "spline.h"

using namespace std;

// for conveniences
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s)
{
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_first_of("}");
  if (found_null != string::npos)
  {
    return "";
  } else if (b1 != string::npos && b2 != string::npos)
  {
    return s.substr(b1, b2 - b1 + 2);
  }
  return "";
}


double distance(double x1, double y1, double x2, double y2)
{
  return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}


int ClosestWaypoint(double x, double y, const vector<double> &maps_x, const vector<double> &maps_y)
{
  double closestLen = 100000; //large number
  int closestWaypoint = 0;

  for(int i = 0; i < maps_x.size(); i++)
  {
    double map_x = maps_x[i];
    double map_y = maps_y[i];
    double dist = distance(x,y,map_x,map_y);
    if(dist < closestLen)
    {
      closestLen = dist;
      closestWaypoint = i;
    }
  }
  return closestWaypoint;
}


int NextWaypoint(double x, double y, double theta, const vector<double> &maps_x, const vector<double> &maps_y)
{
  int closestWaypoint = ClosestWaypoint(x,y,maps_x,maps_y);

  double map_x = maps_x[closestWaypoint];
  double map_y = maps_y[closestWaypoint];

  double heading = atan2((map_y-y),(map_x-x));

  double angle = fabs(theta-heading);
  angle = min(2*pi() - angle, angle);

  if(angle > pi()/4)
  {
    closestWaypoint++;
    if (closestWaypoint == maps_x.size())
    {
      closestWaypoint = 0;
    }
  }
  return closestWaypoint;
}


// Transform from Cartesian x,y coordinates to Frenet s,d coordinates
vector<double> getFrenet(double x, double y, double theta, const vector<double> &maps_x, const vector<double> &maps_y)
{
  int next_wp = NextWaypoint(x,y, theta, maps_x,maps_y);

  int prev_wp;
  prev_wp = next_wp-1;

  if(next_wp == 0)
  {
    prev_wp  = maps_x.size()-1;
  }

  double n_x = maps_x[next_wp]-maps_x[prev_wp];
  double n_y = maps_y[next_wp]-maps_y[prev_wp];
  double x_x = x - maps_x[prev_wp];
  double x_y = y - maps_y[prev_wp];

  // find the projection of x onto n
  double proj_norm = (x_x*n_x+x_y*n_y)/(n_x*n_x+n_y*n_y);
  double proj_x = proj_norm*n_x;
  double proj_y = proj_norm*n_y;

  double frenet_d = distance(x_x,x_y,proj_x,proj_y);

  //see if d value is positive or negative by comparing it to a center point

  double center_x = 1000-maps_x[prev_wp];
  double center_y = 2000-maps_y[prev_wp];
  double centerToPos = distance(center_x,center_y,x_x,x_y);
  double centerToRef = distance(center_x,center_y,proj_x,proj_y);

  if(centerToPos <= centerToRef)
  {
    frenet_d *= -1;
  }

  // calculate s value
  double frenet_s = 0;
  for(int i = 0; i < prev_wp; i++)
  {
    frenet_s += distance(maps_x[i],maps_y[i],maps_x[i+1],maps_y[i+1]);
  }

  frenet_s += distance(0,0,proj_x,proj_y);

  return {frenet_s,frenet_d};
}


// Transform from Frenet s,d coordinates to Cartesian x,y
vector<double> getXY(double s, double d, const vector<double> &maps_s, const vector<double> &maps_x, const vector<double> &maps_y)
{
  int prev_wp = -1;

  while(s > maps_s[prev_wp+1] && (prev_wp < (int)(maps_s.size()-1) ))
  {
    prev_wp++;
  }

  int wp2 = (prev_wp+1)%maps_x.size();

  double heading = atan2((maps_y[wp2]-maps_y[prev_wp]),(maps_x[wp2]-maps_x[prev_wp]));
  // the x,y,s along the segment
  double seg_s = (s-maps_s[prev_wp]);

  double seg_x = maps_x[prev_wp]+seg_s*cos(heading);
  double seg_y = maps_y[prev_wp]+seg_s*sin(heading);

  double perp_heading = heading-pi()/2;

  double x = seg_x + d*cos(perp_heading);
  double y = seg_y + d*sin(perp_heading);

  return {x,y};
}


// Calculate target lane from frenet d value
int calc_target_lane(float d)
{
  int target_lane = -1;

  // Determine target vehicle's lane
  if ( d > 0 && d < 4 )
  {
    target_lane = 0; // left most lane
  }
  else if ( d > 4 && d < 8 )
  {
    target_lane = 1; // middle lane
  }
  else if ( d > 8 && d < 12 )
  {
    target_lane = 2; // right most lane
  }
  return target_lane;
}


int main() {
  uWS::Hub h;

  // Load up map values for waypoint's x,y,s and d normalized normal vectors
  vector<double> map_waypoints_x;
  vector<double> map_waypoints_y;
  vector<double> map_waypoints_s;
  vector<double> map_waypoints_dx;
  vector<double> map_waypoints_dy;

  // Waypoint map to read from
  string map_file_ = "../data/highway_map.csv";
  // The max s value before wrapping around the track back to 0
  double max_s = 6945.554;

  ifstream in_map_(map_file_.c_str(), ifstream::in);

  string line;

  while (getline(in_map_, line))
  {
    istringstream iss(line);
    double x;
    double y;
    float s;
    float d_x;
    float d_y;
    iss >> x;
    iss >> y;
    iss >> s;
    iss >> d_x;
    iss >> d_y;
    map_waypoints_x.push_back(x);
    map_waypoints_y.push_back(y);
    map_waypoints_s.push_back(s);
    map_waypoints_dx.push_back(d_x);
    map_waypoints_dy.push_back(d_y);
  }

  // define lane variable {0:leftmost_lane, 1:middle_lane, 2:rightmost_lane}
  int lane = 1;

  // define target velocity in m/s
  double v_ref = 0;

  const double MAX_SPEED = 49.5; // Speed limit
  const double MAX_ACC = 0.224; // Max acceleration


  h.onMessage([&v_ref, &map_waypoints_x, &map_waypoints_y, &map_waypoints_s, &map_waypoints_dx,
    &map_waypoints_dy, &lane, &MAX_ACC, &MAX_SPEED](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode)
    {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    //auto sdata = string(data).substr(0, length);
    //cout << sdata << endl;
      if (length && length > 2 && data[0] == '4' && data[1] == '2') {
        auto s = hasData(data);

        if (s != "")
        {
          auto j = json::parse(s);
          string event = j[0].get<string>();

          if (event == "telemetry")
          {
            // j[1] is the data JSON object
            // Main car's localization Data
            double car_x = j[1]["x"];
            double car_y = j[1]["y"];
            double car_s = j[1]["s"];
            double car_d = j[1]["d"];
            double car_yaw = j[1]["yaw"];
            double car_speed = j[1]["speed"];

            // Previous path data given to the Planner
            auto previous_path_x = j[1]["previous_path_x"];
            auto previous_path_y = j[1]["previous_path_y"];

            // Previous path's end s and d values 
            double end_path_s = j[1]["end_path_s"];
            double end_path_d = j[1]["end_path_d"];

            // Sensor Fusion Data, a list of all other cars on the same side of the road.
            auto sensor_fusion = j[1]["sensor_fusion"];

            // Fetch number of points in previous path
            int prev_path_size = previous_path_x.size();

            // set current s to last path s if we travelled
            if(prev_path_size > 0)
            {
              car_s = end_path_s;
            }

            /* Analyse positions of other cars on the road by iterating over sensor fusion object list */
            bool target_front = false;
            bool target_left = false;
            bool target_right = false;

            for ( int i = 0; i < sensor_fusion.size(); i++ )
            {
              float d = sensor_fusion[i][6];
              // calculate target vehicle's lane
              int target_lane = calc_target_lane(d);
              if (target_lane < 0)
              {
                continue;
              }
              // Calculate car speed
              double vx = sensor_fusion[i][3];
              double vy = sensor_fusion[i][4];
              double target_speed = sqrt(vx*vx + vy*vy);
              double target_s = sensor_fusion[i][5];

              // Determine target's s position at the end of current cycle
              target_s += ((double)prev_path_size*0.02*target_speed);

              // Determine if there are target's in front, left or right of ego vehicle
              if ( target_lane == lane ) 
              {
                // target is in ego lane, check if collision is imminent
                if (target_s > car_s && (target_s - car_s) < 30)
                {
                  target_front = true;
                }
              }
              else if ( target_lane - lane == -1 )
              {
                // target is in left lane, check if it's unsafe to change lane left
                if ((car_s - 30) < target_s && (car_s + 30) > target_s)
                {
                  target_left = true;
                }
              }
              else if ( target_lane - lane == 1 )
              {
                // target is in right lane, check if it's unsafe to change lane right
                if ((car_s - 30) < target_s && (car_s + 30) > target_s)
                {
                  target_right = true;
                }
              }
            }

            /* Behavior planning based on analysis of vehicles on the road */
            double delta_v = 0;

            if (target_front)
            {
              // there is a target in front of ego
              if ( !target_left && lane > 0)  // check if it's safe to change lane left
              {
                lane--; // Change lane left.
              }
              else if ( !target_right && lane != 2) // check if it's safe to change lane right
              {
                lane++; // Change lane right.
              }
              else
              {
                delta_v -= MAX_ACC; // no lane change possible, decrease velocity to avoid collision
              }
            }

            else 
            {
              // there is nothing in front of ego
              if ( lane != 1 ) // check if we are in the middle lane
               {
                /* If we are in the left most lane, check if it's safe to lane change right or, 
                 * if we are in the right most lane, check if it's safe to lane change left */
                if ( (lane == 0 && !target_right) || (lane == 2 && !target_left) ) 
                {
                  lane = 1; // Back to center.
                }
              }
              // If we are going below speed limit
              if ( v_ref < MAX_SPEED ) 
              {
                delta_v += MAX_ACC; // floor it
              }
            }

            json msgJson;

            // space for storing the planned trajectory
            vector<double> next_x_vals;
            vector<double> next_y_vals;

            // define points being used to generate a smooth spline path
            vector<double> pts_x;
            vector<double> pts_y;

            // define reference ego state from last cycle
            double ref_ego_x = car_x;
            double ref_ego_y = car_y;
            double ref_ego_yaw = deg2rad(car_yaw);

            // Fill some waypoints for spline curve fitting
            if(prev_path_size >= 2)
              // we have sufficient points from previous cycle
            {
              ref_ego_x = previous_path_x[prev_path_size - 1]; // update ego state x from last cycle
              ref_ego_y = previous_path_y[prev_path_size - 1]; // update ego state y from last cycle

              double prev_ref_ego_x = previous_path_x[prev_path_size - 2]; // update ego state x from second last cycle
              double prev_ref_ego_y = previous_path_y[prev_path_size - 2]; // update ego state y from second last cycle

              // store waypoints for spline curve fitting and avoiding jerk at the beginning
              pts_x.push_back(prev_ref_ego_x);
              pts_x.push_back(ref_ego_x);
              pts_y.push_back(prev_ref_ego_y);
              pts_y.push_back(ref_ego_y);

              // update ego state yaw
              ref_ego_yaw = atan2(ref_ego_y - prev_ref_ego_y, ref_ego_x - prev_ref_ego_x);
            }
            else
              // we do not have sufficient points in previous path executed, 
              // so we'll use current ego state and create an extra point
              // looking backward which is tanget to current ego yaw
            {
              // create another point for second last cycle based on current yaw
              // and at unit distance from current position
              double prev_car_x = car_x - cos(ref_ego_yaw); 
              double prev_car_y = car_y - sin(ref_ego_yaw);

              // store waypoints for spline curve fitting and avoiding jerk at the beginning
              pts_x.push_back(prev_car_x);
              pts_x.push_back(car_x);
              pts_y.push_back(prev_car_y);
              pts_y.push_back(car_y);
            }

            // create some more waypoints for spline path creation
            vector<double> wp0 = getXY(car_s + 30, 2+4*lane, map_waypoints_s, map_waypoints_x, map_waypoints_y);
            vector<double> wp1 = getXY(car_s + 60, 2+4*lane, map_waypoints_s, map_waypoints_x, map_waypoints_y);
            vector<double> wp2 = getXY(car_s + 90, 2+4*lane, map_waypoints_s, map_waypoints_x, map_waypoints_y);

            // store newly created waypoints
            pts_x.push_back(wp0[0]);
            pts_x.push_back(wp1[0]);
            pts_x.push_back(wp2[0]);

            pts_y.push_back(wp0[1]);
            pts_y.push_back(wp1[1]);
            pts_y.push_back(wp2[1]);

            // transform coordinates from inertial to car's frame of reference
            for (int i = 0; i < pts_x.size(); i++)
            {
              // calculate deltas
              double delta_x = pts_x[i] - ref_ego_x;
              double delta_y = pts_y[i] - ref_ego_y;

              // update point
              pts_x[i] = delta_x*cos(0 - ref_ego_yaw) - delta_y*sin(0 - ref_ego_yaw);
              pts_y[i] = delta_x*sin(0 - ref_ego_yaw) + delta_y*cos(0 - ref_ego_yaw);
            }

            // create a spline
            tk::spline sp;

            // fit spline to waypoints
            sp.set_points(pts_x, pts_y);

            // push points from previous cycles
            for (int i = 0; i < prev_path_size; i++)
            {
              next_x_vals.push_back(previous_path_x[i]);
              next_y_vals.push_back(previous_path_y[i]);
            }

            // space spline points to travel at desired ego velocity
            double spacing_x = 30;
            double spacing_y = sp(spacing_x);
            double spacing = sqrt(spacing_x*spacing_x + spacing_y*spacing_y);

            double x_cumulative = 0; // cumulative x

            for(int i = 1; i <= 50 - prev_path_size; i++)
            {
              v_ref += delta_v;
              if ( v_ref > MAX_SPEED ) {
                v_ref = MAX_SPEED;
              } else if ( v_ref < MAX_ACC ) {
                v_ref = MAX_ACC;
              } 
              // calculate number of points to regulate speed
              double N = (spacing_x/(0.02*v_ref/2.24));
              
              // point coordinates in car's frame
              double x_car_frame = x_cumulative + spacing_x/N;
              double y_car_frame = sp(x_car_frame);

              x_cumulative = x_car_frame;

              // point coordinates in inertial frame
              double x_current = ref_ego_x + x_car_frame * cos(ref_ego_yaw) - y_car_frame*sin(ref_ego_yaw);
              double y_current = ref_ego_y + x_car_frame * sin(ref_ego_yaw) + y_car_frame*cos(ref_ego_yaw);

              // push point in to trajectory vectors
              next_x_vals.push_back(x_current);
              next_y_vals.push_back(y_current);
            }

            msgJson["next_x"] = next_x_vals;
            msgJson["next_y"] = next_y_vals;

            auto msg = "42[\"control\","+ msgJson.dump()+"]";

            //this_thread::sleep_for(chrono::milliseconds(1000));
            ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          }

        } // end hasData() check in s

        else
        {
        // Manual driving
          std::string msg = "42[\"manual\",{}]";
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        } // end Manual driving
      } // end websocket message event
    }); // end onMessage

  // We don't need this since we're not using HTTP but if it's removed the
  // program doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t)
  {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    } else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });


  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req)
  {
    std::cout << "Connected!!!" << std::endl;
  });


  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length)
  {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });


  int port = 4567;

  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }

  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }

  h.run();
}

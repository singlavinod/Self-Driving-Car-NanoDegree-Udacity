;; Auto-generated. Do not edit!


(when (boundp 'styx_msgs::TrafficLight)
  (if (not (find-package "STYX_MSGS"))
    (make-package "STYX_MSGS"))
  (shadow 'TrafficLight (find-package "STYX_MSGS")))
(unless (find-package "STYX_MSGS::TRAFFICLIGHT")
  (make-package "STYX_MSGS::TRAFFICLIGHT"))

(in-package "ROS")
;;//! \htmlinclude TrafficLight.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(intern "*UNKNOWN*" (find-package "STYX_MSGS::TRAFFICLIGHT"))
(shadow '*UNKNOWN* (find-package "STYX_MSGS::TRAFFICLIGHT"))
(defconstant styx_msgs::TrafficLight::*UNKNOWN* 4)
(intern "*GREEN*" (find-package "STYX_MSGS::TRAFFICLIGHT"))
(shadow '*GREEN* (find-package "STYX_MSGS::TRAFFICLIGHT"))
(defconstant styx_msgs::TrafficLight::*GREEN* 2)
(intern "*YELLOW*" (find-package "STYX_MSGS::TRAFFICLIGHT"))
(shadow '*YELLOW* (find-package "STYX_MSGS::TRAFFICLIGHT"))
(defconstant styx_msgs::TrafficLight::*YELLOW* 1)
(intern "*RED*" (find-package "STYX_MSGS::TRAFFICLIGHT"))
(shadow '*RED* (find-package "STYX_MSGS::TRAFFICLIGHT"))
(defconstant styx_msgs::TrafficLight::*RED* 0)
(defclass styx_msgs::TrafficLight
  :super ros::object
  :slots (_header _pose _state ))

(defmethod styx_msgs::TrafficLight
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:pose __pose) (instance geometry_msgs::PoseStamped :init))
    ((:state __state) 0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _pose __pose)
   (setq _state (round __state))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:pose
   (&rest __pose)
   (if (keywordp (car __pose))
       (send* _pose __pose)
     (progn
       (if __pose (setq _pose (car __pose)))
       _pose)))
  (:state
   (&optional __state)
   (if __state (setq _state __state)) _state)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; geometry_msgs/PoseStamped _pose
    (send _pose :serialization-length)
    ;; uint8 _state
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; geometry_msgs/PoseStamped _pose
       (send _pose :serialize s)
     ;; uint8 _state
       (write-byte _state s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; geometry_msgs/PoseStamped _pose
     (send _pose :deserialize buf ptr-) (incf ptr- (send _pose :serialization-length))
   ;; uint8 _state
     (setq _state (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;;
   self)
  )

(setf (get styx_msgs::TrafficLight :md5sum-) "444a7e648c334df4cc0678bcfbd971da")
(setf (get styx_msgs::TrafficLight :datatype-) "styx_msgs/TrafficLight")
(setf (get styx_msgs::TrafficLight :definition-)
      "Header header
geometry_msgs/PoseStamped pose
uint8 state

uint8 UNKNOWN=4
uint8 GREEN=2
uint8 YELLOW=1
uint8 RED=0

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

================================================================================
MSG: geometry_msgs/PoseStamped
# A Pose with reference coordinate frame and timestamp
Header header
Pose pose

================================================================================
MSG: geometry_msgs/Pose
# A representation of pose in free space, composed of position and orientation. 
Point position
Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w

")



(provide :styx_msgs/TrafficLight "444a7e648c334df4cc0678bcfbd971da")



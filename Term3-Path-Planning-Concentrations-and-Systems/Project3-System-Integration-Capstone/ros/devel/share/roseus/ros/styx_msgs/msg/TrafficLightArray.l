;; Auto-generated. Do not edit!


(when (boundp 'styx_msgs::TrafficLightArray)
  (if (not (find-package "STYX_MSGS"))
    (make-package "STYX_MSGS"))
  (shadow 'TrafficLightArray (find-package "STYX_MSGS")))
(unless (find-package "STYX_MSGS::TRAFFICLIGHTARRAY")
  (make-package "STYX_MSGS::TRAFFICLIGHTARRAY"))

(in-package "ROS")
;;//! \htmlinclude TrafficLightArray.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass styx_msgs::TrafficLightArray
  :super ros::object
  :slots (_header _lights ))

(defmethod styx_msgs::TrafficLightArray
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:lights __lights) (let (r) (dotimes (i 0) (push (instance styx_msgs::TrafficLight :init) r)) r))
    )
   (send-super :init)
   (setq _header __header)
   (setq _lights __lights)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:lights
   (&rest __lights)
   (if (keywordp (car __lights))
       (send* _lights __lights)
     (progn
       (if __lights (setq _lights (car __lights)))
       _lights)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; styx_msgs/TrafficLight[] _lights
    (apply #'+ (send-all _lights :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; styx_msgs/TrafficLight[] _lights
     (write-long (length _lights) s)
     (dolist (elem _lights)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; styx_msgs/TrafficLight[] _lights
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _lights (let (r) (dotimes (i n) (push (instance styx_msgs::TrafficLight :init) r)) r))
     (dolist (elem- _lights)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get styx_msgs::TrafficLightArray :md5sum-) "c7bc7e70513c9a0e00aae005e6355eee")
(setf (get styx_msgs::TrafficLightArray :datatype-) "styx_msgs/TrafficLightArray")
(setf (get styx_msgs::TrafficLightArray :definition-)
      "Header header
TrafficLight[] lights

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
MSG: styx_msgs/TrafficLight
Header header
geometry_msgs/PoseStamped pose
uint8 state

uint8 UNKNOWN=4
uint8 GREEN=2
uint8 YELLOW=1
uint8 RED=0

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



(provide :styx_msgs/TrafficLightArray "c7bc7e70513c9a0e00aae005e6355eee")



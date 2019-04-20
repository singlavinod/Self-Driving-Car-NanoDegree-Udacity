; Auto-generated. Do not edit!


(cl:in-package styx_msgs-msg)


;//! \htmlinclude TrafficLightArray.msg.html

(cl:defclass <TrafficLightArray> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (lights
    :reader lights
    :initarg :lights
    :type (cl:vector styx_msgs-msg:TrafficLight)
   :initform (cl:make-array 0 :element-type 'styx_msgs-msg:TrafficLight :initial-element (cl:make-instance 'styx_msgs-msg:TrafficLight))))
)

(cl:defclass TrafficLightArray (<TrafficLightArray>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <TrafficLightArray>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'TrafficLightArray)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name styx_msgs-msg:<TrafficLightArray> is deprecated: use styx_msgs-msg:TrafficLightArray instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <TrafficLightArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader styx_msgs-msg:header-val is deprecated.  Use styx_msgs-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'lights-val :lambda-list '(m))
(cl:defmethod lights-val ((m <TrafficLightArray>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader styx_msgs-msg:lights-val is deprecated.  Use styx_msgs-msg:lights instead.")
  (lights m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <TrafficLightArray>) ostream)
  "Serializes a message object of type '<TrafficLightArray>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'lights))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'lights))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <TrafficLightArray>) istream)
  "Deserializes a message object of type '<TrafficLightArray>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'lights) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'lights)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'styx_msgs-msg:TrafficLight))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<TrafficLightArray>)))
  "Returns string type for a message object of type '<TrafficLightArray>"
  "styx_msgs/TrafficLightArray")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'TrafficLightArray)))
  "Returns string type for a message object of type 'TrafficLightArray"
  "styx_msgs/TrafficLightArray")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<TrafficLightArray>)))
  "Returns md5sum for a message object of type '<TrafficLightArray>"
  "c7bc7e70513c9a0e00aae005e6355eee")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'TrafficLightArray)))
  "Returns md5sum for a message object of type 'TrafficLightArray"
  "c7bc7e70513c9a0e00aae005e6355eee")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<TrafficLightArray>)))
  "Returns full string definition for message of type '<TrafficLightArray>"
  (cl:format cl:nil "Header header~%TrafficLight[] lights~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: styx_msgs/TrafficLight~%Header header~%geometry_msgs/PoseStamped pose~%uint8 state~%~%uint8 UNKNOWN=4~%uint8 GREEN=2~%uint8 YELLOW=1~%uint8 RED=0~%~%================================================================================~%MSG: geometry_msgs/PoseStamped~%# A Pose with reference coordinate frame and timestamp~%Header header~%Pose pose~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'TrafficLightArray)))
  "Returns full string definition for message of type 'TrafficLightArray"
  (cl:format cl:nil "Header header~%TrafficLight[] lights~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: styx_msgs/TrafficLight~%Header header~%geometry_msgs/PoseStamped pose~%uint8 state~%~%uint8 UNKNOWN=4~%uint8 GREEN=2~%uint8 YELLOW=1~%uint8 RED=0~%~%================================================================================~%MSG: geometry_msgs/PoseStamped~%# A Pose with reference coordinate frame and timestamp~%Header header~%Pose pose~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <TrafficLightArray>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'lights) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <TrafficLightArray>))
  "Converts a ROS message object to a list"
  (cl:list 'TrafficLightArray
    (cl:cons ':header (header msg))
    (cl:cons ':lights (lights msg))
))

// Auto-generated. Do not edit!

// (in-package styx_msgs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let TrafficLight = require('./TrafficLight.js');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class TrafficLightArray {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.lights = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('lights')) {
        this.lights = initObj.lights
      }
      else {
        this.lights = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type TrafficLightArray
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [lights]
    // Serialize the length for message field [lights]
    bufferOffset = _serializer.uint32(obj.lights.length, buffer, bufferOffset);
    obj.lights.forEach((val) => {
      bufferOffset = TrafficLight.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type TrafficLightArray
    let len;
    let data = new TrafficLightArray(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [lights]
    // Deserialize array length for message field [lights]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.lights = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.lights[i] = TrafficLight.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    object.lights.forEach((val) => {
      length += TrafficLight.getMessageSize(val);
    });
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'styx_msgs/TrafficLightArray';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'c7bc7e70513c9a0e00aae005e6355eee';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new TrafficLightArray(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.lights !== undefined) {
      resolved.lights = new Array(msg.lights.length);
      for (let i = 0; i < resolved.lights.length; ++i) {
        resolved.lights[i] = TrafficLight.Resolve(msg.lights[i]);
      }
    }
    else {
      resolved.lights = []
    }

    return resolved;
    }
};

module.exports = TrafficLightArray;

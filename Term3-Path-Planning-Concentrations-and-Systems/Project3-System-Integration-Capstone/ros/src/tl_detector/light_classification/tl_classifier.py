import os
import numpy as np
import tensorflow as tf
from styx_msgs.msg import TrafficLight
import rospy
import yaml


CWD = os.path.dirname(os.path.realpath(__file__))


class TLClassifier(object):
    def __init__(self):
        # load classifier model
        os.chdir(CWD)
        config_string = rospy.get_param("/traffic_light_config")
        self.config = yaml.load(config_string)
        self.is_site = self.config["is_site"]
        MODEL_FNAME = self.config["model_fname"]

        if self.is_site:
            print("*** Testing on site! Yay! ***")
        else:
            print("*** Testing in simulator ***")

        print("Loaded model file name: ", MODEL_FNAME)

        # setup tensorflow graph
        self.graph = tf.Graph()

        with self.graph.as_default():
            od_graph = tf.GraphDef()
            with tf.gfile.GFile(MODEL_FNAME, 'rb') as fid:
                serialized_graph = fid.read()
                od_graph.ParseFromString(serialized_graph)
                tf.import_graph_def(od_graph, name='')

            self.sess = tf.Session(graph=self.graph)
            self.image_tensor = self.graph.get_tensor_by_name('image_tensor:0')
            self.scores = self.graph.get_tensor_by_name('detection_scores:0')
            self.classes = self.graph.get_tensor_by_name('detection_classes:0')


    def get_class(self, image):
        """Determines the color of the traffic light in the image
        Args:
            image (cv::Mat): image containing the traffic light
        Returns:
            int: ID of traffic light color (specified in styx_msgs/TrafficLight)"""

        with self.graph.as_default():
            exp_img = np.expand_dims(image, axis=0)
            (scores, classes) = self.sess.run([self.scores, self.classes], feed_dict={self.image_tensor: exp_img})

        classes = list(np.squeeze(classes))
        scores = np.squeeze(scores)

        if len(scores) == 0 or scores[0] < 0.1:
            return TrafficLight.UNKNOWN
        else:
            result_class = classes[0]
            if result_class == 2:
                return TrafficLight.RED
            elif result_class == 1:
                return TrafficLight.GREEN
            elif result_class == 3:
                return TrafficLight.YELLOW

        return TrafficLight.UNKNOWN

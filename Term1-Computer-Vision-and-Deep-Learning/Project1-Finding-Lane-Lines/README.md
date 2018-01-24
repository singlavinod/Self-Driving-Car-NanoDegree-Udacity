# **Project 1: Finding Lane Lines on the Road** 

## Introduction

For a self driving vehicle to stay in a lane, the first step is to identify lane lines before issuing commands to the control system. Since the lane lines can be of different colors (white, yellow) or forms (solid, dashed) this seemingly trivial task becomes increasingly difficult. Moreover, the situation is further exacerbated with variations in lighting conditions. Thankfully, there are a number of mathematical tools and approaches available nowadays to effectively extract lane lines from an image or dashcam video. In this project, a program is written in python to identify lane lines on the road, first in an image, and later in a video stream. After a thorough discussion of the methodology, potential shortcomings and future improvements are suggested.

## Methodology
Before attempting to detect lane lines in a video, a software pipeline is developed for lane detection in a series of images. Only after ensuring that it works satisfactorily for test images, the pipeline is employed for lane detection in a video. 
The pipeline consisted of 5 major steps excluding reading and writing the image. 

Consider the test image given below:

![Figure 1](https://github.com/vsingla2/Self-Driving-Car-NanoDegree-Udacity/blob/master/Term1-Computer-Vision-and-Deep-Learning/Project1-Finding-Lane-Lines/writeup_images/solidWhiteRight.jpg)

The test image is first converted to grayscale from RGB using the helper function grayscale(). This produces the below image.

![Figure 2](https://github.com/vsingla2/Self-Driving-Car-NanoDegree-Udacity/blob/master/Term1-Computer-Vision-and-Deep-Learning/Project1-Finding-Lane-Lines/writeup_images/_gray_solidWhiteRight.jpg)

The grayscaled image is given a gaussian blur to remove noise or spurious gradients. The blurred image is given below.

![Figure 3](https://github.com/vsingla2/Self-Driving-Car-NanoDegree-Udacity/blob/master/Term1-Computer-Vision-and-Deep-Learning/Project1-Finding-Lane-Lines/writeup_images/_blur_gray_solidWhiteRight.jpg)

Canny edge detection is applied on this blurred image and a binary image shown below is produced.

![Figure 4](https://github.com/vsingla2/Self-Driving-Car-NanoDegree-Udacity/blob/master/Term1-Computer-Vision-and-Deep-Learning/Project1-Finding-Lane-Lines/writeup_images/_edges_solidWhiteRight.jpg)

This image contains edges that are not relevant for lane finding problem. A region of interest is defined to separate the lanes from sorrounding environment and a masked image containing only the lanes is extracted using cv2.bitwise_and() function from opencv library. This can be seen below.

![Figure 5](https://github.com/vsingla2/Self-Driving-Car-NanoDegree-Udacity/blob/master/Term1-Computer-Vision-and-Deep-Learning/Project1-Finding-Lane-Lines/writeup_images/_masked_edges_solidWhiteRight.jpg)

This binary image of identified lane lines is finally merged with the original image using cv2.addweighted() function from opencv library. This produces an image given below. Note that, this is without making any modifications to the drawlines() helper function. It can be observed that the lines are not continuous as required.

![Figure 6](https://github.com/vsingla2/Self-Driving-Car-NanoDegree-Udacity/blob/master/Term1-Computer-Vision-and-Deep-Learning/Project1-Finding-Lane-Lines/writeup_images/_lines_edges_solidWhiteRight.jpg)

## Modification to drawlines() helper function
Since the resulting line segments after the processing the image through the pipeline are not continuous, a modification is made to the drawlines() helper function. Consider the code snippet below:

```
def draw_lines_robust(img, lines, color=[200, 0, 0], thickness = 10):
  
    x_left = []
    y_left = []
    x_right = []
    y_right = []
    imshape = image.shape
    ysize = imshape[0]
    ytop = int(0.6*ysize) # need y coordinates of the top and bottom of left and right lane
    ybtm = int(ysize) #  to calculate x values once a line is found
    
    for line in lines:
        for x1,y1,x2,y2 in line:
            slope = float(((y2-y1)/(x2-x1)))
            if (slope > 0.5): # if the line slope is greater than tan(26.52 deg), it is the left line
                    x_left.append(x1)
                    x_left.append(x2)
                    y_left.append(y1)
                    y_left.append(y2)
            if (slope < -0.5): # if the line slope is less than tan(153.48 deg), it is the right line
                    x_right.append(x1)
                    x_right.append(x2)
                    y_right.append(y1)
                    y_right.append(y2)
    # only execute if there are points found that meet criteria, this eliminates borderline cases i.e. rogue frames
    if (x_left!=[]) & (x_right!=[]) & (y_left!=[]) & (y_right!=[]): 
        left_line_coeffs = np.polyfit(x_left, y_left, 1)
        left_xtop = int((ytop - left_line_coeffs[1])/left_line_coeffs[0])
        left_xbtm = int((ybtm - left_line_coeffs[1])/left_line_coeffs[0])
        right_line_coeffs = np.polyfit(x_right, y_right, 1)
        right_xtop = int((ytop - right_line_coeffs[1])/right_line_coeffs[0])
        right_xbtm = int((ybtm - right_line_coeffs[1])/right_line_coeffs[0])
        cv2.line(img, (left_xtop, ytop), (left_xbtm, ybtm), color, thickness)
        cv2.line(img, (right_xtop, ytop), (right_xbtm, ybtm), color, thickness)
```
Observe that a classification of lines identified through houghlines criteria is made based on their slope. Evidently, lines with positive slope are classified as being on the left lane and lines with negative slope are classified as being on the right lane. Flat lines having slope below absolute value of 0.5 are discarded. After storing points for respective left and right lanes, a linear curve fit (degree 1) using polyfit() function from numpy library is done to obtain the slope and intercept of left and right lanes. Following this, x coordinates are found for respective y top and btm coordinates (user defined) using the lane equations for both lanes. This gives us starting and ending coordinates for both left and right lane. Finally, lines are drawn using cv2.line() function to connect these points and the image is merged with the original image as before to produce the below result.

![Figure 7](https://github.com/vsingla2/Udacity-SDCND-Term1-P1/blob/master/writeup_images/_lines_edges_solidWhiteRight_draw_lines_modification.jpg)

## Implementing the pipeline on test videos

The pipeline developed in the project is implemented on 2 different test videos.

1. The first test video consists of a solid white right lane and dashed white left lane. As it can be observed, the pipeline produces acceptable results for this case.

[![Figure 8](http://i.imgur.com/xxYWezT.jpg)](https://www.youtube.com/watch?v=Td0nwyttV7g)

2. The second test video consists of a solid yellow left lane and dashed white right lane. As it can be observed, the pipeline produces acceptable results for this case.

[![Figure 8](http://i.imgur.com/StGtIIA.jpg)](https://youtu.be/vGnH1O8CUIE)

## Shortcomings observed in the current pipeline

1. Since the first step is converting the image to grayscale from RGB, shadows and light variations in the environment are difficult to capture. This can be gleaned from the fact that the current pipeline while working reasonably well for the first two test videos breaks down for the challenge video.

1. The lane lines detected in the resulting output are not as stable as the ones in "P1_example" video. This is not desirable since it is difficult to follow rapidly changing steering commands.

## Possible improvements to the current pipeline

1. Instead of converting the image to grayscale directly, the test image can be preprocessed using RGB normalization. This can help in mitigating effect of shadows and lighting variations and make the current pipeline more robust. A function definition for producing a normalized RGB image is shown below. This approach did not produce an improvement when implemented in it's current form and can be further improved. 

```
# Reference: http://akash0x53.github.io/blog/2013/04/29/RGB-Normalization/
def normalized_rgb(img):
        imshape = img.shape
        ysize = imshape[0]
        xsize = imshape[1]
        norm=np.zeros((ysize,xsize,3),np.float32)
        norm_rgb=np.zeros((ysize,xsize,3),np.uint8)
        b=img[:,:,0]
        g=img[:,:,1]
        r=img[:,:,2]
        sum = b + g + r
        norm[:,:,0]=b/sum*255.0
        norm[:,:,1]=g/sum*255.0
        norm[:,:,2]=r/sum*255.0
        norm_rgb=cv2.convertScaleAbs(norm)
        return norm_rgb
```

2. Another way of dealing with shadows and variations in light is to use a different colorspace, for example, say HSV instead of RGB. A function definition for applying RGB to HSV transform is shown below. The test image before being fed to the pipeline is given an RGB to HSV transform. This approach while producing acceptable results for the first two test videos did not result in any significant improvements in the performance of identifying lane lines for challenge video.

```
def rgbtohsv(img):
    "Applies rgb to hsv transform"
    return cv2.cvtColor(img, cv2.COLOR_RGB2HSV)
```

3. Another improvement in the pipeline can be to include a running average of slopes for identified lane lines so that there is a smooth transition from one frame to the next. This avoids rapid changes in commands to the steering control system.

1. Further, machine learning approaches can be explored to make the lane finding pipeline more robust in future.

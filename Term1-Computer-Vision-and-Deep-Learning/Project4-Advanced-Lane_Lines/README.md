## Advanced Lane Finding Project
---

The goals / steps of this project are the following:

* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.

[//]: # (Image References)

[image1]: ./output_images/image1.png "Distortion correction for calibration image"
[image2]: ./output_images/image2.png "Distortion correction for test image"
[image3]: ./output_images/image3.png "Binary thresholded image"
[image4]: ./output_images/image4.png "Perspective transform"
[image5]: ./output_images/image5.png "Identify lane lines"
[image6]: ./output_images/image6.png "Lane lines mapped"
[video1]: ./output_video.mp4 "Video"

#### [Rubric Points](https://review.udacity.com/#!/rubrics/571/view) 

Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  
---

### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.

You're reading it!

### Camera Calibration

#### 1. Briefly state how you computed the camera matrix and distortion coefficients. Provide an example of a distortion corrected calibration image.

The code for this step is contained in the third code cell of the IPython notebook located in "./Advanced-Lane-Lines.ipynb".  

I start by preparing "object points", which will be the (x, y, z) coordinates of the chessboard corners in the world. Here I am assuming the chessboard is fixed on the (x, y) plane at z=0, such that the object points are the same for each calibration image.  Thus, `objp` is just a replicated array of coordinates, and `objpoints` will be appended with a copy of it every time I successfully detect all chessboard corners in a test image.  `imgpoints` will be appended with the (x, y) pixel position of each of the corners in the image plane with each successful chessboard detection.  

I then used the output `objpoints` and `imgpoints` to compute the camera calibration and distortion coefficients using the `cv2.calibrateCamera()` function.  I applied this distortion correction to the test image using the `cv2.undistort()` function. 

![Distortion correction for calibration image][image1]

### Pipeline (single images)

#### 1. Provide an example of a distortion-corrected image.

To demonstrate this step, I will describe how I apply the distortion correction to one of the test images like this one:

![Distortion correction for test image][image2]

#### 2. Describe how (and identify where in your code) you used color transforms, gradients or other methods to create a thresholded binary image.  Provide an example of a binary image result.

I used a combination of color and gradient thresholds to generate a binary image (thresholding steps in cell 6 of jupyter notebook).  Here's an example of my output for this step.

![Binary thresholded image][image3]

#### 3. Describe how (and identify where in your code) you performed a perspective transform and provide an example of a transformed image.

The code for my perspective transform includes a function called `perspective_trans()`, which appears in the helper functions in cell two of the ipython notebook. The `perspective_trans()` function takes image (`img`) as an input. I chose the hardcode the source and destination points in the following manner:

```python
src = np.float32([[588,470], [245,719], [1142, 719],[734,470]])
dst = np.float32([[320,0], [320,720], [960, 720],[960,0]])
```

This resulted in the following source and destination points:

| Source        | Destination   | 
|:-------------:|:-------------:| 
| 588, 470      | 320, 0        | 
| 245, 719      | 320, 720      |
| 1142, 719     | 960, 720      |
| 734, 470      | 960, 0        |

I verified that my perspective transform was working as expected by drawing the `src` and `dst` points onto a test image and its warped counterpart to verify that the lines appear parallel in the warped image.

![Perspective transform][image4]

#### 4. Describe how (and identify where in your code) you identified lane-line pixels and fit their positions with a polynomial?

Then I used a sliding window approach (see cell 8 in the jupyter notebook) to fit my lane lines with a 2nd order polynomial as shown below:

![Identify lane lines][image5]

#### 5. Describe how (and identify where in your code) you calculated the radius of curvature of the lane and the position of the vehicle with respect to center.

I wrote a function `curvature_eval()` to calculate offset and radius of curvature of the lane. See cell 2 and cell 8 of the ipython notebook.

#### 6. Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.

I implemented this step using the function `map_color()` in cell 2 of the jupyter notebook. Here is an example of my result on a test image:

![Lane lines mapped][image6]

---

### Pipeline (video)

#### 1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (wobbly lines are ok but no catastrophic failures that would cause the car to drive off the road!).

Here's a [link to my video result](https://youtu.be/WQ1hgCh3bq0)

---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?
Challenges:
* Getting the thresholds right
* Implementing class structure for discarding outliers
* Tuning sanity checks to ensure lane detection pipeline doesn't stagnate between frames

Likely failures of pipeline:
* on steep bends/curves
* challenge videos

Future Work:
* Implementing dynamic thresholding
* Look ahead filter


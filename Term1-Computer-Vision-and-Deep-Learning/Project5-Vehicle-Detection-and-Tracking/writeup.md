## Vehicle Detection Project

The goals / steps of this project are the following:

* Perform a Histogram of Oriented Gradients (HOG) feature extraction on a labeled training set of images and train a classifier Linear SVM classifier
* Optionally, you can also apply a color transform and append binned color features, as well as histograms of color, to your HOG feature vector. 
* Note: for those first two steps don't forget to normalize your features and randomize a selection for training and testing.
* Implement a sliding-window technique and use your trained classifier to search for vehicles in images.
* Run your pipeline on a video stream (start with the test_video.mp4 and later implement on full project_video.mp4) and create a heat map of recurring detections frame by frame to reject outliers and follow detected vehicles.
* Estimate a bounding box for vehicles detected.

[//]: # "Image References"
[image1]: ./writeup_imgs/car_not_car.png
[image2]: ./writeup_imgs/HOG_example.jpg
[image3]: ./writeup_imgs/visualize_colorspaces_rgb.PNG
[image4]: ./writeup_imgs/visualize_colorspaces_hsv.PNG
[image5]: ./writeup_imgs/visualize_colorspaces_ycrcb.PNG
[image6]: ./writeup_imgs/visualize_colorspaces_luv.PNG
[image7]: ./writeup_imgs/visualize_colorspaces_yuv.PNG
[image8]: ./writeup_imgs/pipeline_detections1.PNG
[image9]: ./writeup_imgs/pipeline_detections2.PNG
[image10]: ./writeup_imgs/pipeline_detections3.PNG
[image11]: ./writeup_imgs/heatmap1.PNG
[image12]: ./writeup_imgs/heatmap2.PNG
[image13]: ./writeup_imgs/heatmap3.PNG
[video1]: ./output_video.mp4

## [Rubric](https://review.udacity.com/#!/rubrics/513/view) Points
### Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---
### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.  You can submit your writeup as markdown or pdf.  

You're reading it!

### Histogram of Oriented Gradients (HOG)

#### 1. Explain how (and identify where in your code) you extracted HOG features from the training images.

The HOG features are extracted using extract_features() function in cell 8 (for training the classifier) and single_img_features() function in cell 9 (for visualizing) of the jupyter notebook code.ipynb.

I started by reading in all the `vehicle` and `non-vehicle` images.  Here are some examples of each of the `vehicle` and `non-vehicle` classes:

![car_not_car example][image1]

I then explored different color spaces and different `skimage.hog()` parameters (`orientations`, `pixels_per_cell`, and `cells_per_block`). I grabbed random images from each of the two classes and displayed them to get a feel for what the `skimage.hog()` output looks like.

Here is an example using the `YCrCb` color space and HOG parameters of `orientations=6`, `pixels_per_cell=(8, 8)` and `cells_per_block=(2, 2)`

![HOG_example][image2]

#### 2. Explain how you settled on your final choice of HOG parameters.

The approach followed to arrive at the final set of parameters was to first freeze some parameters like colorspace, which features to use and number of hog channels based on visualizations and then train a classifier by varying other parameters. The final decision was made to choose a combination of parameters that resulted in maximum test set accuracy while making the predictions in real time.

Colorspaces: Based on color histogram binning across all channels for different colorspaces (RGB, LUV, YUV, YCrCb, HSV), an attempt was made to pinpoint what separates a car image from a not-car image. Each of these are discussed individually below:

* RGB, HSV: All three channels have different spectrum for car and non-car images.

![RGB][image3]

![HSV][image4]

* YCrCb, LUV, YUV: Channel one in the car image appears to be the one having distinct spectrum as compared to the non-car image.

![YCrCb][image5]

![LUV][image6]

![YUV][image7]

HOG features: After visualizing individual hog channel features for several images, a decision was made to retain all hog channels for training the classifier. This is because, different hog channels appear to capture different features from the car class. For example, in the image shown below, channel-0 and channel-1 capture the overall structure of the car while channel-2 distinctly captures the taillights.

![HOG_example][image2]

One of the objectives of extracting features from cars is to achieve color invariance. As such, YUV and YCrCb colorspaces which implement the differences in color in their second and third channels become more lucrative than others. A feature vector containing HOG features from all three channels, color histogram and spatial binning features for YUV and YCrCb colorspaces was selected. The classifier was trained using different `pix_per_cell` values and the combination of parameters which resulted in maximum test accuracy was selected.

#### 3. Describe how (and identify where in your code) you trained a classifier using your selected HOG features (and color features if you used them).

I trained a linear SVM using histogram of gradients, color histogram and spatial color features. The code for training the classifier is given in cell 17 of the project jupyter notebook. The test accuracy achieved across multiple runs was consistently greater than 98 %.

### Sliding Window Search

#### 1. Describe how (and identify where in your code) you implemented a sliding window search.  How did you decide what scales to search and how much to overlap windows?

The sliding window search is done using the find_cars function located in cell 10 of the jupyter notebook. I started off by deciding the region of interest that can contain cars (ystart, ystop) and then gradually increased scale from 1.5 to 3.0. The threshold was increased as more number of scaled searches were performed to remove the outliers. The overlap (either 50 % or 75 %) was set by deciding displacement of the cells rather than the value of overlap itself. 

#### 2. Show some examples of test images to demonstrate how your pipeline is working.  What did you do to optimize the performance of your classifier?

To optimize the performance of classifier, I tried various scales (1.5, 1.75, 2.0, 2.25) and overlaps (50% or 75%) to classify images. Ultimately I searched on two scales (1.5, 1.75) using YCrCb 3-channel HOG features plus spatially binned color and histograms of color in the feature vector, which provided a nice result.  Here are some example images:

![alt text][image8]
![alt text][image9]
![alt text][image10]

---

### Video Implementation

#### 1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (somewhat wobbly or unstable bounding boxes are ok as long as you are identifying the vehicles most of the time with minimal false positives.)
Here's a [link to my video result](./output_video.mp4)


#### 2. Describe how (and identify where in your code) you implemented some kind of filter for false positives and some method for combining overlapping bounding boxes.

I recorded the positions of positive detections in each frame of the video.  From the positive detections I created a heatmap and then thresholded that map to identify vehicle positions.  I then used `scipy.ndimage.measurements.label()` to identify individual blobs in the heatmap.  I then assumed each blob corresponded to a vehicle.  I constructed bounding boxes to cover the area of each blob detected.  

Here's an example result showing the heatmap from a series of frames of video, the corresponding labels and the bounding boxes then overlaid on the last frame of video:

![alt text][image11]

### Here are a few more frames and their corresponding heatmaps &labels:

![alt text][image12]
![alt text][image13]

---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

Since the amount of parameters to tune in this project is large, a lot of trial and error was required. However, during the process, I developed insights that connected the inference to lesson material and resulted in great personal satisfaction. I'll address the individual concerns below:

Data: I tried augmenting the vehicles data with the newly released Udacity data. After successfully playing around with colorspaces (see Explore mpimg & cv2 imread.ipynb and Explore_colorspaces.ipynb), cropping and resizing frames from the dataset (see Augment_dataset.ipynb) as per the associated labels csv and training the classifier with approximately same number of car and non-car images, the predictions I was getting had a lot of false positives. I would like to work on this in future to find out what went wrong when I should have expecting a more generalized model.

GridSearchCV and RandomizedSearchCV: I also tried using the inbuilt functions in sklearn library to optimize the C parameter. However, after running my computer for a long time it was still running to no avail. I would like to incorporate this in future and see if it improves performance. The associated code looked something like this:

~~~
# Check the training time for the SVC
t = time.time()
parameters = {'kernel':['linear'], 'C':[1, 10, 100]}
svr = svm.SVC()
svc = GridSearchCV(svr, parameters)
svc.fit(X_train, y_train)
print('Best parameter combination: ' + svc.best_params_)
t2 = time.time()
print(round(t2 - t, 2), 'Seconds to train SVC...')
# Check the score of the SVC
print('Test Accuracy of SVC = ', round(svc.score(X_test, y_test), 4))
~~~

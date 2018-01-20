## Behavioral Cloning Project

The goals / steps of this project are the following:
* Use the simulator to collect data of good driving behavior
* Build, a convolution neural network in Keras that predicts steering angles from images
* Train and validate the model with a training and validation set
* Test that the model successfully drives around track one without leaving the road
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./write_up_images/Fig1.jpg "center"
[image2]: ./write_up_images/Fig2.jpg "right"
[image3]: ./write_up_images/Fig3.jpg "mid"
[image4]: ./write_up_images/Fig4.jpg "center"
[image5]: ./write_up_images/Fig5.jpg "flipped"
[image6]: ./write_up_images/Fig6.jpg "cropped"
[image7]: ./write_up_images/Fig7.png "Loss vs Epoch"

### Rubric Points
Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/432/view) individually and describe how I addressed each point in my implementation.  

---
### Files Submitted & Code Quality

#### 1. Submission includes all required files and can be used to run the simulator in autonomous mode

My project includes the following files:
* model.py containing the script to create and train the model
* drive.py for driving the car in autonomous mode
* model.h5 containing a trained convolution neural network 
* writeup_report.md or writeup_report.pdf summarizing the results

#### 2. Submission includes functional code

Using the Udacity provided simulator and my drive.py file, the car can be driven autonomously around the track by executing 
```sh
python drive.py model.h5
```

#### 3. Submission code is usable and readable

The model.py file contains the code for training and saving the convolution neural network. The file shows the pipeline I used for training and validating the model, and it contains comments to explain how the code works.

### Model Architecture and Training Strategy

#### 1. An appropriate model architecture has been employed

My model consists of a convolution neural network with 2x2 filter sizes and depths between 24 and 64 (model.py lines 86-90).

The model includes RELU layers to introduce nonlinearity (code lines 86-90), and the data is normalized in the model using a Keras lambda layer (code line 85). 

#### 2. Attempts to reduce overfitting in the model

The model was trained and validated on different data sets to ensure that the model was not overfitting (code line 14-15). The model was tested by running it through the simulator and ensuring that the vehicle could stay on the track.

#### 3. Model parameter tuning

The model used an adam optimizer, so the learning rate was not tuned manually (model.py line 99).

#### 4. Appropriate training data

Training data was chosen to keep the vehicle driving on the road. I used a combination of center lane driving, recovering from the left and right sides of the road. This was done for both tracks to help generalize the model.

For details about how I created the training data, see the next section. 

### Model Architecture and Training Strategy

#### 1. Solution Design Approach

The model architecture I used was inspired from a similar network employed by NVIDIA team for steering control of an autonomous vehicle.
I thought this model is appropriate because there are 5 convolutional layers and documented success of the network for steering control. In order to gauge how well the model was working, I split my image and steering angle data into a training and validation set in 80 to 20 ratio. The final step was to run the simulator to see how well the car was driving around the track. There were a few spots where the vehicle fell off the track on increasing the set speed in drive.py file. To improve the driving behavior in these cases, I augmented the training data with driving data from track 2.

At the end of the process, the vehicle is able to drive autonomously around the track without leaving the road.

#### 2. Final Model Architecture

The final model architecture (model.py lines 85-95) consisted of a convolutional neural network with the following layers and layer sizes.

| Layer                            |    Size       |
| --------------------             |:-------------:|
| Input                            | 65 x 320 x 3  |
| Lambda (normalization)           | 65 x 320 x 3  |
| Convolution with relu activation | 5 x 5 x 24 with 2x2 filters  |
| Convolution with relu activation | 5 x 5 x 36 with 2x2 filters  |
| Convolution with relu activation | 5 x 5 x 48 with 2x2 filters  |
| Convolution with relu activation | 3 x 3 x 64   |
| Convolution with relu activation | 3 x 3 x 64   |
| Flatten                          |              |
| Fully connected                  | 100          |
| Fully connected                  | 50          |
| Fully connected                  | 10          |
| Output                           | 1          |

#### 3. Creation of the Training Set & Training Process

To capture good driving behavior, I first recorded two laps on track one using center lane driving. Here is an example image of center lane driving:

![center][image1]

I then recorded the vehicle recovering from the left side and right sides of the road back to center so that the vehicle would learn to navigate the turns instead of memorizing the track. These images show what a recovery looks like starting from right to center :

![right][image2]

![mid][image3]

![center][image4]

Then I repeated this process on track two in order to get more data points.

To augment the data sat, I also flipped images and steering measurements thinking that this would help generalize the model. For example, here is a view from the first figure that has then been flipped:

![center][image1]             ![flipped][image5]

After the collection process, I had 47181 number of data points. I then preprocessed this data by cropping irrelevant data (sample below) from the top and bottom of the image. This led to a final image size of 65 x 320 x 3 which significantly reduced the computational requirements.

![center][image1]             ![cropped][image6]

I finally randomly shuffled the data set and put 20% of the data into a validation set. 

I used this training data for training the model. The validation set helped determine if the model was over or under fitting. The ideal number of epochs was 3 as evidenced by increasing loss after 3 epochs (see below). I used an adam optimizer so that manually training the learning rate wasn't necessary.

![loss_vs_epoch][image7]

**Further improvements can be made to the simple PI controller used in drive.py to smoothen steering angles for the current model.**

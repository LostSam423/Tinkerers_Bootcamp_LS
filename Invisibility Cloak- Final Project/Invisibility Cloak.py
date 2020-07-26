#!/usr/bin/env python
# coding: utf-8

# # Invisibility Cloak
# Let's get started with creating an invivibility cloak using OpenCV. Import libraries to be used.

# In[ ]:


import cv2
import numpy as np
import time 


# Read from the video recording.

# In[ ]:


fourcc = cv2.VideoWriter_fourcc(*'MP4V')
out = cv2.VideoWriter('cloak.mp4', fourcc, 20.0, (640, 480))


# Reading from the webcam.

# In[ ]:


cap = cv2.VideoCapture(0)


# Allow the system to sleep for 3 seconds before the webcam starts and initialize variables.

# In[ ]:


time.sleep(3)
count = 0
back = 0


# Capture the background in range of 60

# In[ ]:


for i in range(60):
    ret, back = cap.read()
back = np.flip(back, axis=1) ## flips the matrix along vertical axis


# # Loop Start
# 
# Read every frame from the webcam, until the camera is open

# In[ ]:


while True:
    ret, img = cap.read()
    if not ret:
        break
    count += 1
    img = np.flip(img, axis=1)

    ## Color converted from BGR to HSV
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    ## Two masks are generated for capturing different shades of red
    lower_red = np.array([0, 120, 50])   
    upper_red = np.array([10, 255,255])
    mask1 = cv2.inRange(hsv, lower_red, upper_red) ## range for the mask1 is specified

    lower_red = np.array([170, 120, 70])
    upper_red = np.array([180, 255, 255])
    mask2 = cv2.inRange(hsv, lower_red, upper_red) ## range for the mask2 is specified
    
    ## both masks are added
    mask1 = mask1 + mask2

    ## MORPH_OPEN and MORPH_DILATE help to reduce the noise and dilate the frames
    mask1 = cv2.morphologyEx(mask1, cv2.MORPH_OPEN, np.ones((3, 3), np.uint8))
    mask1 = cv2.morphologyEx(mask1, cv2.MORPH_DILATE, np.ones((3, 3), np.uint8))

    ## Inverted mask is created and overwritten on mask2
    mask2 = cv2.bitwise_not(mask1)

    ## And operator selects out the red portion i.e. cloth and mask2 removes that part (as it is inverse of red)
    res1 = cv2.bitwise_and(img, img, mask=mask2)

    ## Image is created only for the removed portion by adding the static background recorded at the start
    res2 = cv2.bitwise_and(back, back, mask=mask1)

    ## Generating the final output and writing into the image file
    finalOutput = cv2.addWeighted(res1, 1, res2, 1, 0)
    out.write(finalOutput)
    cv2.imshow("magic", finalOutput)
    if cv2.waitKey(1)== 27:
        break

## Release the cap and out variables and destroy all windows
cap.release()  
out.release()   
cv2.destroyAllWindows()


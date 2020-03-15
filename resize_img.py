import cv2
imgName="pic.jpeg"
img=cv2.imread(imgName)
size = (500,500)  
shrink = cv2.resize(img, size)
cv2.imwrite("dst.png",shrink)  
import cv2
im=cv2.imread("test.png")
print im.shape
#cv2.rectangle(im,(830,140),(1530,600),(0,255,0),3)
im2=im[240:600,830:1530,:]
size = (500,500)  
shrink = cv2.resize(im2, size)
cv2.imwrite("dst2.png",shrink)
cv2.imshow('dst',im)
cv2.waitKey(0)
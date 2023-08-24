import cv2
import sys
import math
import numpy as np
import transform

WINDOW_NAME = "scanner"

def resize_img(img : cv2.UMat, new_height: int) -> cv2.UMat:
    (w, h) = img.shape[:2]
    aspect = w // h
    new_width = int(aspect * new_height)
    return cv2.resize(img, (new_width, new_height))

def detect_edges(img):
    gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    #Since edge detection is susceptible to noise in the image, 
    #first step is to remove the noise in the image with a Gaussian filter.
    blured = cv2.GaussianBlur(gray_img, (5, 5), 0)

    edged = cv2.Canny(blured, 50, 200)

    return edged

def detect_countours(img):
    cnts, hierarchy = cv2.findContours(img.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    #There coud be Countours of very small areas being detected, like the countour of a letter
    #or any other artifact that isnt really relevant to the paper detection,
    #so we sort the countours by area and only keep the bigger ones, which are the most likely
    # to be of interest.
    cnts = sorted(cnts, key = cv2.contourArea, reverse = True)[:5]

    return cnts

def main():
    img = cv2.imread("assets/receipt2.jpg")

    if img is None:
        print("failed to load image")
        return

    img = resize_img(img, 500)
    edged = detect_edges(img)
    cnts = detect_countours(edged)
    print(len(cnts))

    contoured_img = img.copy()
    paper_cnt_index = -1
    max_area = float('-inf')
    aprox = cv2.UMat

    for i, c in enumerate(cnts):

        peri = cv2.arcLength(c, True)
        aproximated = cv2.approxPolyDP(c, 0.02 * peri, True)
        area = cv2.contourArea(c)

        #paper_cnt_index = i
        if len(aproximated) == 4 and area > max_area:
            paper_cnt_index = i
            max_area = area
            aprox = aproximated
            print(f"num vertices: {len(aproximated)}, area {area}")


    assert paper_cnt_index != -1

    screen_cnt = cnts[paper_cnt_index]
    cv2.drawContours(contoured_img, [screen_cnt], 0, (240, 0, 159), 2)
    warped = transform.four_point_transform(img, aprox.reshape(4, 2))


    while True:
        cv2.imshow(WINDOW_NAME, contoured_img)
        cv2.imshow("not edged", warped)

        key = cv2.waitKey(1)
        if key == 27 or cv2.getWindowProperty(WINDOW_NAME, cv2.WND_PROP_VISIBLE) < 1:
            break


if __name__ == '__main__':
    main()

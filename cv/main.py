import cv2
import sys
import math
import numpy as np

WINDOW_NAME = "scanner"

def detect_edges(img):
    gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    #Since edge detection is susceptible to noise in the image, 
    #first step is to remove the noise in the image with a Gaussian filter.
    blured = cv2.GaussianBlur(gray_img, (11, 11), 0)
    edged = cv2.Canny(blured, 100, 200)

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
    img = cv2.imread("assets/receipt.jpg")

    if img is None:
        print("failed to load image")
        return

    (w, h) = img.shape[:2]
    apsect = w / h
    img = cv2.resize(img, (w // 2, h))

    edged = detect_edges(img)
    cnts = detect_countours(edged)


    contoured_img = img.copy()
    paper_cnt_index = -1
    max_peri = float('-inf')

    for i, c in enumerate(cnts):
        peri = cv2.arcLength(c, True)
        aproximated = cv2.approxPolyDP(c, 0.1 * peri, True)

        cv2.drawContours(contoured_img, [aproximated], 0, (240, 0, 159), 1)

        print(f"num vertices: {len(aproximated)}, peri {peri}")
        #paper_cnt_index = i
        if len(aproximated) == 4 and peri > max_peri:
            paper_cnt_index = i
            max_peri = peri

    #assert paper_cnt_index != -1



    while True:
        cv2.imshow(WINDOW_NAME, contoured_img)
        #cv2.imshow("not edged", contoured_img)

        key = cv2.waitKey(1)
        if key == 27 or cv2.getWindowProperty(WINDOW_NAME, cv2.WND_PROP_VISIBLE) < 1:
            break


if __name__ == '__main__':
    main()

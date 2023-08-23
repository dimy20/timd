import cv2
import sys
import math
import numpy as np

def rotate(img, angle):
    (h, w) = img.shape[:2]
    (cx, cy) = (w // 2, h // 2)

    M = cv2.getRotationMatrix2D((cx, cy), -angle, 1.0)

    # cos sin Tx
    # sin cos Ty

    cos = np.abs(M[0, 0])
    sin = np.abs(M[0, 1])

    # rotation matrix (getRotationMatrix2D also contains translation in case we pass a center of rotation),
    R = np.array([[cos, sin],
                  [sin, cos]])

    # Consider the vector made up of the w and h components,
    # By applying the same rotation to that vector, we get new
    # scaled x and y axis for the rotated image.
    v = np.array([[w],
                  [h]])

    res = np.dot(R, v).astype(int)

    new_w = res[0, 0]
    new_h = res[1, 0]

    # The rotation Transformation is made of 3 matrices as follows.
    # T(cx, cy) * R * T(-cx, -cy) => 
    # translate to the origin, rotate, translate back
    # When we translate back we need to shift the positions by the difference with the new center,
    # So that vectors are translated back taking the new center as reference.

    M[0, 2] += (res[0] / 2) - cx
    M[1, 2] += (res[1] / 2) - cy

    # Apply transfomation to vectors.
    return cv2.warpAffine(img, M, (new_w, new_h))

def main():
    img = cv2.imread("./assets/test_image.jpg")
    img = cv2.resize(img, (img.shape[0] // 2, img.shape[1] // 2))
    (w, h, d) = img.shape
    center = (w // 2, h // 2)

    while True:
        T_img = rotate(img, 45)
        cv2.imshow("nice rotation", T_img)

        key = cv2.waitKey(1)
        if key == 27:
            break

if __name__ == '__main__':
    main()

import cv2
import numpy as np
import sys

def sort_points(pts : np.ndarray) -> np.ndarray:
    xy_sums = pts.sum(axis = 1)
    xy_diff = np.diff(pts, axis = 1)

    print(xy_diff)
    print(xy_sums)

    top_left = pts[np.argmin(xy_sums)]
    top_right = pts[np.argmin(xy_diff)]

    bottom_right = pts[np.argmax(xy_sums)]
    bottom_left = pts[np.argmax(xy_diff)]

    # *0    *1
    #
    # *3    *2
    ret = np.zeros((4, 2), dtype = np.float32)

    ret[0] = top_left
    ret[1] = top_right
    ret[2] = bottom_right
    ret[3] = bottom_left

    return ret

def line_length(point_a, point_b):
    dx = point_a[0] - point_b[0]
    dy = point_a[1] - point_b[1]
    return np.sqrt((dx ** 2) + (dy ** 2))

def four_point_transform(img : cv2.UMat, pts: np.ndarray):
    src_points = sort_points(pts)
    (t_left, t_right, b_right, b_left) = src_points

    bottom_line_length = int(line_length(b_left, b_right))
    top_line_length = int(line_length(t_left, t_right))

    dest_width = max(bottom_line_length, top_line_length)

    left_line_length = int(line_length(b_left, t_left))
    right_line_length = int(line_length(b_right, t_left))

    dest_height = max(left_line_length, right_line_length)

    dest_points = np.array([
        [0, 0],
        [dest_width -1, 0],
        [dest_width -1, dest_height -1],
        [0, dest_height -1]
        ], dtype = np.float32)
    
    proj_matrix = cv2.getPerspectiveTransform(src_points, dest_points)
    projected = cv2.warpPerspective(img, proj_matrix, (dest_width, dest_height))

    return projected

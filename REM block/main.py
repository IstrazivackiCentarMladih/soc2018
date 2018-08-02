# import the necessary packages
from scipy.spatial import distance as dist
from imutils.video import FileVideoStream
from imutils.video import VideoStream
from imutils import face_utils
import numpy as np
import argparse
import imutils
import time
import dlib
import cv2
import serial
import time

ser = serial.Serial('COM5', 9600)
brojac = 0

# varijable za izrcun srednje vrijednosti EAR-a
zbroj = 0
n = 0
ar_sr = 0



def eye_aspect_ratio(eye):
    # compute the euclidean distances between the two sets of
    # vertical eye landmarks (x, y)-coordinates
    A = dist.euclidean(eye[1], eye[5])
    B = dist.euclidean(eye[2], eye[4])

    # compute the euclidean distance between the horizontal
    # eye landmark (x, y)-coordinates
    C = dist.euclidean(eye[0], eye[3])

    # compute the eye aspect ratio
    ear = (A + B) / (2.0 * C)

    # return the eye aspect ratio
    return ear


# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
#ap.add_argument("-p", "--shape_predictor_68_face_landmarks.dat", required=True,
#                help="C:/Users/karlo/Desktop/D3US/Projekt/shape_predictor_68_face_landmarks.dat")
# ap.add_argument("-v", "--video", type=str, default="",
# help="path to input video file")
args = vars(ap.parse_args())

# define two constants, one for the eye aspect ratio to indicate
# blink and then a second constant for the number of consecutive
# frames the eye must be below the threshold
EYE_AR_CONSEC_FRAMES = 72

# initialize the frame counters and the total number of blinks
COUNTER = 0
TOTAL = 0

# initialize dlib's face detector (HOG-based) and then create
# the facial landmark predictor
print("[INFO] Ucitavanje detektora lica")
detector = dlib.get_frontal_face_detector()

print(args)
args["shape_predictor"] = "shape_predictor_68_face_landmarks.dat"
predictor = dlib.shape_predictor(args["shape_predictor"])

# grab the indexes of the facial landmarks for the left and
# right eye, respectively
(lStart, lEnd) = face_utils.FACIAL_LANDMARKS_IDXS["left_eye"]
(rStart, rEnd) = face_utils.FACIAL_LANDMARKS_IDXS["right_eye"]

# start the video stream thread
print("[INFO] Pokretanje video prijenosa")
# vs = FileVideoStream(args["video"]).start()
fileStream = True
vs = VideoStream(src=0).start()
# vs = VideoStream(usePiCamera=True).start()
fileStream = False
time.sleep(1.0)

# loop over frames from the video stream
while True:
    # if this is a file video stream, then we need to check if
    # there any more frames left in the buffer to process
    if fileStream and not vs.more():
        break

    # grab the frame from the threaded video file stream, resize
    # it, and convert it to grayscale
    # channels)
    frame = vs.read()
    frame = imutils.resize(frame, width=520)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # detect faces in the grayscale frame
    rects = detector(gray, 0)

    if not rects:
        brojac += 1
        n = 0
        zbroj = 0
        ar_sr = 0
    else:
        brojac = 0
    if brojac == 72:
        brojac = 0
        ser.write(b'1')

    # loop over the face detections
    for rect in rects:
        # determine the facial landmarks for the face region, then
        # convert the facial landmark (x, y)-coordinates to a NumPy
        # array
        shape = predictor(gray, rect)
        shape = face_utils.shape_to_np(shape)

        # extract the left and right eye coordinates, then use the
        # coordinates to compute the eye aspect ratio for both eyes
        leftEye = shape[lStart:lEnd]
        rightEye = shape[rStart:rEnd]
        leftEAR = eye_aspect_ratio(leftEye)
        rightEAR = eye_aspect_ratio(rightEye)

        # average the eye aspect ratio together for both eyes
        ear = (leftEAR + rightEAR) / 2.0

        # izracun srednje vrijednosti EAR-a
        if n <= 96:
            zbroj += ear
            n += 1
            ar_sr = zbroj/n

        # prilagodba granice zmirenja
        EYE_AR_THRESH = ar_sr - 0.05


        # compute the convex hull for the left and right eye, then
        # visualize each of the eyes
        leftEyeHull = cv2.convexHull(leftEye)
        rightEyeHull = cv2.convexHull(rightEye)
        cv2.drawContours(frame, [leftEyeHull], -1, (0, 255, 0), 1)
        cv2.drawContours(frame, [rightEyeHull], -1, (0, 255, 0), 1)

        # check to see if the eye aspect ratio is below the blink
        # threshold, and if so, increment the blink frame counter
        if ear < EYE_AR_THRESH:
            COUNTER += 1
        else:
            COUNTER = 0

        # otherwise, the eye aspect ratio is not below the blink
        # threshold

        # if the eyes were closed for a sufficient number of
        # then increment the total number of blinks
        if COUNTER >= EYE_AR_CONSEC_FRAMES:
            TOTAL += 1
            COUNTER = 0
            ser.write(b'1')

        # draw the total number of blinks on the frame along with
        # the computed eye aspect ratio for the frame
        cv2.putText(frame, "Srednji EAR: {:.2f}".format(ar_sr), (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 1)
        cv2.putText(frame, "EAR: {:.2f}".format(ear), (300, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 1)

    # show the frame
    cv2.imshow("Frame", frame)
    key = cv2.waitKey(1) & 0xFF

    # if the `q` key was pressed, break from the loop
    if key == ord("q"):
        break

# do a bit of cleanup
cv2.destroyAllWindows()
vs.stop()
exit()

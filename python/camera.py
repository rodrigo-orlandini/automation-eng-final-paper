import os
import cv2
from exceptions import CameraException
from settings import CAM_PORT

class Camera:
    def __init__(self):
        self._camera = False
    
    def start(self):
        cv2.namedWindow("camera")
        self._camera = cv2.VideoCapture(CAM_PORT)

    def capture(self):
        if self._camera.isOpened():
            ret, frame = self._camera.read()
            rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2BGRA)
            cv2.imshow("frame", rgb)

            cv2.imwrite("data/{}.jpg".format(Camera.get_next_picture_id()), frame)
        else:
            print("Waiting camera...")
            raise CameraException("It has not a camera window opened.")

    def stop(self):
        if self._camera.isOpened():
            self._camera.release()
            cv2.destroyWindow("camera")

    @staticmethod
    def get_next_picture_id():
        files = os.listdir("./data")
        if len(files) == 0:
            return 1

        images = []
        for filename in files:
            images.append(int(filename[0:filename.index("."):1]))

        images.sort()
        images.reverse()

        return images[0] + 1
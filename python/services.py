from roboflow import Roboflow
from camera import Camera
from exceptions import RoboflowAPIException
from settings import (
    LATENCY,
    REALITY_PROPORTION,
    MOTOR_VELOCITY,
    IMAGES_PATH, 
    ROBOFLOW_API_KEY, 
    ROBOFLOW_PROJECT_NAME, 
    ROBOFLOW_PROJECT_VERSION,
    ROBOFLOW_CONFIDENCE_LEVEL,
    ROBOFLOW_OVERLAP_LEVEL,
    IMAGE_PIXEL_WIDTH
)

class RoboflowAPI:
    def __init__(self):
        self._rf = Roboflow(api_key=ROBOFLOW_API_KEY)
        self._project = self._rf.workspace().project(ROBOFLOW_PROJECT_NAME)
        self._model = self._project.version(ROBOFLOW_PROJECT_VERSION).model
        self._last_prediction = None

    def _last_model_prediction(self):
        if Camera.get_next_picture_id() - 1 == 0:
            raise RoboflowAPIException("There's no picture to process.")

        return self._model.predict(
            "{}{}.jpg".format(IMAGES_PATH, Camera.get_next_picture_id() - 1), 
            confidence=ROBOFLOW_CONFIDENCE_LEVEL, 
            overlap=ROBOFLOW_OVERLAP_LEVEL
        )

    def get_image_predicted(self):
        self._last_model_prediction().save("./predictions/{}.jpg".format(Camera.get_next_picture_id() - 1))

    def get_json_predicted(self):
        self._last_prediction = self._last_model_prediction().json()["predictions"]
        return self._last_prediction

    def get_motor_activity_time(self):
        if self._last_prediction == None:
            raise RoboflowAPIException("There's no prediction executed yet.")

        if len(self._last_prediction) == 0:
            print("There's none strawberry identified")
            return -1

        ripe = list(filter(lambda strawberry: strawberry["class"] == "ripe strawberry", self._last_prediction))
        if len(ripe) == 0:
            print("There's only immature strawberries here")
            return -1

        selected = ripe[0]
        for strawberry in ripe:
            if strawberry["height"] > selected["height"]:
                selected = strawberry
        
        IMAGE_CENTER = IMAGE_PIXEL_WIDTH / 2
        X_PIXEL_POSITION = selected["x"] - IMAGE_CENTER

        return round((X_PIXEL_POSITION / (MOTOR_VELOCITY * REALITY_PROPORTION)) - LATENCY, 2)
import cv2
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision
import serial
import time
import os
import pyautogui 

try:
    arduino = serial.Serial('COM6', 115200, timeout=0.1)
    print(" تم الاتصال بالأردوينو ")
except:
    arduino = None
    print(" تعمل الآن بدون أردوينو (الكليك فقط)")

# 2. إعداد المحرك
current_folder = os.path.dirname(os.path.abspath(__file__))
model_path = os.path.join(current_folder, 'face_landmarker.task')

base_options = python.BaseOptions(model_asset_path=model_path)
options = vision.FaceLandmarkerOptions(
    base_options=base_options,
    output_face_blendshapes=True,
    num_faces=1
)
detector = vision.FaceLandmarker.create_from_options(options)

cap = cv2.VideoCapture(0)


WINK_LIMIT = 0.02  
BLINK_MAX_DIFF = 0.005 

print("\n النظام جاهز")

while cap.isOpened():
    success, frame = cap.read()
    if not success: break
    
    frame = cv2.flip(frame, 1)
    rgb_frame = mp.Image(image_format=mp.ImageFormat.SRGB, data=cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
    detection_result = detector.detect(rgb_frame)

    if detection_result.face_landmarks:
        mesh = detection_result.face_landmarks[0]
        
        left_eye = mesh[145].y - mesh[159].y
        right_eye = mesh[374].y - mesh[386].y
        diff = abs(left_eye - right_eye)

        if diff > BLINK_MAX_DIFF:
            if left_eye < WINK_LIMIT:
                pyautogui.click(button='left') 
                print("Left Click Executed")
                cv2.putText(frame, "LEFT CLICK", (50, 150), cv2.FONT_HERSHEY_COMPLEX, 1.2, (0, 255, 0), 3)
                time.sleep(0.4)
            
            elif right_eye < WINK_LIMIT:
                pyautogui.click(button='right') 
                print("Right Click Executed")
                cv2.putText(frame, "RIGHT CLICK", (frame.shape[1]-400, 150), cv2.FONT_HERSHEY_COMPLEX, 1.2, (255, 0, 0), 3)
                time.sleep(0.4)

       
        for id in [159, 145, 386, 374]:
            x, y = int(mesh[id].x * frame.shape[1]), int(mesh[id].y * frame.shape[0])
            cv2.circle(frame, (x, y), 2, (0, 255, 255), -1)

    cv2.imshow('Python Direct Click Mode', frame)
    if cv2.waitKey(1) & 0xFF == 27: break

cap.release()
cv2.destroyAllWindows()
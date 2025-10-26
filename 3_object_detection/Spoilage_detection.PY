"""
Performs YOLOv8 -based spoilage detection on captured images and writes results to Firebase.
Also monitors crop tags in Firebase; when a tag is set, 
triggers capture + detection and updates the tag result.
"""

import requests
import subprocess
import cv2
from ultralytics import YOLO
import time

# Firebase Configurations
database_url = 'https://iembotics-default-rtdb.firebaseio.com'
secret_key = 'jLUqSu7FodpwQBXMK7xaF8bcOZvkPKNZ2JigNoWa'

# Firebase tags to monitor (corresponding to crop types)
tags = [
    "/tomato",
    "/cucumber",
    "/eggplant",
    "/grapes",
    "/spinach"
]

# Define class labels from the trained YOLO dataset
class_names = [
    'cucumber_healthy', 'cucumber_rotten',
    'eggplant_healthy', 'eggplant_rotten',
    'grapes_healthy', 'grapes_rotten',
    'spinach_healthy', 'spinach_rotten',
    'tomato_healthy', 'tomato_rotten'
]
# Load the YOLOv8 model (ensure correct model path)
model = YOLO('/home/pi/Desktop/smart_agro/best.pt')

# Image Capture and Detection
def capture_image(filename):
    """Captures an image using the Raspberry Pi camera via libcamera."""
    subprocess.run(['libcamera-still', '-o', filename])


def run_detection():
    """Captures an image, runs YOLO object detection,and returns the predicted class name."""
    image_path = '/home/pi/Desktop/smart_agro/cap.jpg'
    capture_image(image_path)

    # Read captured image
    frame = cv2.imread(image_path)
    if frame is None:
        print("Error: Unable to load image")
        return ""

    # Resize image for YOLO input
    resized_frame = cv2.resize(frame, (640, 640))
    results = model(resized_frame)

    # Parse YOLO output to get class prediction
    result_string = ""
    for result in results:
        for box in result.boxes:
            class_id = int(box.cls[0])
            confidence = float(box.conf[0])
            class_name = class_names[class_id]
            result_string = class_name
            print(f"Predicted: {class_name} (conf: {confidence:.2f})")

    return result_string

# Firebase Communication
def update_firebase(tag, result_string):
    """Updates Firebase with detection result:
    - 1 for healthy, 0 for rotten, Resets tag value to 5 after 5 seconds (ready state)"""
    result_tag = f"{tag}result"

    # Determine numeric result based on detection
    if '_healthy' in result_string:
        result_value = 1
    elif '_rotten' in result_string:
        result_value = 0
    else:
        result_value = ""

    # Push result to Firebase
    url = f"{database_url}{result_tag}.json?auth={secret_key}"
    response = requests.put(url, json=result_value)

    if response.status_code == 200:
        print(f"Updated {result_tag} → {result_value}")
    else:
        print(f"Failed to update {result_tag}: {response.status_code}")

    # Reset result value to 5 after 5 seconds 
    time.sleep(5)
    reset_value = 5
    response = requests.put(url, json=reset_value)
    if response.status_code == 200:
        print(f"Reset {result_tag} → 5 after delay")
    else:
        print(f"Failed to reset {result_tag}: {response.status_code}")

# Monitoring Loop
def check_tags():
    """Reads each tag from Firebase.If tag value == 1 → triggers image capture and detection.
    Updates result in Firebase accordingly."""
    for tag in tags:
        url = f"{database_url}{tag}.json?auth={secret_key}"
        try:
            response = requests.get(url)
            response.raise_for_status()
            value = response.json()

            # Convert string values to integers if needed
            if isinstance(value, str):
                value = int(value)

            if value == 1:
                print(f"{tag} → Triggered (value: 1)")

                # Run detection and interpret results
                result_string = run_detection()
                tag_name = tag.strip('/')
                result_base_name = result_string.split('_')[0]

                # Only update Firebase if tag and detected class align
                if tag_name == result_base_name:
                    update_firebase(tag, result_string)
                else:
                    update_firebase(tag, "")
                    print(f"Tag mismatch → expected {tag_name}, got {result_string}")

            elif value == 0:
                print(f"{tag} → Idle (value: 0)")
            else:
                print(f"{tag} → Unexpected value: {value}")

        except requests.RequestException as e:
            print(f"Network error while reading {tag}: {e}")
        except ValueError as e:
            print(f"Data format error for {tag}: {e}")

if __name__ == "__main__":
    try:
        while True:
            check_tags()
            time.sleep(10)  # Poll Firebase every 10 seconds
    except KeyboardInterrupt:
        print("Program terminated by user.")

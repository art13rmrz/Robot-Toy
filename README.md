
# 🤖 Thumbs-Up Gesture Controlled Robotic Toy

This project uses an **Arduino Mega 2560**, an **ArduCAM Mini 2MP** camera, and **TensorFlow** to create a low-cost robot that recognizes hand gestures (like thumbs up) and moves motors in response. It demonstrates real-time gesture recognition using embedded vision and stepper motor control.

---

## 📁 Repository Structure

| File/Folder | Description |
|-------------|-------------|
| `toyrobotML.ipynb` | TensorFlow script to train a CNN to classify gesture images |
| `Arduino_Capture_and_Predict.ipynb` | Host-side Python script to capture images from ArduCAM, predict gesture, and send command to Arduino |
| `ArduCAM_Capture_and_Move_Motors.ino` | Arduino sketch: captures image, streams to Python, and rotates motors if told |
| `namechange.py` | Utility script to rename and reorganize image folders for gesture classes |
| `dataset/` | Folder containing raw gesture class images (see below for setup) |

---

## 🚀 How to Run the Project

### 1. 📸 Prepare Image Dataset
Place your gesture class images into subfolders under `dataset/`, such as:

```
dataset/
  Thumbs Up/
  Thumbs Down/
  Left Swipe/
  Right Swipe/
  Stop Gesture/
```

Run `namechange.py` to copy and rename all images into labeled folders like `Thumbs_Up/`.

```bash
python namechange.py
```

### 2. 🧠 Train the ML Model
Open `toyrobotML.ipynb` in Jupyter Notebook.

- It loads the renamed image folders, preprocesses the images, builds a CNN model, and trains it.
- Save the trained model (usually as `gesture_model.h5`).

### 3. 🔌 Upload Arduino Sketch
Open `ArduCAM_Capture_and_Move_Motors.ino` in the Arduino IDE.

- Upload it to your Arduino Mega 2560.
- Make sure ArduCAM pins and stepper motor driver pins are connected as described in the script.

### 4. 🧪 Test the Pipeline
Open `Arduino_Capture_and_Predict.ipynb` in Jupyter.

- Sends a “START” command to the Arduino.
- Receives the image over serial.
- Runs the image through the trained CNN model.
- If the gesture is recognized as “Thumbs Up,” sends “ROTATE” back to Arduino.

---

## ⚙️ Hardware Requirements

- Arduino Mega 2560
- ArduCAM Mini 2MP (OV2640)
- 2x 28BYJ-48 stepper motors + ULN2003 driver boards
- HW-131 5V power module (for external motor power)
- USB cable for serial connection to PC

---

## 🗣 Future Enhancements

- Integrate a microphone module to respond to voice commands (e.g., “hello”)
- Package system into an enclosed toy
- Add support for additional gestures and spoken keywords

#include <TinyMLShield.h>

bool captureFlag = false; // Flag to indicate image capture
bool stopFlag = false;    // Flag to indicate when to stop capturing data

// Image buffer;
byte image[176 * 144 * 2]; // QCIF: 176x144 x 2 bytes per pixel (RGB565)
int bytesPerFrame;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  initializeShield();

  // Initialize the OV7675 camera
  if (!Camera.begin(QCIF, RGB565, 1, OV7675)) {
    Serial.println("Failed to initialize camera");
    while (1);
  }
  bytesPerFrame = Camera.width() * Camera.height() * Camera.bytesPerPixel();

  Serial.println("Welcome to the OV7675 test\n");
}

void loop() {
  if (captureFlag && !stopFlag) {
    // Capture the image
    Camera.readFrame(image);

    // Send the raw image data to the Python script
    for (int i = 0; i < bytesPerFrame; i++) {
      // Send the byte value as a decimal number
      Serial.print(image[i]);
      Serial.print(" ");
    }

    // Reset the capture flag
    captureFlag = false;
  }

  // Read incoming commands from serial monitor
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "capture") {
      // Set the capture flag when the "capture" command is received
      captureFlag = true;
      Serial.println("Image capture initiated.");
    } else if (command == "stop") {
      // Set the stop flag when the "stop" command is received
      stopFlag = true;
      Serial.println("Data capture stopped.");
    }
  }
}

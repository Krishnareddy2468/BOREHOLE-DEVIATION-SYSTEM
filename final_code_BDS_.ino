#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <SPI.h>
#include <SD.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 myIMU = Adafruit_BNO055();
const int chipSelect = 10;

bool storeDataFlag = true; // Flag to indicate whether to store data in SD card

void setup() {
  Serial.begin(9600);
  myIMU.begin();
  delay(1000);
  int8_t temp = myIMU.getTemp();
  myIMU.setExtCrystalUse(true);

  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }

  Serial.println("SD card initialized.");

  // Write headers to the CSV file
  File dataFile = SD.open("imu_data.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("acc_x,acc_y,acc_z,gyro_x,gyro_y,gyro_z,mag_x,mag_y,mag_z");
    dataFile.close();
  } else {
    Serial.println("Error opening imu_data.csv for writing headers.");
  }
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 'R' || command == 'r') {
      // Toggle the flag to stop/start storing data in SD card
      storeDataFlag = !storeDataFlag;
      if (storeDataFlag) {
        Serial.println("Resumed data storage.");
      } else {
        Serial.println("Stopped data storage.");
      }
    } else if (command == 'D' || command == 'd') {
      clearDataOnSDCard();
      Serial.println("Cleared data on SD card.");
    } else if (command == 'B' || command == 'b') {
      resetSDCard();
    }
  }

  imu::Vector<3> acc = myIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> gyro = myIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> mag = myIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

   

  if (storeDataFlag) {
    writeIMUDataToFile(acc, gyro, mag);
  } else {
    readDataFromSDCard();
  }

  delay(BNO055_SAMPLERATE_DELAY_MS);
}

void writeIMUDataToFile(imu::Vector<3> acc, imu::Vector<3> gyro, imu::Vector<3> mag) {
  File dataFile = SD.open("imu_data.csv", FILE_WRITE);

  if (dataFile) {
    dataFile.print(acc.x());
    dataFile.print(",");
    dataFile.print(acc.y());
    dataFile.print(",");
    dataFile.print(acc.z());
    dataFile.print(",");

    dataFile.print(gyro.x());
    dataFile.print(",");
    dataFile.print(gyro.y());
    dataFile.print(",");
    dataFile.print(gyro.z());
    dataFile.print(",");

    dataFile.print(mag.x());
    dataFile.print(",");
    dataFile.print(mag.y());
    dataFile.print(",");
    dataFile.println(mag.z());

    dataFile.close();
  } else {
    Serial.println("Error opening imu_data.csv for writing.");
  }
}

void readDataFromSDCard() {
  File dataFile = SD.open("imu_data.csv");

  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
      delay(10);
    }
    dataFile.close();
  } else {
    Serial.println("Error opening imu_data.csv for reading.");
  }

  // Wait for a command before returning
  while (!Serial.available()) {
    // Do nothing, keep waiting for a command
  }
  Serial.read(); // Read and discard the command character
}

void clearDataOnSDCard() {
  if (SD.remove("imu_data.csv")) {
    Serial.println("Cleared data on SD card.");
    delay(5);
  } else {
    Serial.println("Error clearing data on SD card.");
  }

  // Wait for a command before returning
  while (!Serial.available()) {
    // Do nothing, keep waiting for a command
  }
  Serial.read(); // Read and discard the command character
}

void resetSDCard() {
  Serial.println("Resetting the Arduino...");
  asm volatile ("  jmp 0");
 

  // Wait for a command before returning
  while (!Serial.available()) {
    // Do nothing, keep waiting for a command
  }
  Serial.read(); // Read and discard the command character
}

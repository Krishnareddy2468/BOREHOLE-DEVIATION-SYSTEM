// Function declaration
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <math.h>
Angles CalculateAngles();


Adafruit_BNO055 myIMU = Adafruit_BNO055();


void IMUIntilization() {
  //IMU intilization
  myIMU.begin();
  delay(1000);
  int8_t temp1 = myIMU.getTemp();
  myIMU.setExtCrystalUse(true);
  millisOld = millis();
}

Angles CalculateAngles() {
  // Create a Result struct to hold the sum and product
  Angles Angle;
  uint8_t system, gyro, accel, mg = 0;
  myIMU.getCalibration(&system, &gyro, &accel, &mg);
  imu::Vector<3> acc = myIMU.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> gyr = myIMU.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu::Vector<3> mag = myIMU.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);

  thetaM = -atan2(acc.x() / 9.8, acc.z() / 9.8) / 2 / 3.141592654 * 360;
  phiM = -atan2(acc.y() / 9.8, acc.z() / 9.8) / 2 / 3.141592654 * 360;
  phiFnew = .95 * phiFold + .05 * phiM;
  thetaFnew = .95 * thetaFold + .05 * thetaM;

  dt = (millis() - millisOld) / 1000.;
  millisOld = millis();
  theta = (theta + gyr.y() * dt) * .95 + thetaM * .05;
  phi = (phi - gyr.x() * dt) * .95 + phiM * .05;
  thetaG = thetaG + gyr.y() * dt;
  phiG = phiG - gyr.x() * dt;

  phiRad = phi / 360 * (2 * 3.14);
  thetaRad = theta / 360 * (2 * 3.14);

  Xm = mag.x() * cos(thetaRad) - mag.y() * sin(phiRad) * sin(thetaRad) + mag.z() * cos(phiRad) * sin(thetaRad);
  Ym = mag.y() * cos(phiRad) + mag.z() * sin(phiRad);

  psi = atan2(Ym, Xm) / (2 * 3.14) * 360;
  //psi_new=.9*psi_new+.1*psi;

  if (psi < 0) {
    psi = 360 + psi;
  }

  if (psi > 358 || psi < 2)
    psi_new = 0 * psi_new + 1 * psi;
  else
    psi_new = .95 * psi_new + .05 * psi;


  // Calculate the sum and product
  Angle.Pitch = theta;
  Angle.Roll = phi;
  Angle.Yaw = psi_new;
  Serial.print(Angle.Pitch);
  Serial.print("\t");
  Serial.print(Angle.Roll);
  Serial.print("\t");
  Serial.print(Angle.Yaw);
  Serial.print("\t");
  Serial.print((sqrt(pow((theta), 2) + pow((phi), 2))));
  Serial.print("\t");
  // Return the Result struct
  phiFold = phiFnew;
  thetaFold = thetaFnew;


  //delay(5);
  return Angle;
}

void calibrateBDS() {
  uint8_t system, gyro, accel, mg = 0;
  bool stopCalibration = false;  // Flag to indicate if calibration should stop
  while (!stopCalibration) {
    myIMU.getCalibration(&system, &gyro, &accel, &mg);
    // Uncomment the following lines if you want to exit the loop when calibration is complete
    // if (system == 3 && gyro == 3 && accel == 3 && mg == 3)
    //   break; // Exit the loop if calibration is complete
    if (Serial.available() > 0) {
      char cc = Serial.read();
      switch (cc) {
        case Calibration_stop:
          // Handle calibration stop
          Serial.println("Calibration Interrupted");
          stopCalibration = true;
          break;  // Exit the switch statement
        case Calibrate_BDS:
          Serial.print(gyro);
          Serial.print(",");
          Serial.print(mg);
          Serial.print(",");
          Serial.print(system);
          Serial.print(",");
          Serial.println(accel);
          break;
          // default:
          //   // Handle other cases if needed
          //   // This might not always be a return, it depends on your requirement
          //   return;
      }
    }
  }
}

void takeNameFromPy() {
  while (!takenName) {
    if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');

      if (command[0] == Group_Name_Sent) {  //if create file name command is sent
        for (int i = 1; i < command.length(); i++) {
          temp += command[i];
        }
        //Serial.println("group name received and file created!");  //debugging purpose only
        groupName = "/" + temp + ".csv";
        temp = "";
        takenName = true;  //exit the loop
      } else if (command == "esc") {
        //Serial.println("Escaped GroupName");
        break;  //break the loop
      } else {
        if (!SD.begin(2) && !myIMU.begin()) {
        Serial.println("SD card and IMU failed to initialize. Check Connections.");
        
      } else if (!SD.begin(2)) {
        Serial.println("SD card failed to initialize. Reinsert the SD card.");
        
      } else if (!myIMU.begin()) {
        Serial.println("IMU failed to initialize. Check Connections. ");
        
      } else {
        // Both SD card and IMU initialized successfully
        // Proceed with your code here
        Serial.println("SD card and IMU initialized Sucessfully.");
      }
      } 
    }
  }
}



void logData(unsigned long int stopWatchTime, float pitch, float roll, float yaw) {                    //sd card logger
  temp = String((stopWatchTime)) + "," + String(pitch) + "," + String(roll) + "," + String(yaw) + "\n";  //generating some random sensor data
  appendFile(SD, groupName.c_str(), temp.c_str());
  Serial.println(stopWatchTime);
  temp = "";  //for saftey to not lead to some undefined case
}


void stopWatch(int adjustDelay = 1) {  //has an infinte loop running in it
  startStopWatch = millis();

  while (true) {
    Angles Angle = CalculateAngles();
    currentStopWatch = millis();
    stopWatchValue = currentStopWatch - startStopWatch;  //save the StopWatch time
    //Serial.println(stopWatchValue);
    logData(stopWatchValue, Angle.Pitch, Angle.Roll, Angle.Yaw);

    


    //Serial.println(String(stopWatchValue));              //sending stop watch value to the python
    //delay(adjustDelay);  //to get StopWatch sync with python stopWatch
    if (Serial.available() > 0) {
      char cc = Serial.read();
      if (cc == Timer_int){
        Serial.println("Timer Interrupted");
        break;
      }
    }
  }
}

// float BatteryMeasure() {
//   unsigned long startTime = millis(); // Record the start time
//   unsigned int numSamples = 0; // Counter for the number of samples
//   float totalVoltage = 0; // Variable to accumulate the total voltage
  
//   while (millis() - startTime < 2000) { // Continue loop for 10 seconds
//     float voltage = ((float)analogRead(1) * 9 / 4095) / 9 * 100; // Read voltage
//     totalVoltage += voltage; // Accumulate the total voltage
//     numSamples++; // Increment the sample count
//     delay(10); // Optional delay to stabilize readings
//   }
  
//   // Calculate the average voltage
//   float averageVoltage = totalVoltage / numSamples;
  
//   return averageVoltage;
// }

float BatteryMeasure() {
  float voltage = 10+(((float)analogRead(1)*9/4095)/9 *100);
  return voltage;
}

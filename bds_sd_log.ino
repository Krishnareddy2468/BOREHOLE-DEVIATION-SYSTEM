#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "variables.h"
#include "sd_functions.h"
#include "IMUFunctions.h"
#include "functions.h"

void setup() {
  Serial.begin(115200);  //on serail com


  IMUIntilization();  //init bno55 9dof sensor

  beginSDCard();     //load the sd card
  takeNameFromPy();  // get name from python

  if (groupName != "") {                                             //this becomes only true if name is given
    writeFile(SD, groupName.c_str(), "TimeStamp,Pitch,ROll,Yaw\n");  //create groupName.csv file with headings
                                                                     // Call clearFileContents to clear the file contents
    clearFileContents(SD, groupName.c_str());
  } else {
    //Serial.println("file not create, no name was given");
  }

  while (true) {  //infinte loop
    if (Serial.available() > 0) {
      temp = Serial.readStringUntil('\n');  //be careful of using temp as it is global variable
      receiveCommandsFromPy();
      temp = "";  //clear the previous data
    }
  }
}

void receiveCommandsFromPy() {
  switch (temp[0]) {  //switch case only works for numbers not strings
    case Check_sensor_status:
      if (!SD.begin(2) && !myIMU.begin()) {
        //Serial.println("XX: Both SD card and IMU failed to initialize.");
        Serial.write('e');
        Serial.print('\n');
        
      } else if (!SD.begin(2)) {
        //Serial.println("XY: SD card failed to initialize.");
        Serial.write('f');
        Serial.print('\n');
      } else if (!myIMU.begin()) {
        //Serial.println("YX: IMU failed to initialize.");
        Serial.write('g');
        Serial.print('\n');
      } else {
        // Both SD card and IMU initialized successfully
        // Proceed with your code here
        Serial.write('h');
        Serial.print('\n');
      }
      break;
    case Battery_Monitor:
      Serial.println(BatteryMeasure());
      break;
    case Calibrate_BDS:
      calibrateBDS();
      break;
    case START_TIMER_COMMAND:
      if (groupName != "") {
        Serial.write(SUCCESS_RESPONSE);
        Serial.print('\n');
        stopWatch();
      } else {
        Serial.println("counter cannot start as name was not given");
      }
      break;
    case Show_SD_Contents:
      listDir(SD, "/", 0);
      break;
    case Delete_SD_Contents:
      //deleteFile(SD, "/mahesh.csv");
      deleteAllCSVFiles(SD, "/");
      // some code here
      // if (temp.length() < 3 || temp[1] != ' ') {  //this is true if all files are requested to be deleted
      //   Serial.println("deleting sd card contents");
      // } else {                     //this is true if a specified file is requested to be deleted
      //   temp = temp.substring(2);  //store data from 3rd character
      //   deleteFile(SD, temp.c_str());
      // }
      break;
    case ReadData:  // format to send is "r /filename.csv"
      if (temp.length() < 3 || temp[1] != ' ') {
        Serial.println("Invalid format. Please enter the filename in the format: r /filename.csv");
      } else {
        readFile(SD, temp.substring(2).c_str());  // Start from the third character
      }
      break;
    default:
      // Serial.println("unknown command!!");
      break;
  }
}

void loop() {}  //no use of this

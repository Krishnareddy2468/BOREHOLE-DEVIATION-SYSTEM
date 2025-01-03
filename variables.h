
String temp = "";

int CS_pin; // Variable to store CS_pin
// defining some things
#define Battery_Monitor 'b'
#define Calibrate_BDS 'c'
#define START_TIMER_COMMAND 's'
#define SUCCESS_RESPONSE 'y'  //acknoledgement
#define Show_SD_Contents 'x'
#define Delete_SD_Contents 'd'
#define Delete_Response 'p'
#define Delete_NRespone 'q'
#define Group_Name_Sent '_'
#define ReadData 'r'
#define BNO055_SAMPLERATE_DELAY_MS (100)
#define Calibration_stop 'z'
#define Timer_int 'i'
#define Check_sensor_status 'l'
//only these are needed for StopWatch to work
unsigned long int currentStopWatch = 0;
unsigned long int startStopWatch = 0;
unsigned long int stopWatchValue = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
float thetaM;
float phiM;
float thetaFold = 0;
float thetaFnew;
float phiFold = 0;
float phiFnew;
float thetaG = 0;
float phiG = 0;
float theta;
float phi;
float thetaRad;
float phiRad;
float Xm;
float Ym;
float psi;
float psi_new;
float dt;
unsigned long millisOld;

int counter = 0;
bool start = false;
bool takenName = false;
bool waitForFileName = false;
String groupName = "";



// Define a struct to hold two float values
struct Angles {
  float Pitch;
  float Roll;
  float Yaw;
};
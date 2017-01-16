//Xylophone
//Adapted for an ArduinoMega 
//from Jenna deBoisblanc and Spiekenzie Labs initial code

//*******************************************************************************************************************
// User settable variables
//*******************************************************************************************************************

int pinRead;
char pinAssignments[16] ={
  'A11','A10','A2','A3','A4','A5','A6','A7','A8','A9','A1','A0'};
byte PadNote[16] = {
  60};         // MIDI notes from 0 to 127 (Mid C = 60)
int PadCutOff[16] = 
{
  180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180};           // Minimum Analog value to cause a drum hit
int MaxPlayTime[16] = {
  200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200};               // Cycles before a 2nd hit is allowed
#define  midichannel 1;                              // MIDI channel from 0 to 15 (+1 in "real world")
boolean VelocityFlag  = false;                           // Velocity ON (true) or OFF (false)

//*******************************************************************************************************************
// Internal Use Variables
//*******************************************************************************************************************
boolean activePad[16] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                   // Array of flags of pad currently playing
int PinPlayTime[16] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};                     // Counter since pad started to play
byte status1;

int pin = 0;     
int hitavg = 0;
//*******************************************************************************************************************
// Setup
//*******************************************************************************************************************
void setup() 
{
  Serial.begin(115200);                                  // SET HAIRLESS TO THE SAME BAUD RATE IN THE SETTINGS

}
//*******************************************************************************************************************
// Main Program
//*******************************************************************************************************************
void loop() 
{
  for(int pin=0; pin < 16; pin++)                          //
  {
    //int pin = 3;
    //   for (pinRead=0; pinRead < 16, pin++){
    hitavg = analogRead(pinAssignments[pin]);  
    //Serial.println(hitavg);   
    // read the input pin

    if((hitavg > PadCutOff[pin]))
    {
      if((activePad[pin] == false))
      {
        if(VelocityFlag == true)
        {
          //          hitavg = 127 / ((1023 - PadCutOff[pin]) / (hitavg - PadCutOff[pin]));    // With full range (Too sensitive ?)
          hitavg = (hitavg / 8) -1 ;                                                 // Upper range
        }
        else
        {
          hitavg = 127;
        }
        MIDI_TX(144,PadNote[pin],hitavg); //note on

        PinPlayTime[pin] = 0;
        activePad[pin] = true;
      }
      else
      {
        PinPlayTime[pin] = PinPlayTime[pin] + 1;
      }
    }
    else if((activePad[pin] == true))
    {
      PinPlayTime[pin] = PinPlayTime[pin] + 1;
      if(PinPlayTime[pin] > MaxPlayTime[pin])
      {
        activePad[pin] = false;
        MIDI_TX(144,PadNote[pin],0); 
      }
    }
  } 
}

//*******************************************************************************************************************
// Transmit MIDI Message
//*******************************************************************************************************************
void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY) 
{
  status1 = MESSAGE + midichannel;
  Serial.write(status1);
  Serial.write(PITCH);
  Serial.write(VELOCITY);

}

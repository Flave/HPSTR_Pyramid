#include <Wire.h>
#include "Nunchuck.h"
#include "Controller.h"
#include "FSR.h"
#include "TiltControl.h"
#include "TouchControl.h"

const unsigned int BAUD_RATE = 19200;

//tilt stuff
Controller controller;
Nunchuck nunchuck;
TiltControl tiltControl1, tiltControl2, tiltControl3;
int side, previousSide, peak, sideChanged;


//Touch stuff
FSR fsr1(A0, 60);
FSR fsr2(A1, 75);
FSR fsr3(A2, 28);
FSR fsr4(A3, 125);

TouchControl touchControl1, touchControl2, touchControl3, touchControl4;

//define the notes for every fsr for each side
int notesSide1[] = {45, 46, 47};
int notesSide2[] = {78, 81, 85};
int notesSide3[] = {78, 81, 85};
int notesSide4[] = {78, 81, 85};

//Toggle Variable to mute/ unmute Tracks/Instruments
boolean soundOn[4];

//////////// SETUP //////////// 

void setup() {
  Serial.begin(BAUD_RATE);
  nunchuck.initialize();
  pinMode(6, INPUT_PULLUP);
  pinMode(11, OUTPUT); 
  digitalWrite(11, HIGH); 
}


//////////// LOOP //////////// 

void loop() {
  
  //Serial.println(String(fsr1.inputValue())+"  "+String(fsr2.inputValue())+"  "+String(fsr3.inputValue())+"  "+String(fsr4.inputValue()));
  
  //if nunchuck data is available start doing dat shit
  if (nunchuck.update()) {
    
    //update controller and get side/peak for convenience reasons
    controller.update(nunchuck.xAmplitude(), nunchuck.yAmplitude(), nunchuck.zAmplitude());    
    side = controller.getSide();
    previousSide = controller.previousSide;
    sideChanged = controller.sideChanged();
    peak = controller.getPeak();
    
        
    //////////// LOOPER CONTROL //////////// 
    
    //Check each edge if mode was changed and send controlChange to switch between record and play
    if(controller.changeModeX() || controller.changeModeY() || controller.changeModeZ()) {
        usbMIDI.sendControlChange(21, 127, side);
        usbMIDI.sendControlChange(21, 0, side);
    }
    
    
    //////////// MAIN SWITCH ////////////
    
    switch (side) {
      case 0:
        //if side == 0 means pyramid is on peak. If peak changed send midiControl to mute current side
        //soundOn[] acts as a toggle for muteing/un-mute each side separately
        if(controller.peakChanged() && peak) {
          
          //if controller is on peak and soundOn is true...
          if(soundOn[peak-1]) {
            //send midi Control Change to unmute Track
            usbMIDI.sendControlChange(22, 127, peak);
            soundOn[peak-1] = false;
          }
          //else, if soundOn[] for current peak is false...
          else if(!soundOn[peak-1]) {
            //send midiControll Change to mute Track
            usbMIDI.sendControlChange(22, 0, peak);
            soundOn[peak-1] = true;
          } 
        }
      
      case 1:
        //if just turned on this side send controlChange to select track in ableton
        if(sideChanged) {
          usbMIDI.sendControlChange(23, 127, side);
          Serial.println(sideChanged);         
        }
        
        //update touch sensors with notes specified at the top
        touchControl1.update(61, fsr2, side); //45
        touchControl2.update(57, fsr3, side); 
        touchControl3.update(58, fsr4, side); //right

        break;
        
      case 2:
        if(sideChanged) {
          usbMIDI.sendControlChange(23, 127, side);    
          Serial.println(sideChanged);
        }

        touchControl1.update(76, fsr1, side);
        touchControl2.update(79, fsr3, side);
        touchControl3.update(83, fsr4, side);             
        
        break;
        
      case 3:
        if(sideChanged) {
          usbMIDI.sendControlChange(23, 127, side);  
          Serial.println(sideChanged);                    
        }             
        
        touchControl1.update(49, fsr1, side);
        touchControl2.update(42, fsr2, side);
        touchControl3.update(36, fsr4, side);             
        
        break;
        
      case 4:
        if(sideChanged) {
          usbMIDI.sendControlChange(23, 127, side);
          Serial.println(sideChanged);          
        }

        touchControl1.update(87, fsr1, side);
        touchControl2.update(89, fsr2, side);
        touchControl3.update(90, fsr3, side);             
        
        break;
     }
     
  ///TILT CONTROL CHANGES///
  tiltControl1.update(24, controller.xSlowChanged(0.02), side);
  tiltControl2.update(25, controller.ySlowChanged(0.02), side);
  tiltControl3.update(26, controller.zSlowChanged(0.02), side);

  
  ///TILT CONTROLL RESETS///
  if(sideChanged) {
    tiltControl1.update(24, 0, previousSide);
    tiltControl2.update(25, 0, previousSide);
    tiltControl3.update(26, 0, previousSide);
  }
  
      
  delay(3);
     
  }
  usbMIDI.read();
}

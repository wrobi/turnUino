# turnUino
**Model railway turnout controller with ESP8266 and PCA9685 for Arduino**

Control your model railway turnouts easily using laptop or tablet. Just click on the selected element to move the turnouts to the desired position. You can make your own map and turnout images with Scarm, WinTrack or any other model railway planner software. 

Compatible with ESP8266 based boards, like NodeMCU, Wemos D1, Wemos D1 mini etc.

Screenshot:
![wiring](https://github.com/wrobi/turnUino/blob/main/screenshot.png?raw=true "Screenshot")
*This background image maded by Scarm.*

## Installation
Prerequisites:
  - NodeMCU or Wemos D1 etc. board.
  - PCA9685 board(s)
  - SG-90 servos
  - cables
  - 5v power supply (360mA / servo)
  - Arduino IDE

Preparations:
  - Make your own map file named: background.png. 
  - Make two turnouts image (turn0.png and turn1.png). 
  - Copy images to /data folder

1. Download & unzip repo
2. Install ESP8266 data uploader [Instructions](https://www.nonscio.com/blog/installing-the-esp8266-file-system-uploader-for-arduino-ide).
3. Open turnUino.uno on Arduino IDE
4. Install included libraries.
5. Add your SSID & PASSWORD.
6. Upload code.
7. Upload datas with ESP8266 Sketch Data Upload.
8. Open serial monitor (at 115200) and press reset button on board. It will show your main IP address.


## Using:
  -Open your browser, and go to main IP address (eg: 192.168.0.8) 
  -Simply press an image of turnout, it will changes the state of specified servo.
  
  For settings press gear icon on top left corner. You can select the id of turnout and change properties.
   - Xpos   : Horizontal position of turnout image in pixels
   - Ypos   : Vertical position of turnout image in pixels
   - Rotate : Angle of image in degree (0 - 359)
   - Mirror : Left or right type turnout
   - Angle0 : Initial servo position for streight state of turnout in degree (0 - 180)
   - Angle1 : Moved servo position for curved state of turnout in degree (0 - 180)
   - Enabled: Set visible or hidden this turnout.

  When you are done editing **all** of the items you want, press Save button. Plase note that for saving data ESP using flash ROM, which has writing lifetime limit. Unsaved modifications remains until reseting the board, so you can test all settings without saving.
  For hiding settings panel, press gear icon again. 

## Wiring:
![wiring](https://github.com/wrobi/turnUino/blob/main/wiring.png?raw=true)

## Donate
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=ZCU25GHQ2Z4SC)

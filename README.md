# turnUino
**Model railway turnout controller with ESP8266 and PCA9685 for Arduino**

Control your model railway turnouts easily using laptop or tablet. In map view, just click on the selected element to move the turnouts to the desired position. You can make your own map and turnout imgages with Scarm, WinTrack or any other model railway planner software. Use **background.png** filename for map, **turn0.png** for default positioned turnout, and **turn1.png** for other positioned turnout. Copy map and turnout images to ***data*** folder and upload to ESP flash rom with Arduino ESP8266 filesystem uploader.

Compatible with ESP8266 based boards, like NodeMCU, Wemos D1, Wemos D1 mini etc.

Screenshot:
![wiring](https://github.com/wrobi/turnUino/blob/main/screenshot.png?raw=true "Screenshot")

Background image maded by Scarm.

## Using:
  If you press an image of turnout, it will changes the state of specified servo.
  
  For settings press gear icon on top left corner. You can select the id of turnout and change properties.
   - Xpos   : Horizontal position of turnout image in pixels
   - Ypos   : Vertical position of turnout image in pixels
   - Rotate : Angle of image in degree (0 - 359)
   - Mirror : Left or right type turnout
   - Angle0 : Initial servo position for streight state of turnout in degree (0 - 180)
   - Angle1 : Moved servo position for curved state of turnout in degree (0 - 180)

  When you are done editing all the items you want, press Save button. Plase note that for saving data ESP using flash ROM, which has writing lifetime limit.
  For hiding settings panel, press gear icon again.

## Wiring:
![wiring](https://github.com/wrobi/turnUino/blob/main/wiring.png?raw=true)

## Donate
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=ZCU25GHQ2Z4SC)

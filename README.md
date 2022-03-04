# turnUino
Modell railway turnout controller with ESP8266 and PCA9685 for Arduino

Screenshot:
![wiring](https://github.com/wrobi/turnUino/blob/main/screenshot.png?raw=true "Screenshot")

Background image maded by Scarm.

Using: 
  For settings press gear icon on top left corner. You can select the id of turnout and change properties.
   - Xpos   : Horizontal position of turnout image in pixels
   - Ypos   : Vertical position of turnout image in pixels
   - Rotate : Angle of image in degree (0 - 359)
   - Mirror : Left or right type turnout
   - Angle0 : Initial servo position for streight state of turnout in degree (0 - 180)
   - Angle1 : Moved servo position for curved state of turnout in degree (0 - 180)

  When you are done editing all the items you want, press Save button. Plase note that for saving data ESP using flash ROM, which has writing lifetime limit.
  For hiding settings panel, press gear icon again.

wiring:
![wiring](https://github.com/wrobi/turnUino/blob/main/wiring.png?raw=true)

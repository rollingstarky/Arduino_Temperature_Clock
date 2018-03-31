# Arduino_Temperature_Clock
An Arduino Project with a LCD display to show date(time) and temperature(humidity）

### Figures
##### The sketch(made with [Fritzing](http://fritzing.org/home/)):
![sketch made with Fritzing](https://github.com/rollingstarky/Arduino_Temperature_Clock/blob/master/images/DS1302.png)
##### The photo:
![photo](https://github.com/rollingstarky/Arduino_Temperature_Clock/blob/master/images/DS1302.jpg)

### Supported commands (through the Serial Monitor)
* Use ``set YY MM DD WEEK HH MM`` To Set The Time
* Such As Type ``set 08 08 08 1 12 00`` To Set The Time As 2008-8-8 Monday 12:00
* Use ``show`` To Display The Time and Temperature(T)/Humidity(H) Now
* **Use ``always`` To Update The Information Every Second (recommended)**
* Use ``alarm`` To Check The Current Alarm

### Attention
* Use the potentiometer to adjust the display (contrast ratio) of LCD
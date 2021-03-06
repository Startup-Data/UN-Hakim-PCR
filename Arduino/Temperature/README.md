# This page is for Cooler and Heater part


1- **First** tried with [this](https://www.instructables.com/DIY-Cooler/) page ([codes are available here](https://github.com/Startup-Data/UN-Hakim-PCR/blob/main/Arduino/Temperature/PCR-cooling-heating/PCR-cooling-heating.ino)):

![image](https://user-images.githubusercontent.com/6679151/119461199-b6ff0180-bd54-11eb-9cdc-41d37eca5c7a.png)

2- Trying to **test the Temperature control** with **Max6675 Thermocouple**, the **Arduino** codes could be seen [**here**](https://github.com/Startup-Data/UN-Hakim-PCR/blob/main/Arduino/Temperature/PCR-cooling-heating-Thermocouple-Max6675_better_LCD_I2C/PCR-cooling-heating-Thermocouple-Max6675_better_LCD_I2C.ino):

![enter image description here](https://i.stack.imgur.com/hlGOf.jpg)

The **Circuit's schematic** is shown below:
![enter image description here](https://i.stack.imgur.com/zmYIz.jpg)

[The below video](https://vimeo.com/555476649) contain **some Description** about **this** part of **the project**:
![image](https://user-images.githubusercontent.com/6679151/119751595-b11e3300-beb0-11eb-921c-35b4fa755a0e.png)

3- With LCD (Codes are [Here](https://github.com/Startup-Data/UN-Hakim-PCR/tree/main/Arduino/Temperature/PCR-cooling-heating-Thermocouple-Max6675_better_LCD_I2C)):

![image](https://user-images.githubusercontent.com/6679151/119862744-106e5880-bf2e-11eb-83e6-cc582c81ec75.png)
3-1 **PID COntrol with Thermocouple Sensor and LCD** ([Codes Link](https://github.com/Startup-Data/UN-Hakim-PCR/tree/main/Arduino/Temperature/PCR-cooling-heating-Thermocouple-Max6675_better_LCD_I2C_PID))

4- With 0.91 Inch OLED (Codes are [Here](https://github.com/Startup-Data/UN-Hakim-PCR/blob/main/Arduino/Temperature/PCR-cooling-heating-Thermocouple-Max6675_better_LCD_I2C/PCR-cooling-heating-Thermocouple-Max6675_better_LCD_I2C.ino) which have **too long Delay**!!!):
![image](https://user-images.githubusercontent.com/6679151/119862935-47dd0500-bf2e-11eb-9918-76a7a136ad35.png)

5- LCD & K-type Thermocouple & Pt100 Temperature Sensor ([link](https://github.com/Startup-Data/UN-Hakim-PCR/tree/main/Arduino/Temperature/PCR-cooling-heating-Thermocouple-Max6675_better_LCD_I2C-pt100)):

-PT100 Cods made from [this question](https://stackoverflow.com/questions/30012866/how-to-read-temperature-using-arduino-uno-board-with-pt100-rtd-sensor) and by The blow Circuit ([Search link](https://www.google.com/search?sxsrf=ALeKk00wFLU-x7WOkhKn6DA9n-Dl3tIgUg:1622751979952&source=univ&tbm=isch&q=pt100+arduino+analog+2+wire&sa=X&ved=2ahUKEwi-2cqapvzwAhWZhv0HHadQDfoQjJkEegQIAhAB#imgrc=VT4HHW5GNIEJ5M))


![image](https://user-images.githubusercontent.com/6679151/120777586-af044600-c53a-11eb-96d1-f3ac89e97451.png)


6- LCD & LM35 Temperature Sensor as you can see below (Some problem and Burning the 2 Arduino Uno Cost around 10$ : [Question link][1]):

![image](https://user-images.githubusercontent.com/6679151/121127829-9ef0ad00-c83f-11eb-9aae-72bcbf6864a5.png)

7-  **LCD**,  **DS18B20** Temperature Sensor as you can see below (Codes are [here](https://github.com/Startup-Data/UN-Hakim-PCR/tree/main/Arduino/Temperature/PCR-cooling-heating-Thermocouple-Max6675_better_LCD_I2C_PID)):

![image](https://user-images.githubusercontent.com/6679151/122235371-e37ce800-ced2-11eb-976d-63701d026a2b.png)

8- **LCD**, **DS18B20** and **Thermocouple** Temperature Sensor as you can see below (Codes are [here](https://github.com/Startup-Data/UN-Hakim-PCR/tree/main/Arduino/Temperature/PCR-cooling-heating-ds18b20-and-Termocouple--better_LCD_I2C)):

![image](https://user-images.githubusercontent.com/6679151/122235903-4cfcf680-ced3-11eb-9e3b-e86900b43c9a.png)

9- The PCB made by EASYEDA available in [here](https://github.com/Startup-Data/UN-Hakim-PCR/blob/main/Arduino/Temperature/Project_KY-019%205V%20relay%20module_2021-10-26_02-08-09.zip) and must imported in [easyeda site](https://easyeda.com/editor#id=3b3edc0bf6f043f8a504502f7aa43c6a|a16932e52ee44f079ceae1651a98b732|b4ede8536cb24395b6bbd5a5fe69a2b5):

![enter image description here](https://i.stack.imgur.com/Vz6n0.png)


## To Do:

 1. Built ***the 3D Printer** Container for
    **TEC** ( Heater and Cooling Parts)
 2. **Calibrate** the temperature Sensor:

    Done and could be at [this codes][2], which could be seen at the below 
    part of main code:

![image](https://user-images.githubusercontent.com/6679151/122644954-00acf300-d12d-11eb-820f-957006704ee4.png)


**Thanks.**


  [1]: https://arduino.stackexchange.com/questions/84593/about-corrupted-lm35-ic-and-arduino-port-problem
  [2]: https://github.com/Startup-Data/UN-Hakim-PCR/tree/main/Arduino/Temperature/PCR-cooling-heating-ds18b20-and-Termocouple--better_LCD_I2C

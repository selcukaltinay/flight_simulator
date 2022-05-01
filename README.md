
[***Flight Simulator***]{.underline}

With this application, you can simulate the data from your boarding pass with labview. I did not upload the full version of the application to github because I was doing academic work. If you need the full version, you can contact me.

![flight_simulator](https://media-exp1.licdn.com/dms/image/C4D22AQE_D7iQpadXzA/feedshare-shrink_800/0/1609002248979?e=2147483647&v=beta&t=fMlwsRRtPeUz-AT5MIwSD_C2nM01SMkLfkeDKQPg7t0)

**On the embedded side of the application:**

As a middleware:

-   FreeRTOS

As a sensor:

1.  *BMP180 pressure sensor*

2.  *MPU6050 gyro and acceleration*

3.  *NEO 6m gps*

4.  HMC5983l

5.  *ADC for BAT*

6.  *Joystick*

7.  *Button*

As a protocol:

1.  UART

2.  I2C

3.  SPI

As a data standart

-   XML

---

My LinkedIn Post:

> During my quarantine period in Germany, I had completed a few rtos courses, but I could not find the opportunity to use the knowledge I gained in any big or small project. I did a small study to evaluate the curfew in my country on weekends. Here I tried to make an interface with sensors like mpu6050, bmp180, neo 6m, hmc5983l and so on using FreeRTOS and STM32F429 MCU. I have observed the effective use of concepts such as semaphore, mutex, queue etc. I transferred data to the computer over UART and displayed the data in the interface. I created this interface with LabVIEW. I also used some API's to obtain some data. The next step will be to transmit data by rf. Then I will try to add some features by integrating it into the system I prepared earlier. Sorry for adding noise to some places to hide my location.

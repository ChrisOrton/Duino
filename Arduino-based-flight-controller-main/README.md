# Arduino-based-Flight-Contoller

![alt text](https://github.com/akarsh98/Arduino-based-flight-controller/blob/main/Flight%20Controller/FC%20architecture.JPG)

In this project, we are going to make an Arduino base Flight Controller for our Arduino based Drone. As the name suggests, a FLight Controller is that part of the drone that controls the flight of the drone. It consists of an IMU(Inertial Motion Unit) which continuously monitors the position and orientation of the drone and according to that it controls the speed of motors to provide stability to the drone. It is also connected to the receiver of the Drone and directly obtains signals from the signals sent by the transmitter. According to these received signals, it modifies the speed of the motors and ultimately gives the drone the direction that we want it to fly in. All the accessories that are attached to the drone are connected to Flight Controller itself.  


![alt text](https://github.com/akarsh98/Arduino-based-flight-controller/blob/main/Flight%20Controller/Schematic.JPG)

The apparatus that we are going to use for making our flight controller are: MPU6050 3 axis accelerometer and Gyroscope Sensor and Arduino Nano. That's it on the Hardware side because most of the functioning is done inside the code.The code is provided to us by Multiwii. Multiwii basically is an open source project that is used to make and test flight controllers for various different types of multicopters and other things as well.


![alt text](https://github.com/akarsh98/Arduino-based-flight-controller/blob/main/Flight%20Controller/multiwii.JPG)

With the Multiwii code, we also have the Multiwii GUI application which is based on Java. In this application we can connect our flight controller, Calibrate it, Tune various values required for functioning of the Flight Controller and at the end we can also visualize the working of our flight controller with the help of various graphs and a drone like structure that appears to move if the Flight Controller is working correctly.

![alt text](https://github.com/akarsh98/Controlling-ESP8266-with-Alexa/blob/master/images/pcbway.JPG?raw=true)

You must check out [PCBWAY](https://www.pcbway.com/) for ordering PCBs online for cheap!

You get 10 good-quality PCBs manufactured and shipped to your doorstep for cheap. You will also get a discount on shipping on your first order. Upload your Gerber files onto [PCBWAY](https://www.pcbway.com/) to get them manufactured with good quality and quick turnaround time. Check out their online [Gerber viewer](https://www.pcbway.com/project/OnlineGerberViewer.html) function. PCBWAY takes care of its customers a lot that's why they offer reward points with every purchase that you make from them these reward points can be redeemed for various useful items from their [gift shop](https://www.pcbway.com/project/gifts.html).
{"mode":"full","isActive":false}

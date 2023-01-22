# LoRa_cat_tracker :smiley_cat:
This cat tracker sends GPS coordinates using LoRa

A Raspberry Pi receives the data and sets a marker at your cat's position onto a map

There is also a portable receiver in case the Raspberry Pi doesn’t receive the data, or you have to search for your cat/collar

<img width="400" alt="leaflet" src="https://user-images.githubusercontent.com/122114833/213924444-d1cb3e24-cf54-416b-8836-d70306a45737.jpg"> <img width="385" alt="leaflet" src="https://user-images.githubusercontent.com/122114833/213924450-9b460592-2f99-4f76-b438-84920b67936a.jpg">

©Gregor Wiebe | [Huckleberryking](https://www.huckleberryking.com/)

<img width="790" alt="leaflet" src="https://user-images.githubusercontent.com/122114833/213924388-980c956e-1a5f-451e-aa88-0f8108bc1841.png">


[map](https://htmlpreview.github.io/?https://github.com/mattyspawprints/LoRa_cat_tracker/blob/main/Leaflet/final.html)

## Parts List
Before you buy one of the LoRa boards, check if the board has the correct frequency
- Europe EU868 MHz 
- South America AU915/AS923-1 MHz 
- North America US915 MHz 
- India IN865 MHz 
- Asia AS923 MHz

| Sender      | portable receiver          | receiver | other parts |
| ------------- |-------------| -----| -----|
| [1x CubeCell GPS-6502](https://www.amazon.de/Seamuing-LoRaWAN-Development-ASR6502-intelligente/dp/B08B1G5THB/ref=sr_1_1_sspa?keywords=seamuing%20lora%20gps%20modul%20sx1262&qid=1673031653&sprefix=seamuing%20,aps,110&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1)     | [1x Heltec WiFi LoRa 32 V2](https://www.amazon.de/Display-Entwicklungsboard-Bluetooth-868MHz-915MHz-Flash-IOT/dp/B0B6G8ZVNT/ref=sr_1_7_sspa?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=3JMCQXZV0BU2V&keywords=heltec%2blora%2b32%2bv2&qid=1673033106&sprefix=heltec%2blora%2b32%2bv2,aps,91&sr=8-7-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9tdGY&th=1) | [1x Raspberry Pi 3](https://www.amazon.de/Raspberry-Pi-Model-ARM-Cortex-A53-Bluetooth/dp/B01CD5VC92/ref=sr_1_2?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=2T141BPKYTVFA&keywords=raspberry%20pi%203&qid=1673298648&sprefix=raspberry%20pi%203,aps,121&sr=8-2) |[2x 1k resistor](https://www.amazon.de/Widerstand-St%C3%BCck-Kohleschicht-Widerst%C3%A4nde-Resistor/dp/B00IYT9MLC/ref=sr_1_8?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=JQM0A6V8XZAL&keywords=1k%20widerst%C3%A4nde&qid=1673299281&sprefix=1k%20widerst%C3%A4nde,aps,87&sr=8-8)|
| [1x Lithium Polymer Battery 3,7V 480mAh](https://www.amazon.de/EEMB-Lithium-Polymer-Ionen-Akku-wiederaufladbarer-Lithium-Ionen-Polymer-Akku-JST-Anschluss/dp/B09DPPXRFY/ref=sr_1_1_sspa?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=3F814O2OD9U27&keywords=lipo%203,7%20480mah&qid=1673031959&sprefix=lipo%203%207%20480mah,aps,85&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1)     | [1x Lithium Polymer Battery 3,7V 1100mAh](https://www.amazon.de/1100mAh-Schutzplatine-Isolierband-1-25-Stecker-ESP32-Entwicklungsplatine/dp/B087LTZW61/ref=sr_1_6?keywords=lipo%201100mah&qid=1673034018&sprefix=lipo%20110,aps,108&sr=8-6) | [1x LoRa breakout board](https://www.amazon.de/gp/product/B084NWDFH1/ref=ppx_yo_dt_b_asin_title_o01_s01?ie=UTF8&psc=1) |[1x 2k resistor](https://www.amazon.de/Widerstand-St%C3%BCck-Kohleschicht-Widerst%C3%A4nde-Resistor/dp/B00IYTI6LE/ref=sr_1_6?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=2BJD7I68YPH69&keywords=2k%20widerst%C3%A4nde&qid=1673299358&sprefix=2k%20widerst%C3%A4nde,aps,85&sr=8-6)|
| [1x active buzzer](https://www.amazon.de/QWORK-Elektronischer-Summer-Active-Kabell%C3%A4nge/dp/B09RFH1T8J/ref=sr_1_6?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&keywords=active%2bbuzzer&qid=1673032533&sr=8-6&th=1)     | [(1x tactile button)](https://www.amazon.de/Youmile-100er-Pack-Miniatur-Mikro-Taster-Tastschalter-Qualit%C3%A4tsschalter-Miniature-6-x-5-mm/dp/B07Q1BXV7T/ref=sxin_16_ac_d_br?ac_md=1-0-WW91bWlsZQ==-ac_d_br_brs_brs&content-id=amzn1.sym.14c3c61f-de95-4d49-b900-67b03889c716:amzn1.sym.14c3c61f-de95-4d49-b900-67b03889c716&cv_ct_cx=tactile%2bswitch&keywords=tactile%2bswitch&pd_rd_i=B07Q1BXV7T&pd_rd_r=5bc5f8b7-a7d6-451b-ad0f-001154cc3578&pd_rd_w=fgnAO&pd_rd_wg=RAPPv&pf_rd_p=14c3c61f-de95-4d49-b900-67b03889c716&pf_rd_r=WSA4WB4VY1KT5RCG8C27&qid=1673032795&sprefix=tactile,aps,124&sr=1-1-26e67082-4f95-42ef-935f-dfc82714a173&th=1) |  |[2x slide switch](https://www.amazon.de/dp/B07VFW4SWS?psc=1&ref=ppx_yo2ov_dt_b_product_details)|
| [1x BLE hm-10 module](https://www.amazon.de/DSD-TECH-Bluetooth-iBeacon-Arduino/dp/B06WGZB2N4/ref=sr_1_1_sspa?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=ALIPIZJPPOMN&keywords=hm%2010%20ble&qid=1673032638&sprefix=hm%2010%20ble,aps,94&sr=8-1-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1)    | [4x Neopixels (1x strip)](https://www.amazon.de/BTF-LIGHTING-adressierbar-Vollfarbiger-wasserdichte-DIY-Projekte/dp/B088FK4SMQ/ref=sr_1_15?__mk_de_DE=%C3%85M%C3%85%C5%BD%C3%95%C3%91&crid=2VIDFA8IPR6WO&keywords=neopixel%2bled%2bstrip%2b20%2bcm&qid=1673298279&sprefix=neopixel%2bled%2bstrip%2b20cm,aps,85&sr=8-15&th=1) |  |[1x ttl converter](https://www.amazon.de/dp/B089QJZ51Z?ref=ppx_yo2ov_dt_b_product_details&th=1)|
|      |      |  |[some jumper wires](https://www.amazon.de/AZDelivery-Jumper-Arduino-Raspberry-Breadboard/dp/B074P726ZR/ref=pd_day0fbt_sccl_1/262-3634548-6437412?pd_rd_w=d4unF&content-id=amzn1.sym.afd474ca-fc60-4653-b4ab-afb840ac5c22&pf_rd_p=afd474ca-fc60-4653-b4ab-afb840ac5c22&pf_rd_r=VBNZ04HRFZF5YE54HP9B&pd_rd_wg=rIgp7&pd_rd_r=2d547436-2e8a-4e9c-bac0-d998afaa835d&pd_rd_i=B074P726ZR&th=1)|
|  |     |     | [1x breadboard](https://www.amazon.de/AZDelivery-Breadboard-Steckbrett-Kontakten-Arduino/dp/B07K8PQ4B5/ref=sr_1_3?keywords=breadboard&qid=1673299816&sprefix=bread,aps,97&sr=8-3&th=1)|

## Case
#### 3D printed
You can print a case for the portable receiver and the sender

<img src="https://user-images.githubusercontent.com/122114833/213924639-75062e52-174e-49db-bfdb-e67605588abb.jpg" width=400> <img src="https://user-images.githubusercontent.com/122114833/213924641-f0b1800a-88e5-4c56-bb19-f1f5d48ca650.jpg" width=400> 
<img src="https://user-images.githubusercontent.com/122114833/213924645-b1bc2972-2f99-4a20-8fad-8d2c221f1f4f.JPG" width=400> <img src="https://user-images.githubusercontent.com/122114833/213925179-1083a861-bc61-4a9b-9aee-cb3dd2c63adb.jpg" width=400>                                              
                                              ©Gregor Wiebe | [Huckleberryking](https://www.huckleberryking.com/)


  but the case for the sender is a little bit too big for a cat, in my opinion
 
  
#### alternative
If you cannot print a 3D case, you also can use heat shrink tubes for the sender and a plastic box for the receiver.
To fit everything in the tube you should use one with at least an inner diameter of 30mm.
For the receiver, you can use, for example, the box that the Heltec board came with. Make holes for the antenna, power switch, display and USB port. In this case, you can use the programable button on the board because the plastic is very thin, and you can push the button through the plastic, so choose the Portable_Receiver_onboardButton sketch and upload it to the white heltec board.

<img src="https://user-images.githubusercontent.com/122114833/213926143-8352107a-4332-4e40-ab1d-830d2ef12afe.jpg" width=400 > <img src="https://user-images.githubusercontent.com/122114833/211643647-39168c9f-5610-4750-bae4-e682f1ebc5c8.JPG" width=400 >

<img src="https://user-images.githubusercontent.com/122114833/213926154-f3d50c89-03f7-4784-98d1-0465fb683cab.jpg" width=400 > <img src="https://user-images.githubusercontent.com/122114833/213926155-75a99bc5-c566-40a3-adf6-37e092536498.jpg" width=390 >

<img src="https://user-images.githubusercontent.com/122114833/213926941-7522b74d-2c05-48f8-b4e4-bb9999c74680.JPG" width=290 > <img src="https://user-images.githubusercontent.com/122114833/213927077-421d0621-b4ad-421a-bd1d-f434e0aec817.jpg" width=500 >

©Gregor Wiebe | [Huckleberryking](https://www.huckleberryking.com/)


 ##  Soldering
 
**Sender**

<img src="https://user-images.githubusercontent.com/122114833/213926430-e52041b5-5a8c-44c1-a639-944f0dbd03e8.JPG" width=700 >

To make the sender thinner, you can try to remove the plastic socket
Or try to desolder the oled display

  <img src="https://user-images.githubusercontent.com/122114833/213926487-fc3031ea-be85-4f41-a309-ebdc92bac8eb.png" width=400 >

BLE module: solder wires to gnd and vcc (before you desolder the pin header, you have to configure your BL module, which will be explained later
Also, the red wire of the buzzer to pin 4 and the black one to gnd

<img src="https://user-images.githubusercontent.com/122114833/213926536-51f8544d-010f-4e1e-84fd-c1739dae58ad.JPG" width=400 >

Take the connector cable of the CubeCell board and connect the red wire with 
The red wire of the BLE module. Solder these cables to the middle pin of the 
Switch. Take the red battery wire and solder it to the correct pin of the switch.
Solder the three black ground wires together.

**Portable Receiver**

<img src="https://user-images.githubusercontent.com/122114833/213926623-0c46abcc-2ec6-4f5c-bf04-0810115d1618.JPG" width=400 >


Take the neopixels and solder three wires to it
The data wire to pin 23 of the Heltec board
Vcc to the 3.3 pin
gnd to gnd

Use the connector cable and solder the red wire to the middle pin of the switch
And the lipo red wire to the correct pin

Do this only if you are using the printed case:
Solder a black wire to another gnd pin of the Heltec board, then solder a 1 k resistor 
to The end of the wire. Take the resistor and solder it to one pin of the tactile
Button. On the correct pin solder a wire to it, and this wire to pin 17 of the 
Board. Now solder a wire to the other pin of the pin and the 3.3V pin

Now you can put everything in the receiver case
  

**Raspberry Pi Receiver**


<img src="https://user-images.githubusercontent.com/122114833/213926671-2d609eb0-06f4-4e25-b5ff-226389bf90a8.png" >


Solder 11 wires to the LoRa breakout board 
And the antenna to the antenna pin
|Raspberry Pi  | Lora – SX1276 Module |
|--|--|
| 3.3V | 3.3V |
| gnd | gnd |
| GPIO 10 | MOSI |
| GPIO 9 | MISO |
| GPIO 11 | SCK |
| GPIO 8 | Nss / Enable |
| GPIO 4 | DIO 0 |
| GPIO 17 | DIO 1 |
| GPIO 18 | DIO 2 |
| GPIO 27 | DIO 3 |
| GPIO 22 | RST |

 ##  Software
 
### Arduino IDE

Go to https://www.arduino.cc/en/software and download the Arduino IDE

Next, we have to include the libraries for the Heltec boards. 
Open the Arduino IDE and go to file -> preferences
Click on the two squares and paste the three links into it

https://dl.espressif.com/dl/package_esp32_index.json                                                                                  
https://github.com/HelTecAutomation/CubeCell-Arduino/releases/download/V1.5.0/package_CubeCell_index.json                                                    
https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/releases/download/0.0.6/package_heltec_esp32_index.json

<img src="https://user-images.githubusercontent.com/122114833/211428441-91f0e410-7b97-4d59-a329-246808be96ff.png" width="500" />  
    

### **Sender**

Connect the CubeCell board to your computer and choose the port and the correct board from the manager.

<img width="440" alt="choosing board" src="https://user-images.githubusercontent.com/122114833/211617519-c21733d7-b2b8-44a0-89aa-54a3c0c5e006.png"> <img width="400" alt="github ports" src="https://user-images.githubusercontent.com/122114833/211617622-b02fab77-1890-47c0-b405-811a166f3d2a.png">
  
To test the CubeCell board upload the oledDisplayGPSInfo.ino file
file->examples->OnBoardGPS ->  oledDisplayGPSInfo -> ctrl + u.
If there’s an compile error try to replace line 8 with this 

`SSD1306Wire  display(0x3c, 500000, SDA, SCL ,GEOMETRY_128_64,GPIO10); // addr, freq, sda, scl,resolution, rst`

Put the board outside and wait until the board gets a GPS fix
First time using it could take some time until it gets a fix
Next time it is much faster

If everything is working open the sender sketch which is in the github folder you’ve downloaded

 <img src="https://user-images.githubusercontent.com/122114833/211618319-06269ece-e87e-4040-b5fd-8b330c67f955.png" width=500 >

 **GPS settings**
 
The default GPS settings are GPS_INIT_TIMEOUT 5 Minutes, 
GPS_SLEEPTIME also 5 Minutes, GPS_UPDATE_TIMEOUT 30 seconds.
If you want that your tracker sleeps longer you can change the sleeptime.
The numbers are in milliseconds. (1 sec = 1000ms)

**LoRa settings**

choose your frequency : 
- Europe EU868 MHz
- South America  AU915/AS923-1 MHz
- North America US915 MHz
- India  IN865 MHz
- Asia AS923 MHz

You also can change the bandwidth, codingrate and spreading factor. Keep in mind if you change these settings you have to change them 
in the code for the receivers too.
Upload the code (ctrl + u)

### **Portable Receiver**

 Now connect the Heltec WiFi board to your computer. Open the portable receiver sketch (downloaded github folder) and choose the Heltec Wifi LoRa V2 and the port.
 
<img width="574" alt="portable receiver frequency" src="https://user-images.githubusercontent.com/122114833/211621529-ab0573fc-110e-43e8-a38c-38d657592c22.png">

In line 284 change the LoRa frequency. This time it’s called band.

<img src="https://user-images.githubusercontent.com/122114833/211621877-b22345c4-cd53-482d-9d48-1d9d882951cd.png" width=400 >

Here you can change the bandwidth, codingrate and sprading factor.
But it has to be the same settings like in the sender sketch.


<img src="https://user-images.githubusercontent.com/122114833/211430393-386a080f-0aac-4464-b913-4023dee51eb2.png" height="400" /> 

On the left side click on the library manager and search for adafruit neopixel. Install the Adafruit NeoPixel by Adafruit.

Upload the code

<img src="https://user-images.githubusercontent.com/122114833/213926747-ac5794c3-bc3b-4afc-86d0-b11c43a1bc61.jpg" width=600 >

BT -> bluetooth connection with senders ble module, the number is the signalstrength

V -> it receives the senderes battery capacity in millivolts

LoRa: 
- zone -> you can set a zone to now wheres your pet at instead of only coordinates
- numbers -> senders coordinates
- RSSI -> LoRa signalstrength
    
### **Raspberry Pi Receiver**

For this receiver I’m using this repository: https://github.com/rpsreal/pySX127x

Download the Raspberry Pi Imager from here: https://www.raspberrypi.com/software/
and Visual Studio Code https://code.visualstudio.com/
Open the imager and insert a micro sd cart into your computer.

<img width="545" alt="rp-imager" src="https://user-images.githubusercontent.com/122114833/211622792-e3721119-0d0b-4ab7-b4b0-c83cf8c499ff.png">

Choose the Raspberry Pi Os (32-Bit) operating system and your sd card.
Click on the gear and configurate following settings.
<img width="762" alt="rp-imager settings" src="https://user-images.githubusercontent.com/122114833/211622811-4d480d6e-ecd7-4f8f-a09f-ba12be704bfe.png">


- enable Hostname: raspberrypi
- enable ssh
- enter pi as username and choose a password
- set your Wifi SSID and password
- set your country

Save everything and click on the write button
After the os is written to your sd card you can remove it and insert it into your Raspberry Pi. Plug the power cable into your Pi.
Open Visual Studio Code and click on the left side on the extensions icon. Search for remote ssh and install it.

<img src="https://user-images.githubusercontent.com/122114833/211623776-aa4069be-b414-4e32-9e3c-7eb20a6bbb4b.png" >

<img src="https://user-images.githubusercontent.com/122114833/211623823-29b6d0f2-1b60-4c98-9e34-f59edbc09d77.png" >

Wait couple minutes to let your Pi boot up.
Click on the green icon in the bottom left corner and choose connect current window to host -> add new ssh host -> write ssh pi@raspberrypi and press enter -> C:\user\.ssh\config -> click again on the green icon in the bottom left corner and choose connect current window to host -> Linux -> raspberrypi -> continue -> enter your password 
Now you are connected to your Pi. Click on file -> open folder -> \home\pi
Terminal -> new terminal
enter every line and press enter if it ask to continue enter y

    sudo apt-get update
    sudo apt-get upgrade
    sudo apt-get install apache2
    sudo mkdir /var/www/html/tracker
  
    sudo raspi-config nonint do_spi 0
    sudo apt-get install python-dev python3-dev
    sudo apt-get install python-pip python3-pip
    
    pip install RPi.GPIO
    pip install spidev
    pip install pyLoRa

    sudo apt-get install git
    git clone https://github.com/rpsreal/pySX127x

After everything is installed enter sudo reboot. Wait a minute and connect with your pi again (green icon in the buttom left corner).

Drag the website.py file from the downloaded github folder and drop it into the pi folder in visual studio code.

<img src="https://user-images.githubusercontent.com/122114833/211624938-0afb3442-229b-4397-93b9-6648514ffd77.png" >

Now right click under the website file -> new file -> array.txt
Go to the terminal again. To run the code enter `python3 website.py`

To stop the code press ctrl + c.

<img src="https://user-images.githubusercontent.com/122114833/211625718-b94708b1-918d-4cbd-9e9e-7f0cb00ab625.png" >

To change the LoRa settings click on the website.py file.
Remember to change your frequency and set the same bandwidth and spreading factor like the sender.


### **Map**

for the map I'm using Leaflet. You can change the pictures and the offset of the paw prints
<img src="https://user-images.githubusercontent.com/122114833/211626506-d85fd92b-cc55-4448-93d5-cb847e2430ce.png" width=500 >

IF you changed the code press ctrl + s and run the code. 

After your Raspberry Pi received some coordinates you can check them out here: http://raspberrypi/tracker/index.html

### **DSD TECH Bluetooth Tools**

Because i use a module from DSD TECH i configurated my module with their software 
http://www.dsdtech-global.com/2017/11/dsd-tech-bluetooth-config-tool.html
Use a ttl converter and connect 
|TTL converter| BLE module |
|--|--|
| 5V | vcc |
| gnd | gnd |
| RX | TX |
| TX | RX |

<img src="https://user-images.githubusercontent.com/122114833/211634261-5c0270c7-6eac-411c-a240-9bd7f29755be.png" height=400> <img src="https://user-images.githubusercontent.com/122114833/213926889-156ebc99-8a32-47a0-bf14-9cbbdf2ea782.JPG" width=540 >


<img src="https://user-images.githubusercontent.com/122114833/211635607-d892b28a-6066-4276-a1a8-f300dcad7f8d.png" >


In the software choose your com port, click on open and go to the hm-10 tab
Click on ‘test’ to check if there is a connection

If in the consol comes a ok back everything is good

First change the modules name (e.g. your cats name) -> set

Auth Mode: Auth not need PIN -> set

PIN: choose a pin  -> set

<img src="https://user-images.githubusercontent.com/122114833/211635969-1ba14390-b8b1-4c11-91f8-285349c03799.png" width=800 >
<img src="https://user-images.githubusercontent.com/122114833/211636070-7222b2fb-aec5-4c05-93af-c5b6ff38c074.png" width=800 >

Go to the AT Comand tab

ASCII: 

AT+PWRM0 -> send		(auto sleep)

AT+MODE2 -> send		(Remote Control Mode + Transmission Mode)

AT+BEFC000 -> send		(Set Module pin output state, After power supplied)


AT+AFTC000 -> send		(Set Module pin output state, After connection is established) 
In my case i wasn’t able to change the service and characteristics uuid with ascii (AT+UUID0xFFE4 | AT+CHAR0xFFE5) so i converted these two camands in hexadecimal

HEX:

41 54 2B 55 55 49 44 30 78 46 46 45 34    ->    send	(Service uuid)

41 54 2B 43 48 41 52 30 78 46 46 45 35    ->    send	(characteristics uuid)

Now your module is configurated. Click on close icon and unplug the ttl converter. Plug it again into your computer and search with your smartphone for bluetooth devices. If you are able to find your module with the name you set everything is fine.
If you have an iPhone it coul be possible that you are unable to see the module in your bluetooth devices.
Take the portable receiver and press the button now it will search for the ble module. When they are connected the neopixels begin to glow and show the distance between these two devices.

#!/usr/bin/env python3

""" This program sends a response whenever it receives the "INF" """

# Copyright 2018 Rui Silva.
#
# This file is part of rpsreal/pySX127x, fork of mayeranalytics/pySX127x.
#
# pySX127x is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
# License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# pySX127x is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
# details.
#
# You can be released from the requirements of the license by obtaining a commercial license. Such a license is
# mandatory as soon as you develop commercial activities involving pySX127x without disclosing the source code of your
# own applications, or shipping pySX127x with a closed source product.
#
# You should have received a copy of the GNU General Public License along with pySX127.  If not, see
# <http://www.gnu.org/licenses/>.

import subprocess


def callCmd(cmd):
    result = subprocess.Popen(cmd,shell=False,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    out, err = result.communicate()
    return [out,err.decode(),result.returncode]


import time
from SX127x.LoRa import *
#from SX127x.LoRaArgumentParser import LoRaArgumentParser
from SX127x.board_config import BOARD
import datetime

BOARD.setup()
BOARD.reset()
#parser = LoRaArgumentParser("Lora tester")


class mylora(LoRa):
    def __init__(self, verbose=False):
        super(mylora, self).__init__(verbose)
        self.set_mode(MODE.SLEEP)
        self.set_dio_mapping([0] * 6)

    def on_rx_done(self):
        #BOARD.led_on()
        #print("\nRxDone")
        self.clear_irq_flags(RxDone=1)
        payload = self.read_payload(nocheck=True )# Receive INF
        print ("Receive: ")
        mens=bytes(payload).decode("utf-8",'ignore')
      
        print("Orginal string:" + mens)
      
        #BOARD.led_off()
      
        rssi_pkt = self.get_pkt_rssi_value() 
        snr = self.get_pkt_snr_value()
        print(str(rssi_pkt))
        print(str(snr))
       
        datee = datetime.datetime.now()
        lat = mens[0:9]
        lng = mens[10:19]

        ##.000000
        f = open('coordinates.txt', 'a')
       
        f.write(datee.strftime("%H:%M:%S   %d-%m-%Y "))
        f.write('             ')
        f.write(mens)
        f.write('\n')                                                               
        f.close()

        latlongs = []
        
        partitionlat = lat.partition('.')
        partitionlng = lng.partition('.')
        
        
        if (
            (partitionlat[0].isdigit() and partitionlat[1] == '.' and partitionlat[2].isdigit()) or (partitionlat[0] == '' and partitionlat[1] == '.' and partitionlat[2].isdigit()) 
            or (partitionlat[0].isdigit() and partitionlat[1] == '.' and partitionlat[2] == '')):
            
            if (
                (partitionlng[0].isdigit() and partitionlng[1] == '.' and partitionlng[2].isdigit()) or (partitionlng[0] == '' and partitionlng[1] == '.' and partitionlng[2].isdigit()) 
                 or (partitionlng[0].isdigit() and partitionlng[1] == '.' and partitionlat[2] == '')):

                    if(len(lat) == 9 and len(lng) == 9):
                    
                        f = open('array.txt', 'a')
                        f.write('[')
                        f.write(lat)
                        f.write(', ')
                        f.write(lng)
                        f.write('], ')
                        f.write('\n')
                        f.close()
            
        f = open('array.txt', 'r')
        latlongs = f.read()
        latlongs = latlongs.strip()
  
        f.close()

        
        lines = ['''
        <!DOCTYPE html>
        <html>
        <head>
        <meta charset='utf-8'>
        <meta http-equiv='X-UA-Compatible' content='IE=edge'>
            <title>Mattys Tracker</title>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
   
            <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.3/dist/leaflet.css" integrity="sha256-kLaT2GOSpHechhsozzB+flnD+zUyjE2LlfWPgU04xyI=" crossorigin=""/>
            <script src="https://unpkg.com/leaflet@1.9.3/dist/leaflet.js" integrity="sha256-WBkoXOwTeyKclOHuWtc+i2uENFpDZ9YPdf5Hf+D7ewM=" crossorigin=""></script>
            <script src="http://hs-augsburg.de/homes/lekr6621/Mattys_Website/leaflet/polylineDecorator/leaflet.polylineDecorator.js"></script>


            <style>
                .leaflet-popup-tip-container {
                    display: none;
                } 


                .mapContainer{
                    background-color:white;
                    width: 1600px;
                    margin-left: 20px;
                    margin-top: 25px;
                }

            </style>


        </head>
        <body>


        <div>
        <div class="mapContainer">
            <div id = "map" style = "width: 1600px; height: 1000px"></div>
            <script>
                var latlngs = [''' + latlongs + '''];
               
               console.log(latlngs);


                // Creating map options
                var mapOptions = {
                    center: latlngs[0],
                    zoom: 20,
                }
                
                // Creating a map object
                var map = new L.map('map', mapOptions);
                
                // Creating a Layer object
                var layer = new L.TileLayer('http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png');
                
                // Adding layer to the map
                map.addLayer(layer);
                
                
                var myIcon = L.icon({
                iconUrl: "http://hs-augsburg.de/~lekr6621/Mattys_Website/leaflet/images/noun-black-cat.png",
                iconSize: [60, 60],
                iconAnchor: [30, 30],
                });

                var myIcon1 = L.icon({
                    iconUrl: "http://hs-augsburg.de/~lekr6621/Mattys_Website/leaflet/images/black_cat_left.png",
                    iconSize: [60, 60],
                    iconAnchor: [30, 30],
                });

                


                
                let tooltipOptions = {
                    permanent: true,
                }



                for(var i = 0; i < latlngs.length; i++){
                    let marker = new L.Marker(latlngs[i], {title: i+1, zIndexOffset: 100});
                    if(i > (latlngs.length/2)) {
                        marker.setIcon(myIcon1);
                    }else{
                        marker.setIcon(myIcon);
                    }
                    marker.addTo(map);  
                }


                var polyline = L.polyline(latlngs, {color: 'red',opacity:0.3}).addTo(map);


                var pfote = L.polylineDecorator(polyline, {
                patterns: [
                { offset: '1%', repeat: '3%', endOffset: '1%', symbol: L.Symbol.marker({rotate: true, markerOptions: {
                icon: L.icon({
                iconUrl: "http://hs-augsburg.de/~lekr6621/Mattys_Website/leaflet/images/pawBlue.png",
                iconAnchor: [7, 7],
                iconSize: [14, 14],

                })
                }})}
                ]
                }).addTo(map);
            </script>
            

             black cat by varvarvarvarra from <a href="https://thenounproject.com/browse/icons/term/black-cat/" target="_blank" title="black cat Icons">Noun Project</a>
         
            </div>

        </div>

        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
        </body>
        </html>
        ''']
        with open('/var/www/html/tracker/index.html', 'w') as f:
            f.writelines(lines)

        self.reset_ptr_rx()
        self.set_mode(MODE.RXCONT)

    def on_tx_done(self):
        print("\nTxDone")
        print(self.get_irq_flags())

    def on_cad_done(self):
        print("\non_CadDone")
        print(self.get_irq_flags())

    def on_rx_timeout(self):
        print("\non_RxTimeout")
        print(self.get_irq_flags())

    def on_valid_header(self):
        print("\non_ValidHeader")
        print(self.get_irq_flags())

    def on_payload_crc_error(self):
        print("\non_PayloadCrcError")
        print(self.get_irq_flags())

    def on_fhss_change_channel(self):
        print("\non_FhssChangeChannel")
        print(self.get_irq_flags())

    def start(self):          
        while True:
            self.reset_ptr_rx()
            self.set_mode(MODE.RXCONT) # Receiver mode
            while True:
                pass;
            
#### einstellungen
lora = mylora(verbose=False)
#args = parser.parse_args(lora) # configs in LoRaArgumentParser.py

#     Slow+long range  Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. 13 dBm
lora.set_pa_config(pa_select=1, max_power=21, output_power=14)
lora.set_bw(BW.BW125)
#lora.set_bw(BW.BW62_5)
# lora.set_bw(BW.BW41_7)
#lora.set_bw(BW.BW31_25)

lora.set_coding_rate(CODING_RATE.CR4_5)
# lora.set_coding_rate(CODING_RATE.CR4_8)
# lora.set_spreading_factor(12)
# lora.set_spreading_factor(10)
lora.set_spreading_factor(9)
# lora.set_spreading_factor(7)

lora.set_rx_crc(True)
#lora.set_rx_crc(False)

lora.set_freq(868.0)


#lora.set_lna_gain(GAIN.G1)

lora.set_implicit_header_mode(False)
#lora.set_implicit_header_mode(True)


# lora.set_low_data_rate_optim(True)
lora.set_low_data_rate_optim(False)

#  Medium Range  Defaults after init are 434.0MHz, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on 13 dBm
#lora.set_pa_config(pa_select=1)



assert(lora.get_agc_auto_on() == 1)

try:
    print("START")
    lora.start()
except KeyboardInterrupt:
    sys.stdout.flush()
    print("Exit")
    sys.stderr.write("KeyboardInterrupt\n")
finally:
    sys.stdout.flush()
    print("Exit")
    lora.set_mode(MODE.SLEEP)
BOARD.teardown()
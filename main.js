var mraa = require('mraa'); //require mraa
console.log('MRAA Version: ' + mraa.getVersion()); //print out the mraa version in IoT XDK console

//Intel(R) Edison & Intel(R) Galileo 
u = new mraa.Uart(0); //Default

//Name:     UART1, the general-purpose TTL-level port (Arduino shield compatibility)
//Location: Pins 0 (RX) and 1 (TX) on the Arduino shield interface headers or the UART slot on the Grove Starter Kit Base Shield
var serialPath = u.getDevicePath(); 

var SerialPort = require("serialport").SerialPort;
var serialPort = new SerialPort(serialPath, {
    baudrate: 9600
});

var request = require('request');

var header = {
    'Authorization': '746be99d4ef39262e059f3afb7a9cb4f',
    'Content-Type': 'application/x-www-form-urlencoded'
}

var option = {
    url: 'http://enigmatech.id/fish/v1/tasks',
    method: 'POST',
    headers: header
}

serialPort.on("open",function() {
    console.log("open");
    console.log("Connected to "+serialPath);
    serialPort.on("data", function(data) { //Read available data from serial port
        console.log("data received: " + data);
        
        var fishingData = data.toString();
	fishingData = fishingData.split(",");
        //console.log("Fishing Data: " + fishingData);
        if(fishingData.length == 3){
            console.log("Jumlah Ikan: " + fishingData[0]);
            console.log("Rata2 Panjang Ikan: " + fishingData[1]);
            console.log("Rata2 Tebal Ikan: " + fishingData[2]);
            
            option.form = {'sensor1':fishingData[0], 'sensor2':fishingData[1], 'sensor3':'-10.239249', 'output':'102.832031'}
            
            request(option, function (error, response, body) {
                //console.log(response);
                if (!error && response.statusCode == 201) {
                    // Print out the response body
                    console.log("Success writing to webservice!");
                }
            })
        }
        
    });
    
});

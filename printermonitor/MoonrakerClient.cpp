/** The MIT License (MIT)

Copyright (c) 2018 David Payne

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// Additional Contributions:
/* 15 Jan 2019 : Owen Carter : Add psucontrol query via POST api call */

#include "MoonrakerClient.h"

MoonrakerClient::MoonrakerClient(String ApiKey, String server, int port, String user, String pass, boolean psu) {
  updatePrintClient(ApiKey, server, port, user, pass, psu);
}

void MoonrakerClient::updatePrintClient(String ApiKey, String server, int port, String user, String pass, boolean psu) {
  server.toCharArray(myServer, 100);
  myApiKey = ApiKey;
  myPort = port;
  encodedAuth = "";
  if (user != "") {
    String userpass = user + ":" + pass;
    base64 b64;
    encodedAuth = b64.encode(userpass, true);
  }
  pollPsu = psu;
}

boolean MoonrakerClient::validate() {
  boolean rtnValue = false;
  printerData.error = "";
  if (String(myServer) == "") {
    printerData.error += "Server address is required; ";
  }
  if (printerData.error == "") {
    rtnValue = true;
  }
  return rtnValue;
}

WiFiClient MoonrakerClient::getSubmitRequest(String apiGetData) {
  WiFiClient printClient;
  printClient.setTimeout(5000);

  Serial.println("Getting Moonraker Data via GET");
  Serial.println(apiGetData);
  result = "";
  if (printClient.connect(myServer, myPort)) {  //starts client connection, checks for connection
    printClient.println(apiGetData);
    printClient.println("Host: " + String(myServer) + ":" + String(myPort));
    //printClient.println("X-Api-Key: " + myApiKey);
    //if (encodedAuth != "") {
    //  printClient.print("Authorization: ");
    //  printClient.println("Basic " + encodedAuth);
    //}
    printClient.println("User-Agent: ArduinoWiFi/1.1");
    printClient.println("Connection: close");
    if (printClient.println() == 0) {
      Serial.println("Connection to " + String(myServer) + ":" + String(myPort) + " failed.");
      Serial.println();
      resetPrintData();
      printerData.error = "Connection to " + String(myServer) + ":" + String(myPort) + " failed.";
      return printClient;
    }
  } 
  else {
    Serial.println("Connection to Moonraker failed: " + String(myServer) + ":" + String(myPort)); //error message if no client connect
    Serial.println();
    resetPrintData();
    printerData.error = "Connection to Moonraker failed: " + String(myServer) + ":" + String(myPort);
    return printClient;
  }
  return printClient;
}

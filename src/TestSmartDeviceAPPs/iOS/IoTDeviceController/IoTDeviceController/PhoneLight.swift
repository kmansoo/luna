//
//  PhoneLight
//  IoTDeviceController
//
//  Created by Kim, Min Ho on 2016. 2. 18..
//  Copyright © 2016년 Luna. All rights reserved.
//

import Foundation
import AVFoundation

class PhoneLight: WebSocketDelegate {
    
    static let sharedInstance = PhoneLight()
    
    var myDeviceInfo : AnyObject?
    var socket: WebSocket!
    var timer = NSTimer()
    
    var controller: Notification?
    var connected: Bool = false
    
    // MARK: - WebSocket Functions
    func websocketDidConnect(ws: WebSocket) {
        do {
            let data = try NSJSONSerialization.dataWithJSONObject(myDeviceInfo!, options: NSJSONWritingOptions.PrettyPrinted)
            let convertedData = NSString(data: data, encoding: NSUTF8StringEncoding)
            
            //  Register..
            var registerCmd = "{"
            registerCmd += "   \"Request\" : true,"
            registerCmd += "   \"Command\" : \"Register\","
            registerCmd += "   \"Info\": "
            registerCmd += (convertedData as! String)
            registerCmd += "}"
            
            socket.writeString(registerCmd)
            
            connected = true
        }
        catch {
            print("error")
        }
    }
    
    func websocketDidDisconnect(ws: WebSocket, error: NSError?) {
        connected = false

        if let e = error {
            print("websocket is disconnected: \(e.localizedDescription)")
        } else {
            print("websocket disconnected")
        }
        
        //  다시 연결 시도
        timer = NSTimer.scheduledTimerWithTimeInterval(5, target: self, selector: "retryConnectToIoTManager", userInfo: nil, repeats: false)
    }
    
    func websocketDidReceiveMessage(ws: WebSocket, text: String) {
        var json = JSON.parse(text);
        
        if (json != JSON.null)
        {
            if (json["Command"] == "SetDevice")
            {
                let light = json["Info"]["Control"].stringValue
                
                if light == "On" {
                    phoneFlash(isOn: true)
                } else {
                    phoneFlash(isOn: false)
                }
            }
        }
    }
    
    func websocketDidReceiveData(ws: WebSocket, data: NSData) {
        print("Received data: \(data.length)")
    }
    
    // MARK: - Helper Functions
    
    func loadJSON() {
        if let filePath = NSBundle.mainBundle().pathForResource("iPhoneDeviceInfo", ofType: "json"), data = NSData(contentsOfFile: filePath) {
            
            do {
                myDeviceInfo = try NSJSONSerialization.JSONObjectWithData(data, options: NSJSONReadingOptions.AllowFragments)
                
                print(myDeviceInfo)
            }
            catch {
                print("error")
            }
        } else {
            print("Invalid filename/path.")
        }
    }
    
    //  for Timer
    // must be internal or public.
    func retryConnectToIoTManager() {
        retryConnectToIoTManagerWith("ws://localhost:8000/ws_iot_deivce")
    }
    
    func retryConnectToIoTManagerWith(uri: String) {
        print("connect to the \(uri)")
        socket = WebSocket(url: NSURL(string: uri)!, protocols: ["chat", "superchat"])
        
        socket.delegate = self
        socket.connect()
    }
    
    func phoneFlash(isOn isOn: Bool) {
        
        let device = AVCaptureDevice.defaultDeviceWithMediaType(AVMediaTypeVideo)
        
        if (device.hasTorch) {
            do {
                try device.lockForConfiguration()
                
                if isOn == true {
                    try device.setTorchModeOnWithLevel(0.5)
                } else {
                    device.torchMode = AVCaptureTorchMode.Off
                }

                device.unlockForConfiguration()
            } catch {
                print(error)
            }
        }
    }
}
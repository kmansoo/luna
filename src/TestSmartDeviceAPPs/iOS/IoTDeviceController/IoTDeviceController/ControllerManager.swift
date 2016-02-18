//
//  ControllerManager.swift
//  IoTDeviceController
//
//  Created by Kim, Min Ho on 2016. 2. 18..
//  Copyright © 2016년 Luna. All rights reserved.
//

import Foundation

class ControllerManager: WebSocketDelegate {
    
    static let sharedInstance = ControllerManager()
    
    var myDeviceInfo : AnyObject?
    var socket: WebSocket!
    var timer = NSTimer()
    
    var viewController: Notification?
    var connected: Bool = false {
        didSet {
            if connected == true {
                viewController?.doSomething()
            }
        }
    }
    
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
            if (json["Command"] == "UpdateDeviceStatus")
            {
                let value = json["Info"]["Value"].stringValue;
                
                //                devicePositionInfo.setValue(Float(json["Info"]["Value"].intValue), animated: false)
                //
                //                value += " cm"
                //                positionDisplay.text = value
                print("\(value)")
            }
        }
    }
    
    func websocketDidReceiveData(ws: WebSocket, data: NSData) {
        print("Received data: \(data.length)")
    }
    
    // MARK: - Helper Functions
    
    func loadJSON() {
        if let filePath = NSBundle.mainBundle().pathForResource("iPhoneControllerInfo", ofType: "json"), data = NSData(contentsOfFile: filePath) {
            
            do {
                myDeviceInfo = try NSJSONSerialization.JSONObjectWithData(data, options: NSJSONReadingOptions.AllowFragments)
                
                print(myDeviceInfo)
                
                // change the 'managerUri' in the JSON file
                //                if let uri = myDeviceInfo!["IoTDeviceMasterUri"] as? String {
                //                    managerUri.text = uri
                //
                //                    print("managerUri.text = '\(managerUri.text)'")
                //                }
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
}
//
//  ViewController.swift
//  IoTDeviceController
//
//  Created by 김만수 on 2016. 2. 5..
//  Copyright © 2016년 Luna. All rights reserved.
//

import UIKit

class ViewController: UIViewController , WebSocketDelegate {
    
    var timer = NSTimer()
    
    @IBOutlet weak var managerUri: UITextField!
    @IBOutlet weak var devicePositionInfo: UISlider!
    @IBOutlet weak var positionDisplay: UILabel!
    
    var socket : WebSocket!
    var myDeviceInfo : AnyObject?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        print("test")
        if let filePath = NSBundle.mainBundle().pathForResource("MyDeviceInfo", ofType: "json"), data = NSData(contentsOfFile: filePath) {
            
            do {
                myDeviceInfo = try NSJSONSerialization.JSONObjectWithData(data, options: NSJSONReadingOptions.AllowFragments)
                
                print(myDeviceInfo)
                
                if let uri = myDeviceInfo!["IoTDeviceMasterUri"] as? String {
                    managerUri.text = uri
                    
                    print("managerUri.text = '\(managerUri.text)'")
                }
            }
            catch {
                print("error")
            }
        } else {
            print("Invalid filename/path.")
        }
        
        retryConnectToIoTManager()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
        
    }
    
    //  for Timer
    // must be internal or public.
    func retryConnectToIoTManager() {
        print("connect to the \(managerUri.text)")
        socket = WebSocket(url: NSURL(string: managerUri.text!)!, protocols: ["chat", "superchat"])
        
        socket.delegate = self
        socket.connect()
    }

    // for websocket
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
        }
        catch {
            print("error")
        }
    }
    
    func websocketDidDisconnect(ws: WebSocket, error: NSError?) {
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
                var value = json["Info"]["Value"].stringValue;
                
                devicePositionInfo.setValue(Float(json["Info"]["Value"].intValue), animated: false)
                
                value += " cm"
                positionDisplay.text = value
            }
        }
    }
    
    func websocketDidReceiveData(ws: WebSocket, data: NSData) {
        print("Received data: \(data.length)")
    }
    
    @IBAction func lightTurnOn(sender: AnyObject) {
        print("All Lights Turn ON ")
        
        var registerCmd = "{"
        registerCmd += "   \"Request\" : true,"
        registerCmd += "   \"Command\" : \"SetControl\","
        registerCmd += "   \"Info\": { "
        registerCmd += "       \"Control\" : \"AllLightsTurnOn\""
        registerCmd += "    }"
        registerCmd += "}"
        
        socket.writeString(registerCmd)
    }

    @IBAction func lightTurnOff(sender: AnyObject) {
        print("All Lights Turn OFF")
        
        var registerCmd = "{"
        registerCmd += "   \"Request\" : true,"
        registerCmd += "   \"Command\" : \"SetControl\","
        registerCmd += "   \"Info\": { "
        registerCmd += "       \"Control\" : \"AllLightsTurnOff\""
        registerCmd += "    }"
        registerCmd += "}"
        
        socket.writeString(registerCmd)
    }
    
    // MARK: Write Text Action
    @IBAction func writeText(sender: UIBarButtonItem) {
        socket.writeString("hello there!")
    }
    
    // MARK: Disconnect Action
    @IBAction func disconnect(sender: UIBarButtonItem) {
        if socket.isConnected {
            sender.title = "Connect"
            socket.disconnect()
        } else {
            sender.title = "Disconnect"
            socket.connect()
        }
    }
}


//
//  LunaConnectController.swift
//  IoTDeviceController
//
//  Created by Kim, Min Ho on 2016. 2. 18..
//  Copyright © 2016년 Luna. All rights reserved.
//

import UIKit

class LunaConnectController: UITableViewController, WebSocketDelegate {

    @IBOutlet weak var lunaURI: UILabel!
    
    var myDeviceInfo : AnyObject?
    var socket : WebSocket!
    var timer = NSTimer()

    // MARK: - Controller Functions
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Uncomment the following line to preserve selection between presentations
        // self.clearsSelectionOnViewWillAppear = false

        // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
        // self.navigationItem.rightBarButtonItem = self.editButtonItem()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    @IBAction func connectButton(sender: UIButton) {
        
        loadJSON()
        retryConnectToIoTManager()
        
//        let alert = UIAlertController(title: "Server Connect", message: "Connecting", preferredStyle: UIAlertControllerStyle.Alert)
//        
//        alert.addAction(UIAlertAction(title: "No", style: UIAlertActionStyle.Default, handler: nil))
//        alert.addAction(UIAlertAction(title:"OK", style: .Default, handler:  { action in
//            self.performSegueWithIdentifier("LogInSegue", sender: self)
//        }))
//        
//        self.presentViewController(alert, animated: true, completion: nil)
    }

    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        
        if segue.identifier == "LogInSegue" {
            
        }
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
        if let uri = lunaURI.text {
            print("connect to the \(uri)")
            socket = WebSocket(url: NSURL(string: uri)!, protocols: ["chat", "superchat"])
            
            socket.delegate = self
            socket.connect()
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
        }
        catch {
            print("error")
        }
        self.performSegueWithIdentifier("LogInSegue", sender: self)
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
}

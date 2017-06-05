//
//  LunaConnectController.swift
//  IoTDeviceController
//
//  Created by Kim, Min Ho on 2016. 2. 18..
//  Copyright © 2016년 Luna. All rights reserved.
//

import UIKit

class LunaConnectController: UITableViewController, Notification {

    // MARK: - Properties
    
    @IBOutlet weak var lunaURI: UILabel!
    @IBOutlet weak var lunaCommand: UILabel!
    
    var phoneController = PhoneController.sharedInstance
    var light = PhoneLight.sharedInstance
    
    // MARK: - Controller Functions
    
    override func viewDidLoad() {
        super.viewDidLoad()

        phoneController.viewController = self
        
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
        if let url = lunaURI.text, let command = lunaCommand.text  {
            let uri = url + command
            phoneController.loadJSON()
            phoneController.retryConnectToIoTManagerWith(uri)

            light.loadJSON()
            light.retryConnectToIoTManagerWith(uri)
        }
    }

    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
        
        if segue.identifier == "LogInSegue" {
            let destination = segue.destinationViewController as! LunaDevicesController
            
            destination.url = self.lunaURI.text
        }
    }
    // MARK: - Notification Protocol
    
    func connect() {
        self.performSegueWithIdentifier("LogInSegue", sender: self)
    }
}

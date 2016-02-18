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
    
    let phoneController = PhoneController.sharedInstance
    let light = PhoneLight.sharedInstance
    
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
        if let uri = lunaURI.text {
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
            
        }
    }
    // MARK: - Notification Protocol
    
    func connect() {
        self.performSegueWithIdentifier("LogInSegue", sender: self)
    }
}

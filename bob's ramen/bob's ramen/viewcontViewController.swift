
//
//  ViewController.swift
//  Bob's Ramen
//
//  Created by Karen Bowman on 9/17/16.
//  Copyright © 2016 Bobs. All rights reserved.
//

import UIKit

class ViewController: UIViewController,UIPickerViewDataSource, UIPickerViewDelegate{
    
    @IBOutlet weak var button: UIButton!
    @IBOutlet weak var hourTextfield: UITextField!
    @IBOutlet weak var minuteTextField: UITextField!
    var spiceLevel = 0
    
        @IBAction func ramenClicked(_ sender: UIButton) {
        //led1on()
        button.backgroundColor = UIColor.red
        urltest()
        
    }
    
    @IBAction func tapped(_ sender: AnyObject) {
        hourTextfield.resignFirstResponder()
        minuteTextField.resignFirstResponder()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        pickerView.delegate = self
        pickerView.dataSource = self
        button.layer.cornerRadius = 10
        //button.layer.borderWidth = 7
        //button.layer.borderColor = UIColor.red.cgColor
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    // URL paths including the ESP8266's IP address and arguments
    
    func urltest(){
        let s = "\(spiceLevel)\(hourTextfield.text!)\(minuteTextField.text!)"
        let url = URL(string: "http://192.168.43.217/makeramen/" + s)
        print("\(s)")
        print("yooooooo")
        let task = URLSession.shared.dataTask(with: url! as URL) { data, response, error in
            
            guard let data = data, error == nil else { return }
            
            print(NSString(data: data, encoding: String.Encoding.utf8.rawValue))
        }
        
        task.resume()
    }
    
    var led1on_URL = "http://192.168.43.217/makeramen"
    // IBAction connected to the "LED1 ON" button
    func led1on() {
        
        // Instantiate an NSURL object using the
        // led1on_URL string
        let requestURL = NSURL(string: led1on_URL)
        // Instantiate an NSURLRequest object using the
        // requestURL NSURL
        
        let request = NSMutableURLRequest(url:requestURL! as URL);
        
        // Set request HTTP method to GET. It could be POST as well
        request.httpMethod = "GET"
        
        // Excute HTTP Request
        let task = URLSession.shared.dataTask(with: request as URLRequest) {
            data, response, error in
            
            // Check for error
            if error != nil
            {
                print("error=\(error)")
                return
            }
            
            // Print out response string
            let responseString = NSString(data: data!, encoding: String.Encoding.utf8.rawValue)
            print("responseString = \(responseString)")
            
            
        }
        task.resume()
        
        
    }
    //MARK:Picker
    
    @IBOutlet weak var pickerView: UIPickerView!
    let pickerData = ["HOT HOT HOT!","Excite Me","Baby steps","No Spice(BOOOOO)"]
    
    //MARK: - Delegates and data sources
    //MARK: Data Sources
    //MARK: - Delegates and data sources
    //MARK: Data Sources
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1
    }
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return pickerData.count
    }    //MARK: Delegates
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        return pickerData[row]
    }
    
    func pickerView(_ pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
        if( pickerData[row]=="Baby steps"){
            spiceLevel = 1
        }
        else if(pickerData[row]=="Excite Me"){
            spiceLevel = 2
        }
        else if(pickerData[row]=="HOT HOT HOT!"){
            spiceLevel = 3
        }
        else{
            spiceLevel = 0
        }
    }
    
}

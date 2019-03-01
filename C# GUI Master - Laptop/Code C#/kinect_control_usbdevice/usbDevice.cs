using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using usbGenericHidCommunications;
using System.Windows.Forms;

// The following namespace allows debugging output (when compiled in debug mode)
using System.Diagnostics;

namespace Kinect_Control_USBDevice
{
    class usbDevice : usbGenericHidCommunication
    {
        /// <summary>
        /// Class constructor - place any initialisation here
        /// </summary>
        /// <param name="vid"></param>
        /// <param name="pid"></param>
        public usbDevice(int vid, int pid)
            : base(vid, pid)
        {
        }

        // Collect debug information from the device
        public String collectDebug()
        {
            // Collect debug information from USB device
            Debug.WriteLine("Reference Application -> Collecting debug information from device");

            // Declare our output buffer
            Byte[] outputBuffer = new Byte[65];

            // Declare our input buffer
            Byte[] inputBuffer = new Byte[65];

            // Byte 0 must be set to 0
           outputBuffer[0] = 0;

            // Byte 1 must be set to our command
       
           outputBuffer[1] = 0x10;

            // Send the collect debug command
            writeRawReportToDevice(outputBuffer);

            // Read the response from the device
            //readRawReportFromDevice(ref inputBuffer, 0x01);
            readSingleReportFromDevice(ref inputBuffer);

            // Byte 1 contains the number of characters transfered
            if (inputBuffer[1] == 0) return String.Empty;

            // Convert the Byte array into a string of the correct length
            string s = System.Text.ASCIIEncoding.ASCII.GetString(inputBuffer, 2, inputBuffer[1]);

            return s;
        }
        public Byte[] CollectData()
        {
            // Collect debug information from USB device
            Debug.WriteLine("Reference Application -> Collecting debug information from device");

            // Declare our output buffer
            Byte[] outputBuffer = new Byte[65];

            // Declare our input buffer
            Byte[] inputBuffer = new Byte[65];

            // Byte 0 must be set to 0
            outputBuffer[0] = 0;

            // Byte 1 must be set to our command

            outputBuffer[1] = 0x10;

            // Send the collect debug command
            writeRawReportToDevice(outputBuffer);

            // Read the response from the device
            readSingleReportFromDevice(ref inputBuffer);

            // Byte 1 contains the number of characters transfered
            if (inputBuffer[1] == 0) return inputBuffer;
            Byte[] array = new Byte[65];
            Array.Copy(inputBuffer, 2, array, 0, inputBuffer[1]);

            return array;
        }
        public String StartResponse()
        {
            // Collect debug information from USB device
            Debug.WriteLine("Reference Application -> Collecting debug information from device");

            // Declare our output buffer
            Byte[] outputBuffer = new Byte[65];

            // Declare our input buffer
            Byte[] inputBuffer = new Byte[65];

            // Byte 0 must be set to 0
            outputBuffer[0] = 0;

            // Byte 1 must be set to our command
            outputBuffer[1] = 0x02;

            // Send the collect debug command
            writeRawReportToDevice(outputBuffer);

            // Read the response from the device
            //readRawReportFromDevice(ref inputBuffer, 0x01);
            readSingleReportFromDevice(ref inputBuffer);

            // Byte 1 contains the number of characters transfered
            if (inputBuffer[1] == 0) return String.Empty;

            // Convert the Byte array into a string of the correct length
            string s = System.Text.ASCIIEncoding.ASCII.GetString(inputBuffer, 2, inputBuffer[1]);

            return s;
        }


        //--------------------------------------------------------------------//

        // Declare our output buffer
        Byte[] outputBuffer = new Byte[65];
        
        // Declare our input buffer
        Byte[] inputBuffer = new Byte[65];

        // 
        

        // Ham su dung kiem tra phan hoi tu VDK
        public bool CheckResponse(byte byteOut, byte byteIn)
        {
            // Byte 0 must be set to 0
            outputBuffer[0] = 0;

            // Byte 1 must be set to our command
            outputBuffer[1] = byteOut;

            // Perform the write command
            bool success;
            success = writeRawReportToDevice(outputBuffer);
            // Only proceed if the write was successful
            if (success)
            {
                // Perform the read
                success = readSingleReportFromDevice(ref inputBuffer);
            }

            if (inputBuffer[3] == byteIn) return true; else return false;
        }

        public bool TURNONSERVO()
        {
            if (CheckResponse(0x20, 0x21) == true) return true; else return false;
        }

        public bool ISMACHINEREADY()
        {
            if (CheckResponse(0x22, 0x23) == true) return true; else return false;
        }

        
        public bool DATASEND(byte code,byte dir1,byte pulse1,byte dir2,byte pulse2,byte dir3, byte pluse3)
        {
            outputBuffer[0] = 0;
            outputBuffer[1] = code;
            outputBuffer[2] = dir1;
            outputBuffer[3] = pulse1;
            outputBuffer[4] = dir2;
            outputBuffer[5] = pulse2;
                    
            writeRawReportToDevice(outputBuffer);
            // Only proceed if the write was successful-Unless, it will be unattached
            // quy dinh frame truyen-
            Byte[] MachineState = new Byte[1];
            MachineState[0] = inputBuffer[0];
            if (MachineState[0] == 0x32) return true; // Done, can countinue
            else return false; // Busy    
            //return true; // Done, can countinue
         
        }
        public void Write_data(byte[] data)
        {
            bool flag_write = false;
            flag_write = !flag_write;
            if (flag_write)
                
            writeRawReportToDevice(data);
        
        }
        
        public bool EMERGENCYSTOP()
        {
            if (CheckResponse(0x40, 0x41) == true) return true; else return false;
        }

        public Byte[] ReadErrorLimitSwitch()
        {
            // Byte 0 must be set to 0
            outputBuffer[0] = 0;

            // Byte 1 must be set to our command
            outputBuffer[1] = 0x42;

            // Perform the write command
            bool success;
            success = writeRawReportToDevice(outputBuffer);

            // Only proceed if the write was successful
            if (success)
            {
                // Perform the read
                readSingleReportFromDevice(ref inputBuffer);
            }

            // Note the push button is active low, so we send false if the reponse was 1
            Byte[] readButtonState = new Byte[1];
            readButtonState[0] = inputBuffer[2];
            return readButtonState;
        }
    }
}

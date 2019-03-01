using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
//using System.Windows.Shapes;
using System.Windows.Threading;
using System.IO.Ports;
using System.Collections;
using STA.Settings;
// DLL using for Kinect Xbox 360
using Microsoft.Kinect;
using Kinect.Toolbox;
// DLL using represent Dialog 
using Microsoft.Win32;
//using System.Drawing;
//using System.Collections.ObjectModel;
using System.Collections.Generic;



namespace Kinect_Control_USBDevice
{
    using usbGenericHidCommunications;
    using Kinect.Toolbox.Record;
    using Kinect.Toolbox.Voice;


    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        #region USBdevice
        INIFile MyINIFile = new INIFile("Settings.ini");
        byte[] data = new byte[65];
        Int32 speed_Base = 0;
        float Base, Shoulder, Elbow, Pitch, Roll, Sum, Gripper;
        Int32 SetPoint = 0, Ki_int = 0, Kp_int = 0, Kd_int = 0;
        private usbDevice theUsbDevice;
        // Timer1 for Send and feedback data
        DispatcherTimer dispatcherTimer1 = new DispatcherTimer();
        //Timer2 to send Data from Kinect
        DispatcherTimer dispatcherTimer2 = new DispatcherTimer();

        DispatcherTimer Send_Gcode_Timer = new DispatcherTimer();

        DispatcherTimer Send_DataGrid_Timer = new DispatcherTimer();

        DispatcherTimer Inc_Pulse_Timer = new DispatcherTimer();

        DispatcherTimer Inc_Pulse_Fast_Timer = new DispatcherTimer();
        #endregion
        /*offset variable*/
        Int32 gBase = 0, gShoulder = 3200, gElbow = 0, gPitch = 1400, gRoll = 1400, gGripper = 1400, flag_teach=0;
        Int32[,] Datafloat_Manual = new Int32[1000, 7]; int Mcounter=0;
        Byte[] data_receive = new Byte[65];
        Int32 inc_pulse;
        bool flag_click = false;
        #region InverseKinematic
        double pi = 3.1415f;
        double d1 = 148, a2 = 270, a3 = 162, d5 = 130;
        double x, y, z, sum, roll, gripper;
        double theta234_r, theta1_r, theta2_r_p, theta2_r_n, theta3_r, theta4_r_n, theta4_r_p;
        double theta1_d, theta2_d_n, theta2_d_p, theta3_d, theta3_d_n, theta3_d_p, theta4_d_n, theta4_d_p, theta234_d;
        double X1, X2, X3, Y1, Y2, Y3;
        double cos_theta3, theta3_r_p, theta3_r_n, sin_theta3_p, sin_theta3_n;
        Int32 basecurrent, basesetpoint, shouldercurrent, shouldersetpoint, elbowcurrent, elbowsetpoint, dpitch, droll, dgripper;
        #endregion
        bool c;
        bool flag_home_true;
        Int32 index;
        Int32 grid_index;
        // configure parameter kinect 
        #region speed_setting
        double Speed_Base=2,Speed_Shoulder,Speed_Elbow;
        #endregion

        #region Kinect
        KinectSensor kinectSensor;
        SwipeGestureDetector swipeGestureRecognizer;
        TemplatedGestureDetector circleGestureRecognizer;
        readonly ColorStreamManager colorManager = new ColorStreamManager();
        readonly DepthStreamManager depthManager = new DepthStreamManager();
        SkeletonAnalyzer _LeftArmSkeletonAnalyzer = new SkeletonAnalyzer();
        SkeletonAnalyzer _RightArmSkeletonAnalyzer = new SkeletonAnalyzer();
        SkeletonAnalyzer _RightGripSkeletonAnalyzer = new SkeletonAnalyzer();
        SkeletonAnalyzer _RightHipSkeletonAnalyzer = new SkeletonAnalyzer();
        SkeletonAnalyzer _BaseAngle = new SkeletonAnalyzer();
        SkeletonAnalyzer _ShoulderAngle = new SkeletonAnalyzer();
        SkeletonAnalyzer _GripperAngle = new SkeletonAnalyzer();
        OpenFileDialog openFileDialog1 = new OpenFileDialog();
        public static string strfilename;
        AudioStreamManager audioManager;
        SkeletonDisplayManager skeletonDisplayManager;
        readonly ContextTracker contextTracker = new ContextTracker();
        readonly AlgorithmicPostureDetector algorithmicPostureRecognizer = new AlgorithmicPostureDetector();
        TemplatedPostureDetector templatePostureDetector;
        private bool recordNextFrameForPosture;
        bool displayDepth;
        string circleKBPath;
        string letterT_KBPath;
        KinectRecorder recorder;
        KinectReplay replay;
        BindableNUICamera nuiCamera;
        private Skeleton[] skeletons;
        VoiceCommander voiceCommander;

        #endregion
        #region variable
        const byte Home = 0x04;
        const byte Emegency = 0x07;
        const byte SendData = 0x05;
        /// <summary>
        /// /////////////////////
        /// </summary>
        const byte Home_Res = 0x03;
        const byte Busy_Res = 0x04;
        const byte Success_Res = 0x05;
        const byte Stop_Res = 0x06;
        const byte Test_Manual= 0x07;
        const byte Test_Auto=0x21;

        #endregion
        float leftArmAngle = 0, rightArmAngle = 0, rightShoulderangle = 0, rightGripangle = 0, Baseangle = 0;
        Graph graph = new Graph();

        // Send Data to control angle      
        private void dispatcherTimer2_Tick(object sender, EventArgs e)
        {
            if (theUsbDevice.isDeviceAttached)
            {

                Send_Angles();
            }

        }

        #region debug_information
        private void dispatcherTimer1_Tick(object sender, EventArgs e)
        {
            String debugString;
            // Only collect debug if the device is attached
            if (theUsbDevice.isDeviceAttached)
            {
                data_receive = theUsbDevice.CollectData();
                switch (data_receive[0])
                {
                    case Home_Res:

                        debugString = "Home Success\n";
                        break;


                    case Busy_Res:

                        debugString = "Busy\n";
                        break;


                    case Success_Res:

                        debugString = "Success\n";
                        break;


                    case Stop_Res:

                        debugString = "Stop\n";
                        break;
                    case Test_Manual:
                        debugString = "Manual Running\n";
                        break;
                    case Test_Auto:
                        debugString = "Auto Running\n";
                        break;
                    default:

                        debugString = "Already\n";
                        break;


                }
                
                // Display the debug information
                if (debugString != String.Empty)
                {
                    var oldFocusedElement = FocusManager.GetFocusedElement(this);
                    this.debugTextbox.AppendText(debugString);
                    this.debugTextbox.CaretIndex = debugTextbox.Text.Length;
                    this.debugTextbox.ScrollToEnd();
                }
                basecurrent = Convert.ToInt32((data_receive[1] << 24) | (data_receive[2] << 16) | (data_receive[3] << 8) | data_receive[4]);
                basesetpoint = Convert.ToInt32((data_receive[5] << 24) | (data_receive[6] << 16) | (data_receive[7] << 8) | data_receive[8]);
                shouldercurrent=Convert.ToInt32((data_receive[9] << 24) | (data_receive[10] << 16) | (data_receive[11] << 8) | data_receive[12]);
                shouldersetpoint=Convert.ToInt32((data_receive[13] << 24) | (data_receive[14] << 16) | (data_receive[15] << 8) | data_receive[16]);
                elbowcurrent  = Convert.ToInt32((data_receive[17] << 24) | (data_receive[18] << 16) | (data_receive[19] << 8) | data_receive[20]);
                elbowsetpoint = Convert.ToInt32((data_receive[21] << 24) | (data_receive[22] << 16) | (data_receive[23] << 8) | data_receive[24]);
                dpitch=Convert.ToInt32((data_receive[25] << 24) | (data_receive[26] << 16) | (data_receive[27] << 8) | data_receive[28]);
                droll=Convert.ToInt32((data_receive[29] << 24) | (data_receive[30] << 16) | (data_receive[31] << 8) | data_receive[32]);
                dgripper = Convert.ToInt32((data_receive[33] << 24) | (data_receive[34] << 16) | (data_receive[35] << 8) | data_receive[36]);
                graph.txt_Base_curent.Text = basecurrent.ToString();
                graph.txt_base_setpoint.Text = basesetpoint.ToString();
                graph.txt_Shoulder_curent.Text = shouldercurrent.ToString();
                graph.txt_Shoulder_setpoint.Text = shouldersetpoint.ToString();
                graph.txt_Elbow_Curent.Text = elbowcurrent.ToString();
                graph.txt_Elbow_Setpoint.Text = elbowsetpoint.ToString();
                txt_pulse_base.Text = basecurrent.ToString();
                txt_pulse_shoulder.Text = shouldercurrent.ToString();
                txt_Pulse_Elbow.Text = elbowcurrent.ToString();
                txt_pulse_pitch.Text = dpitch.ToString();
                txt_pulse_roll.Text = droll.ToString();
                txt_pulse_gripper.Text = dgripper.ToString();
            }
            else
            {
                // Clear the debug window
                this.debugTextbox.Clear();
                  debugTextbox.CaretIndex = debugTextbox.Text.Length;
                debugTextbox.ScrollToEnd();

            }

        }
        #endregion

        public MainWindow()
        {
            InitializeComponent();
                       
            // Instantiate two instances of the SkeletonAnalyzer, once for each arm where a calculation
            dispatcherTimer2.Tick += new EventHandler(dispatcherTimer2_Tick);
            dispatcherTimer2.Interval = new TimeSpan(0, 0, 0, 2, 500);
            Send_Gcode_Timer.Tick += new EventHandler(Send_Gcode_Timer_Tick);
            Send_Gcode_Timer.Interval = new TimeSpan(0, 0, 0, 2,500);
            Send_DataGrid_Timer.Tick += new EventHandler(Send_DataGrid_Timer_Tick);
            Send_DataGrid_Timer.Interval = new TimeSpan(0, 0, 0, 2, 500);
            Inc_Pulse_Timer.Tick += new EventHandler(Inc_Pulse_Timer_Tick);
            Inc_Pulse_Timer.Interval = new TimeSpan(0, 0, 0, 0, 100);
            Inc_Pulse_Fast_Timer.Tick += new EventHandler(Inc_Pulse_Fast_Timer_Tick);
            Inc_Pulse_Fast_Timer.Interval = new TimeSpan(0, 0, 0, 0, 300);
            #region initializevector
            _LeftArmSkeletonAnalyzer.ReverseCoordinates = true;
            _RightArmSkeletonAnalyzer.ReverseCoordinates = true;
            _RightGripSkeletonAnalyzer.ReverseCoordinates = true;
            _RightHipSkeletonAnalyzer.ReverseCoordinates = true;
            _LeftArmSkeletonAnalyzer.SetBodySegments(JointType.WristLeft, JointType.ElbowLeft, JointType.ShoulderLeft);
            _RightArmSkeletonAnalyzer.SetBodySegments(JointType.ShoulderRight, JointType.ElbowRight, JointType.WristRight);
            _RightGripSkeletonAnalyzer.SetBodySegments(JointType.ElbowRight, JointType.WristRight, JointType.HandRight);
            _RightHipSkeletonAnalyzer.SetBodySegments(JointType.ElbowRight, JointType.ShoulderRight, JointType.HipRight);
            _ShoulderAngle.SetBodySegments_4points(JointType.ShoulderCenter, JointType.Spine, JointType.ShoulderRight, JointType.ElbowRight);
            _GripperAngle.SetBodySegments_4points(JointType.ShoulderCenter, JointType.Spine, JointType.WristRight, JointType.HandRight);
            _BaseAngle.SetBodySegments_4points(JointType.ShoulderLeft, JointType.ElbowLeft, JointType.ShoulderCenter, JointType.Spine);
            #endregion
            
        }
        #region Kinect
        void Kinects_StatusChanged(object sender, StatusChangedEventArgs e)
        {
            switch (e.Status)
            {
                case KinectStatus.Connected:
                    if (kinectSensor == null)
                    {
                        kinectSensor = e.Sensor;
                        Initialize();
                    }
                    break;
                case KinectStatus.Disconnected:
                    if (kinectSensor == e.Sensor)
                    {
                        Clean();
                        MessageBox.Show("Kinect was disconnected");
                    }
                    break;
                case KinectStatus.NotReady:
                    break;
                case KinectStatus.NotPowered:
                    if (kinectSensor == e.Sensor)
                    {
                        Clean();
                        MessageBox.Show("Kinect is no more powered");
                    }
                    break;
                default:
                    MessageBox.Show("Unhandled Status: " + e.Status);
                    break;
            }
        }
        private void Initialize()
        {
            if (kinectSensor == null)
                return;

            audioManager = new AudioStreamManager(kinectSensor.AudioSource);
            audioBeamAngle.DataContext = audioManager;

            kinectSensor.ColorStream.Enable(ColorImageFormat.RgbResolution640x480Fps30);
            kinectSensor.ColorFrameReady += kinectRuntime_ColorFrameReady;

            kinectSensor.DepthStream.Enable(DepthImageFormat.Resolution320x240Fps30);
            kinectSensor.DepthFrameReady += kinectSensor_DepthFrameReady;

            kinectSensor.SkeletonStream.Enable(new TransformSmoothParameters
            {
                Smoothing = 0.5f,
                Correction = 0.5f,
                Prediction = 0.5f,
                JitterRadius = 0.05f,
                MaxDeviationRadius = 0.04f
            });



            /*
            float leftArmAngle = (float)_LeftArmSkeletonAnalyzer.GetBodySegmentAngle(Joints);
            angleLeftForeArmUpperArm.Text = leftArmAngle.ToString();
            float rightArmAngle = (float)_RightArmSkeletonAnalyzer.GetBodySegmentAngle(Joints);
            angleRightForeArmUpperArm.Text = rightArmAngle.ToString();
             */



            kinectSensor.SkeletonFrameReady += kinectRuntime_SkeletonFrameReady;

            swipeGestureRecognizer = new SwipeGestureDetector();
            swipeGestureRecognizer.OnGestureDetected += OnGestureDetected;

            skeletonDisplayManager = new SkeletonDisplayManager(kinectSensor, kinectCanvas);

            kinectSensor.Start();

            LoadCircleGestureDetector();
            LoadLetterTPostureDetector();

            nuiCamera = new BindableNUICamera(kinectSensor);

            //elevationSlider.DataContext = nuiCamera;

            voiceCommander = new VoiceCommander("record", "stop");
            // Use when create Audio Checkbox
            //voiceCommander.OrderDetected += voiceCommander_OrderDetected;

            StartVoiceCommander();
            // Display Kinect Data to WPF
            kinectDisplay.DataContext = colorManager;
        }
        void kinectSensor_DepthFrameReady(object sender, DepthImageFrameReadyEventArgs e)
        {
            if (replay != null && !replay.IsFinished)
                return;

            using (var frame = e.OpenDepthImageFrame())
            {
                if (frame == null)
                    return;

                if (recorder != null && ((recorder.Options & KinectRecordOptions.Depth) != 0))
                {
                    recorder.Record(frame);
                }

                if (!displayDepth)
                    return;

                depthManager.Update(frame);
            }
        }

        void kinectRuntime_ColorFrameReady(object sender, ColorImageFrameReadyEventArgs e)
        {
            if (replay != null && !replay.IsFinished)
                return;

            using (var frame = e.OpenColorImageFrame())
            {
                if (frame == null)
                    return;

                if (recorder != null && ((recorder.Options & KinectRecordOptions.Color) != 0))
                {
                    recorder.Record(frame);
                }

                if (displayDepth)
                    return;

                colorManager.Update(frame);
            }
        }

        void kinectRuntime_SkeletonFrameReady(object sender, SkeletonFrameReadyEventArgs e)
        {

            if (replay != null && !replay.IsFinished)
                return;

            using (SkeletonFrame frame = e.OpenSkeletonFrame())
            {
                if (frame == null)
                    return;

                if (recorder != null && ((recorder.Options & KinectRecordOptions.Skeletons) != 0))
                    recorder.Record(frame);

                frame.GetSkeletons(ref skeletons);

                if (skeletons.All(s => s.TrackingState == SkeletonTrackingState.NotTracked))
                    return;

                ProcessFrame(frame);
            }
        }

        void ProcessFrame(ReplaySkeletonFrame frame)
        {
            Dictionary<int, string> stabilities = new Dictionary<int, string>();
            foreach (var skeleton in frame.Skeletons)
            {
                if (skeleton.TrackingState != SkeletonTrackingState.Tracked)
                    continue;

                //if (eyeTracker == null)
                //    eyeTracker = new EyeTracker(kinectSensor);

                //eyeTracker.Track(skeleton);



                //if (eyeTracker.IsLookingToSensor.HasValue && eyeTracker.IsLookingToSensor == false)
                //    continue;

                foreach (Joint joint in skeleton.Joints)
                {
                    //Calculate Angle :)
                    leftArmAngle = (float)_LeftArmSkeletonAnalyzer.GetBodySegmentAngle(skeleton.Joints);
                   // left_hipangle.Text = leftArmAngle.ToString();
                   // right_elbowangle.Text = leftArmAngle.ToString();
                    rightArmAngle = (float)_RightArmSkeletonAnalyzer.GetBodySegmentAngle(skeleton.Joints);
                    // Elbow angle
                    right_elbowangle.Text = rightArmAngle.ToString();
                    /* */
                    //rightHipangle = (float)_RightHipSkeletonAnalyzer.GetBodySegmentAngle(skeleton.Joints);
                    rightShoulderangle = (float)_ShoulderAngle.GetBodySegmentAngle_4points(skeleton.Joints);
                    // Shoulder angle
                    right_hipangle.Text = rightShoulderangle.ToString();
                    /* */
                    rightGripangle = (float)_RightGripSkeletonAnalyzer.GetBodySegmentAngle(skeleton.Joints);
                    // grip angle
                    // right_gripangle.Text = rightGripangle.ToString();
                    //
                    Baseangle = (float)_BaseAngle.GetBodySegmentAngle_4points(skeleton.Joints);
                    right_gripangle.Text = Baseangle.ToString();
                    if (joint.TrackingState != JointTrackingState.Tracked)
                        continue;

                    if (joint.JointType == JointType.HandRight)
                    {
                        circleGestureRecognizer.Add(joint.Position, kinectSensor);
                    }
                    else if (joint.JointType == JointType.HandLeft)
                    {
                        swipeGestureRecognizer.Add(joint.Position, kinectSensor);
                        if (controlMouse.IsChecked == true)
                            MouseController.Current.SetHandPosition(kinectSensor, joint, skeleton);
                    }


                }

                algorithmicPostureRecognizer.TrackPostures(skeleton);
                templatePostureDetector.TrackPostures(skeleton);

                if (recordNextFrameForPosture)
                {
                    templatePostureDetector.AddTemplate(skeleton);
                    recordNextFrameForPosture = false;
                }
            }

            skeletonDisplayManager.Draw(frame.Skeletons, seatedMode.IsChecked == true);


            stabilitiesList.ItemsSource = stabilities;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            Clean();
        }

        private void Clean()
        {
            if (swipeGestureRecognizer != null)
            {
                swipeGestureRecognizer.OnGestureDetected -= OnGestureDetected;
            }

            if (audioManager != null)
            {
                audioManager.Dispose();
                audioManager = null;
            }

            /*
            if (parallelCombinedGestureDetector != null)
            {
                parallelCombinedGestureDetector.Remove(swipeGestureRecognizer);
                parallelCombinedGestureDetector.Remove(circleGestureRecognizer);
                parallelCombinedGestureDetector = null;
            }
            */

            CloseGestureDetector();

            ClosePostureDetector();


            if (voiceCommander != null)
            {
                // Use when create Audio checkbox
                //voiceCommander.OrderDetected -= voiceCommander_OrderDetected;
                voiceCommander.Stop();
                voiceCommander = null;
            }

            if (recorder != null)
            {
                recorder.Stop();
                recorder = null;
            }

            /*
             if (eyeTracker != null)
             {
                 eyeTracker.Dispose();
                 eyeTracker = null;
             }
             */

            if (kinectSensor != null)
            {
                kinectSensor.DepthFrameReady -= kinectSensor_DepthFrameReady;
                kinectSensor.SkeletonFrameReady -= kinectRuntime_SkeletonFrameReady;
                kinectSensor.ColorFrameReady -= kinectRuntime_ColorFrameReady;
                kinectSensor.Stop();
                kinectSensor = null;
            }
        }
        void replay_DepthImageFrameReady(object sender, ReplayDepthImageFrameReadyEventArgs e)
        {
            if (!displayDepth)
                return;

            depthManager.Update(e.DepthImageFrame);
        }

        void replay_ColorImageFrameReady(object sender, ReplayColorImageFrameReadyEventArgs e)
        {
            if (displayDepth)
                return;

            colorManager.Update(e.ColorImageFrame);
        }

        void replay_SkeletonFrameReady(object sender, ReplaySkeletonFrameReadyEventArgs e)
        {
            ProcessFrame(e.SkeletonFrame);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            displayDepth = !displayDepth;

            if (displayDepth)
            {
                viewButton.Content = "View Color";
                kinectDisplay.DataContext = depthManager;
            }
            else
            {
                viewButton.Content = "View Depth";
                kinectDisplay.DataContext = colorManager;
            }
        }

        #endregion
        #region USBEvent
        private void usbEvent_receiver(object o, EventArgs e)
        {

            this.toolStripStatusLabelMachine.Content = "Machine is not ready!";

            // Check the status of the USB device and update the form accordingly
            if (theUsbDevice.isDeviceAttached) // Device is currently attached
            {
                // Update the status label
                this.toolStripStatusLabelUSB.Content = "USB Device is attached";
            }
            else // Device is currently unattached
            {
                // Update the status label
                this.toolStripStatusLabelUSB.Content = "USB Device is detached";

            }
        }
        #endregion
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // Create the USB reference device object (passing VID and PID)
            // usbdesc.c is define that variables
            theUsbDevice = new usbDevice(0x0483, 0x5710);
            // Add a listener for usb events
            theUsbDevice.usbEvent += new usbDevice.usbEventsHandler(usbEvent_receiver);

            // Perform an initial search for the target device
            theUsbDevice.findTargetDevice();
            dispatcherTimer1.Tick += new EventHandler(dispatcherTimer1_Tick);
            dispatcherTimer1.Interval = new TimeSpan(0, 0, 0, 0, 250);
            //dispatcherTimer1.Start();




            #region Kinect
            circleKBPath = Path.Combine(Environment.CurrentDirectory, @"data\circleKB.save");
            letterT_KBPath = Path.Combine(Environment.CurrentDirectory, @"data\t_KB.save");
            try
            {
                //listen to any status change for Kinects
                KinectSensor.KinectSensors.StatusChanged += Kinects_StatusChanged;

                //loop through all the Kinects attached to this PC, and start the first that is connected without an error.
                foreach (KinectSensor kinect in KinectSensor.KinectSensors)
                {
                    if (kinect.Status == KinectStatus.Connected)
                    {
                        kinectSensor = kinect;
                        break;
                    }
                }

                if (KinectSensor.KinectSensors.Count == 0)
                    MessageBox.Show("No Kinect found");
                else
                    Initialize();

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            #endregion

        }
        bool flag_send;
        private void Pb_Send_Click(object sender, RoutedEventArgs e)
        {
            flag_send = true;
            if (flag_send)
            { Send_Factor_To_Chip(); }

        }
        private void Pb_SendXYZ_Click(object sender, RoutedEventArgs e)
        {
            InverseKinematic();
        }

        void InverseKinematic()
        {
            int i;
            double vBase, vShoulder, vElbow, vPitch, vRoll, vSum, vGripper;
            char[] cutchar = { ' ', ',', '.', ':', '\t' };
            char[] cutletter = { 'X', 'Y', 'Z', 'S', 'R', 'G' };
            string value, s, s1;
            string[] store;
            if (String.IsNullOrEmpty(Sendcoordinates.Text))
            {
                MessageBox.Show("Please fill in textbox");
            }
            else
            {
                String text = Func(Sendcoordinates.Text);
                string[] words = text.Split(cutchar);

                value = String.Concat(words);
                store = value.Split(cutletter);
                x = Convert.ToDouble(store[1]);
                y = Convert.ToDouble(store[2]);
                z = Convert.ToDouble(store[3]);
                sum = Convert.ToDouble(store[4]);
                roll = Convert.ToDouble(store[5]);
                gripper = Convert.ToDouble(store[6]);

                /* Convert to radian */
                theta234_r = Degrees_to_Radians(sum);
                theta1_r = Math.Atan2(x, y);
                /* Convert to degree */
                theta1_d = Radians_to_Degrees(theta1_r);
                X1 = Math.Sqrt((x * x) + (y * y));
                Y1 = z;
                X2 = X1 - d5 * Math.Cos(theta234_r);
                Y2 = Y1 - d5 * Math.Sin(theta234_r);
                X3 = X2;
                Y3 = Y2 - d1;
                if ((X3 * X3 + Y3 * Y3 - a2 * a2 - a3 * a3) > 2 * a2 * a3)
                {
                    MessageBox.Show("Out of range-Refill Again");
                }
                else
                {
                    cos_theta3 = ((X3 * X3 + Y3 * Y3 - a2 * a2 - a3 * a3) / (2 * a2 * a3));
                    sin_theta3_n = (double)((-1) * Math.Sqrt(1 - cos_theta3 * cos_theta3));
                    theta3_r_n = Math.Atan2(sin_theta3_n, cos_theta3);
                    theta3_d_n = Radians_to_Degrees(theta3_r_n);
                    theta2_r_p = +Math.Atan2(Y3, X3) + Math.Atan2(a3 * Math.Sin(-theta3_r_n), a2 + a3 * Math.Cos(-theta3_r_n));
                    theta2_d_p = Radians_to_Degrees(theta2_r_p);
                    theta4_d_n = sum - theta2_d_p - theta3_d_n;
                    s = Convert.ToString(theta1_d) + " " + Convert.ToString(theta2_d_p) + " " + Convert.ToString(theta3_d_n) + " " + Convert.ToString(theta4_d_n);
                    //  GcodeTextbox.Text = s;
                    AddColouredText(s);
                    vBase = (theta1_d / 90) * 5100;
                    vShoulder = 1750 + theta2_d_p / 90 * 1525;
                    vElbow = (95 + theta3_d_n) / 90 * 19150;
                    vPitch = (-theta4_d_n * 11.2 + 1400);
                    vSum = sum * 11.2 + 1400;
                    vRoll = roll * 11.2 + 1400;
                    if (gripper == 0) { vGripper = 600; }
                    else { vGripper = 1500; }
                    testing.Text = Convert.ToString(vShoulder);
                    if (vShoulder >= 3200 | vShoulder <= 1750 | vBase <= 0 | vBase >= 14000 | vElbow >= 38300)
                    {
                        MessageBox.Show("Out of range-Refill Again");
                    }
                    else
                    {
                        s1 = Convert.ToString(vBase) + " " + Convert.ToString(vShoulder) + " " + Convert.ToString(vElbow) + " " + Convert.ToString(vPitch);
                        testing.Text = s1;
                        byte[] code = BitConverter.GetBytes(0x05);
                        byte[] Base_Array = BitConverter.GetBytes(Convert.ToInt32(vBase));
                        byte[] Shoulder_Array = BitConverter.GetBytes(Convert.ToInt32(vShoulder));
                        byte[] Elbow_Array = BitConverter.GetBytes(Convert.ToInt32(vElbow));
                        byte[] Pitch_Array = BitConverter.GetBytes(Convert.ToInt32(vPitch));
                        byte[] Roll_Array = BitConverter.GetBytes(Convert.ToInt32(vRoll));
                        byte[] Gripper_Array = BitConverter.GetBytes(Convert.ToInt32(vGripper));
                        byte[] buffer = new byte[32];
                        Array.Copy(code, 0, buffer, 0, 1);
                        Array.Copy(Base_Array, 0, buffer, 1, 4);
                        Array.Copy(Shoulder_Array, 0, buffer, 5, 4);
                        Array.Copy(Elbow_Array, 0, buffer, 9, 4);
                        Array.Copy(Pitch_Array, 0, buffer, 13, 4);
                        Array.Copy(Roll_Array, 0, buffer, 17, 4);
                        Array.Copy(Gripper_Array, 0, buffer, 21, 4);

                        if (theUsbDevice.isDeviceAttached)
                        {


                            if (buffer.Length < 65)
                            {
                                for (i = 0; i < buffer.Length; i++)
                                {
                                    data[i + 1] = buffer[i];
                                }
                                for (int j = buffer.Length + 1; j < data.Length; j++)
                                {
                                    data[j] = 0;
                                }
                                theUsbDevice.Write_data(data);
                            }
                            else
                                MessageBox.Show("over lenght");

                        }
                    }
                }
            }
        }


        private double Degrees_to_Radians(double Deg)
        {
            double Radian;
            Radian = (double)(Deg * 0.0174533);
            return Radian;
        }
        private double Radians_to_Degrees(double Rad)
        {
            double Deg;
            Deg = (float)(Rad * 57.2958);
            return Deg;
        }
        private string Func(String Strs)
        {
            Char[] chars = Strs.ToCharArray();
            String str = "";
            foreach (var c in chars)
            {
                if (Convert.ToInt32(c) >= 'a' && Convert.ToInt32(c) <= 'z')
                {
                    str += c.ToString().ToUpper();

                }
                else
                {
                    str += c.ToString();
                }

            }
            return str;
        }

        void Send_Factor_To_Chip()
        {

            int i;
            string sBase, sShoulder, sElbow, sPitch, sRoll, sGripper;
            string[] store;

            char[] cutchar = { ' ', ',', '.', ':', '\t' };
            char[] cutletter = { 'B', 'S', 'E', 'P', 'R', 'G' };

            string value;
            if (String.IsNullOrEmpty(Sendmanual.Text))
            {
                MessageBox.Show("Please fill in textbox");

            }
            else
            {
                String text = Func(Sendmanual.Text);
                string[] words = text.Split(cutchar);

                value = String.Concat(words);
                store = value.Split(cutletter);
                sBase = store[1]; sShoulder = store[2]; sElbow = store[3];
                sPitch = store[4]; sRoll = store[5]; sGripper = store[6];
                Base = Convert.ToSingle(sBase);
                Shoulder = Convert.ToSingle(sShoulder);
                Elbow = Convert.ToSingle(sElbow);
                Pitch = Convert.ToSingle(sPitch);
                Roll = Convert.ToSingle(sRoll);
                Gripper = Convert.ToSingle(sGripper);
                byte[] code = BitConverter.GetBytes(0x05);
                byte[] Base_Array = BitConverter.GetBytes(Convert.ToInt32(Base));
                byte[] Shoulder_Array = BitConverter.GetBytes(Convert.ToInt32(Shoulder));
                byte[] Elbow_Array = BitConverter.GetBytes(Convert.ToInt32(Elbow));
                byte[] Pitch_Array = BitConverter.GetBytes(Convert.ToInt32(Pitch));
                byte[] Roll_Array = BitConverter.GetBytes(Convert.ToInt32(Roll));
                byte[] Sum_Array = BitConverter.GetBytes(Convert.ToInt32(Gripper));
                byte[] buffer = new byte[32];
                Array.Copy(code, 0, buffer, 0, 1);
                Array.Copy(Base_Array, 0, buffer, 1, 4);
                Array.Copy(Shoulder_Array, 0, buffer, 5, 4);
                Array.Copy(Elbow_Array, 0, buffer, 9, 4);
                Array.Copy(Pitch_Array, 0, buffer, 13, 4);
                Array.Copy(Roll_Array, 0, buffer, 17, 4);
                Array.Copy(Sum_Array, 0, buffer, 21, 4);

                if (theUsbDevice.isDeviceAttached)
                {


                    if (buffer.Length < 65)
                    {
                        for (i = 0; i < buffer.Length; i++)
                        {
                            data[i + 1] = buffer[i];
                        }
                        for (int j = buffer.Length + 1; j < data.Length; j++)
                        {
                            data[j] = 0;
                        }
                        theUsbDevice.Write_data(data);
                    }
                    else
                        MessageBox.Show("over lenght");

                }

            }



        }
        void Send_Angles()
        {

            rightArmAngle = 180 - rightArmAngle;
            leftArmAngle = 180 - leftArmAngle;
            //  rightHipangle = rightHipangle - 20;
            if (Baseangle < 0) { Baseangle = 0; }
            else if (Baseangle > 100) { Baseangle = 100; }
            if (rightArmAngle > 180) { rightArmAngle = 180; }
            else if (rightArmAngle < 0) { rightArmAngle = 0; }
            if (leftArmAngle > 180) { leftArmAngle = 180; }
            else if (leftArmAngle < 0) { leftArmAngle = 0; }
            if (rightGripangle > 270) { rightGripangle = 270; }
            else if (rightGripangle < 90) { rightGripangle = 90; }
            if (rightShoulderangle < 0) { rightShoulderangle = 0; }
            else if (rightShoulderangle > 90) { rightShoulderangle = 90; }
            Int32 angle1, angle2, angle3 = 0, angle4 = 0, angle5 = 0, angle6 = 0;
            float rsrightArmAngle = 0, rsrightShoulderangle = 0, rsPitch = 0, rsBaseangle = 0,rsleftArmAngle;
            rsBaseangle = Baseangle / 180 * 10400;
            if (rsBaseangle > 10400)
            { rsBaseangle = 10400; }
            if (leftArmAngle > 45) { rsleftArmAngle = 600; }
            else { rsleftArmAngle = 1500; }
            
            rsrightShoulderangle = 1750 + rightShoulderangle / 90 * 1300;
            if (rsrightShoulderangle <= 1750) { rsrightShoulderangle=1750;}
            if (rsrightShoulderangle >= 3100) { rsrightShoulderangle = 3100; }
            rsrightArmAngle = (rightArmAngle / 90 * 19250);
            if (rsrightArmAngle > 23000) { rsrightArmAngle = 23000; }
            rightGripangle = rightGripangle - 180;
            rsPitch = 1400 + (rightGripangle * 11);
            if (rsPitch > 2200) { rsPitch = 2200; }
            angle1 = Convert.ToInt32(rsBaseangle);
            angle2 = Convert.ToInt32(rsrightShoulderangle);
            angle3 = Convert.ToInt32(rsrightArmAngle);
            angle4 = Convert.ToInt32(rsPitch);
            angle5 = Convert.ToInt32(90);
            angle6 = Convert.ToInt32(rsleftArmAngle);
            byte[] code = BitConverter.GetBytes(0x06);
            byte[] byteangle1 = BitConverter.GetBytes(angle1);
            byte[] byteangle2 = BitConverter.GetBytes(angle2);
            byte[] byteangle3 = BitConverter.GetBytes(angle3);
            byte[] byteangle4 = BitConverter.GetBytes(angle4);
            byte[] byteangle5 = BitConverter.GetBytes(angle5);
            byte[] byteangle6 = BitConverter.GetBytes(angle6);
            byte[] buffer = new byte[32];
            Array.Copy(code, 0, buffer, 0, 1);

            Array.Copy(byteangle1, 0, buffer, 1, 4);
            Array.Copy(byteangle2, 0, buffer, 5, 4);
            Array.Copy(byteangle3, 0, buffer, 9, 4);
            Array.Copy(byteangle4, 0, buffer, 13, 4);
            Array.Copy(byteangle5, 0, buffer, 17, 4);
            Array.Copy(byteangle6, 0, buffer, 21, 4);
            if (theUsbDevice.isDeviceAttached)
            {

                if (buffer.Length < 65)
                {
                    for (int i = 0; i < buffer.Length; i++)
                    {
                        data[i + 1] = buffer[i];
                    }
                    for (int j = buffer.Length + 1; j < data.Length; j++)
                    {
                        data[j] = 0;
                    }
                    

                    theUsbDevice.Write_data(data);
                }
                else
                    MessageBox.Show("over lenght");
            }


        }
        private void StartButton(object sender, RoutedEventArgs e)
        {
            String debugString;

            // Only collect debug if the device is attached
            if (theUsbDevice.isDeviceAttached)
            {
                this.toolStripStatusLabelMachine.Content = "Machine is ready!";

                // Collect the debug information from the device
                debugString = theUsbDevice.StartResponse();
                //debugString = "minhmap";
                // Display the debug information
                if (debugString != String.Empty)
                {
                    var oldFocusedElement = FocusManager.GetFocusedElement(this);
                    this.debugTextbox.AppendText(debugString);
                    this.debugTextbox.Focus();
                    this.debugTextbox.CaretIndex = debugTextbox.Text.Length;
                    this.debugTextbox.ScrollToEnd();
                }
            }
            else
            {
                // Clear the debug window
                this.debugTextbox.Clear();
                debugTextbox.Focus();
                debugTextbox.CaretIndex = debugTextbox.Text.Length;
                debugTextbox.ScrollToEnd();
                
            }

        }
        // Ini file
        private void loadINI(object sender, EventArgs e)
        {
            // Creating INIFile object for \"Settings.ini\"...
            INIFile MyINIFile = new INIFile("Settings.ini");

            // Load parameter
            Kp_int = MyINIFile.GetValue("Variables", "Kp", 0);
            Ki_int = MyINIFile.GetValue("Variables", "Ki", 0);
            Kd_int = MyINIFile.GetValue("Variables", "Kd", 0);
            SetPoint = MyINIFile.GetValue("Speed", "Speed_Shoulder", 0);


        }
        private void txtSetpoint_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }


        private void elevationSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {

        }

        private void stabilitiesList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
        #region Kinect
        private void seatedMode_Checked_1(object sender, RoutedEventArgs e)
        {
            if (kinectSensor == null)
                return;

            kinectSensor.SkeletonStream.TrackingMode = SkeletonTrackingMode.Seated;
        }

        private void seatedMode_Unchecked_1(object sender, RoutedEventArgs e)
        {
            if (kinectSensor == null)
                return;

            kinectSensor.SkeletonStream.TrackingMode = SkeletonTrackingMode.Default;
        }
        #endregion

        private void txtKP_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void Pb_Auto_Manual_Click(object sender, RoutedEventArgs e)
        {

            
            ImageBrush image1 = new ImageBrush();
            ImageBrush image2 = new ImageBrush();

            image1.ImageSource = new BitmapImage(
                new Uri(@"C:\Users\PINK_RAIN\Dropbox\UPDATE THESIS\12-12kinect\12-12\kinect_control_usbdevice\RESOURCE\Hand_icon.png", UriKind.Relative)
                );
            image2.ImageSource = new BitmapImage(
                new Uri(@"C:\Users\PINK_RAIN\Dropbox\UPDATE THESIS\12-12kinect\12-12\kinect_control_usbdevice\RESOURCE\Reload_icon.jpg", UriKind.Relative)
                );
            
            flag_click = !flag_click;
            if (flag_click)
            {

                MessageBoxResult kq1 = MessageBox.Show("Are You Want to change to Manual", "TEAMQM", MessageBoxButton.YesNo, MessageBoxImage.Question);
                if (kq1 == MessageBoxResult.Yes)
                {
                    MessageBox.Show("Wating for Second", "TEAMQM");
                    Sendmanual.IsEnabled = true;
                    Sendcoordinates.IsEnabled = true;
                    Pb_Send.IsEnabled = true;
                    Pb_SendXYZ.IsEnabled = true;
                    Pb_Auto_Manual.Background = image1;
                    dispatcherTimer2.Stop();
                    right_hipangle.IsEnabled=false;
                    right_elbowangle.IsEnabled=false;
                    right_gripangle.IsEnabled = false;
                    Start_Gcode.IsEnabled = true;
                    Reload.IsEnabled = true;
                    Edit.IsEnabled = true;
                    Open.IsEnabled = true;
                    Stop_Gcode.IsEnabled = true;
                    richTextBoxGCode.IsEnabled = true;
                    ImportExcel.IsEnabled = true;
                    Drawing_Response.IsEnabled = true;
                    pb_Set_Pulse_grid.IsEnabled = true;
                    Clearbutton.IsEnabled = true;
                    Enable_All_Button_Pulse();
                }

            }
            else
            {
                MessageBoxResult kq2 = MessageBox.Show("Are You Want to change to Auto", "TEAMQM", MessageBoxButton.YesNo, MessageBoxImage.Question);
                if (kq2 == MessageBoxResult.Yes)
                {
                    MessageBox.Show("Wating for Second", "TEAMQM");
                    Pb_Auto_Manual.Background = image2;
                    Sendmanual.IsEnabled = false;
                    Sendcoordinates.IsEnabled = false;
                    Pb_Send.IsEnabled = false;
                    Pb_SendXYZ.IsEnabled = false;
                    right_hipangle.IsEnabled = true;
                    right_elbowangle.IsEnabled = true;
                    right_gripangle.IsEnabled = true;
                    Start_Gcode.IsEnabled = false;
                    Reload.IsEnabled = false;
                    Edit.IsEnabled = false;
                    Open.IsEnabled = false;
                    Stop_Gcode.IsEnabled = false;
                    richTextBoxGCode.IsEnabled = false;
                    ImportExcel.IsEnabled = false;
                    Drawing_Response.IsEnabled = false;
                    Disable_All_Button_Pulse();
                    pb_Set_Pulse_grid.IsEnabled = false;
                    Clearbutton.IsEnabled = false;
                    dispatcherTimer2.Start();
                }

            }

        }
        /* Gcode process */


        private void OpenMenuStrip_Click(object sender, RoutedEventArgs e)
        {
            Stream myStream;

            // Set filter options and filter index.
            openFileDialog1.Filter = "CNC files (*.CNC)|*.CNC|All files (*.*)|*.*";
            openFileDialog1.Multiselect = true;
            openFileDialog1.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            openFileDialog1.Title = "Please select an G-Code file!";
            openFileDialog1.FileName = "G-Code.CNC";
            if (openFileDialog1.ShowDialog() == true)// Test result.
            {
                if ((myStream = openFileDialog1.OpenFile()) != null)
                {
                    strfilename = openFileDialog1.FileName;

                    if (strfilename != null)
                        Title = "Control Scorobot [ " + strfilename + " ]";
                    else Title = "Control Scorobot";

                    File.Copy(strfilename, System.IO.Directory.GetCurrentDirectory() + "\\CNCCodes\\GCode_Test.CNC", true);

                    ReloadMenuStrip_Click(sender, e);
                }
                myStream.Close();
            }
        }

        public void ReloadMenuStrip_Click(object sender, RoutedEventArgs e)
        {
            //Font font = new Font("Times New Roman", 10);
            //  GcodeTextbox.Text = File.ReadAllText(System.IO.Directory.GetCurrentDirectory() + "\\CNCCodes\\GCode_Test.CNC");
            string filetext = File.ReadAllText(System.IO.Directory.GetCurrentDirectory() + "\\CNCCodes\\GCode_Test.CNC");
            AddColouredText(filetext);

            if (strfilename != null)
                Title = "Control Scorobot [ " + strfilename + " ]";
            else Title = "Control Scorobot";
            /*Doc Gcode dua Data vao Data array -chua viet*/
            readGcode(System.IO.Directory.GetCurrentDirectory() + "\\CNCCodes\\GCode_Test.CNC");
        }
        private void EditdMenuStrip_Click(object sender, RoutedEventArgs e)
        {
            System.Diagnostics.Process.Start(System.IO.Directory.GetCurrentDirectory() + "\\CNCCodes\\GCode_Test.CNC");
        }

        private void debugTextbox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {

            MessageBoxResult kq = MessageBox.Show("Bạn thực sự muốn thoát", "TEAMQM", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (kq == MessageBoxResult.Yes)
            {
                MessageBox.Show("Cảm ơn bạn đã sử dụng chương trình", "TEAMQM");
                Close();
            }
        }

        private void Emergency_Click(object sender, RoutedEventArgs e)
        {
            int i;
            byte[] BUFFER = new byte[32];
            BUFFER[0] = Emegency;
            Send_Gcode_Timer.Stop();
            dispatcherTimer2.Stop();
            Send_DataGrid_Timer.Stop();
            Inc_Pulse_Timer.Stop();
          
            if (BUFFER.Length < 65)
            {
                for (i = 0; i < BUFFER.Length; i++)
                {
                    data[i + 1] = BUFFER[i];
                }
                for (int j = BUFFER.Length + 1; j < data.Length; j++)
                {
                    data[j] = 0;
                }
                theUsbDevice.Write_data(data);
            }
            else
                MessageBox.Show("over lenght");
        }


        private void Pb_Home_Click(object sender, RoutedEventArgs e)
        {
            int i;
            byte[] buffer = new byte[32];
            buffer[0] = Home;
            if (buffer.Length < 65)
            {
                for (i = 0; i < buffer.Length; i++)
                {
                    data[i + 1] = buffer[i];
                }
                for (int j = buffer.Length + 1; j < data.Length; j++)
                {
                    data[j] = 0;
                }
                theUsbDevice.Write_data(data);
            }
            else
                MessageBox.Show("over lenght");


        }
        bool Debug_click;
        private void Pb_Debug_Click(object sender, RoutedEventArgs e)
        {
            Debug_click = !Debug_click;
            if (Debug_click)
            {
                dispatcherTimer1.Start();
            }
            else
            {

                dispatcherTimer1.Stop();
            }
        }

        private void Send_Gcode_Timer_Tick(object sender, EventArgs e)
        {
            SelectLine_PeterMap(index+3);
            Receive_Respose_And_Send_Gcode();
            SelectLine_Normal(index+2);
        }
      
       private void Send_DataGrid_Timer_Tick(object sender, EventArgs e)
        {
            Receive_Response_And_Send_Data_Grid();
          
        }
        private void Receive_Respose_And_Send_Gcode()
        {
            string debugString;
            debugString = theUsbDevice.collectDebug();
            byte[] buffer = new byte[32];
            byte code = (0x05);
            Int32 angle1, angle2, angle3, angle4, angle5, angle6;
            if (index >= (counter - 1))
            {
                index = counter - 1;
            }
            else
            {          //const byte Busy_Res = 0x04;
                       //const byte Success_Res = 0x05;
                if (data_receive[0] == Success_Res)
                {

                    index = index + 1;
                }
                else
                {
                    index = index + 0;
                }

            }
                 
            //test_debug.Text = debugString;
            angle1 = Convert.ToInt32(Datafloat_Gcode[index, 1]);
            angle2 = Convert.ToInt32(Datafloat_Gcode[index, 2]);
            angle3 = Convert.ToInt32(Datafloat_Gcode[index, 3]);
            angle4 = Convert.ToInt32(Datafloat_Gcode[index, 4]);
            angle5 = Convert.ToInt32(Datafloat_Gcode[index, 5]);
            angle6 = Convert.ToInt32(Datafloat_Gcode[index, 6]);
            byte[] Base = BitConverter.GetBytes(angle1);
            byte[] Shoulder = BitConverter.GetBytes(angle2);
            byte[] Elbow = BitConverter.GetBytes(angle3);
            byte[] Pitcth = BitConverter.GetBytes(angle4);
            byte[] Roll = BitConverter.GetBytes(angle5);
            byte[] Sum = BitConverter.GetBytes(angle6);

            buffer[0] = code;

            Array.Copy(Base, 0, buffer, 1, 4);
            Array.Copy(Shoulder, 0, buffer, 5, 4);
            Array.Copy(Elbow, 0, buffer, 9, 4);
            Array.Copy(Pitcth, 0, buffer, 13, 4);
            Array.Copy(Roll, 0, buffer, 17, 4);
            Array.Copy(Sum, 0, buffer, 21, 4);

            if (theUsbDevice.isDeviceAttached)
            {

                if (buffer.Length < 65)
                {
                    for (int i = 0; i < buffer.Length; i++)
                    {
                        data[i + 1] = buffer[i];
                    }
                    for (int j = buffer.Length + 1; j < data.Length; j++)
                    {
                        data[j] = 0;
                    }

                    theUsbDevice.Write_data(data);
                }
                else
                    MessageBox.Show("over lenght");
            }

        }

        private void Receive_Response_And_Send_Data_Grid()
        {
            // Datafloat_Manual[Mcounter, 1] 
            Byte[] receive = new Byte[65];
            receive = theUsbDevice.CollectData();
            byte[] buffer = new byte[32];
            byte code = (0x05);
            Int32 angle1, angle2, angle3, angle4, angle5, angle6;
            if (grid_index >= (Mcounter - 1))
            {
                grid_index = Mcounter - 1;
            }
            else
            {    //const byte Busy_Res = 0x04;
                //const byte Success_Res = 0x05;
                if (receive[0] == Success_Res)
                {

                    grid_index = grid_index + 1;
                }
                else
                {
                    grid_index = grid_index + 0;
                }

            }

            //test_debug.Text = debugString;
            angle1 = Convert.ToInt32(Datafloat_Manual[grid_index, 1]);
            angle2 = Convert.ToInt32(Datafloat_Manual[grid_index, 2]);
            angle3 = Convert.ToInt32(Datafloat_Manual[grid_index, 3]);
            angle4 = Convert.ToInt32(Datafloat_Manual[grid_index, 4]);
            angle5 = Convert.ToInt32(Datafloat_Manual[grid_index, 5]);
            angle6 = Convert.ToInt32(Datafloat_Manual[grid_index, 6]);
            byte[] Base = BitConverter.GetBytes(angle1);
            byte[] Shoulder = BitConverter.GetBytes(angle2);
            byte[] Elbow = BitConverter.GetBytes(angle3);
            byte[] Pitcth = BitConverter.GetBytes(angle4);
            byte[] Roll = BitConverter.GetBytes(angle5);
            byte[] Sum = BitConverter.GetBytes(angle6);

            buffer[0] = code;

            Array.Copy(Base, 0, buffer, 1, 4);
            Array.Copy(Shoulder, 0, buffer, 5, 4);
            Array.Copy(Elbow, 0, buffer, 9, 4);
            Array.Copy(Pitcth, 0, buffer, 13, 4);
            Array.Copy(Roll, 0, buffer, 17, 4);
            Array.Copy(Sum, 0, buffer, 21, 4);

            if (theUsbDevice.isDeviceAttached)
            {

                if (buffer.Length < 65)
                {
                    for (int i = 0; i < buffer.Length; i++)
                    {
                        data[i + 1] = buffer[i];
                    }
                    for (int j = buffer.Length + 1; j < data.Length; j++)
                    {
                        data[j] = 0;
                    }

                    theUsbDevice.Write_data(data);
                }
                else
                    MessageBox.Show("over lenght");
            }
        }

        bool flag_Gcode_Click;

        private void Start_Gcode_Click(object sender, RoutedEventArgs e)
        {
           // flag_Gcode_Click = !flag_Gcode_Click;
          //  if (flag_Gcode_Click)
            {
                index = 0;
                Send_Gcode_Timer.Start();
                Start_Gcode.IsEnabled = false;
                Stop_Gcode.IsEnabled = true;
                Disable_All_Button_Pulse();
                pb_Set_Pulse_grid.IsEnabled = false;
                Clearbutton.IsEnabled = false;
            }

        }

        private void Stop_Gcode_Click(object sender, RoutedEventArgs e)
        {


            Send_Gcode_Timer.Stop();
            Start_Gcode.IsEnabled = true;
            Stop_Gcode.IsEnabled = false;
            pb_Set_Pulse_grid.IsEnabled = true;
            Clearbutton.IsEnabled = true;
            Enable_All_Button_Pulse();
        }

        private void About_Click(object sender, RoutedEventArgs e)
        {
            frAbout frabout = new frAbout();
            frabout.ShowDialog();
        }

        private void richTextBoxGCode_Unloaded(object sender, RoutedEventArgs e)
        {
           // SelectLine(index);
        }

        private void debugTextbox_TextInput(object sender, TextCompositionEventArgs e)
        {

        }
        bool flag_Set_Puse_Click;
        private void Enable_All_Button_Pulse()
        {
            txt_pulse_base.IsEnabled = true;
            txt_Pulse_Elbow.IsEnabled = true;
            txt_pulse_gripper.IsEnabled = true;
            txt_pulse_pitch.IsEnabled = true;
            txt_pulse_roll.IsEnabled = true;
            txt_pulse_shoulder.IsEnabled = true;
           
            Pb_Inc_Pulse_Elbow.IsEnabled = true;
            Pb_Inc_Pulse_Pitch.IsEnabled = true;
            Pb_Inc_Pulse_Roll.IsEnabled = true;
            Pb_Inc_Puse_Shoulder.IsEnabled = true;
            Pb_Inc_Pulse_Gripper.IsEnabled = true;
            Pb_Inc_Puse_base.IsEnabled = true;

            Pb_Dec_Puse_Shoulder.IsEnabled = true;
            Pb_Dec_Puse_Base.IsEnabled = true;
            Pb_Dec_Pulse_Roll.IsEnabled = true;
            Pb_Dec_Pulse_Pitch.IsEnabled = true;
            Pb_Dec_Pulse_Gripper.IsEnabled = true;
            Pb_Dec_Pulse_Elbow.IsEnabled = true;
            Savebutton.IsEnabled = true;
            Start_Gcode.IsEnabled = true;
        }
        private void Disable_All_Button_Pulse()
        {
            txt_pulse_base.IsEnabled = false;
            txt_Pulse_Elbow.IsEnabled = false;
            txt_pulse_gripper.IsEnabled = false;
            txt_pulse_pitch.IsEnabled = false;
            txt_pulse_roll.IsEnabled = false;
            txt_pulse_shoulder.IsEnabled = false;

            Pb_Inc_Pulse_Elbow.IsEnabled = false;
            Pb_Inc_Pulse_Pitch.IsEnabled = false;
            Pb_Inc_Pulse_Roll.IsEnabled = false;
            Pb_Inc_Puse_Shoulder.IsEnabled = false;
            Pb_Inc_Pulse_Gripper.IsEnabled = false;
            Pb_Inc_Puse_base.IsEnabled = false;

            Pb_Dec_Puse_Shoulder.IsEnabled = false;
            Pb_Dec_Puse_Base.IsEnabled = false;
            Pb_Dec_Pulse_Roll.IsEnabled = false;
            Pb_Dec_Pulse_Pitch.IsEnabled = false;
            Pb_Dec_Pulse_Gripper.IsEnabled = false;
            Pb_Dec_Pulse_Elbow.IsEnabled = false;
            Savebutton.IsEnabled = false;
            Start_Gcode.IsEnabled = false;
        }
        private void pb_Set_Pulse_Click(object sender, RoutedEventArgs e)
        {
            flag_Set_Puse_Click= !flag_Set_Puse_Click;
            if (flag_Set_Puse_Click)
            {
                Disable_All_Button_Pulse();
                
                grid_index = 0;
                Send_DataGrid_Timer.Start();
            }
            else
            {
                Enable_All_Button_Pulse();
                Send_DataGrid_Timer.Stop();
                
            }

           
        }

        private void controlMouse_Checked(object sender, RoutedEventArgs e)
        {

        }

        /* manual click */
      
        private void Pb_Dec_Puse_Shoulder_Click(object sender, RoutedEventArgs e)
        {
            gShoulder = gShoulder - 250;
            updatePulse_SEND();
            //txt_pulse_shoulder.Text = Convert.ToString(gShoulder);
           
        }
        private void Pb_Inc_Puse_Shoulder_Click(object sender, RoutedEventArgs e)
        {
           
            gShoulder = gShoulder + 250;
            updatePulse_SEND();
           // txt_pulse_shoulder.Text = Convert.ToString(gShoulder);
            
           
            
        }
        private void Pb_Dec_Puse_Elbow_Click(object sender, RoutedEventArgs e)
        {
            gElbow = gElbow - 200;
            updatePulse_SEND();
           // txt_Pulse_Elbow.Text = Convert.ToString(gElbow);
            
        }
        private void Pb_Inc_Puse_Elbow_Click(object sender, RoutedEventArgs e)
        {
         
           
            gElbow = gElbow + 200;
            updatePulse_SEND();
           // txt_Pulse_Elbow.Text = Convert.ToString(gElbow);
         
        }
        private void Pb_Dec_Puse_pitch_Click(object sender, RoutedEventArgs e)
        {
            gPitch = gPitch - 100;
            updatePulse_SEND();
           // txt_pulse_pitch.Text = Convert.ToString(gPitch);
        }
        private void Pb_Inc_Puse_pitch_Click(object sender, RoutedEventArgs e)
        {
            gPitch = gPitch + 100;
            updatePulse_SEND();
           // txt_pulse_pitch.Text = Convert.ToString(gPitch);
        }
        private void Pb_Dec_Puse_roll_Click(object sender, RoutedEventArgs e)
        {
            gRoll = gRoll - 100;
            updatePulse_SEND();
           // txt_pulse_roll.Text = Convert.ToString(gRoll);
        }
        private void Pb_Inc_Puse_roll_Click(object sender, RoutedEventArgs e)
        {
            gRoll = gRoll + 100;
            updatePulse_SEND();
          //  txt_pulse_roll.Text = Convert.ToString(gRoll);
        }
        private void Pb_Dec_Puse_gripper_Click(object sender, RoutedEventArgs e)
        {
            gGripper = gGripper - 100;
            updatePulse_SEND();
            txt_pulse_gripper.Text = Convert.ToString(gGripper);
        }
        private void Pb_Inc_Puse_gripper_Click(object sender, RoutedEventArgs e)
        {
            gGripper = gGripper + 100;
            updatePulse_SEND();
          //  txt_pulse_gripper.Text = Convert.ToString(gGripper);
        }
        private void Savebutton_Click(object sender, RoutedEventArgs e)
        {
            Datafloat_Manual[Mcounter, 1] = basecurrent; // value Base
            Datafloat_Manual[Mcounter, 2] = shouldercurrent; // value Shoulder
            Datafloat_Manual[Mcounter, 3] = elbowcurrent; // value Elbow
            Datafloat_Manual[Mcounter, 4] = dpitch; // value Pitch
            Datafloat_Manual[Mcounter, 5] = droll; // value Roll
            Datafloat_Manual[Mcounter, 6] = dgripper; // value Gripper
            Mcounter = Mcounter + 1;
            //ObservableCollection<Datagrid> grid_data = GetData();
            List<Datagrid> data_grid = new List<Datagrid>();
            for (int i = 0; i < Mcounter; i++)
            {
                data_grid.Add(new Datagrid() { Number = i+1, gtBase = Datafloat_Manual[i, 1], gtShoulder = Datafloat_Manual[i, 2], gtElbow = Datafloat_Manual[i, 3], gtPitch = Datafloat_Manual[i, 4], gtRoll = Datafloat_Manual[i, 5], gtGripper = Datafloat_Manual[i, 6] });
            }
            DataTeach.ItemsSource = data_grid;
                        
        }
        
        private void Clearbutton_Click(object sender, RoutedEventArgs e)
        {
            gBase = basecurrent;
            gShoulder = shouldercurrent;
            gElbow = elbowcurrent;
            gPitch = dpitch;
            gRoll = droll;
            Gripper = dgripper;
            int i = 0,j = 0;
            for (i = 0; i <= Mcounter;i++)
            {
                for (j = 1; j <= 6; j++)
                {
                    Datafloat_Manual[i, j] = 0; 
                }
            }
            Mcounter = 0;
        }
        public class Datagrid
        {
            public int Number { get; set; }
            public float gtBase { get; set; }
            public float gtShoulder { get; set; }
            public float gtElbow { get; set; }
            public float gtPitch { get; set; }
            public float gtRoll { get; set; }
            public float gtGripper { get; set; }
        }
        void updatePulse_SEND()
        {
            // gBase=0, gShoulder=1750, gElbow=0,gPitch=1400, gRoll=1400, gGripper=1400
            if (gShoulder > 3200 | gShoulder < 1750 | gBase < 0 | gBase > 14000 | gElbow >= 38300 | gElbow < 0)
            {
                //MessageBox.Show("Out of range, it will be offset value");
                if (gShoulder > 3200) { gShoulder = 3200; }
                if (gShoulder < 1750) { gShoulder = 1750; }
                if (gBase < 0) { gBase = 0; }
                if (gBase >14000) { gBase = 14000; }
                if (gElbow < 0) { gElbow = 0; }

            }
            
               // MessageBox.Show("Out of range, it will be offset value");
                if (gPitch > 2000) { gPitch = 2000; }
                if (gPitch < 600) { gPitch = 600; }
                if (gRoll > 2000) { gRoll =2000; }
                if (gRoll < 600) { gRoll = 600; }
                if (gGripper > 1400) { gGripper = 1400; }
                if (gGripper < 600) { gGripper = 600; }
            
            byte[] code = BitConverter.GetBytes(0x20);
            byte[] Base_Array = BitConverter.GetBytes(Convert.ToInt32(gBase));
            byte[] Shoulder_Array = BitConverter.GetBytes(Convert.ToInt32(gShoulder));
            byte[] Elbow_Array = BitConverter.GetBytes(Convert.ToInt32(gElbow));
            byte[] Pitch_Array = BitConverter.GetBytes(Convert.ToInt32(gPitch));
            byte[] Roll_Array = BitConverter.GetBytes(Convert.ToInt32(gRoll));
            byte[] Gripper_Array = BitConverter.GetBytes(Convert.ToInt32(gGripper));
            byte[] buffer = new byte[32];
            Array.Copy(code, 0, buffer, 0, 1);
            Array.Copy(Base_Array, 0, buffer, 1, 4);
            Array.Copy(Shoulder_Array, 0, buffer, 5, 4);
            Array.Copy(Elbow_Array, 0, buffer, 9, 4);
            Array.Copy(Pitch_Array, 0, buffer, 13, 4);
            Array.Copy(Roll_Array, 0, buffer, 17, 4);
            Array.Copy(Gripper_Array, 0, buffer, 21, 4);

            if (theUsbDevice.isDeviceAttached)
            {


                if (buffer.Length < 65)
                {
                    for (i = 0; i < buffer.Length; i++)
                    {
                        data[i + 1] = buffer[i];
                    }
                    for (int j = buffer.Length + 1; j < data.Length; j++)
                    {
                        data[j] = 0;
                    }
                    theUsbDevice.Write_data(data);
                }
                else
                    MessageBox.Show("over lenght");


            }
        }
        void updatePulse_nonPID_Forward() 
        {
            byte[] code = BitConverter.GetBytes(0x25);
            byte[] speed = BitConverter.GetBytes(speed_Base);
            byte[] buffer = new byte[32];
            Array.Copy(code, 0, buffer, 0, 1);
            Array.Copy(speed, 0, buffer, 1, 4);
            if (theUsbDevice.isDeviceAttached)
            {


                if (buffer.Length < 65)
                {
                    for (i = 0; i < buffer.Length; i++)
                    {
                        data[i + 1] = buffer[i];
                    }
                    for (int j = buffer.Length + 1; j < data.Length; j++)
                    {
                        data[j] = 0;
                    }
                    theUsbDevice.Write_data(data);
                }
                else
                    MessageBox.Show("over lenght");


            }
        }
        void updatePulse_nonPID_Reverse()
        {
            byte[] code = BitConverter.GetBytes(0x26);
            byte[] speed = BitConverter.GetBytes(speed_Base);
            byte[] buffer = new byte[32];
            Array.Copy(code, 0, buffer, 0, 1);
            Array.Copy(speed, 0, buffer, 1, 4);
            if (theUsbDevice.isDeviceAttached)
            {


                if (buffer.Length < 65)
                {
                    for (i = 0; i < buffer.Length; i++)
                    {
                        data[i + 1] = buffer[i];
                    }
                    for (int j = buffer.Length + 1; j < data.Length; j++)
                    {
                        data[j] = 0;
                    }
                    theUsbDevice.Write_data(data);
                }
                else
                    MessageBox.Show("over lenght");


            }
        }
        void updatePulse_nonPID_StopMotor()
        {
            byte[] code = BitConverter.GetBytes(0x27);
            byte[] buffer = new byte[32];
            Array.Copy(code, 0, buffer, 0, 1);
            if (theUsbDevice.isDeviceAttached)
            {


                if (buffer.Length < 65)
                {
                    for (i = 0; i < buffer.Length; i++)
                    {
                        data[i + 1] = buffer[i];
                    }
                    for (int j = buffer.Length + 1; j < data.Length; j++)
                    {
                        data[j] = 0;
                    }
                    theUsbDevice.Write_data(data);
                }
                else
                    MessageBox.Show("over lenght");


            }
        }
        void updatePulse_Shoulder_nonPID_Forward()
        {
            byte[] code = BitConverter.GetBytes(0x28);
            byte[] buffer = new byte[32];
            Array.Copy(code, 0, buffer, 0, 1);
            if (theUsbDevice.isDeviceAttached)
            {


                if (buffer.Length < 65)
                {
                    for (i = 0; i < buffer.Length; i++)
                    {
                        data[i + 1] = buffer[i];
                    }
                    for (int j = buffer.Length + 1; j < data.Length; j++)
                    {
                        data[j] = 0;
                    }
                    theUsbDevice.Write_data(data);
                }
                else
                    MessageBox.Show("over lenght");


            }
        }
        void updatePulse_Shoulder_nonPID_Reverse()
        {
            byte[] code = BitConverter.GetBytes(0x29);
            byte[] buffer = new byte[32];
            Array.Copy(code, 0, buffer, 0, 1);
            if (theUsbDevice.isDeviceAttached)
            {


                if (buffer.Length < 65)
                {
                    for (i = 0; i < buffer.Length; i++)
                    {
                        data[i + 1] = buffer[i];
                    }
                    for (int j = buffer.Length + 1; j < data.Length; j++)
                    {
                        data[j] = 0;
                    }
                    theUsbDevice.Write_data(data);
                }
                else
                    MessageBox.Show("over lenght");


            }
        }
        bool flag_graph;
        private void Drawing_Response_click(object sender, RoutedEventArgs e)
        {
            flag_graph = !flag_graph;
            if(flag_graph)
            { graph.Show(); }
            else
            { graph.Hide(); }
            
        }

 
        private void Inc_Pulse_Timer_Tick(object sender, EventArgs e)
        {
            if (flag_teach == 1)
            {
                gBase = gBase + 100;
               
                updatePulse_SEND();
              
            }
            if (flag_teach == 2)
            {
                gBase = gBase - 100;
               
                updatePulse_SEND();
               
            }

            if (flag_teach==11)
            {
              
                for (int i = 0; i <= 10; i++)
                {
                    gShoulder = gShoulder + i;
                    updatePulse_SEND();
                }

            }
            if (flag_teach == 12)
            {
                for (int i = 0; i <= 10; i++)
                {
                    gShoulder = gShoulder - i;
                    updatePulse_SEND();
                }
            }
            if (flag_teach == 5)
            {
                gPitch = gPitch - 50;
                updatePulse_SEND();

            }
            if (flag_teach == 6)
            {
                gPitch = gPitch + 50;
                updatePulse_SEND();
            }
            if (flag_teach == 7)
            {
                gRoll = gRoll + 50;
                updatePulse_SEND();
            }
            if (flag_teach == 8)
            {
                gRoll = gRoll - 50;
                updatePulse_SEND();
            }
            if (flag_teach == 9)
            {
                gGripper = gGripper + 50;
                updatePulse_SEND();
            }
            if (flag_teach == 10)
            {
                gGripper = gGripper - 50;
                updatePulse_SEND();
            }
        }
        private void Inc_Pulse_Fast_Timer_Tick(object sender,EventArgs e)
        {

            if (flag_teach == 3)
            {
                gElbow = gElbow + 500;
                
                updatePulse_SEND();
                
            }
            if (flag_teach == 4)
            {
                gElbow = gElbow - 500;
                
                updatePulse_SEND();
                
            }
          
        }
        private void Pb_Inc_Puse_base_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
           /*
            flag_teach = 1;
            Inc_Pulse_Timer.Start();
          */
            updatePulse_nonPID_Forward(); //send code 0x25
           
        }

        private void Pb_Inc_Puse_base_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            /*
            flag_teach = 0;
            Inc_Pulse_Timer.Stop();
             */
            updatePulse_nonPID_StopMotor(); // send code 0x26
        }

        private void Pb_Dec_Puse_Base_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            /*
            flag_teach = 2;
            Inc_Pulse_Timer.Start();
             */
            updatePulse_nonPID_Reverse(); //send code 0x26
             
        }

        private void Pb_Dec_Puse_Base_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
           /*
            flag_teach = 0;
            Inc_Pulse_Timer.Stop();
            */
            updatePulse_nonPID_StopMotor(); //send code 0x27
          
        }
        /* flag_teach = 3; inc Elbow
         * flag_teach = 4; dec Elbow
         * flag_teach = 5; inc Pitch
         * flag_teach = 6; dec Pitch
         * flag_teach = 7; inc Roll
         * flag_teach = 8; dec Roll
         * flag_teach = 9; inc Gripper
         * flag_teach = 10; dec Gripper
         * */
        private void Pb_Inc_Pulse_Elbow_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 3;
            Inc_Pulse_Fast_Timer.Start();
        }

        private void Pb_Inc_Pulse_Elbow_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 0;
            Inc_Pulse_Fast_Timer.Stop();
        }
        private void Pb_Dec_Pulse_Elbow_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 4;
            Inc_Pulse_Fast_Timer.Start();
        }

        private void Pb_Dec_Pulse_Elbow_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 0;
            Inc_Pulse_Fast_Timer.Stop();
        }
        private void Pb_Inc_Pulse_Pitch_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 5;
            Inc_Pulse_Timer.Start();
        }

        private void Pb_Inc_Pulse_Pitch_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 0;
            Inc_Pulse_Timer.Stop();
        }
        private void Pb_Dec_Pulse_Pitch_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 6;
            Inc_Pulse_Timer.Start();
        }

        private void Pb_Dec_Pulse_Pitch_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 0;
            Inc_Pulse_Timer.Stop();
        }
        private void Pb_Inc_Pulse_Roll_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 7;
            Inc_Pulse_Timer.Start();
        }

        private void Pb_Inc_Pulse_Roll_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 0;
            Inc_Pulse_Timer.Stop();
        }
        private void Pb_Dec_Pulse_Roll_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 8;
            Inc_Pulse_Timer.Start();
        }

        private void Pb_Dec_Pulse_Roll_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 0;
            Inc_Pulse_Timer.Stop();
        }
        private void Pb_Inc_Pulse_Gripper_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 9;
            Inc_Pulse_Timer.Start();
        }

        private void Pb_Inc_Pulse_Gripper_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 0;
            Inc_Pulse_Timer.Stop();
        }
        private void Pb_Dec_Pulse_Gripper_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 10;
            Inc_Pulse_Timer.Start();
        }

        private void Pb_Dec_Pulse_Gripper_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            flag_teach = 0;
            Inc_Pulse_Timer.Stop();
        }

        private void Pb_Inc_Puse_Shoulder_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            /*
            flag_teach = 11;
            Inc_Pulse_Timer.Start();
             */
            updatePulse_Shoulder_nonPID_Forward();
        }

        private void Pb_Inc_Puse_Shoulder_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            /*
            flag_teach = 0;
            Inc_Pulse_Timer.Stop();
             */
            updatePulse_nonPID_StopMotor();
        }
        private void Pb_Dec_Puse_Shoulder_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            /*
            flag_teach = 12;
            Inc_Pulse_Timer.Start();
             */
            updatePulse_Shoulder_nonPID_Reverse();
        }
        private void Pb_Dec_Puse_Shoulder_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            /*
            flag_teach = 0;
            Inc_Pulse_Timer.Stop();
             */
            updatePulse_nonPID_StopMotor();
        }
        public void SelectLine_PeterMap(int line)
        {
            int c = 0;
            TextRange checking;

            foreach (var item in richTextBoxGCode.Document.Blocks)
            {
                if (line == c)
                {
                    checking = new TextRange(item.ContentStart, item.ContentEnd);
                    if (checking.Text.Trim().Equals(""))
                    {
                        continue;
                    }
                    checking.ApplyPropertyValue(TextElement.BackgroundProperty, Brushes.Red);
                    checking.ApplyPropertyValue(TextElement.ForegroundProperty, Brushes.White);
                    return;
                }
                c++;
            }
        }
        public void SelectLine_Normal(int line)
        {
            int c = 0;
            TextRange checking;

            foreach (var item in richTextBoxGCode.Document.Blocks)
            {
                if (line == c)
                {
                    checking = new TextRange(item.ContentStart, item.ContentEnd);
                    if (checking.Text.Trim().Equals(""))
                    {
                        continue;
                    }
                    checking.ApplyPropertyValue(TextElement.BackgroundProperty, Brushes.Green);
                    checking.ApplyPropertyValue(TextElement.ForegroundProperty, Brushes.Yellow);
                    return;
                }
                c++;
            }
        }

        private void Slider_ValueChange(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            speed_Base=Convert.ToInt32(Slider_speed.Value);
        }
       
    

    

     
        

    }
}


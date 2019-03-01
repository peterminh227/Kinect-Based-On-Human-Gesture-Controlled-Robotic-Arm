using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using STA.Settings; // Ini file
using System.IO;
//using System.Drawing;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows;
using System.Windows.Media;
//using Microsoft.Office.Core;
//using Microsoft.Office.Interop.Excel;
using Excel = Microsoft.Office.Interop.Excel; 
namespace Kinect_Control_USBDevice
{
    partial class MainWindow
    {
        // Data array
        int i;
        public char[,][] Tool_Gcode = new char[10, 2][];
        public string Tool_Number, Tool_Diameter, test;
        public char[,][] Data_Gcode = new char[1000, 7][];
        public float[,] Datafloat_Gcode = new float[1000, 8];
        public float[] Datafloat_GcodeInverse;
        public float Data_Gcode_Buffer;

        public string Data_Tool, Data_X, Data_Y, Data_Z, Data_G, Data_R, Data_S , Data_Ba, Data_Sh, Data_El, Data_Pi, Data_Ro, Data_Gr;
        public int counter = 0, PosBeginPro = 0;
        // Flag for Gcode
        public static bool SpindleMode = true; // true: Auto - false: Manual
        public static bool SpindleOn = false; // true: On - false: Off (chi co the dung khi SpindleMode = false)
        public static bool StartRead = false; // Bat dau doc Gcode khi gap dong Oxxxx. Vi du: O0001

        public void readGcode(string CNCfile)
        {
            int j = 0, k = 0;
            string line;
            System.Drawing.Color[] ToolColor = new System.Drawing.Color[10]{
                                System.Drawing.Color.Yellow, 
                                System.Drawing.Color.Green, 
                                System.Drawing.Color.Blue, 
                                System.Drawing.Color.Pink, 
                                System.Drawing.Color.Silver,
                                System.Drawing.Color.YellowGreen,
                                System.Drawing.Color.Tomato,
                                System.Drawing.Color.Snow,
                                System.Drawing.Color.Purple,
                                System.Drawing.Color.Red
                                };
            counter = 0;
            for (i = 0; i < Data_Gcode.GetLength(0); i++)
            {
                
                /*
                Data_Gcode[i, 1] = new char[8] { '0', '0', '0', '0', '0', '0', '0', '0' }; // X
                Data_Gcode[i, 2] = new char[8] { '0', '0', '0', '0', '0', '0', '0', '0' }; // Y
                Data_Gcode[i, 3] = new char[8] { '0', '0', '0', '0', '0', '0', '0', '0' }; // Z
                Data_Gcode[i, 4] = new char[8] { '0', '0', '0', '0', '0', '0', '0', '0' }; // Gripper
                Data_Gcode[i, 5] = new char[8] { '0', '0', '0', '0', '0', '0', '0', '0' }; // Roll
                 */
                Data_Gcode[i, 0] = new char[3] { '0', '0', '0' };// thoi gian noi suy hinh thang
                Data_Gcode[i, 1] = new char[4] {  '0', '0', '0', '0' }; // X
                Data_Gcode[i, 2] = new char[4] {  '0', '0', '0', '0' }; // Y
                Data_Gcode[i, 3] = new char[4] {  '0', '0', '0', '0' }; // Z
                Data_Gcode[i, 4] = new char[4] {  '0', '0', '0', '0' }; // Sum
                Data_Gcode[i, 5] = new char[4] {  '0', '0', '0', '0' }; // Roll
                Data_Gcode[i, 6] = new char[4] { '0', '0', '0', '0' }; // Gripper
                


            }
            System.IO.StreamReader fileread = new System.IO.StreamReader(CNCfile);
              // Ghi vao ma tran Data
            while ((line = fileread.ReadLine()) != null)
            {
                //if (currentMachineMode == 1) // Mem Mode
                {
                    if (line.Contains("O")) StartRead = true; // Mo dau chuong trinh
                    else if (!StartRead) PosBeginPro++;
                }
                if (StartRead && !line.Contains("O"))
                {
                    #region X
                    if (line.Contains("X"))
                    {
                        i = 0;
                        while (line.IndexOf('X') + i < line.Length - 1) // Chua ket thuc line thi doc tiep
                        {
                            if (line[line.IndexOf('X') + i + 1] == ' ') break; // Gap ki tu ' ' thi dung
                            i++;
                        }

                        // Ghi du lieu vao mang (ghi vao cac o nho cuoi. Vi du: 011)
                        for (k = 0; k < i; k++)
                            Data_Gcode[counter, 1][k + ((Data_Gcode[counter, 1].Length - 1) - (i - 1))] = line[line.IndexOf('X') + k + 1];
                      // testing.Text += new string(Data_Gcode[counter, 1]) + " \r\n";
                    }
                    else if (counter > 0) Data_Gcode[counter, 1] = Data_Gcode[counter - 1, 1]; // Giu nguyen gia tri phia truoc 
                    Datafloat_Gcode[counter,1] = CharArrayToFloat(Data_Gcode[counter, 1]);
                 //   testing.Text += Convert.ToString( Datafloat_Gcode[counter,1]) + " \r\n";

                    #endregion
                    #region Y
                    if (line.Contains("Y"))
                    {
                        i = 0;
                        while (line.IndexOf('Y') + i < line.Length - 1) // Chua ket thuc line thi doc tiep
                        {
                            if (line[line.IndexOf('Y') + i + 1] == ' ') break; // Gap ki tu ' ' thi dung
                            i++;
                        }

                        // Ghi du lieu vao mang (ghi vao cac o nho cuoi. Vi du: 011)
                        for (k = 0; k < i; k++)
                            Data_Gcode[counter, 2][k + ((Data_Gcode[counter, 2].Length - 1) - (i - 1))] = line[line.IndexOf('Y') + k + 1];
                            //testing.Text += new string(Data_Gcode[counter, 2]) + " \r\n";
                       
                    }
                    else if (counter > 0) Data_Gcode[counter, 2] = Data_Gcode[counter - 1, 2]; // Giu nguyen gia tri phia truoc
                    Datafloat_Gcode[counter, 2] = CharArrayToFloat(Data_Gcode[counter, 2]);
                    #endregion
                    #region Z
                    if (line.Contains("Z"))
                    {
                        i = 0;
                        while (line.IndexOf('Z') + i < line.Length - 1) // Chua ket thuc line thi doc tiep
                        {
                            if (line[line.IndexOf('Z') + i + 1] == ' ') break; // Gap ki tu ' ' thi dung
                            i++;
                        }

                        // Ghi du lieu vao mang (ghi vao cac o nho cuoi. Vi du: 011)
                        for (k = 0; k < i; k++)
                        Data_Gcode[counter, 3][k + ((Data_Gcode[counter, 3].Length - 1) - (i - 1))] = line[line.IndexOf('Z') + k + 1];
                        //testing.Text += new string(Data_Gcode[counter, 3]) + " \r\n";

                       
                    }
                    else if (counter > 0) Data_Gcode[counter, 3] = Data_Gcode[counter - 1, 3]; // Giu nguyen gia tri phia truoc 
                    Datafloat_Gcode[counter, 3] = CharArrayToFloat(Data_Gcode[counter, 3]);
                    #endregion
                    #region S
                    if (line.Contains("S"))
                    {
                        i = 0;
                        while (line.IndexOf('S') + i < line.Length - 1) // Chua ket thuc line thi doc tiep
                        {
                            if (line[line.IndexOf('S') + i + 1] == ' ') break; // Gap ki tu ' ' thi dung
                            i++;
                        }

                        // Ghi du lieu vao mang (ghi vao cac o nho cuoi. Vi du: 011)
                        for (k = 0; k < i; k++)
                            Data_Gcode[counter, 4][k + ((Data_Gcode[counter, 4].Length - 1) - (i - 1))] = line[line.IndexOf('S') + k + 1];
                        //testing.Text += new string(Data_Gcode[counter, 4]) + " \r\n";
                    }
                    else if (counter > 0) Data_Gcode[counter, 4] = Data_Gcode[counter - 1, 4]; // Giu nguyen gia tri phia truoc 
                    Datafloat_Gcode[counter, 4] = CharArrayToFloat(Data_Gcode[counter, 4]);
                    #endregion
                    #region R
                    if (line.Contains("R"))
                    {
                        i = 0;
                        while (line.IndexOf('R') + i < line.Length - 1) // Chua ket thuc line thi doc tiep
                        {
                            if (line[line.IndexOf('R') + i + 1] == ' ') break; // Gap ki tu ' ' thi dung
                            i++;
                        }

                        // Ghi du lieu vao mang (ghi vao cac o nho cuoi. Vi du: 011)
                        for (k = 0; k < i; k++)
                            Data_Gcode[counter, 5][k + ((Data_Gcode[counter, 5].Length - 1) - (i - 1))] = line[line.IndexOf('R') + k + 1];
                       // testing.Text += new string(Data_Gcode[counter, 5]) + " \r\n";
                    }
                    else if (counter > 0) Data_Gcode[counter, 5] = Data_Gcode[counter - 1, 5]; // Giu nguyen gia tri phia truoc 
                    Datafloat_Gcode[counter, 5] = CharArrayToFloat(Data_Gcode[counter, 5]);
                    #endregion
                    #region G
                    if (line.Contains("G"))
                    {
                        i = 0;
                        while (line.IndexOf('G') + i < line.Length - 1) // Chua ket thuc line thi doc tiep
                        {
                            if (line[line.IndexOf('G') + i + 1] == ' ') break; // Gap ki tu ' ' thi dung
                            i++;
                        }

                        // Ghi du lieu vao mang (ghi vao cac o nho cuoi. Vi du: 011)
                        for (k = 0; k < i; k++)
                            Data_Gcode[counter, 6][k + ((Data_Gcode[counter, 6].Length - 1) - (i - 1))] = line[line.IndexOf('G') + k + 1];
                        // testing.Text += new string(Data_Gcode[counter, 5]) + " \r\n";
                    }
                    else if (counter > 0) Data_Gcode[counter, 6] = Data_Gcode[counter - 1, 6]; // Giu nguyen gia tri phia truoc 
                    Datafloat_Gcode[counter, 6] = CharArrayToFloat(Data_Gcode[counter, 6]);
                    #endregion
                   // testing.Text += Convert.ToString(Datafloat_Gcode[counter, 1]) + " \r\n";
                    Datafloat_GcodeInverse=inverse_Cal(Datafloat_Gcode[counter, 1], Datafloat_Gcode[counter, 2], Datafloat_Gcode[counter, 3], Datafloat_Gcode[counter, 4], Datafloat_Gcode[counter, 5], Datafloat_Gcode[counter,6]);
                    Datafloat_Gcode[counter, 1] = Datafloat_GcodeInverse[1]; // value Base
                    Datafloat_Gcode[counter, 2] = Datafloat_GcodeInverse[2]; // value Shoulder
                    Datafloat_Gcode[counter, 3] = Datafloat_GcodeInverse[3]; // value Elbow
                    Datafloat_Gcode[counter, 4] = Datafloat_GcodeInverse[4]; // value Pitch
                    Datafloat_Gcode[counter, 5] = Datafloat_GcodeInverse[5]; // value Roll
                    Datafloat_Gcode[counter, 6] = Datafloat_GcodeInverse[6]; // value Gripper
                    testing.Text +=  Convert.ToString(Datafloat_Gcode[counter, 1]) + " \r\n";
                   // testing.Text += new string(Data_Gcode[counter, 1]) + " \r\n";
                    counter++;
                    if (line.Contains("M30")) // Ket thuc chuong trinh
                    {
                        StartRead = false;
                        
                        break;
                    }
                    
                }

            }
            fileread.Close();
        
        }
        // Ham chuyen doi Char array sang float de lay du lieu tu Data_Gcode
        public float CharArrayToFloat(Char[] In)
        {
            float Data_Buffer = 0;
            char[] Data = In;
            int k = 0;
            string s;

            // Chuyen Char array -> String
            s = new string(Data);

            // Kiem tra dau am '-'
            while (Data[k] != '-')
            {
                k++;
                if (k == Data.Length)
                { k = 0; break; }
            }

            // Loai bo cac ki tu thua. Vi du: 0-12.23 -> -12.23
            s = s.Remove(0, k);

            // Chuyen String -> Float
            Data_Buffer = Convert.ToSingle(s);

            return Data_Buffer;
        }

        // Ham chuyen doi Float sang Char array de nhap du lieu vao Data_Gcode
        public char[] FloatToCharArray(float In, int LengthOut)
        {
            float Data_Buffer = In;
            Char[] Data = new char[LengthOut];
            string s;

            // Chuyen Float -> String
            s = In.ToString();
            //textBox1.Text += s + " hehe\r\n";

            // Kiem tra do dai String co nho hon mang Char array quy dinh ko? Neu nho hon thi phai them ki tu 0 vao dau
            while (s.Length < LengthOut)
                s = s.Insert(0, "0");

            // Kiem tra do dai String co vuot qua mang Char array quy dinh ko? Neu vuot qua thi phai xoa bot ki tu
            if (s.Length > LengthOut)
                s = s.Remove(LengthOut, s.Length - LengthOut);

            // Chuyen String -> Char array
            Data = s.ToCharArray();

            return Data;
        }
        public  float[] inverse_Cal(float x1, float y1, float z1, float sum1, float roll1, float gripper1)
        {
            double x, y, z, gripper, roll, sum;
            double vBase, vShoulder, vElbow, vPitch, vRoll, vGripper;
            //double value = 0, value1 = 0;
            float[] Gcodefloat=new float[8] ;
            x = Convert.ToDouble(x1);
          //  testing.Text =   Convert.ToString(roll1) ;
            y = Convert.ToDouble(y1);
            z = Convert.ToDouble(z1);
            sum = Convert.ToDouble(sum1);
            roll = Convert.ToDouble(roll1);
            gripper = Convert.ToDouble(gripper1);
            /* Convert to radian */
            theta234_r = Degrees_to_Radians(sum);
            theta1_r = Math.Atan2(x, y);
            /* Convert to degree */
            theta1_d = Radians_to_Degrees(theta1_r);
       //    testing.Text = Convert.ToString(theta1_d) + " " + Convert.ToString(x) + " " + Convert.ToString(y);
            X1 = Math.Sqrt((x * x) + (y * y));
            Y1 = z;
            X2 = X1 - d5 * Math.Cos(theta234_r);
            Y2 = Y1 - d5 * Math.Sin(theta234_r);
            X3 = X2;
            Y3 = Y2 - d1;
            cos_theta3 = ((X3 * X3 + Y3 * Y3 - a2 * a2 - a3 * a3) / (2 * a2 * a3));
            sin_theta3_n = (double)((-1) * Math.Sqrt(1 - cos_theta3 * cos_theta3));
            theta3_r_n = Math.Atan2(sin_theta3_n, cos_theta3);
            theta3_d_n = Radians_to_Degrees(theta3_r_n);
            theta2_r_p = +Math.Atan2(Y3, X3) + Math.Atan2(a3 * Math.Sin(-theta3_r_n), a2 + a3*Math.Cos(-theta3_r_n));
            theta2_d_p = Radians_to_Degrees(theta2_r_p);
            theta4_d_n = sum - theta2_d_p - theta3_d_n;
            vBase = (theta1_d / 90) * 7100;
            vShoulder = 1750 + theta2_d_p / 90 * 1525;
            vElbow = (95 + theta3_d_n) / 90 * 19150;
            vPitch = (-theta4_d_n * 11.2 + 1400);
            if (gripper == 0) { vGripper=600;}
            else { vGripper = 1500; }
            
            vRoll = roll * 11.2 + 1400;
           // s1 = Convert.ToString(vBase) + " " + Convert.ToString(vShoulder) + " " + Convert.ToString(vElbow) + " " + Convert.ToString(vPitch);
            //testing.Text = s1;
           // if (vShoulder >= 3200 | vShoulder <= 1750 | vBase <= 0 | vBase >= 34200 | vElbow >= 38300)
            if (vShoulder>=3200) {vShoulder = 3200;}
            if (vShoulder<=1750) {vShoulder=1750;}  
            if (vBase < 0) { vBase = 0; }
            if (vBase >= 10500) { vBase = 10500; }
            if (vElbow>=23000) {vElbow=23000;}
         //   testing.Text += Convert.ToString(vShoulder) + " \r\n";
            Gcodefloat[1] = Convert.ToSingle(vBase);
            Gcodefloat[2] = Convert.ToSingle(vShoulder);
            Gcodefloat[3] = Convert.ToSingle(vElbow);
            Gcodefloat[4] = Convert.ToSingle(vPitch);
            Gcodefloat[5] = Convert.ToSingle(vRoll);
            Gcodefloat[6] = Convert.ToSingle(vGripper);
             return Gcodefloat;
          }
        // Hien thi Gcode ra RichTextBox
        private void AddColouredText(string strTextToAdd)
        {  


            //Use the RichTextBox to create the initial RTF code
            //richTextBoxGCode.
            richTextBoxGCode.Document.Blocks.Clear();
            richTextBoxGCode.AppendText(strTextToAdd);

               string strRTF = string.Empty;
            using(MemoryStream ms=new MemoryStream())
            {
                TextRange range2 = new TextRange(richTextBoxGCode.Document.ContentStart, richTextBoxGCode.Document.ContentEnd);
                range2.Save(ms, DataFormats.Rtf);
                ms.Seek(0, SeekOrigin.Begin);
                using (StreamReader sr=new StreamReader(ms))
                {
                    strRTF = sr.ReadToEnd();
                }
            }
            richTextBoxGCode.Document.Blocks.Clear();

            /* 
             * ADD COLOUR TABLE TO THE HEADER FIRST 
             * */

            // Search for colour table info, if it exists (which it shouldn't)
            // remove it and replace with our one
            int iCTableStart = strRTF.IndexOf("colortbl;");

            if (iCTableStart != -1) //then colortbl exists
            {
                //find end of colortbl tab by searching
                //forward from the colortbl tab itself
                int iCTableEnd = strRTF.IndexOf('}', iCTableStart);
                strRTF = strRTF.Remove(iCTableStart, iCTableEnd - iCTableStart);

                //now insert new colour table at index of old colortbl tag
                strRTF = strRTF.Insert(iCTableStart,
                    // CHANGE THIS STRING TO ALTER COLOUR TABLE
                    //"colortbl ;\\red255\\green0\\blue0;\\red0\\green128\\blue0;\\red0\\green0\\blue255;}");
                    "\\colortbl ;" +
                    "\\red255\\green0\\blue0;" +
                    "\\red0\\green128\\blue0;" +
                    "\\red0\\green0\\blue255;" +
                    "\\red160\\green32\\blue240;" +
                    "\\red255\\green140\\blue0;" +
                    "\\red255\\green20\\blue147;" +
                    "\\red0\\green191\\blue255;" +
                    "\\red0\\green0\blue0;}");
            }

            //colour table doesn't exist yet, so let's make one
            else
            {
                // find index of start of header
                int iRTFLoc = strRTF.IndexOf("\\rtf");
                // get index of where we'll insert the colour table
                // try finding opening bracket of first property of header first                
                int iInsertLoc = strRTF.IndexOf('{', iRTFLoc);

                // if there is no property, we'll insert colour table
                // just before the end bracket of the header
                if (iInsertLoc == -1) iInsertLoc = strRTF.IndexOf('}', iRTFLoc) - 1;

                // insert the colour table at our chosen location                
                strRTF = strRTF.Insert(iInsertLoc,
                    // CHANGE THIS STRING TO ALTER COLOUR TABLE
                    "{\\colortbl ;" +
                    "\\red255\\green0\\blue0;" +
                    "\\red0\\green128\\blue0;" +
                    "\\red0\\green0\\blue255;" +
                    "\\red160\\green32\\blue240;" +
                    "\\red255\\green140\\blue0;" +
                    "\\red255\\green20\\blue147;" +
                    "\\red0\\green191\\blue255;" +
                    "\\red0\\green0\blue0;}");
            }

            /*
             * NOW PARSE THROUGH RTF DATA, ADDING RTF COLOUR TAGS WHERE WE WANT THEM
             * In our colour table we defined:
             * cf1 = red  
             * cf2 = green
             * cf3 = blue
             * cf4 = Purple
             * cf5 = Dark Orange
             * cf6 = Deep Pink
             * cf7 = Deep Sky Blue
             * cf8 = Black
             * */

            for (int i = 0; i < strRTF.Length; i++)
            {
                if (strRTF[i] == '%')
                {
                    strRTF = strRTF.Insert(i, "\\cf8 ");
                    i += 6;
                }
                else if (strRTF[i] == 'G')
                {
                    strRTF = strRTF.Insert(i, "\\cf4 ");
                    i += 6;
                }
                else if (strRTF[i] == 'R')
                {
                    strRTF = strRTF.Insert(i, "\\cf7 ");
                    i += 6;
                }
                else if (strRTF[i] == 'N' && strRTF[i + 1] != 'o')
                {
                    strRTF = strRTF.Insert(i, "\\cf5 ");
                    i += 6;
                }
                else if (strRTF[i] == 'T' || strRTF[i] == 'C')
                {
                    strRTF = strRTF.Insert(i, "\\cf7 ");
                    i += 6;
                }
                else if (strRTF[i] == 'F' || strRTF[i] == 'S')
                {
                    strRTF = strRTF.Insert(i, "\\cf4 ");
                    i += 6;
                }
                else if (strRTF[i] == 'C')
                {
                    strRTF = strRTF.Insert(i, "\\cf7 ");
                    i += 6;
                }
                else if (strRTF[i] == 'X' || strRTF[i] == 'Y' || strRTF[i] == 'Z' )
                {
                    //add RTF tags after symbol 
                    //Check for comments tags 
                    strRTF = strRTF.Insert(i + 1, "\\cf3 ");
                    //add RTF before symbol
                    strRTF = strRTF.Insert(i, "\\cf1 ");

                    //skip forward past the characters we've just added
                    //to avoid getting trapped in the loop
                    i += 6;
                }
            }
            MemoryStream stream = new MemoryStream(ASCIIEncoding.Default.GetBytes(strRTF));
            TextRange range = new TextRange(richTextBoxGCode.Document.ContentStart, richTextBoxGCode.Document.ContentEnd);
            range.Load(stream, DataFormats.Rtf);
       
        }


              private void richTextBoxGCode_SelectionChanged(object sender, RoutedEventArgs e)
                 {
                    // SelectLine(index);
                    
                 }

    /* Vu Quan ga vai */
        public void SelectLine(int line)
              {
   
    TextPointer start = richTextBoxGCode.CaretPosition.GetLineStartPosition(line);
    TextPointer stop = richTextBoxGCode.CaretPosition.GetLineStartPosition(line+ 1);
    TextRange textrange = new TextRange(start, stop);
    textrange.ApplyPropertyValue(TextElement.ForegroundProperty, new SolidColorBrush(Colors.Red));
    
              }
        public void exportxlsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /*
             * Excel._Application myExcelApp;
            Excel.Workbooks myExcelWorkbooks;
            Excel.Workbook myExcelWorkbook;
            object misValue = System.Reflection.Missing.Value;

            myExcelApp = new Excel.Application();
            myExcelApp.Visible = true;
            myExcelWorkbooks = myExcelApp.Workbooks;
            
            // Mo file xls
            String fileName = System.IO.Directory.GetCurrentDirectory() + "\\CNCCodes\\CNC.xls";
            myExcelWorkbook = myExcelWorkbooks.Open(fileName, misValue, misValue, misValue, misValue, misValue, misValue, misValue, misValue, misValue, misValue, misValue, misValue, misValue, misValue);

            Excel.Worksheet myExcelWorksheet = (Excel.Worksheet)myExcelWorkbook.ActiveSheet;

            // Doc du lieu trong o A2
            String cellFormulaAsString = myExcelWorksheet.get_Range("A2", misValue).Formula.ToString();
            
            // Ghi du lieu vao o A1
            myExcelWorksheet.get_Range("A1", misValue).Formula = cellFormulaAsString;

            MessageBox.Show(cellFormulaAsString);
            */

                ExcelExample.logic.CreateExcelDoc excell_app = new ExcelExample.logic.CreateExcelDoc();
                //creates the main header
                excell_app.createHeaders(1, 1, "Data for Simulink", "A1", "H1", 7, "Turquoise", true, 10, "n");
                excell_app.createHeaders(1, 9, "Data for Inverse Kinematic", "I1", "N1", 7, "GREEN", true, 10, "n");
                //creates subheaders
                excell_app.createHeaders(2, 1, "Time", "A2", "A2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 2, "Tool", "B2", "B2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 3, "X", "C2", "C2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 4, "Y", "D2", "D2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 5, "Z", "E2", "E2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 6, "S", "F2", "F2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 7, "R", "G2", "G2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 8, "G", "H2", "H2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 9, "Base", "I2", "I2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 10, "Shoulder", "J2", "J2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 11, "Elbow", "K2", "K2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 12, "Pitch", "L2", "L2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 13, "Roll", "M2", "M2", 0, "GRAY", true, 10, "");
                excell_app.createHeaders(2, 14, "Gripper", "N2", "N2", 0, "GRAY", true, 10, "");
                //add Data to cells - Convert XYZ G91 to XYZ G90 for Simulation
                for (i = 0; i < counter; i++)
                {
                    excell_app.addData(i + 3, 1, i.ToString(), "A" + (i + 3).ToString(), "A" + (i + 3).ToString(), "#,##0"); // Time

                    //
                    Data_Tool = new string(Data_Gcode[i, 0]); // Tool Number
                    excell_app.addData(i + 3, 2, Data_Tool, "B" + (i + 3).ToString(), "B" + (i + 3).ToString(), "#,##0");

                    //
                    if ((Data_Gcode[i, 4].ToString() == "1") && (i > 0)) // G91 -> Must be converted to G90
                    {
                        Data_X = (CharArrayToFloat(Data_Gcode[i, 1]) + CharArrayToFloat(Data_Gcode[i - 1, 1])).ToString(); // X

                        Data_Y = (CharArrayToFloat(Data_Gcode[i, 2]) + CharArrayToFloat(Data_Gcode[i - 1, 2])).ToString(); // Y

                        Data_Z = (CharArrayToFloat(Data_Gcode[i, 3]) + CharArrayToFloat(Data_Gcode[i - 1, 3])).ToString(); // Z
                    }
                    else // G90 -> Not convert
                    {
                        Data_X = new string(Data_Gcode[i, 1]); // X
                        Data_Y = new string(Data_Gcode[i, 2]); // Y
                        Data_Z = new string(Data_Gcode[i, 3]); // Z
                        Data_S = new string(Data_Gcode[i, 4]); // S
                        Data_R = new string(Data_Gcode[i, 5]); // R
                        Data_G = new string(Data_Gcode[i, 6]); //G
                        Data_Ba = Convert.ToString(Datafloat_Gcode[i, 1]);

                        Data_Sh = Convert.ToString(Datafloat_Gcode[i, 2]);
                        Data_El = Convert.ToString(Datafloat_Gcode[i, 3]);
                        Data_Pi = Convert.ToString(Datafloat_Gcode[i, 4]);
                        Data_Ro = Convert.ToString(Datafloat_Gcode[i, 5]);
                        Data_Gr = Convert.ToString(Datafloat_Gcode[i, 6]);
                       // testing.Text += Data_Sh;
                    
                    }

                    excell_app.addData(i + 3, 3, "=" + Data_X, "C" + (i + 3).ToString(), "C" + (i + 3).ToString(), "#,##0.000");
                    excell_app.addData(i + 3, 4, "=" + Data_Y, "D" + (i + 3).ToString(), "D" + (i + 3).ToString(), "#,##0.000");
                    excell_app.addData(i + 3, 5, "=" + Data_Z, "E" + (i + 3).ToString(), "E" + (i + 3).ToString(), "#,##0.000");
                    excell_app.addData(i + 3, 6, "=" + Data_S, "F" + (i + 3).ToString(), "F" + (i + 3).ToString(), "#,##0.000");
                    excell_app.addData(i + 3, 7, "=" + Data_R, "G" + (i + 3).ToString(), "G" + (i + 3).ToString(), "#,##0.000");
                    excell_app.addData(i + 3, 8, "=" + Data_G, "H" + (i + 3).ToString(), "H" + (i + 3).ToString(), "#,##0.000");
                    excell_app.addData(i + 3, 9, "=" + Data_Ba, "I" + (i + 3).ToString(), "I" + (i + 3).ToString(), "#,##0.000");
                    excell_app.addData(i + 3, 10, "=" + Data_Sh, "J" + (i + 3).ToString(), "J" + (i + 3).ToString(), "#,##0.000");
                    excell_app.addData(i + 3, 11, "=" + Data_El, "K" + (i + 3).ToString(), "K" + (i + 3).ToString(), "#,##0.000");
                    excell_app.addData(i + 3, 12, "=" + Data_Pi, "L" + (i + 3).ToString(), "L" + (i + 3).ToString(), "#,##0.000");
                    excell_app.addData(i + 3, 13, "=" + Data_Ro, "M" + (i + 3).ToString(), "M" + (i + 3).ToString(), "#,##0.000");
                    excell_app.addData(i + 3, 14, "=" + Data_Gr, "N" + (i + 3).ToString(), "N" + (i + 3).ToString(), "#,##0.000");
                    
                
            }
        }



    }
}

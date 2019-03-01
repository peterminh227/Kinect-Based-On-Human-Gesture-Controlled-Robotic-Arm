using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ZedGraph;
namespace Kinect_Control_USBDevice
{
    public partial class Graph : Form
    {
        public Graph()
        {
            InitializeComponent();
            Data_Init_Base();
            Data_Init_Shoulder();
            Data_Init_Elbow();
        }
        int TickStart = Environment.TickCount;
        int draw = 0;

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (draw == 1)
            {
                DrawBase(txt_base_setpoint.Text, txt_Base_curent.Text);
                DrawShoulder(txt_Shoulder_setpoint.Text,txt_Shoulder_curent.Text);
                DrawElbow(txt_Elbow_Setpoint.Text,txt_Elbow_Curent.Text);
            }
        }
        private void DrawBase(string setpoint, string current)
        {
            double intsetpoint;
            double intcurrent;
            double.TryParse(setpoint, out intsetpoint);
            double.TryParse(current, out intcurrent);
            if (zedGraph1.GraphPane.CurveList.Count <= 0)
                return;

            LineItem curve = zedGraph1.GraphPane.CurveList[0] as LineItem;
            LineItem curve1 = zedGraph1.GraphPane.CurveList[1] as LineItem;
            if (curve == null)
                return;
            if (curve1 == null)
                return;

            IPointListEdit list = curve.Points as IPointListEdit;
            IPointListEdit list1 = curve1.Points as IPointListEdit;
            if (list == null)
                return;
            if (list1 == null)
                return;
            double time = (Environment.TickCount - TickStart) / 250.0;

            list.Add(time, intsetpoint);
            list1.Add(time, intcurrent);

            Scale xScale = zedGraph1.GraphPane.XAxis.Scale;
            if (time > xScale.Max - xScale.MajorStep)
            {
                if (graph_mode == 1)
                {
                    xScale.Max = time + xScale.MajorStep;
                    xScale.Min = xScale.Max - 30.0;
                }
                else
                {
                    xScale.Max = time + xScale.MajorStep;
                    xScale.Min = 0;
                }
            }
            zedGraph1.AxisChange();
            zedGraph1.Invalidate();
        }
        // Graph display mode
        int graph_mode = 1;
        private void rdScroll_CheckedChanged(object sender, EventArgs e)
        {
            if (this.rdScroll.Checked == true)
            {
                graph_mode = 1;
            }
            else graph_mode = 0;
        }
        private void rdCompact_CheckedChanged(object sender, EventArgs e)
        {
            if (this.rdCompact.Checked == true)
            {
                graph_mode = 0;
            }
            else graph_mode = 1;
        }
        private void PbRecord_Click(object sender, EventArgs e)
        {
            if (PbRecord.Text == "RECORD")
            {
                draw = 1;
                PbRecord.Text = "STOP RECORD";
            }
            else
            {
                draw = 0;
                PbRecord.Text = "RECORD";
            }
        }
        private void PbClear_Click(object sender, EventArgs e)
        {
            Clear_Graph();
            TickStart = Environment.TickCount;
        }
        void Clear_Graph()
        {
            zedGraph1.GraphPane.CurveList.Clear();
            zedGraph1.GraphPane.GraphObjList.Clear();
            zedGraph1.AxisChange();
            zedGraph1.Invalidate();
            zedGraph2.GraphPane.CurveList.Clear();
            zedGraph2.GraphPane.GraphObjList.Clear();
            zedGraph2.AxisChange();
            zedGraph2.Invalidate();
            zedGraph3.GraphPane.CurveList.Clear();
            zedGraph3.GraphPane.GraphObjList.Clear();
            zedGraph3.AxisChange();
            zedGraph3.Invalidate();
            Data_Init_Base();
            Data_Init_Shoulder();
            Data_Init_Elbow();
        }
        private void Data_Init_Base()
        {
            //khai bao zed.graph
            GraphPane myPane = zedGraph1.GraphPane;

            myPane.Title.Text = "Position Value";
            myPane.XAxis.Title.Text = "Time,Seconds";
            myPane.YAxis.Title.Text = "Pulse";
            // Khai bao list diem
            // Hien thi 6000 diem, toc do lay mau 10ms => thoi gian 60000
            //khai bao 2 lít diem nen co 2 do thi
            RollingPointPairList list = new RollingPointPairList(60000);
            RollingPointPairList list1 = new RollingPointPairList(60000);
            // Khoi tao ve 2 curve, no symbols
            LineItem curve = myPane.AddCurve("Position_Curent ", list, Color.Blue, SymbolType.None);
            LineItem curve1 = myPane.AddCurve("Position_Set", list1, Color.Red, SymbolType.None);
            // Khoi tao timer de lay mau
            timer1.Interval = 10;
            timer1.Enabled = true;
            timer1.Start();
            // Cai dat hien thi cho truc X
            myPane.XAxis.Scale.Min = 0;//min la 0
            myPane.XAxis.Scale.Max = 30;//Max la 30
            myPane.XAxis.Scale.MinorStep = 1;//Do chia nho nhat la 1
            myPane.XAxis.Scale.MajorStep = 5;// do chia lon nhat la 4
            // gọi hàm xác định cỡ trục
            zedGraph1.AxisChange();

        }
        private void Data_Init_Shoulder()
        {
            //khai bao zed.graph
            GraphPane myPane = zedGraph2.GraphPane;

            myPane.Title.Text = "Graph of Shoulder Joint";
            myPane.XAxis.Title.Text = "Time,Seconds";
            myPane.YAxis.Title.Text = "Pulse";
            // Khai bao list diem
            // Hien thi 6000 diem, toc do lay mau 10ms => thoi gian 60000
            //khai bao 2 lít diem nen co 2 do thi
            RollingPointPairList list = new RollingPointPairList(60000);
            RollingPointPairList list1 = new RollingPointPairList(60000);
            // Khoi tao ve 2 curve, no symbols
            LineItem curve = myPane.AddCurve("Position_Curent ", list, Color.Blue, SymbolType.None);
            LineItem curve1 = myPane.AddCurve("Position_Set", list1, Color.Red, SymbolType.None);
            // Khoi tao timer de lay mau
            timer1.Interval = 10;
            timer1.Enabled = true;
            timer1.Start();
            // Cai dat hien thi cho truc X
            myPane.XAxis.Scale.Min = 0;//min la 0
            myPane.XAxis.Scale.Max = 30;//Max la 30
            myPane.XAxis.Scale.MinorStep = 1;//Do chia nho nhat la 1
            myPane.XAxis.Scale.MajorStep = 5;// do chia lon nhat la 4
            // gọi hàm xác định cỡ trục
            zedGraph2.AxisChange();
            
        }
        private void Data_Init_Elbow()
        {
            //khai bao zed.graph
            GraphPane myPane = zedGraph3.GraphPane;

            myPane.Title.Text = "Graph of Elbow Joint";
            myPane.XAxis.Title.Text = "Time,Seconds";
            myPane.YAxis.Title.Text = "Pulse";
            // Khai bao list diem
            // Hien thi 6000 diem, toc do lay mau 10ms => thoi gian 60000
            //khai bao 2 lít diem nen co 2 do thi
            RollingPointPairList list = new RollingPointPairList(60000);
            RollingPointPairList list1 = new RollingPointPairList(60000);
            // Khoi tao ve 2 curve, no symbols
            LineItem curve = myPane.AddCurve("Position_Curent ", list, Color.Green, SymbolType.None);
            LineItem curve1 = myPane.AddCurve("Position_Set", list1, Color.Red, SymbolType.None);
            // Khoi tao timer de lay mau
            timer1.Interval = 10;
            timer1.Enabled = true;
            timer1.Start();
            // Cai dat hien thi cho truc X
            myPane.XAxis.Scale.Min = 0;//min la 0
            myPane.XAxis.Scale.Max = 30;//Max la 30
            myPane.XAxis.Scale.MinorStep = 1;//Do chia nho nhat la 1
            myPane.XAxis.Scale.MajorStep = 5;// do chia lon nhat la 4
            // gọi hàm xác định cỡ trục
            zedGraph3.AxisChange();

        }
        private void DrawShoulder(string setpoint, string current)
        {
            double intsetpoint;
            double intcurrent;
            double.TryParse(setpoint, out intsetpoint);
            double.TryParse(current, out intcurrent);
            if (zedGraph2.GraphPane.CurveList.Count <= 0)
                return;

            LineItem curve = zedGraph2.GraphPane.CurveList[0] as LineItem;
            LineItem curve1 = zedGraph2.GraphPane.CurveList[1] as LineItem;
            if (curve == null)
                return;
            if (curve1 == null)
                return;

            IPointListEdit list = curve.Points as IPointListEdit;
            IPointListEdit list1 = curve1.Points as IPointListEdit;
            if (list == null)
                return;
            if (list1 == null)
                return;
            double time = (Environment.TickCount - TickStart) / 250.0;

            list.Add(time, intsetpoint);
            list1.Add(time, intcurrent);

            Scale xScale = zedGraph2.GraphPane.XAxis.Scale;
            if (time > xScale.Max - xScale.MajorStep)
            {
                if (graph_mode == 1)
                {
                    xScale.Max = time + xScale.MajorStep;
                    xScale.Min = xScale.Max - 30.0;
                }
                else
                {
                    xScale.Max = time + xScale.MajorStep;
                    xScale.Min = 0;
                }
            }
            zedGraph2.AxisChange();
            zedGraph2.Invalidate();
        }
        private void DrawElbow(string setpoint, string current)
        {
            double intsetpoint;
            double intcurrent;
            double.TryParse(setpoint, out intsetpoint);
            double.TryParse(current, out intcurrent);
            if (zedGraph3.GraphPane.CurveList.Count <= 0)
                return;

            LineItem curve = zedGraph3.GraphPane.CurveList[0] as LineItem;
            LineItem curve1 = zedGraph3.GraphPane.CurveList[1] as LineItem;
            if (curve == null)
                return;
            if (curve1 == null)
                return;

            IPointListEdit list = curve.Points as IPointListEdit;
            IPointListEdit list1 = curve1.Points as IPointListEdit;
            if (list == null)
                return;
            if (list1 == null)
                return;
            double time = (Environment.TickCount - TickStart) / 250.0;

            list.Add(time, intsetpoint);
            list1.Add(time, intcurrent);

            Scale xScale = zedGraph3.GraphPane.XAxis.Scale;
            if (time > xScale.Max - xScale.MajorStep)
            {
                if (graph_mode == 1)
                {
                    xScale.Max = time + xScale.MajorStep;
                    xScale.Min = xScale.Max - 30.0;
                }
                else
                {
                    xScale.Max = time + xScale.MajorStep;
                    xScale.Min = 0;
                }
            }
            zedGraph3.AxisChange();
            zedGraph3.Invalidate();
        }
    }
}

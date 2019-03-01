namespace Kinect_Control_USBDevice
{
    partial class Graph
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.zedGraph1 = new ZedGraph.ZedGraphControl();
            this.PbClear = new System.Windows.Forms.Button();
            this.PbRecord = new System.Windows.Forms.Button();
            this.rdScroll = new System.Windows.Forms.RadioButton();
            this.rdCompact = new System.Windows.Forms.RadioButton();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.txt_base_setpoint = new System.Windows.Forms.TextBox();
            this.txt_Base_curent = new System.Windows.Forms.TextBox();
            this.zedGraph2 = new ZedGraph.ZedGraphControl();
            this.zedGraph3 = new ZedGraph.ZedGraphControl();
            this.txt_Shoulder_setpoint = new System.Windows.Forms.TextBox();
            this.txt_Shoulder_curent = new System.Windows.Forms.TextBox();
            this.txt_Elbow_Setpoint = new System.Windows.Forms.TextBox();
            this.txt_Elbow_Curent = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // zedGraph1
            // 
            this.zedGraph1.Location = new System.Drawing.Point(13, 4);
            this.zedGraph1.Name = "zedGraph1";
            this.zedGraph1.ScrollGrace = 0D;
            this.zedGraph1.ScrollMaxX = 0D;
            this.zedGraph1.ScrollMaxY = 0D;
            this.zedGraph1.ScrollMaxY2 = 0D;
            this.zedGraph1.ScrollMinX = 0D;
            this.zedGraph1.ScrollMinY = 0D;
            this.zedGraph1.ScrollMinY2 = 0D;
            this.zedGraph1.Size = new System.Drawing.Size(365, 209);
            this.zedGraph1.TabIndex = 0;
            // 
            // PbClear
            // 
            this.PbClear.Location = new System.Drawing.Point(604, 305);
            this.PbClear.Name = "PbClear";
            this.PbClear.Size = new System.Drawing.Size(73, 33);
            this.PbClear.TabIndex = 1;
            this.PbClear.Text = "CLEAR";
            this.PbClear.UseVisualStyleBackColor = true;
            this.PbClear.Click += new System.EventHandler(this.PbClear_Click);
            // 
            // PbRecord
            // 
            this.PbRecord.Location = new System.Drawing.Point(604, 219);
            this.PbRecord.Name = "PbRecord";
            this.PbRecord.Size = new System.Drawing.Size(73, 34);
            this.PbRecord.TabIndex = 2;
            this.PbRecord.Text = "RECORD";
            this.PbRecord.UseVisualStyleBackColor = true;
            this.PbRecord.Click += new System.EventHandler(this.PbRecord_Click);
            // 
            // rdScroll
            // 
            this.rdScroll.AutoSize = true;
            this.rdScroll.Location = new System.Drawing.Point(604, 259);
            this.rdScroll.Name = "rdScroll";
            this.rdScroll.Size = new System.Drawing.Size(67, 17);
            this.rdScroll.TabIndex = 3;
            this.rdScroll.TabStop = true;
            this.rdScroll.Text = "SCROLL";
            this.rdScroll.UseVisualStyleBackColor = true;
            this.rdScroll.CheckedChanged += new System.EventHandler(this.rdScroll_CheckedChanged);
            // 
            // rdCompact
            // 
            this.rdCompact.AutoSize = true;
            this.rdCompact.Location = new System.Drawing.Point(604, 282);
            this.rdCompact.Name = "rdCompact";
            this.rdCompact.Size = new System.Drawing.Size(77, 17);
            this.rdCompact.TabIndex = 4;
            this.rdCompact.TabStop = true;
            this.rdCompact.Text = "COMPACT";
            this.rdCompact.UseVisualStyleBackColor = true;
            this.rdCompact.CheckedChanged += new System.EventHandler(this.rdCompact_CheckedChanged);
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // txt_base_setpoint
            // 
            this.txt_base_setpoint.Location = new System.Drawing.Point(412, 234);
            this.txt_base_setpoint.Name = "txt_base_setpoint";
            this.txt_base_setpoint.Size = new System.Drawing.Size(103, 20);
            this.txt_base_setpoint.TabIndex = 5;
            this.txt_base_setpoint.Visible = false;
            // 
            // txt_Base_curent
            // 
            this.txt_Base_curent.Location = new System.Drawing.Point(412, 271);
            this.txt_Base_curent.Name = "txt_Base_curent";
            this.txt_Base_curent.Size = new System.Drawing.Size(103, 20);
            this.txt_Base_curent.TabIndex = 6;
            this.txt_Base_curent.Visible = false;
            // 
            // zedGraph2
            // 
            this.zedGraph2.Location = new System.Drawing.Point(422, 4);
            this.zedGraph2.Name = "zedGraph2";
            this.zedGraph2.ScrollGrace = 0D;
            this.zedGraph2.ScrollMaxX = 0D;
            this.zedGraph2.ScrollMaxY = 0D;
            this.zedGraph2.ScrollMaxY2 = 0D;
            this.zedGraph2.ScrollMinX = 0D;
            this.zedGraph2.ScrollMinY = 0D;
            this.zedGraph2.ScrollMinY2 = 0D;
            this.zedGraph2.Size = new System.Drawing.Size(365, 209);
            this.zedGraph2.TabIndex = 7;
            // 
            // zedGraph3
            // 
            this.zedGraph3.Location = new System.Drawing.Point(13, 234);
            this.zedGraph3.Name = "zedGraph3";
            this.zedGraph3.ScrollGrace = 0D;
            this.zedGraph3.ScrollMaxX = 0D;
            this.zedGraph3.ScrollMaxY = 0D;
            this.zedGraph3.ScrollMaxY2 = 0D;
            this.zedGraph3.ScrollMinX = 0D;
            this.zedGraph3.ScrollMinY = 0D;
            this.zedGraph3.ScrollMinY2 = 0D;
            this.zedGraph3.Size = new System.Drawing.Size(366, 223);
            this.zedGraph3.TabIndex = 8;
            // 
            // txt_Shoulder_setpoint
            // 
            this.txt_Shoulder_setpoint.Enabled = false;
            this.txt_Shoulder_setpoint.Location = new System.Drawing.Point(412, 313);
            this.txt_Shoulder_setpoint.Name = "txt_Shoulder_setpoint";
            this.txt_Shoulder_setpoint.Size = new System.Drawing.Size(102, 20);
            this.txt_Shoulder_setpoint.TabIndex = 9;
            // 
            // txt_Shoulder_curent
            // 
            this.txt_Shoulder_curent.Enabled = false;
            this.txt_Shoulder_curent.Location = new System.Drawing.Point(412, 350);
            this.txt_Shoulder_curent.Name = "txt_Shoulder_curent";
            this.txt_Shoulder_curent.Size = new System.Drawing.Size(102, 20);
            this.txt_Shoulder_curent.TabIndex = 10;
            // 
            // txt_Elbow_Setpoint
            // 
            this.txt_Elbow_Setpoint.Enabled = false;
            this.txt_Elbow_Setpoint.Location = new System.Drawing.Point(412, 381);
            this.txt_Elbow_Setpoint.Name = "txt_Elbow_Setpoint";
            this.txt_Elbow_Setpoint.Size = new System.Drawing.Size(102, 20);
            this.txt_Elbow_Setpoint.TabIndex = 11;
            // 
            // txt_Elbow_Curent
            // 
            this.txt_Elbow_Curent.Enabled = false;
            this.txt_Elbow_Curent.Location = new System.Drawing.Point(414, 419);
            this.txt_Elbow_Curent.Name = "txt_Elbow_Curent";
            this.txt_Elbow_Curent.Size = new System.Drawing.Size(99, 20);
            this.txt_Elbow_Curent.TabIndex = 12;
            // 
            // Graph
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(814, 469);
            this.Controls.Add(this.txt_Elbow_Curent);
            this.Controls.Add(this.txt_Elbow_Setpoint);
            this.Controls.Add(this.txt_Shoulder_curent);
            this.Controls.Add(this.txt_Shoulder_setpoint);
            this.Controls.Add(this.zedGraph3);
            this.Controls.Add(this.zedGraph2);
            this.Controls.Add(this.txt_Base_curent);
            this.Controls.Add(this.txt_base_setpoint);
            this.Controls.Add(this.rdCompact);
            this.Controls.Add(this.rdScroll);
            this.Controls.Add(this.PbRecord);
            this.Controls.Add(this.PbClear);
            this.Controls.Add(this.zedGraph1);
            this.Name = "Graph";
            this.Text = "Graph";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public ZedGraph.ZedGraphControl zedGraph1;
        public System.Windows.Forms.Button PbClear;
        public System.Windows.Forms.Button PbRecord;
        public System.Windows.Forms.RadioButton rdScroll;
        public System.Windows.Forms.RadioButton rdCompact;
        public System.Windows.Forms.Timer timer1;
        public System.Windows.Forms.TextBox txt_base_setpoint;
        public System.Windows.Forms.TextBox txt_Base_curent;
        public ZedGraph.ZedGraphControl zedGraph2;
        public ZedGraph.ZedGraphControl zedGraph3;
        public System.Windows.Forms.TextBox txt_Shoulder_setpoint;
        public System.Windows.Forms.TextBox txt_Shoulder_curent;
        public System.Windows.Forms.TextBox txt_Elbow_Setpoint;
        public System.Windows.Forms.TextBox txt_Elbow_Curent;
    }
}
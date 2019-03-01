using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace Kinect_Control_USBDevice
{
    public class ContextPoint
    {
        public DateTime Time
        {
            get;
            set;
        }

        public Vector3 Position
        {
            get;
            set;
        }
    }
}

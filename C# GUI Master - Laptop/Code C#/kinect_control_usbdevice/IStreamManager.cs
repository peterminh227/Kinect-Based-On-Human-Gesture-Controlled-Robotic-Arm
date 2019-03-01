using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace Kinect_Control_USBDevice
{
    public interface IStreamManager
    {
        WriteableBitmap Bitmap { get; }
    }
}

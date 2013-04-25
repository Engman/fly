using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Interop;

namespace FlyEditUI
{
	public partial class FlyEdit
	{
		private void RenderWindow_MouseClick(object sender, MouseEventArgs e)
		{
			this.FlyCLI.Pick(e.X, e.Y);
		}
	}
}

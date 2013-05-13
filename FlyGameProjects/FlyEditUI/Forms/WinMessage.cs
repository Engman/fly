using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FlyEditUI
{
	partial class FlyEdit	:IMessageFilter
	{
		const int WM_LBUTTONUP		= 0x0202;
		const int WM_RBUTTONUP		= 0x0205;
		const int WM_MBUTTONUP		= 0x0208;
		const int WM_LBUTTONDOWN	= 0x0201;
		const int WM_RBUTTONDOWN	= 0x0204;
		const int WM_MBUTTONDOWN	= 0x0207;
		const int WM_INPUT			= 0x00FF;

		public bool PreFilterMessage(ref Message m)
		{
			switch (m.Msg)
			{
				case WM_INPUT:
					
				break;
				case WM_LBUTTONUP:
					
				break;
				case WM_RBUTTONUP:
				
				break;
				case WM_MBUTTONUP:
				
				break;
				case WM_LBUTTONDOWN:
					
				break;
				case WM_RBUTTONDOWN:

				break;
				case WM_MBUTTONDOWN:

				break;
			}
			return false;

		}
	}
}

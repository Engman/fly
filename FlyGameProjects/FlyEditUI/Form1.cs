using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System;
using System.Windows.Interop;

namespace FlyEditUI
{
	public partial class Form1 : Form
	{
		FlyEditCLIWrapper FlyCLI = null;

		public Form1()
		{
			InitializeComponent();
			this.FlyCLI = new FlyEditCLIWrapper();
		}

		public void Run()
		{
			this.FlyCLI.Init(RenderWin.Handle, RenderWin.Width, RenderWin.Height);

			while (this.Created)
			{
				this.FlyCLI.ProcessFrame();
				Application.DoEvents();
			}
		}
	}
}

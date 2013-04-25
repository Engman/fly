using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Interop;

namespace FlyEditUI
{
	public partial class FlyEdit : Form
	{
		FlyEditCLIWrapper FlyCLI = null;
		Output outWin = null;
		bool engineRuning = false;

		public FlyEdit()
		{
			InitializeComponent();
			this.FlyCLI = new FlyEditCLIWrapper();
			this.outWin = new Output();
		}

		public void Run()
		{
			this.outWin.Show();
			this.outWin.SetDesktopLocation(this.Location.X + this.Size.Width, this.Location.Y);

			if (!this.FlyCLI.Init(RenderWin.Handle, RenderWin.Width, RenderWin.Height))
			{
				MessageBox.Show(this, "Failed to initialize core");
				this.Close() ;
			}
			else
			{
				this.outWin.addText("Core initialized!");
				engineRuning = true;
			}
			while (this.Created)
			{
				if (this.engineRuning)
					this.FlyCLI.ProcessFrame();
				
				Application.DoEvents();
			}
		}

	}
}

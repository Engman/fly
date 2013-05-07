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
		FlyEditCLIWrapper flyCLI = null;

		public FlyEditCLIWrapper FlyCLI
		{
			get { return flyCLI; }
		}
		

		Output outWin = null;
		bool engineRuning = false;
		

		public FlyEdit()
		{
			InitializeComponent();

			this.flyCLI = new FlyEditCLIWrapper();
			this.outWin = new Output();
			this.Move += new EventHandler(MoveSubForm);
			this.CameraDropBox.Select(0, 1);
		}

		public void Run()
		{
			this.outWin.Show(this);
			this.outWin.SetDesktopLocation(this.Location.X + this.Size.Width, this.Location.Y);
			this.outWin.TopLevel = true;
			
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

				int s = this.GetCurrentResource();
				
				Application.DoEvents();
			}
		}
	}
}

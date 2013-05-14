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
	
		Output outWin = null;
		bool engineRuning = false;
		bool terminate = false;
		public static bool renderWinLocked = true;
		ToolTip pictureBoxLockTooltip = new ToolTip();
		Point lastMousePos = new Point(0, 0);
		int currentSelectedID = -1;

		LevelGenerator levelGen = null;
		

		public FlyEdit()
		{
			InitializeComponent();

			this.flyCLI = new FlyEditCLIWrapper();
			this.outWin = new Output();
			Application.AddMessageFilter(this);
			this.Move += new EventHandler(MoveSubForm);
			this.levelGen = new LevelGenerator();
		}


		public void Run()
		{
			while (!this.terminate)
			{
				if (this.engineRuning)
				{
					this.flyCLI.ProcessFrame();
					this.UpdateCoreData();
				}
				Application.DoEvents();
			}

			Application.RemoveMessageFilter(this);
		}


		public bool InitiateCore()
		{
			this.outWin.Show(this);
			this.outWin.SetDesktopLocation(this.Location.X + this.Size.Width, this.Location.Y);
			this.outWin.TopLevel = true;

			if (!this.flyCLI.Init(RenderWin.Handle, RenderWin.Width, RenderWin.Height))
			{
				MessageBox.Show(this, "Failed to initialize core");
				return false;
			}
			else
			{
				this.outWin.addText("Core initialized!");
				engineRuning = true;
				
				this.flyCLI.GetCameras(this.levelGen.Cameras);
				string[] camNames = this.levelGen.Cameras.Keys.ToArray();
				for (int i = 0; i < camNames.Length; i++)
				{
					this.CameraDropBox.Items.Add(camNames[i]);
				}
				this.CameraDropBox.SelectedIndex = 0;
			}

			return true;
		}


		void UpdateCoreData()
		{
			String name = "";
			int id = -1;
			float rotX = 0.0f;
			float rotY = 0.0f;
			float rotZ = 0.0f;
			float scaleX = 0.0f;
			float scaleY = 0.0f;
			float scaleZ = 0.0f;
			this.flyCLI.GetSelected(ref name, ref id, ref rotX, ref rotY, ref rotZ, ref scaleX, ref scaleY, ref scaleZ);

			if (id != -1 && this.currentSelectedID != id)
			{
				this.currentSelectedID = id;
				this.textBox_SelectedName.Text = name;
				oldName = name;
				this.trackBar_RotationX.Value = (int)rotX;
				this.trackBar_RotationY.Value = (int)rotY;
				this.trackBar_RotationZ.Value = (int)rotZ;

				this.trackBar_ScaleX.Value = (int)scaleX;
				this.trackBar_ScaleY.Value = (int)scaleY;
				this.trackBar_ScaleZ.Value = (int)scaleZ;
			}

			if (!this.flyCLI.GetFlyState() && !renderWinLocked)
				RenderLockPictureLockClicked(null, null);
		}

		

	}
}

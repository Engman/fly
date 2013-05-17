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
		public enum ReourceNodeIndex
		{
			NodeIndex_Mesh,
			NodeIndex_Pickup,
			NodeIndex_Light,
			NodeIndex_Camera,
			NodeIndex_Terrain,
		}

		public static readonly int KEY_W				= 0x57;
		public static readonly int KEY_A				= 0x41;
		public static readonly int KEY_S				= 0x53;
		public static readonly int KEY_D				= 0x44;
		public static readonly int KEY_ESC				= 0x1B;
		public static readonly int KEY_SPACE			= 0x20;
		public static readonly int KEY_LCTRL			= 0xA2;
		public static readonly int KEY_RCTRL			= 0xA3;
		public static readonly int KEY_ALT				= 0x12;
		public static readonly int KEY_LSHIFT			= 0xA0;
		public static readonly int KEY_RSHIFT			= 0xA1;
		public static readonly int KEY_MOUSE_LBTN		= 0x01;
		public static readonly int KEY_MOUSE_RBTN		= 0x02;
		public static readonly int KEY_MOUSE_MBTN		= 0x04;

		public static FlyEditCLIWrapper flyCLI = null;
		
		Output outWin = null;
		bool engineRuning = false;
		bool terminate = false;
		public static bool renderWinLocked = true;
		ToolTip pictureBoxLockTooltip = new ToolTip();
		Point lastMousePos = new Point(0, 0);
		int currentSelectedID = -1;
		bool scale = false;
		bool rotate = false;
		LevelGenerator levelGen = null;
		

		public FlyEdit()
		{
			InitializeComponent();

			FlyEdit.flyCLI = new FlyEditCLIWrapper();
			this.outWin = new Output();
			this.levelGen = new LevelGenerator();
			this.KeyPreview = true;

			this.Move += new EventHandler(MoveSubForm);


			this.KeyUp		+= new KeyEventHandler(FlyEdit_KeyUpEvent);


			(this.RenderWin as Control).MouseWheel	+= new MouseEventHandler(RenderWin_MouseScrollEvent);
			(this.RenderWin as Control).MouseMove	+= new MouseEventHandler(RenderWin_MouseMoveEvent);
			(this.RenderWin as Control).MouseDown	+= new MouseEventHandler(RenderWin_MouseDownEvent);
			(this.RenderWin as Control).MouseUp		+= new MouseEventHandler(RenderWin_MouseUpEvent);
			(this.RenderWin as Control).KeyDown		+= new KeyEventHandler(RenderWin_KeyDownEvent);
			(this.RenderWin as Control).KeyUp		+= new KeyEventHandler(RenderWin_KeyUpEvent);
	
			
			this.trackBar_ScaleX.ValueChanged		+= new EventHandler(trackBar_Scale_Scroll);
			this.trackBar_ScaleY.ValueChanged		+= new EventHandler(trackBar_Scale_Scroll);
			this.trackBar_ScaleZ.ValueChanged		+= new EventHandler(trackBar_Scale_Scroll);
			this.trackBar_RotationX.ValueChanged	+= new EventHandler(trackBar_Rotation_Scroll);
			this.trackBar_RotationY.ValueChanged	+= new EventHandler(trackBar_Rotation_Scroll);
			this.trackBar_RotationZ.ValueChanged	+= new EventHandler(trackBar_Rotation_Scroll);

			this.panel_Geometry.Location = new Point(6, 229);
			this.panel_Lights.Location = new Point(6, 229);
			this.panel_Pickups.Location = new Point(6, 229);
			this.panel_Terrain.Location = new Point(6, 229);
			this.panel_Camera.Location = new Point(6, 229);
		}

	
		public void Run()
		{
			Timer t = new Timer();
			t.Interval = 100;
			t.Start();
			t.Tick += this.UpdateCoreData;
			while (!this.terminate)
			{
				if (this.engineRuning)
				{
					FlyEdit.flyCLI.ProcessFrame();
				}
				Application.DoEvents();
			}
		}


		public bool InitiateCore()
		{
			this.outWin.Show(this);
			this.outWin.SetDesktopLocation(this.Location.X + this.Size.Width, this.Location.Y);
			this.outWin.TopLevel = true;

			if (!FlyEdit.flyCLI.Init(RenderWin.Handle, RenderWin.Width, RenderWin.Height))
			{
				MessageBox.Show(this, "Failed to initialize core");
				return false;
			}
			else
			{
				this.outWin.addText("Core initialized!");
				engineRuning = true;

				FlyEdit.flyCLI.GetCameras(this.levelGen.Cameras);
				string[] camNames = this.levelGen.Cameras.Keys.ToArray();
				for (int i = 0; i < camNames.Length; i++)
				{
					this.ResourceTree.Nodes[3].Nodes.Add(camNames[i]);
				}
			}

			return true;
		}


		void UpdateCoreData(object sender, EventArgs e)
		{
			String name = "";
			int id = -1;
			float rotX = 0.0f;
			float rotY = 0.0f;
			float rotZ = 0.0f;
			float scaleX = 0.0f;
			float scaleY = 0.0f;
			float scaleZ = 0.0f;
			float speed = 0.0f;
			FlyEdit.flyCLI.GetSpeed(ref speed);
			this.CameraSpeedSelector.Value = (Decimal)((float)speed * 100.0f);
			FlyEdit.flyCLI.GetSelected(ref name, ref id, ref rotX, ref rotY, ref rotZ, ref scaleX, ref scaleY, ref scaleZ);

			if (id != -1 && id != currentSelectedID)
			{
				this.label_selectedName.Text = "Selected: " + name;
				oldName = name;
				if (!this.rotate)
				{
					this.trackBar_RotationX.Value = (int)(rotX * (180.0f / (float)Math.PI));
					this.trackBar_RotationY.Value = (int)(rotY * (180.0f / (float)Math.PI));
					this.trackBar_RotationZ.Value = (int)(rotZ * (180.0f / (float)Math.PI));
				}
				if (!this.scale)
				{
					this.trackBar_ScaleX.Value = (int)(scaleX * 1000.0f);
					this.trackBar_ScaleY.Value = (int)(scaleY * 1000.0f);
					this.trackBar_ScaleZ.Value = (int)(scaleZ * 1000.0f);
				}
				this.ActiveControl = this.RenderWin;
			}
			else if (id == -1)
			{
				this.label_selectedName.Text = "Selected: null";
				if (!this.scale)
				{
					this.trackBar_ScaleX.Value = 1;
					this.trackBar_ScaleY.Value = 1;
					this.trackBar_ScaleZ.Value = 1;
				}
				if (!this.rotate)
				{
					this.trackBar_RotationX.Value = 0;
					this.trackBar_RotationY.Value = 0;
					this.trackBar_RotationZ.Value = 0;
				}
			}
			this.currentSelectedID = id;
		}

	}
}

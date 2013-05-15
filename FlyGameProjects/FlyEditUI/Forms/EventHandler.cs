using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Interop;
using System.Threading;
using System.Resources;
using System.Drawing;

namespace FlyEditUI { public partial class FlyEdit {


	String oldName = "";
	bool alt = false;
	bool shift = false;
	bool ctrl = false;

	private void loadGeomrtyToolStripMenuItem_Click(object sender, EventArgs e)
	{
		OpenFileDialog f = new OpenFileDialog();
		String temp = Directory.GetCurrentDirectory();
		temp = temp.Substring(0, temp.LastIndexOf('\\'));
		temp += "\\Resources\\Models";
		f.InitialDirectory = temp;
		f.Filter = "Fly Game Model|*.fgm";
		f.Multiselect = true;
		
		DialogResult result = f.ShowDialog();
		if (result != System.Windows.Forms.DialogResult.OK)
			return;
		else
		{
			
			Dictionary<string, int> objects = new Dictionary<string,int>();

			if(!FlyEdit.flyCLI.LoadResources(f.FileNames, objects))
				MessageBox.Show("Failed to load reources!");
			
			this.outWin.addText(objects.Count.ToString() + " models loaded!");
			this.AppendNewData(objects);
		}
	}
	private void MoveSubForm(object sender, EventArgs e)
	{
		if (this.outWin != null)
		{
			this.outWin.Left = this.Left + this.Width - 1;
			this.outWin.Top = this.Top;
		}
	}
	private void WindowResizeBegin(object sender, EventArgs e)
	{
		if (this.WindowState == FormWindowState.Maximized)
		{
			//this.outWin.WindowState = FormWindowState.Normal;
			//this.flyCLI.OnResize(this.RenderWin.Width, this.RenderWin.Height);
		}
		else if (this.WindowState == FormWindowState.Normal)
		{
			//this.outWin.WindowState = FormWindowState.Normal;
			//this.flyCLI.OnResize(this.RenderWin.Width, this.RenderWin.Height);
		}
	}
	private void WindowResizeEnd(object sender, EventArgs e)
	{
		FlyEdit.flyCLI.OnResize(this.RenderWin.Width, this.RenderWin.Height);
	}
	private void outputWindowToolStripMenuItem_Click(object sender, EventArgs e)
	{
		if (!this.outWin.Visible)
		{
			this.outWin.Show();
		}
	}
	private void FormClosingEvent(object sender, FormClosingEventArgs e)
	{
		this.engineRuning = false;
		this.terminate = true;
	}
	private void exitToolStripMenuItem_Click(object sender, EventArgs e)
	{
		this.engineRuning = false;
		this.terminate = true;
	}
	private void RenderLockPictureLockClicked(object sender, EventArgs e)
	{
		if (renderWinLocked)
		{

			this.RenderLockPictureBox.Image = (Image)FlyEditUI.Properties.Resources.ResourceManager.GetObject("opend");
			FlyEdit.renderWinLocked = false;
			Cursor.Hide();
			Point location = PointToScreen(this.RenderWin.Location);
			Rectangle rect = new Rectangle(location, this.RenderWin.Size);
			Cursor.Clip = rect;
			FlyEdit.flyCLI.SetFlyMode(true);
			this.ActiveControl = this.RenderWin;
		}
		else
		{
			this.RenderLockPictureBox.Image = (Image)FlyEditUI.Properties.Resources.ResourceManager.GetObject("locked");
			FlyEdit.renderWinLocked = true;
			Cursor.Show();
			Cursor.Clip = SystemInformation.VirtualScreen ;
			FlyEdit.flyCLI.SetFlyMode(false);
		}
	}
	private void RenderLockPictureHover(object sender, EventArgs e)
	{
		this.pictureBoxLockTooltip.Show("Turns 3D navigation mode on/off (Shortcut Esc)", this);
	}
	private void FormLoadedEvent(object sender, EventArgs e)
	{
		this.pictureBoxLockTooltip.SetToolTip(this.RenderLockPictureBox, "Turns 3D navigation mode on/off (Shortcut Esc)");
	}
	private void KeyPressEvent(object sender, KeyPressEventArgs e)
	{
		
		RenderLockPictureLockClicked(null, null);
	}
	private void CameraDropBox_SelectedIndexChanged(object sender, EventArgs e)
	{
		FlyEdit.flyCLI.ChangeView(this.levelGen.Cameras[((ComboBox)sender).SelectedItem.ToString()]);
		this.ActiveControl = this.RenderWin;
		
	}
	private void AppendNewData(Dictionary<String, int> data)
	{

		Dictionary<string, int>.Enumerator it = data.GetEnumerator();

		while (it.MoveNext())
		{
			this.levelGen.MeshResource.Add(it.Current.Key, it.Current.Value);

			this.LoadedMesh_name.Items.Add(it.Current.Key);
			//this.listBox_id.Items.Add(it.Current.Value);
		}
	}
	private void splitter1_SplitterMoved(object sender, SplitterEventArgs e)
	{
		FlyEdit.flyCLI.OnResize(this.RenderWin.Width, this.RenderWin.Height);
	}
	private void CurrentObjChanged(object sender, EventArgs e)
	{
		int val = -1;

		if (this.LoadedMesh_name.SelectedIndex != -1)
		{
			val = this.levelGen.MeshResource[((string)this.LoadedMesh_name.SelectedItem)];
		}

		if (val != -1)
		{
			if (!FlyEdit.flyCLI.SelectObject(val))
				MessageBox.Show("Could not select entity");
		}
		this.ActiveControl = this.RenderWin;
	}
	private void shutdownEngineToolStripMenuItem_Click(object sender, EventArgs e)
	{
		if (this.engineRuning)		((ToolStripMenuItem)sender).Text = "Resume rendering";
		else						((ToolStripMenuItem)sender).Text = "Suspend rendering";
		this.engineRuning = !this.engineRuning;
	}
	private void NewtoolStripMenuItem_Click(object sender, EventArgs e)
	{
		this.engineRuning = false;
		//this.flyCLI.Reset();
		//this.Reset();
		this.engineRuning = true;
	}
	private void saveToolStripMenuItem_Click(object sender, EventArgs e)
	{
		SaveFileDialog sd = new SaveFileDialog();
		sd.DefaultExt = "fl";
		sd.Filter = "Fly level file|*.fl";
		sd.AddExtension = true;
		if (sd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
		{
			
		}
	}
	private void loadToolStripMenuItem_Click(object sender, EventArgs e)
	{
		OpenFileDialog od = new OpenFileDialog();
		od.DefaultExt = "fl";

		if (od.ShowDialog() == System.Windows.Forms.DialogResult.OK)
		{
			if (FlyEdit.flyCLI.LoadLevel(od.FileName, this.levelGen.MeshResource, this.levelGen.Lights, this.levelGen.Pickups))
			{
				
			}
			else
			{
				MessageBox.Show("Failed to load level");
			}
		}
		
	}
	private void trackBar_Rotation_Scroll(object sender, EventArgs e)
	{
		float x = this.trackBar_RotationX.Value * 0.017777777777777777777777777777778f;
		float y = this.trackBar_RotationY.Value * 0.017777777777777777777777777777778f;
		float z = this.trackBar_RotationZ.Value * 0.017777777777777777777777777777778f;

		FlyEdit.flyCLI.SetRotation(x, y, z);
		this.ActiveControl = this.RenderWin;
	}
	private void trackBar_Scale_Scroll(object sender, EventArgs e)
	{
		float x = 1.0f;
		float y = 1.0f;
		float z = 1.0f;

		if (this.checkBox_uniformScale.Checked)
		{
			x += ((TrackBar)sender).Value * 0.01f;
			y += ((TrackBar)sender).Value * 0.01f;
			z += ((TrackBar)sender).Value * 0.01f;

			int val = ((TrackBar)sender).Value;
			this.trackBar_ScaleX.Value = val;
			this.trackBar_ScaleY.Value = val;
			this.trackBar_ScaleZ.Value = val;
		}
		else
		{
			x += this.trackBar_ScaleX.Value * 0.005f;
			y += this.trackBar_ScaleY.Value * 0.005f;
			z += this.trackBar_ScaleZ.Value * 0.005f;
		}

		FlyEdit.flyCLI.SetScale(x, y, z);
		this.ActiveControl = this.RenderWin;
	}
	private void checkBox_uniformScale_CheckedChanged(object sender, EventArgs e)
	{
		if (!this.checkBox_uniformScale.Checked)
		{
			this.trackBar_ScaleX.Value = 1;
			this.trackBar_ScaleY.Value = 1;
			this.trackBar_ScaleZ.Value = 1;
		}
		this.ActiveControl = this.RenderWin;	
	}

	
	
	private void RenderWin_MouseDownEvent(object sender, MouseEventArgs e)
	{
		int key = -1;
			
		if (e.Button.Equals(MouseButtons.Left)) key = KEY_MOUSE_LBTN;
		else if (e.Button.Equals(MouseButtons.Right)) key = KEY_MOUSE_RBTN;
		else if (e.Button.Equals(MouseButtons.Middle)) key = KEY_MOUSE_MBTN;

		FlyEdit.flyCLI.OnMouseBtnEvent(key, false, this.ctrl, this.shift, this.alt);
	}
	private void RenderWin_MouseUpEvent(object sender, MouseEventArgs e)
	{
		int key = -1;

		if (e.Button.Equals(MouseButtons.Left)) key = KEY_MOUSE_LBTN;
		else if (e.Button.Equals(MouseButtons.Right)) key = KEY_MOUSE_RBTN;
		else if (e.Button.Equals(MouseButtons.Middle)) key = KEY_MOUSE_MBTN;

		FlyEdit.flyCLI.OnMouseBtnEvent(key, true, this.ctrl, this.shift, this.alt);
	}
	private void RenderWin_MouseMoveEvent(object sender, MouseEventArgs e)
	{
		FlyEdit.flyCLI.OnMouseMoveEvent(e.X, e.Y, e.X - this.lastMousePos.X, e.Y - this.lastMousePos.Y, this.ctrl, this.shift, this.alt);

		this.lastMousePos.X = e.X;
		this.lastMousePos.Y = e.Y;
	}
	private void RenderWin_MouseScrollEvent(object sender, MouseEventArgs e)
	{
		if (sender.Equals(this.RenderWin))
		{
			FlyEdit.flyCLI.OnMouseScrollEvent(e.Delta, this.ctrl, this.shift, this.alt);
		}
	}
	private void RenderWin_KeyDownEvent(object sender, KeyEventArgs e)
	{
		this.shift = e.Shift;
		this.ctrl = e.Control;
		this.alt = e.Alt;
		int key = -1;

		if (!FlyEdit.renderWinLocked)
		{
			if (e.KeyCode.Equals(Keys.W)) key = KEY_W;
			else if (e.KeyCode.Equals(Keys.A)) key = KEY_A;
			else if (e.KeyCode.Equals(Keys.S)) key = KEY_S;
			else if (e.KeyCode.Equals(Keys.D)) key = KEY_D;
		}

		FlyEdit.flyCLI.OnKeyEvent(key, false, this.ctrl, this.shift, this.alt);

	}
	private void RenderWin_KeyUpEvent(object sender, KeyEventArgs e)
	{
		int key = -1;
		
			 if (e.KeyCode.Equals(Keys.W))		key = KEY_W;
		else if (e.KeyCode.Equals(Keys.A))		key = KEY_A;
		else if (e.KeyCode.Equals(Keys.S))		key = KEY_S;
		else if (e.KeyCode.Equals(Keys.D))		key = KEY_D;
		else if (e.KeyCode.Equals(Keys.Escape)) key = KEY_ESC;
		
		FlyEdit.flyCLI.OnKeyEvent(key, true, this.ctrl, this.shift, this.alt);
		
	}


	private void FlyEdit_KeyUpEvent(object sender, KeyEventArgs e)
	{
		this.shift = e.Shift;
		this.ctrl = e.Control;
		this.alt = e.Alt;

		if (e.KeyCode.Equals(Keys.Escape))
		{
			this.RenderLockPictureLockClicked(null, null);
		}
		
	}

	
	private void CameraSpeedSelector_ValueChanged(object sender, EventArgs e)
	{
		this.ActiveControl = null;
		float _s = (float)this.CameraSpeedSelector.Value * 0.01f;
		FlyEdit.flyCLI.SetSpeed(_s);
	}


}}

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

			if(!this.flyCLI.LoadResources(f.FileNames, objects))
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
		this.flyCLI.OnResize(this.RenderWin.Width, this.RenderWin.Height);
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
			this.lastMousePos = Cursor.Position;
			Cursor.Hide();
			Point location = PointToScreen(this.RenderWin.Location);
			Rectangle rect = new Rectangle(location, this.RenderWin.Size);
			Cursor.Clip = rect;
			this.flyCLI.FreeFly(true);
		}
		else
		{
			this.RenderLockPictureBox.Image = (Image)FlyEditUI.Properties.Resources.ResourceManager.GetObject("locked");
			FlyEdit.renderWinLocked = true;
			Cursor.Show();
			Cursor.Clip = SystemInformation.VirtualScreen ;

			Cursor.Position = this.lastMousePos;
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
		this.flyCLI.ChangeView(this.levelGen.Cameras[((ComboBox)sender).SelectedItem.ToString()]);
		this.ActiveControl = null;
		
	}

	public void AppendNewData(Dictionary<String, int> data)
	{

		Dictionary<string, int>.Enumerator it = data.GetEnumerator();

		while (it.MoveNext())
		{
			this.levelGen.MeshResource.Add(it.Current.Key, it.Current.Value);

			this.LoadedResources_name.Items.Add(it.Current.Key);
			//this.listBox_id.Items.Add(it.Current.Value);
		}
	}

	private void splitter1_SplitterMoved(object sender, SplitterEventArgs e)
	{
		this.flyCLI.OnResize(this.RenderWin.Width, this.RenderWin.Height);
	}

	private void CurrentObjChanged(object sender, EventArgs e)
	{
		int val = -1;

		if (this.LoadedResources_name.SelectedIndex != -1)
		{
			val = this.levelGen.MeshResource[((string)this.LoadedResources_name.SelectedItem)];
		}

		if (val != -1)
		{
			if (!this.flyCLI.SelectObject(val))
				MessageBox.Show("Could not select entity");
		}
		this.ActiveControl = null;
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

		}
		
	}

	private void trackBar_Rotation_Scroll(object sender, EventArgs e)
	{
		float x = this.trackBar_RotationX.Value * 0.017777777777777777777777777777778f;
		float y = this.trackBar_RotationY.Value * 0.017777777777777777777777777777778f;
		float z = this.trackBar_RotationZ.Value * 0.017777777777777777777777777777778f;

		this.flyCLI.SetRotation(x, y, z);
		this.ActiveControl = null;
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

		this.flyCLI.SetScale(x, y, z);
		this.ActiveControl = null;
	}

	private void checkBox_uniformScale_CheckedChanged(object sender, EventArgs e)
	{
		if (!this.checkBox_uniformScale.Checked)
		{
			this.trackBar_ScaleX.Value = 1;
			this.trackBar_ScaleY.Value = 1;
			this.trackBar_ScaleZ.Value = 1;
		}
		this.ActiveControl = null;	
	}

	private void textBox_SelectedName_TextChanged(object sender, EventArgs e)
	{
		if (this.LoadedResources_name.Items.Count == 0)
		{
			return;
		}

		if (this.textBox_SelectedName.Text.Length > 3 && this.textBox_SelectedName.Text != "" && this.textBox_SelectedName.Text != oldName)
		{
			if(oldName != "")
				this.button_nameApply.Enabled = true;
		}
	}

	private void button_nameApply_Click(object sender, EventArgs e)
	{
		if (this.flyCLI.SetName(this.textBox_SelectedName.Text))
		{
			
			for (int i = 0; i < this.LoadedResources_name.Items.Count; i++)
			{
				if (this.LoadedResources_name.Items[i].ToString() == oldName)
				{
					this.LoadedResources_name.Items[i] = this.textBox_SelectedName.Text;
					oldName = this.textBox_SelectedName.Text;
				}
			}
		}

		this.button_nameApply.Enabled = false;
		this.ActiveControl = null;
	}
}}

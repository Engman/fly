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
	bool geometryLoaded = false;

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
	private void splitter1_SplitterMoved(object sender, SplitterEventArgs e)
	{
		FlyEdit.flyCLI.OnResize(this.RenderWin.Width, this.RenderWin.Height);
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
				//Models
				Dictionary<string, int>.Enumerator m_it = levelGen.MeshResource.GetEnumerator();
				while (m_it.MoveNext())
					this.ResourceTree.Nodes[(int)ReourceNodeIndex.NodeIndex_Mesh].Nodes.Add(m_it.Current.Key);
				
				//Pickups
				Dictionary<string, int>.Enumerator p_it = this.levelGen.Pickups.GetEnumerator();
				while (p_it.MoveNext())
					this.ResourceTree.Nodes[(int)ReourceNodeIndex.NodeIndex_Pickup].Nodes.Add(p_it.Current.Key);
				
				//Lights
				Dictionary<string, int>.Enumerator l_it = this.levelGen.Lights.GetEnumerator();
				while (l_it.MoveNext())
					this.ResourceTree.Nodes[(int)ReourceNodeIndex.NodeIndex_Light].Nodes.Add(l_it.Current.Key);

				//Terrain
				//this.ResourceTree.Nodes[(int)ReourceNodeIndex.NodeIndex_Light].Nodes.Add();
			}
			else
			{
				MessageBox.Show("Failed to load level");
			}
		}
		
	}
	private void trackBar_Rotation_Scroll(object sender, EventArgs e)
	{

		float x = (float)Math.PI * this.trackBar_RotationX.Value / 180.0f;
		float y = (float)Math.PI * this.trackBar_RotationY.Value / 180.0f;
		float z = (float)Math.PI * this.trackBar_RotationZ.Value / 180.0f;

		FlyEdit.flyCLI.SetRotation(x, y, z);
	}
	private void trackBar_Scale_Scroll(object sender, EventArgs e)
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		if (this.checkBox_uniformScale.Checked)
		{
			x += ((TrackBar)sender).Value * 0.001f;
			y += ((TrackBar)sender).Value * 0.001f;
			z += ((TrackBar)sender).Value * 0.001f;

			int val = ((TrackBar)sender).Value;
			this.trackBar_ScaleX.Value = val;
			this.trackBar_ScaleY.Value = val;
			this.trackBar_ScaleZ.Value = val;
		}
		else
		{
			x += this.trackBar_ScaleX.Value / 1000.0f;
			y += this.trackBar_ScaleY.Value / 1000.0f;
			z += this.trackBar_ScaleZ.Value / 1000.0f;
		}

		FlyEdit.flyCLI.SetScale(x, y, z);
	}
	private void checkBox_uniformScale_CheckedChanged(object sender, EventArgs e)
	{
		if (!this.checkBox_uniformScale.Checked)
		{
			this.trackBar_ScaleX.Value = 1;
			this.trackBar_ScaleY.Value = 1;
			this.trackBar_ScaleZ.Value = 1;
		}
	}
	private void button_LoadGeometry_Click(object sender, EventArgs e)
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

			Dictionary<string, int> objects = new Dictionary<string, int>();

			if (!FlyEdit.flyCLI.LoadResources(f.FileNames, objects))
				MessageBox.Show("Failed to load reources!");

			this.outWin.addText(objects.Count.ToString() + " models loaded!");

			Dictionary<string, int>.Enumerator it = objects.GetEnumerator();
			while (it.MoveNext())
			{
				this.levelGen.MeshResource.Add(it.Current.Key, it.Current.Value);
				this.ResourceTree.Nodes[0].Nodes.Add(it.Current.Value.ToString(), it.Current.Key); 
			}
		}
	}
	private void button_LoadTerrain_Click(object sender, EventArgs e)
	{
		if (!this.geometryLoaded)
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
				if (!FlyEdit.flyCLI.LoadTerrain(f.FileName))
					MessageBox.Show("Failed to load terrain!");
				else
				{
					this.button_LoadTerrain.Enabled = false;
					this.ResourceTree.Nodes[4].Nodes.Add(f.FileName.Substring(f.FileName.LastIndexOf('\\')+1, f.FileName.Length - f.FileName.LastIndexOf('\\') - 5));
				}
			}
		}
	}
	
	private void RenderWin_MouseDownEvent(object sender, MouseEventArgs e)
	{
		this.ActiveControl = this.RenderWin;
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
		FlyEdit.flyCLI.OnMouseScrollEvent(e.Delta, this.ctrl, this.shift, this.alt);
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
		else if (e.KeyCode.Equals(Keys.Escape))
		{
			RenderLockPictureLockClicked(null, null);
			key = KEY_ESC;
		}
		FlyEdit.flyCLI.OnKeyEvent(key, true, this.ctrl, this.shift, this.alt);
		
	}
	private void ScaleSliderMouseDownEvent(object sender, MouseEventArgs e)
	{
		this.scale = true;
	}
	private void ScaleSliderMouseUpEvent(object sender, MouseEventArgs e)
	{
		this.scale = false;
	}
	private void RotationSliderMoseUpEven(object sender, MouseEventArgs e)
	{
		this.rotate = false;
	}
	private void RotationSliderMouseDownEvent(object sender, MouseEventArgs e)
	{
		this.rotate = true;
	}

	private void FlyEdit_KeyUpEvent(object sender, KeyEventArgs e)
	{
		this.shift = e.Shift;
		this.ctrl = e.Control;
		this.alt = e.Alt;
	}

	
	private void CameraSpeedSelector_ValueChanged(object sender, EventArgs e)
	{
		float _s = (float)this.CameraSpeedSelector.Value * 0.01f;
		FlyEdit.flyCLI.SetSpeed(_s);
	}



	static Panel current = null;
	private void ResourceTree_AfterSelect(object sender, TreeViewEventArgs e)
	{
		
		int topLevel = -1;
		TreeNode temp = e.Node;
		//Find parent
		while (temp != null)
		{
			topLevel = temp.Index;
			temp = temp.Parent;
		}
		switch (topLevel)
		{
			case (int)ReourceNodeIndex.NodeIndex_Mesh:
				if (current != null && !current.Equals(this.panel_Geometry))
					current.Visible = false;
				current = this.panel_Geometry;
				current.Visible = true;
				MeshNodeSelected(e.Node);
			break;

			case (int)ReourceNodeIndex.NodeIndex_Pickup:
				if (current != null && !current.Equals(this.panel_Pickups))
					current.Visible = false;
				current = this.panel_Pickups;
				current.Visible = true;
				PickupNodeSelected(e.Node);
			break;

			case (int)ReourceNodeIndex.NodeIndex_Light:
				if (current != null && !current.Equals(this.panel_Lights))
					current.Visible = false;
				current = this.panel_Lights;
				current.Visible = true;
				LightNodeSelected(e.Node);
			break;

			case (int)ReourceNodeIndex.NodeIndex_Camera:
				if (current != null && !current.Equals(this.panel_Camera))
					current.Visible = false;
				current = this.panel_Camera;
				current.Visible = true;

				CameraNodeSelected(e.Node);
			break;

			case (int)ReourceNodeIndex.NodeIndex_Terrain:
			if (current != null && !current.Equals(this.panel_Terrain))
					current.Visible = false;
				current = this.panel_Terrain;
				current.Visible = true;
				TerrainNodeSelected(e.Node);
			break;

			default:
			break;
		}
	}
	private void MeshNodeSelected(TreeNode node)
	{
		if (node.Level == 0)
		{ }
		else
		{
			int val = -1;

			val = this.levelGen.MeshResource[node.Text];
			
			if (val != -1)
			{
				if (!FlyEdit.flyCLI.SelectObject(val))
					MessageBox.Show("Could not select entity");
			}
		}

	}
	private void PickupNodeSelected(TreeNode node)
	{
		
	}
	private void LightNodeSelected(TreeNode node)
	{

	}
	private void CameraNodeSelected(TreeNode node)
	{
		if (node.Level == 0)
		{}
		else
		{
			for (int i = 0; i < this.ResourceTree.Nodes[(int)ReourceNodeIndex.NodeIndex_Camera].Nodes.Count; i++)
				this.ResourceTree.Nodes[(int)ReourceNodeIndex.NodeIndex_Camera].Nodes[i].ForeColor = Color.Black;

			node.ForeColor = Color.White;
			FlyEdit.flyCLI.ChangeView(this.levelGen.Cameras[node.Text]);
		}
	}
	private void TerrainNodeSelected(TreeNode node)
	{

	}
}}

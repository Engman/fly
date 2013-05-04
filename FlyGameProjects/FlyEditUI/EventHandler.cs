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

	public static bool renderWinLocked = true;
	ToolTip pictureBoxLockTooltip = new ToolTip();
	Point lastMousePos = new Point(0,0);

	private void RenderWindow_MouseClick(object sender, MouseEventArgs e)
	{
		this.FlyCLI.OnMouseClick(true, e.X, e.Y);
	}

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
			this.FlyCLI.LoadResources(f.FileNames, objects);
			
			this.outWin.addText(objects.Count.ToString() + " models loaded!");
		}
	}

	void WindowResizeBegin(object sender, EventArgs e)
	{
		if (this.WindowState == FormWindowState.Maximized)
		{
			this.outWin.WindowState = FormWindowState.Normal;
			this.outWin.Show();
			this.FlyCLI.OnResize(this.RenderWin.Width, this.RenderWin.Height);
		}
		else if (this.WindowState == FormWindowState.Normal)
		{
			this.outWin.WindowState = FormWindowState.Normal;
			this.outWin.Show();
			this.FlyCLI.OnResize(this.RenderWin.Width, this.RenderWin.Height);
		}
	}

	void WindowResizeEnd(object sender, EventArgs e)
	{
		this.FlyCLI.OnResize(this.RenderWin.Width, this.RenderWin.Height);
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
		if (this.engineRuning)
			this.FlyCLI.Shutdown();
		e.Cancel = false;
	}

	private void exitToolStripMenuItem_Click(object sender, EventArgs e)
	{
		this.Close();
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

}}

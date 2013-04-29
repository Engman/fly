using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Interop;
using System.Threading;

namespace FlyEditUI
{
	public partial class FlyEdit
	{
		private void RenderWindow_MouseClick(object sender, MouseEventArgs e)
		{
			this.FlyCLI.OnMouseDown(e.X, e.Y);
		}

		private void loadGeomrtyToolStripMenuItem_Click(object sender, EventArgs e)
		{
			FolderBrowserDialog f = new FolderBrowserDialog();
			f.ShowNewFolderButton = false;
			String temp = Directory.GetCurrentDirectory();
			temp = temp.Substring(0, temp.LastIndexOf('\\'));
			temp += "\\Resources\\Models";
			f.SelectedPath = temp;
			DialogResult result = f.ShowDialog();
			if (result != System.Windows.Forms.DialogResult.OK)
				return;
			else
			{
				int count = this.FlyCLI.LoadResources(f.SelectedPath);
				String text = count.ToString() + " models loaded!";
				this.outWin.addText(text);
			}
		}

		void WindowResizeBegin(object sender, EventArgs e)
		{
			if (this.WindowState == FormWindowState.Maximized)
			{
				this.FlyCLI.OnResize(this.RenderWin.Width, this.RenderWin.Height);
			}
			else if (this.WindowState == FormWindowState.Normal)
			{
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
				this.outWin.Show(this);
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

		private void shutdownEngineToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (!this.engineRuning)
			{
				engineRuning = false;
				this.FlyCLI.Shutdown();
			}
			else
			{
				if (!this.FlyCLI.Init(RenderWin.Handle, RenderWin.Width, RenderWin.Height))
				{
					MessageBox.Show(this, "Failed to initialize core");
					this.Close();
				}
				else
				{
					engineRuning = true;
				}
			}
		}
	}
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Interop;

namespace FlyWindEditor
{
	public partial class FlyWind : Form
	{
		FlyWindEdit flyEngine = null;
		LuaEdit luaEditor = null;

		public FlyWind()
		{
			InitializeComponent();

			flyEngine = new FlyWindEdit();

			this.luaEditor = new LuaEdit();
			this.Move += new EventHandler(MoveSubForm);
			this.Resize += new EventHandler(MoveSubForm);
			this.luaEditor.StartPosition = FormStartPosition.Manual;
			this.luaEditor.Left = this.Left + this.Width;
			this.luaEditor.Top = this.Top;
			this.luaEditor.Show(this);
		}

		public void Run()
		{
			if (!this.flyEngine.Init(this.RenderBox.Handle, this.RenderBox.Width, this.RenderBox.Height))
			{
				MessageBox.Show(this, "Failed to initialize core");
				this.Close() ;
			}

			String[] model = { "..\\Resources\\Models\\WindEditFloor.fgm", "..\\Resources\\Models\\WindEditSphere.fgm" };
			Dictionary<string, int> loaded = new Dictionary<string,int>();
			this.flyEngine.LoadResources(model, loaded);

			while (this.Created)
			{
				this.flyEngine.ProcessFrame();
				
				Application.DoEvents();
			}
		}

		  protected void MoveSubForm(object sender, EventArgs e)
		  {
			 if (this.luaEditor != null)
			 {
				 this.luaEditor.Left = this.Left + this.Width-1;
				 this.luaEditor.Top = this.Top;
			 }
		  }

		  private void LuaEditVisibility_Click(object sender, EventArgs e)
		  {
			  if (this.luaEditor.Visible)
			  {
				  this.luaEditor.Hide();
				  this.LuaEditVisibility.Text = ">";
			  }
			  else
			  {
				  this.luaEditor.Show();
				  this.LuaEditVisibility.Text = "<";
			  }
		  }

		  private void quitToolStripMenuItem_Click(object sender, EventArgs e)
		  {
			  this.Close();
		  }

		  private void saveScriptToolStripMenuItem_Click(object sender, EventArgs e)
		  {
			  KeyEventArgs ke = new KeyEventArgs(Keys.Control|Keys.S);
			  this.luaEditor.KeyDownEvent(this.luaEditor, ke);
		  }

		  private void loadScriptToolStripMenuItem_Click(object sender, EventArgs e)
		  {
			  KeyEventArgs k = new KeyEventArgs(Keys.Control | Keys.O);
			  this.luaEditor.KeyDownEvent(this.luaEditor, k);
		  }

		  private void FormClosingEvent(object sender, FormClosingEventArgs e)
		  {
			  if (this.luaEditor.UnsavedChanges)
			  {
				  if (MessageBox.Show(this, "You have unsaved changes in your script,\nSave?", "Save", MessageBoxButtons.YesNo) == System.Windows.Forms.DialogResult.Yes)
				  {
					  KeyEventArgs k = new KeyEventArgs(Keys.Control|Keys.S);
					  this.luaEditor.KeyDownEvent(this.luaEditor, k);
				  }
			  }
		  }

		  private void newToolStripMenuItem_Click(object sender, EventArgs e)
		  {
			  MessageBox.Show("Nothing here yet...");
		  }

		  private void SliderValueChanged(object sender, EventArgs e)
		  {
			  switch (int.Parse(((TrackBar)sender).Tag.ToString()))
			  {
				  case 1:	//Wind Velocity
					  break;
				  case 2:	//Wind Direction
					  break;
				  case 3:	//Cone Angle
					  break;
				  case 4:	//Wind Length
					  break;
				  case 5:	//Sphere Size
					  break;
				  case 6:	//Object Weight
					  break;
				  case 7:	//Object Velocity
					  break;
				  case 8:	//Global Gravitation
					  break;
			  }
		  }
	}
}

namespace FlyEditUI
{
	partial class FlyEdit
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.RenderWin = new System.Windows.Forms.Panel();
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.geometryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadGeomrtyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.scriptToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadScriptToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openScriptEditToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.eventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.newEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadEventToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.runToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.menuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// RenderWin
			// 
			this.RenderWin.BackColor = System.Drawing.SystemColors.ControlLight;
			this.RenderWin.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.RenderWin.Location = new System.Drawing.Point(44, 60);
			this.RenderWin.Name = "RenderWin";
			this.RenderWin.Size = new System.Drawing.Size(508, 355);
			this.RenderWin.TabIndex = 0;
			this.RenderWin.MouseDown += new System.Windows.Forms.MouseEventHandler(this.RenderWindow_MouseClick);
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.geometryToolStripMenuItem,
            this.scriptToolStripMenuItem,
            this.eventToolStripMenuItem,
            this.aboutToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(602, 24);
			this.menuStrip1.TabIndex = 1;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.runToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.loadToolStripMenuItem,
            this.exitToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
			this.fileToolStripMenuItem.Text = "File";
			// 
			// saveToolStripMenuItem
			// 
			this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
			this.saveToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.saveToolStripMenuItem.Text = "Save";
			// 
			// loadToolStripMenuItem
			// 
			this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
			this.loadToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.loadToolStripMenuItem.Text = "Load";
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.exitToolStripMenuItem.Text = "Exit";
			// 
			// geometryToolStripMenuItem
			// 
			this.geometryToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadGeomrtyToolStripMenuItem});
			this.geometryToolStripMenuItem.Name = "geometryToolStripMenuItem";
			this.geometryToolStripMenuItem.Size = new System.Drawing.Size(71, 20);
			this.geometryToolStripMenuItem.Text = "Geometry";
			// 
			// loadGeomrtyToolStripMenuItem
			// 
			this.loadGeomrtyToolStripMenuItem.Name = "loadGeomrtyToolStripMenuItem";
			this.loadGeomrtyToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.loadGeomrtyToolStripMenuItem.Text = "Load Geomtry";
			// 
			// scriptToolStripMenuItem
			// 
			this.scriptToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadScriptToolStripMenuItem,
            this.openScriptEditToolStripMenuItem});
			this.scriptToolStripMenuItem.Name = "scriptToolStripMenuItem";
			this.scriptToolStripMenuItem.Size = new System.Drawing.Size(49, 20);
			this.scriptToolStripMenuItem.Text = "Script";
			// 
			// loadScriptToolStripMenuItem
			// 
			this.loadScriptToolStripMenuItem.Name = "loadScriptToolStripMenuItem";
			this.loadScriptToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
			this.loadScriptToolStripMenuItem.Text = "Load";
			// 
			// openScriptEditToolStripMenuItem
			// 
			this.openScriptEditToolStripMenuItem.Name = "openScriptEditToolStripMenuItem";
			this.openScriptEditToolStripMenuItem.Size = new System.Drawing.Size(124, 22);
			this.openScriptEditToolStripMenuItem.Text = "ScriptEdit";
			// 
			// eventToolStripMenuItem
			// 
			this.eventToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newEventToolStripMenuItem,
            this.loadEventToolStripMenuItem,
            this.saveToolStripMenuItem1});
			this.eventToolStripMenuItem.Name = "eventToolStripMenuItem";
			this.eventToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
			this.eventToolStripMenuItem.Text = "Event";
			// 
			// newEventToolStripMenuItem
			// 
			this.newEventToolStripMenuItem.Name = "newEventToolStripMenuItem";
			this.newEventToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
			this.newEventToolStripMenuItem.Text = "New";
			// 
			// loadEventToolStripMenuItem
			// 
			this.loadEventToolStripMenuItem.Name = "loadEventToolStripMenuItem";
			this.loadEventToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
			this.loadEventToolStripMenuItem.Text = "Load";
			// 
			// saveToolStripMenuItem1
			// 
			this.saveToolStripMenuItem1.Name = "saveToolStripMenuItem1";
			this.saveToolStripMenuItem1.Size = new System.Drawing.Size(100, 22);
			this.saveToolStripMenuItem1.Text = "Save";
			// 
			// aboutToolStripMenuItem
			// 
			this.aboutToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem1});
			this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
			this.aboutToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
			this.aboutToolStripMenuItem.Text = "Help";
			// 
			// aboutToolStripMenuItem1
			// 
			this.aboutToolStripMenuItem1.Name = "aboutToolStripMenuItem1";
			this.aboutToolStripMenuItem1.Size = new System.Drawing.Size(107, 22);
			this.aboutToolStripMenuItem1.Text = "About";
			// 
			// runToolStripMenuItem
			// 
			this.runToolStripMenuItem.Name = "runToolStripMenuItem";
			this.runToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.runToolStripMenuItem.Text = "Run";
			// 
			// FlyEdit
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(602, 427);
			this.Controls.Add(this.RenderWin);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "FlyEdit";
			this.Text = "FlyEdit";
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Panel RenderWin;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem geometryToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadGeomrtyToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem scriptToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadScriptToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem openScriptEditToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem eventToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem newEventToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadEventToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem runToolStripMenuItem;
	}
}


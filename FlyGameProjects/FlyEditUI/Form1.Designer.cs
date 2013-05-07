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
			this.RenderLockPictureBox = new System.Windows.Forms.PictureBox();
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.runToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.shutdownEngineToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
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
			this.soundToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.outputWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolContainer = new System.Windows.Forms.Panel();
			this.CameraDropBox = new System.Windows.Forms.ComboBox();
			this.listBox_id = new System.Windows.Forms.ListBox();
			this.LoadedResources_name = new System.Windows.Forms.ListBox();
			this.splitter1 = new System.Windows.Forms.Splitter();
			this.RenderWin.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.RenderLockPictureBox)).BeginInit();
			this.menuStrip1.SuspendLayout();
			this.ToolContainer.SuspendLayout();
			this.SuspendLayout();
			// 
			// RenderWin
			// 
			this.RenderWin.BackColor = System.Drawing.SystemColors.ControlLight;
			this.RenderWin.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.RenderWin.Controls.Add(this.RenderLockPictureBox);
			this.RenderWin.Dock = System.Windows.Forms.DockStyle.Fill;
			this.RenderWin.Location = new System.Drawing.Point(0, 24);
			this.RenderWin.Name = "RenderWin";
			this.RenderWin.Size = new System.Drawing.Size(766, 504);
			this.RenderWin.TabIndex = 0;
			this.RenderWin.MouseClick += new System.Windows.Forms.MouseEventHandler(this.RenderWindow_MouseClick);
			// 
			// RenderLockPictureBox
			// 
			this.RenderLockPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.RenderLockPictureBox.BackColor = System.Drawing.Color.Transparent;
			this.RenderLockPictureBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
			this.RenderLockPictureBox.Image = global::FlyEditUI.Properties.Resources.locked;
			this.RenderLockPictureBox.Location = new System.Drawing.Point(737, 3);
			this.RenderLockPictureBox.Name = "RenderLockPictureBox";
			this.RenderLockPictureBox.Size = new System.Drawing.Size(22, 31);
			this.RenderLockPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.RenderLockPictureBox.TabIndex = 0;
			this.RenderLockPictureBox.TabStop = false;
			this.RenderLockPictureBox.Click += new System.EventHandler(this.RenderLockPictureLockClicked);
			this.RenderLockPictureBox.MouseHover += new System.EventHandler(this.RenderLockPictureHover);
			// 
			// menuStrip1
			// 
			this.menuStrip1.BackColor = System.Drawing.Color.PaleGreen;
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.geometryToolStripMenuItem,
            this.scriptToolStripMenuItem,
            this.eventToolStripMenuItem,
            this.soundToolStripMenuItem,
            this.viewToolStripMenuItem,
            this.aboutToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(927, 24);
			this.menuStrip1.TabIndex = 1;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.runToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.loadToolStripMenuItem,
            this.shutdownEngineToolStripMenuItem,
            this.exitToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
			this.fileToolStripMenuItem.Text = "File";
			// 
			// runToolStripMenuItem
			// 
			this.runToolStripMenuItem.Name = "runToolStripMenuItem";
			this.runToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
			this.runToolStripMenuItem.Text = "Run";
			// 
			// saveToolStripMenuItem
			// 
			this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
			this.saveToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
			this.saveToolStripMenuItem.Text = "Save";
			// 
			// loadToolStripMenuItem
			// 
			this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
			this.loadToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
			this.loadToolStripMenuItem.Text = "Load";
			// 
			// shutdownEngineToolStripMenuItem
			// 
			this.shutdownEngineToolStripMenuItem.Name = "shutdownEngineToolStripMenuItem";
			this.shutdownEngineToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
			this.shutdownEngineToolStripMenuItem.Text = "Shutdown Engine";
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.Size = new System.Drawing.Size(167, 22);
			this.exitToolStripMenuItem.Text = "Exit";
			this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
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
			this.loadGeomrtyToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
			this.loadGeomrtyToolStripMenuItem.Text = "Load Geomtry";
			this.loadGeomrtyToolStripMenuItem.Click += new System.EventHandler(this.loadGeomrtyToolStripMenuItem_Click);
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
			// soundToolStripMenuItem
			// 
			this.soundToolStripMenuItem.Name = "soundToolStripMenuItem";
			this.soundToolStripMenuItem.Size = new System.Drawing.Size(53, 20);
			this.soundToolStripMenuItem.Text = "Sound";
			// 
			// viewToolStripMenuItem
			// 
			this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.outputWindowToolStripMenuItem});
			this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
			this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
			this.viewToolStripMenuItem.Text = "View";
			// 
			// outputWindowToolStripMenuItem
			// 
			this.outputWindowToolStripMenuItem.Name = "outputWindowToolStripMenuItem";
			this.outputWindowToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
			this.outputWindowToolStripMenuItem.Text = "Output window";
			this.outputWindowToolStripMenuItem.Click += new System.EventHandler(this.outputWindowToolStripMenuItem_Click);
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
			// ToolContainer
			// 
			this.ToolContainer.BackColor = System.Drawing.Color.Gray;
			this.ToolContainer.Controls.Add(this.CameraDropBox);
			this.ToolContainer.Controls.Add(this.listBox_id);
			this.ToolContainer.Controls.Add(this.LoadedResources_name);
			this.ToolContainer.Dock = System.Windows.Forms.DockStyle.Right;
			this.ToolContainer.Location = new System.Drawing.Point(770, 24);
			this.ToolContainer.Name = "ToolContainer";
			this.ToolContainer.Size = new System.Drawing.Size(157, 504);
			this.ToolContainer.TabIndex = 2;
			// 
			// CameraDropBox
			// 
			this.CameraDropBox.FormattingEnabled = true;
			this.CameraDropBox.Items.AddRange(new object[] {
            "Top Camera",
            "First Person"});
			this.CameraDropBox.Location = new System.Drawing.Point(3, 5);
			this.CameraDropBox.Name = "CameraDropBox";
			this.CameraDropBox.Size = new System.Drawing.Size(110, 21);
			this.CameraDropBox.TabIndex = 3;
			this.CameraDropBox.SelectedIndexChanged += new System.EventHandler(this.CameraDropBox_SelectedIndexChanged);
			// 
			// listBox_id
			// 
			this.listBox_id.BackColor = System.Drawing.Color.Gray;
			this.listBox_id.ForeColor = System.Drawing.Color.White;
			this.listBox_id.FormattingEnabled = true;
			this.listBox_id.IntegralHeight = false;
			this.listBox_id.Location = new System.Drawing.Point(119, 32);
			this.listBox_id.Name = "listBox_id";
			this.listBox_id.SelectionMode = System.Windows.Forms.SelectionMode.None;
			this.listBox_id.Size = new System.Drawing.Size(35, 74);
			this.listBox_id.Sorted = true;
			this.listBox_id.TabIndex = 2;
			// 
			// LoadedResources_name
			// 
			this.LoadedResources_name.BackColor = System.Drawing.Color.Gray;
			this.LoadedResources_name.ForeColor = System.Drawing.Color.White;
			this.LoadedResources_name.FormattingEnabled = true;
			this.LoadedResources_name.IntegralHeight = false;
			this.LoadedResources_name.Location = new System.Drawing.Point(3, 32);
			this.LoadedResources_name.Name = "LoadedResources_name";
			this.LoadedResources_name.Size = new System.Drawing.Size(110, 74);
			this.LoadedResources_name.Sorted = true;
			this.LoadedResources_name.TabIndex = 1;
			// 
			// splitter1
			// 
			this.splitter1.BackColor = System.Drawing.Color.PaleGreen;
			this.splitter1.Dock = System.Windows.Forms.DockStyle.Right;
			this.splitter1.Location = new System.Drawing.Point(766, 24);
			this.splitter1.Name = "splitter1";
			this.splitter1.Size = new System.Drawing.Size(4, 504);
			this.splitter1.TabIndex = 3;
			this.splitter1.TabStop = false;
			this.splitter1.SplitterMoved += new System.Windows.Forms.SplitterEventHandler(this.splitter1_SplitterMoved);
			// 
			// FlyEdit
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(927, 528);
			this.Controls.Add(this.RenderWin);
			this.Controls.Add(this.splitter1);
			this.Controls.Add(this.ToolContainer);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "FlyEdit";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "FlyEdit";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormClosingEvent);
			this.Load += new System.EventHandler(this.FormLoadedEvent);
			this.ResizeBegin += new System.EventHandler(this.WindowResizeBegin);
			this.ResizeEnd += new System.EventHandler(this.WindowResizeEnd);
			this.MouseClick += new System.Windows.Forms.MouseEventHandler(this.RenderWindow_MouseClick);
			this.RenderWin.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.RenderLockPictureBox)).EndInit();
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.ToolContainer.ResumeLayout(false);
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
		private System.Windows.Forms.ToolStripMenuItem soundToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem outputWindowToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem shutdownEngineToolStripMenuItem;
		private System.Windows.Forms.PictureBox RenderLockPictureBox;
		private System.Windows.Forms.Panel ToolContainer;
		private System.Windows.Forms.Splitter splitter1;
		private System.Windows.Forms.ListBox LoadedResources_name;
		private System.Windows.Forms.ListBox listBox_id;
		private System.Windows.Forms.ComboBox CameraDropBox;
	}
}


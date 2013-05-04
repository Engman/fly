namespace FlyWindEditor
{
	partial class FlyWind
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
			this.RenderBox = new System.Windows.Forms.Panel();
			this.LuaEditVisibility = new System.Windows.Forms.Button();
			this.menuStrip = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadScriptToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveScriptToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.quitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.SliderWindVelocity = new System.Windows.Forms.TrackBar();
			this.SliderWindDirection = new System.Windows.Forms.TrackBar();
			this.SliderWindAngle = new System.Windows.Forms.TrackBar();
			this.SliderWindLength = new System.Windows.Forms.TrackBar();
			this.SliderWindSpehereSize = new System.Windows.Forms.TrackBar();
			this.SliderObjectWeight = new System.Windows.Forms.TrackBar();
			this.label_WindVelocity = new System.Windows.Forms.Label();
			this.label_WindDirection = new System.Windows.Forms.Label();
			this.label_WindAngle = new System.Windows.Forms.Label();
			this.label_WindLength = new System.Windows.Forms.Label();
			this.label_WindSphereSize = new System.Windows.Forms.Label();
			this.label_ObjectWeight = new System.Windows.Forms.Label();
			this.label_ObjectVelocity = new System.Windows.Forms.Label();
			this.SliderObjectVelocity = new System.Windows.Forms.TrackBar();
			this.label_Gravitation = new System.Windows.Forms.Label();
			this.SliderGravitation = new System.Windows.Forms.TrackBar();
			this.panel_WindSliders = new System.Windows.Forms.Panel();
			this.panel_ObjectSliders = new System.Windows.Forms.Panel();
			this.label_WindCollection = new System.Windows.Forms.Label();
			this.label_ObjectCollection = new System.Windows.Forms.Label();
			this.menuStrip.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.SliderWindVelocity)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderWindDirection)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderWindAngle)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderWindLength)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderWindSpehereSize)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderObjectWeight)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderObjectVelocity)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderGravitation)).BeginInit();
			this.panel_WindSliders.SuspendLayout();
			this.panel_ObjectSliders.SuspendLayout();
			this.SuspendLayout();
			// 
			// RenderBox
			// 
			this.RenderBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.RenderBox.Location = new System.Drawing.Point(201, 31);
			this.RenderBox.Name = "RenderBox";
			this.RenderBox.Size = new System.Drawing.Size(430, 332);
			this.RenderBox.TabIndex = 0;
			// 
			// LuaEditVisibility
			// 
			this.LuaEditVisibility.BackColor = System.Drawing.Color.Gray;
			this.LuaEditVisibility.Dock = System.Windows.Forms.DockStyle.Right;
			this.LuaEditVisibility.FlatAppearance.BorderColor = System.Drawing.Color.Gray;
			this.LuaEditVisibility.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Silver;
			this.LuaEditVisibility.FlatAppearance.MouseOverBackColor = System.Drawing.Color.DarkGray;
			this.LuaEditVisibility.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.LuaEditVisibility.Location = new System.Drawing.Point(646, 24);
			this.LuaEditVisibility.Name = "LuaEditVisibility";
			this.LuaEditVisibility.Size = new System.Drawing.Size(22, 343);
			this.LuaEditVisibility.TabIndex = 1;
			this.LuaEditVisibility.Text = "<";
			this.LuaEditVisibility.UseVisualStyleBackColor = false;
			this.LuaEditVisibility.Click += new System.EventHandler(this.LuaEditVisibility_Click);
			// 
			// menuStrip
			// 
			this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
			this.menuStrip.Location = new System.Drawing.Point(0, 0);
			this.menuStrip.Name = "menuStrip";
			this.menuStrip.Size = new System.Drawing.Size(668, 24);
			this.menuStrip.TabIndex = 2;
			this.menuStrip.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.loadScriptToolStripMenuItem,
            this.saveScriptToolStripMenuItem,
            this.quitToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
			this.fileToolStripMenuItem.Text = "File";
			// 
			// newToolStripMenuItem
			// 
			this.newToolStripMenuItem.Name = "newToolStripMenuItem";
			this.newToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
			this.newToolStripMenuItem.Text = "New";
			this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
			// 
			// loadScriptToolStripMenuItem
			// 
			this.loadScriptToolStripMenuItem.Name = "loadScriptToolStripMenuItem";
			this.loadScriptToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
			this.loadScriptToolStripMenuItem.Text = "Load script";
			this.loadScriptToolStripMenuItem.Click += new System.EventHandler(this.loadScriptToolStripMenuItem_Click);
			// 
			// saveScriptToolStripMenuItem
			// 
			this.saveScriptToolStripMenuItem.Name = "saveScriptToolStripMenuItem";
			this.saveScriptToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
			this.saveScriptToolStripMenuItem.Text = "Save script";
			this.saveScriptToolStripMenuItem.Click += new System.EventHandler(this.saveScriptToolStripMenuItem_Click);
			// 
			// quitToolStripMenuItem
			// 
			this.quitToolStripMenuItem.Name = "quitToolStripMenuItem";
			this.quitToolStripMenuItem.Size = new System.Drawing.Size(132, 22);
			this.quitToolStripMenuItem.Text = "Quit";
			this.quitToolStripMenuItem.Click += new System.EventHandler(this.quitToolStripMenuItem_Click);
			// 
			// SliderWindVelocity
			// 
			this.SliderWindVelocity.LargeChange = 10;
			this.SliderWindVelocity.Location = new System.Drawing.Point(75, 9);
			this.SliderWindVelocity.Maximum = 100;
			this.SliderWindVelocity.Name = "SliderWindVelocity";
			this.SliderWindVelocity.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.SliderWindVelocity.Size = new System.Drawing.Size(104, 45);
			this.SliderWindVelocity.TabIndex = 60;
			this.SliderWindVelocity.TabStop = false;
			this.SliderWindVelocity.Tag = "1";
			this.SliderWindVelocity.TickStyle = System.Windows.Forms.TickStyle.None;
			this.SliderWindVelocity.Value = 50;
			this.SliderWindVelocity.Scroll += new System.EventHandler(this.SliderValueChanged);
			// 
			// SliderWindDirection
			// 
			this.SliderWindDirection.LargeChange = 10;
			this.SliderWindDirection.Location = new System.Drawing.Point(75, 40);
			this.SliderWindDirection.Maximum = 100;
			this.SliderWindDirection.Name = "SliderWindDirection";
			this.SliderWindDirection.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.SliderWindDirection.Size = new System.Drawing.Size(104, 45);
			this.SliderWindDirection.TabIndex = 61;
			this.SliderWindDirection.TabStop = false;
			this.SliderWindDirection.Tag = "2";
			this.SliderWindDirection.TickStyle = System.Windows.Forms.TickStyle.None;
			this.SliderWindDirection.Value = 50;
			// 
			// SliderWindAngle
			// 
			this.SliderWindAngle.LargeChange = 10;
			this.SliderWindAngle.Location = new System.Drawing.Point(75, 70);
			this.SliderWindAngle.Maximum = 100;
			this.SliderWindAngle.Name = "SliderWindAngle";
			this.SliderWindAngle.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.SliderWindAngle.Size = new System.Drawing.Size(104, 45);
			this.SliderWindAngle.TabIndex = 62;
			this.SliderWindAngle.TabStop = false;
			this.SliderWindAngle.Tag = "3";
			this.SliderWindAngle.TickStyle = System.Windows.Forms.TickStyle.None;
			this.SliderWindAngle.Value = 50;
			// 
			// SliderWindLength
			// 
			this.SliderWindLength.LargeChange = 10;
			this.SliderWindLength.Location = new System.Drawing.Point(75, 102);
			this.SliderWindLength.Maximum = 100;
			this.SliderWindLength.Name = "SliderWindLength";
			this.SliderWindLength.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.SliderWindLength.Size = new System.Drawing.Size(104, 45);
			this.SliderWindLength.TabIndex = 63;
			this.SliderWindLength.TabStop = false;
			this.SliderWindLength.Tag = "4";
			this.SliderWindLength.TickStyle = System.Windows.Forms.TickStyle.None;
			this.SliderWindLength.Value = 50;
			// 
			// SliderWindSpehereSize
			// 
			this.SliderWindSpehereSize.LargeChange = 10;
			this.SliderWindSpehereSize.Location = new System.Drawing.Point(75, 132);
			this.SliderWindSpehereSize.Maximum = 100;
			this.SliderWindSpehereSize.Name = "SliderWindSpehereSize";
			this.SliderWindSpehereSize.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.SliderWindSpehereSize.Size = new System.Drawing.Size(104, 45);
			this.SliderWindSpehereSize.TabIndex = 64;
			this.SliderWindSpehereSize.TabStop = false;
			this.SliderWindSpehereSize.Tag = "5";
			this.SliderWindSpehereSize.TickStyle = System.Windows.Forms.TickStyle.None;
			this.SliderWindSpehereSize.Value = 50;
			// 
			// SliderObjectWeight
			// 
			this.SliderObjectWeight.LargeChange = 10;
			this.SliderObjectWeight.Location = new System.Drawing.Point(70, 5);
			this.SliderObjectWeight.Maximum = 100;
			this.SliderObjectWeight.Name = "SliderObjectWeight";
			this.SliderObjectWeight.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.SliderObjectWeight.Size = new System.Drawing.Size(104, 45);
			this.SliderObjectWeight.TabIndex = 66;
			this.SliderObjectWeight.TabStop = false;
			this.SliderObjectWeight.Tag = "6";
			this.SliderObjectWeight.TickStyle = System.Windows.Forms.TickStyle.None;
			this.SliderObjectWeight.Value = 50;
			// 
			// label_WindVelocity
			// 
			this.label_WindVelocity.AutoSize = true;
			this.label_WindVelocity.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
			this.label_WindVelocity.Location = new System.Drawing.Point(6, 9);
			this.label_WindVelocity.Name = "label_WindVelocity";
			this.label_WindVelocity.Size = new System.Drawing.Size(44, 13);
			this.label_WindVelocity.TabIndex = 10;
			this.label_WindVelocity.Text = "Velocity";
			// 
			// label_WindDirection
			// 
			this.label_WindDirection.AutoSize = true;
			this.label_WindDirection.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
			this.label_WindDirection.Location = new System.Drawing.Point(6, 40);
			this.label_WindDirection.Name = "label_WindDirection";
			this.label_WindDirection.Size = new System.Drawing.Size(49, 13);
			this.label_WindDirection.TabIndex = 11;
			this.label_WindDirection.Text = "Direction";
			// 
			// label_WindAngle
			// 
			this.label_WindAngle.AutoSize = true;
			this.label_WindAngle.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
			this.label_WindAngle.Location = new System.Drawing.Point(6, 70);
			this.label_WindAngle.Name = "label_WindAngle";
			this.label_WindAngle.Size = new System.Drawing.Size(34, 13);
			this.label_WindAngle.TabIndex = 12;
			this.label_WindAngle.Text = "Angle";
			// 
			// label_WindLength
			// 
			this.label_WindLength.AutoSize = true;
			this.label_WindLength.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
			this.label_WindLength.Location = new System.Drawing.Point(6, 102);
			this.label_WindLength.Name = "label_WindLength";
			this.label_WindLength.Size = new System.Drawing.Size(63, 13);
			this.label_WindLength.TabIndex = 13;
			this.label_WindLength.Text = "Max Length";
			// 
			// label_WindSphereSize
			// 
			this.label_WindSphereSize.AutoSize = true;
			this.label_WindSphereSize.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
			this.label_WindSphereSize.Location = new System.Drawing.Point(6, 134);
			this.label_WindSphereSize.Name = "label_WindSphereSize";
			this.label_WindSphereSize.Size = new System.Drawing.Size(64, 13);
			this.label_WindSphereSize.TabIndex = 14;
			this.label_WindSphereSize.Text = "Sphere Size";
			// 
			// label_ObjectWeight
			// 
			this.label_ObjectWeight.AutoSize = true;
			this.label_ObjectWeight.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
			this.label_ObjectWeight.Location = new System.Drawing.Point(3, 9);
			this.label_ObjectWeight.Name = "label_ObjectWeight";
			this.label_ObjectWeight.Size = new System.Drawing.Size(41, 13);
			this.label_ObjectWeight.TabIndex = 15;
			this.label_ObjectWeight.Text = "Weight";
			// 
			// label_ObjectVelocity
			// 
			this.label_ObjectVelocity.AutoSize = true;
			this.label_ObjectVelocity.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
			this.label_ObjectVelocity.Location = new System.Drawing.Point(3, 37);
			this.label_ObjectVelocity.Name = "label_ObjectVelocity";
			this.label_ObjectVelocity.Size = new System.Drawing.Size(44, 13);
			this.label_ObjectVelocity.TabIndex = 17;
			this.label_ObjectVelocity.Text = "Velocity";
			// 
			// SliderObjectVelocity
			// 
			this.SliderObjectVelocity.LargeChange = 10;
			this.SliderObjectVelocity.Location = new System.Drawing.Point(70, 37);
			this.SliderObjectVelocity.Maximum = 100;
			this.SliderObjectVelocity.Name = "SliderObjectVelocity";
			this.SliderObjectVelocity.RightToLeft = System.Windows.Forms.RightToLeft.No;
			this.SliderObjectVelocity.Size = new System.Drawing.Size(104, 45);
			this.SliderObjectVelocity.TabIndex = 67;
			this.SliderObjectVelocity.TabStop = false;
			this.SliderObjectVelocity.Tag = "7";
			this.SliderObjectVelocity.TickStyle = System.Windows.Forms.TickStyle.None;
			this.SliderObjectVelocity.Value = 50;
			// 
			// label_Gravitation
			// 
			this.label_Gravitation.AutoSize = true;
			this.label_Gravitation.Location = new System.Drawing.Point(13, 341);
			this.label_Gravitation.Name = "label_Gravitation";
			this.label_Gravitation.Size = new System.Drawing.Size(58, 13);
			this.label_Gravitation.TabIndex = 19;
			this.label_Gravitation.Text = "Gravitation";
			// 
			// SliderGravitation
			// 
			this.SliderGravitation.LargeChange = 10;
			this.SliderGravitation.Location = new System.Drawing.Point(84, 341);
			this.SliderGravitation.Maximum = 100;
			this.SliderGravitation.Name = "SliderGravitation";
			this.SliderGravitation.Size = new System.Drawing.Size(104, 45);
			this.SliderGravitation.TabIndex = 65;
			this.SliderGravitation.TabStop = false;
			this.SliderGravitation.Tag = "8";
			this.SliderGravitation.TickStyle = System.Windows.Forms.TickStyle.None;
			this.SliderGravitation.Value = 50;
			// 
			// panel_WindSliders
			// 
			this.panel_WindSliders.BackColor = System.Drawing.Color.Gray;
			this.panel_WindSliders.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel_WindSliders.Controls.Add(this.SliderWindSpehereSize);
			this.panel_WindSliders.Controls.Add(this.SliderWindLength);
			this.panel_WindSliders.Controls.Add(this.SliderWindAngle);
			this.panel_WindSliders.Controls.Add(this.SliderWindDirection);
			this.panel_WindSliders.Controls.Add(this.label_WindSphereSize);
			this.panel_WindSliders.Controls.Add(this.label_WindLength);
			this.panel_WindSliders.Controls.Add(this.label_WindAngle);
			this.panel_WindSliders.Controls.Add(this.label_WindDirection);
			this.panel_WindSliders.Controls.Add(this.label_WindVelocity);
			this.panel_WindSliders.Controls.Add(this.SliderWindVelocity);
			this.panel_WindSliders.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
			this.panel_WindSliders.Location = new System.Drawing.Point(12, 53);
			this.panel_WindSliders.Name = "panel_WindSliders";
			this.panel_WindSliders.Size = new System.Drawing.Size(183, 169);
			this.panel_WindSliders.TabIndex = 68;
			// 
			// panel_ObjectSliders
			// 
			this.panel_ObjectSliders.BackColor = System.Drawing.Color.Gray;
			this.panel_ObjectSliders.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.panel_ObjectSliders.Controls.Add(this.SliderObjectVelocity);
			this.panel_ObjectSliders.Controls.Add(this.SliderObjectWeight);
			this.panel_ObjectSliders.Controls.Add(this.label_ObjectWeight);
			this.panel_ObjectSliders.Controls.Add(this.label_ObjectVelocity);
			this.panel_ObjectSliders.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
			this.panel_ObjectSliders.Location = new System.Drawing.Point(12, 260);
			this.panel_ObjectSliders.Name = "panel_ObjectSliders";
			this.panel_ObjectSliders.Size = new System.Drawing.Size(181, 68);
			this.panel_ObjectSliders.TabIndex = 69;
			// 
			// label_WindCollection
			// 
			this.label_WindCollection.AutoSize = true;
			this.label_WindCollection.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_WindCollection.Location = new System.Drawing.Point(12, 28);
			this.label_WindCollection.Name = "label_WindCollection";
			this.label_WindCollection.Size = new System.Drawing.Size(51, 22);
			this.label_WindCollection.TabIndex = 70;
			this.label_WindCollection.Text = "Wind";
			// 
			// label_ObjectCollection
			// 
			this.label_ObjectCollection.AutoSize = true;
			this.label_ObjectCollection.Font = new System.Drawing.Font("Microsoft Sans Serif", 13F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_ObjectCollection.Location = new System.Drawing.Point(12, 235);
			this.label_ObjectCollection.Name = "label_ObjectCollection";
			this.label_ObjectCollection.Size = new System.Drawing.Size(62, 22);
			this.label_ObjectCollection.TabIndex = 71;
			this.label_ObjectCollection.Text = "Object";
			// 
			// FlyWind
			// 
			this.AccessibleRole = System.Windows.Forms.AccessibleRole.Window;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
			this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
			this.ClientSize = new System.Drawing.Size(668, 367);
			this.Controls.Add(this.LuaEditVisibility);
			this.Controls.Add(this.label_ObjectCollection);
			this.Controls.Add(this.label_WindCollection);
			this.Controls.Add(this.panel_ObjectSliders);
			this.Controls.Add(this.panel_WindSliders);
			this.Controls.Add(this.label_Gravitation);
			this.Controls.Add(this.RenderBox);
			this.Controls.Add(this.menuStrip);
			this.Controls.Add(this.SliderGravitation);
			this.DoubleBuffered = true;
			this.ForeColor = System.Drawing.Color.Green;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
			this.MainMenuStrip = this.menuStrip;
			this.Name = "FlyWind";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Wind editor";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormClosingEvent);
			this.menuStrip.ResumeLayout(false);
			this.menuStrip.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.SliderWindVelocity)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderWindDirection)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderWindAngle)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderWindLength)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderWindSpehereSize)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderObjectWeight)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderObjectVelocity)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.SliderGravitation)).EndInit();
			this.panel_WindSliders.ResumeLayout(false);
			this.panel_WindSliders.PerformLayout();
			this.panel_ObjectSliders.ResumeLayout(false);
			this.panel_ObjectSliders.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Panel RenderBox;
		private System.Windows.Forms.Button LuaEditVisibility;
		private System.Windows.Forms.MenuStrip menuStrip;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadScriptToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveScriptToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem quitToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
		private System.Windows.Forms.TrackBar SliderWindVelocity;
		private System.Windows.Forms.TrackBar SliderWindDirection;
		private System.Windows.Forms.TrackBar SliderWindAngle;
		private System.Windows.Forms.TrackBar SliderWindLength;
		private System.Windows.Forms.TrackBar SliderWindSpehereSize;
		private System.Windows.Forms.TrackBar SliderObjectWeight;
		private System.Windows.Forms.Label label_WindVelocity;
		private System.Windows.Forms.Label label_WindDirection;
		private System.Windows.Forms.Label label_WindAngle;
		private System.Windows.Forms.Label label_WindLength;
		private System.Windows.Forms.Label label_WindSphereSize;
		private System.Windows.Forms.Label label_ObjectWeight;
		private System.Windows.Forms.Label label_ObjectVelocity;
		private System.Windows.Forms.TrackBar SliderObjectVelocity;
		private System.Windows.Forms.Label label_Gravitation;
		private System.Windows.Forms.TrackBar SliderGravitation;
		private System.Windows.Forms.Panel panel_WindSliders;
		private System.Windows.Forms.Panel panel_ObjectSliders;
		private System.Windows.Forms.Label label_WindCollection;
		private System.Windows.Forms.Label label_ObjectCollection;
	}
}


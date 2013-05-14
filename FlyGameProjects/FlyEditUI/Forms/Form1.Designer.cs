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
			this.NewtoolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.shutdownEngineToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.geometryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadGeomrtyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.outputWindowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolContainer = new System.Windows.Forms.Panel();
			this.button_nameApply = new System.Windows.Forms.Button();
			this.panel_SelectedRotation = new System.Windows.Forms.Panel();
			this.label_SelectedRotation = new System.Windows.Forms.Label();
			this.trackBar_RotationX = new System.Windows.Forms.TrackBar();
			this.trackBar_RotationZ = new System.Windows.Forms.TrackBar();
			this.trackBar_RotationY = new System.Windows.Forms.TrackBar();
			this.panel_SelectedScale = new System.Windows.Forms.Panel();
			this.checkBox_uniformScale = new System.Windows.Forms.CheckBox();
			this.trackBar_ScaleX = new System.Windows.Forms.TrackBar();
			this.trackBar_ScaleZ = new System.Windows.Forms.TrackBar();
			this.label_SelectedScale = new System.Windows.Forms.Label();
			this.trackBar_ScaleY = new System.Windows.Forms.TrackBar();
			this.textBox_SelectedName = new System.Windows.Forms.TextBox();
			this.label_selectedName = new System.Windows.Forms.Label();
			this.label_FGM = new System.Windows.Forms.Label();
			this.CameraDropBox = new System.Windows.Forms.ComboBox();
			this.LoadedResources_name = new System.Windows.Forms.ListBox();
			this.splitter1 = new System.Windows.Forms.Splitter();
			((System.ComponentModel.ISupportInitialize)(this.RenderLockPictureBox)).BeginInit();
			this.menuStrip1.SuspendLayout();
			this.ToolContainer.SuspendLayout();
			this.panel_SelectedRotation.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationX)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationZ)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationY)).BeginInit();
			this.panel_SelectedScale.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleX)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleZ)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleY)).BeginInit();
			this.SuspendLayout();
			// 
			// RenderWin
			// 
			this.RenderWin.BackColor = System.Drawing.SystemColors.ControlLight;
			this.RenderWin.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.RenderWin.Dock = System.Windows.Forms.DockStyle.Fill;
			this.RenderWin.Location = new System.Drawing.Point(0, 24);
			this.RenderWin.Name = "RenderWin";
			this.RenderWin.Size = new System.Drawing.Size(593, 386);
			this.RenderWin.TabIndex = 0;
			// 
			// RenderLockPictureBox
			// 
			this.RenderLockPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.RenderLockPictureBox.BackColor = System.Drawing.Color.Transparent;
			this.RenderLockPictureBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
			this.RenderLockPictureBox.Image = global::FlyEditUI.Properties.Resources.locked;
			this.RenderLockPictureBox.Location = new System.Drawing.Point(123, 0);
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
            this.viewToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(754, 24);
			this.menuStrip1.TabIndex = 1;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.NewtoolStripMenuItem,
            this.saveToolStripMenuItem,
            this.loadToolStripMenuItem,
            this.shutdownEngineToolStripMenuItem,
            this.exitToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
			this.fileToolStripMenuItem.Text = "File";
			// 
			// NewtoolStripMenuItem
			// 
			this.NewtoolStripMenuItem.Name = "NewtoolStripMenuItem";
			this.NewtoolStripMenuItem.Size = new System.Drawing.Size(173, 22);
			this.NewtoolStripMenuItem.Text = "New";
			this.NewtoolStripMenuItem.Click += new System.EventHandler(this.NewtoolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
			this.saveToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
			this.saveToolStripMenuItem.Text = "Save level";
			this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
			// 
			// loadToolStripMenuItem
			// 
			this.loadToolStripMenuItem.Name = "loadToolStripMenuItem";
			this.loadToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
			this.loadToolStripMenuItem.Text = "Load level";
			this.loadToolStripMenuItem.Click += new System.EventHandler(this.loadToolStripMenuItem_Click);
			// 
			// shutdownEngineToolStripMenuItem
			// 
			this.shutdownEngineToolStripMenuItem.Name = "shutdownEngineToolStripMenuItem";
			this.shutdownEngineToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
			this.shutdownEngineToolStripMenuItem.Text = "Suspend rendering";
			this.shutdownEngineToolStripMenuItem.Click += new System.EventHandler(this.shutdownEngineToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
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
			this.loadGeomrtyToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.loadGeomrtyToolStripMenuItem.Text = "Load Geomtry";
			this.loadGeomrtyToolStripMenuItem.Click += new System.EventHandler(this.loadGeomrtyToolStripMenuItem_Click);
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
			// ToolContainer
			// 
			this.ToolContainer.BackColor = System.Drawing.Color.Gray;
			this.ToolContainer.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
			this.ToolContainer.Controls.Add(this.button_nameApply);
			this.ToolContainer.Controls.Add(this.panel_SelectedRotation);
			this.ToolContainer.Controls.Add(this.panel_SelectedScale);
			this.ToolContainer.Controls.Add(this.textBox_SelectedName);
			this.ToolContainer.Controls.Add(this.label_selectedName);
			this.ToolContainer.Controls.Add(this.label_FGM);
			this.ToolContainer.Controls.Add(this.RenderLockPictureBox);
			this.ToolContainer.Controls.Add(this.CameraDropBox);
			this.ToolContainer.Controls.Add(this.LoadedResources_name);
			this.ToolContainer.Dock = System.Windows.Forms.DockStyle.Right;
			this.ToolContainer.Location = new System.Drawing.Point(597, 24);
			this.ToolContainer.MinimumSize = new System.Drawing.Size(157, 0);
			this.ToolContainer.Name = "ToolContainer";
			this.ToolContainer.Size = new System.Drawing.Size(157, 386);
			this.ToolContainer.TabIndex = 2;
			// 
			// button_nameApply
			// 
			this.button_nameApply.Enabled = false;
			this.button_nameApply.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button_nameApply.Location = new System.Drawing.Point(41, 139);
			this.button_nameApply.Margin = new System.Windows.Forms.Padding(0);
			this.button_nameApply.Name = "button_nameApply";
			this.button_nameApply.Size = new System.Drawing.Size(52, 17);
			this.button_nameApply.TabIndex = 12;
			this.button_nameApply.Text = "apply";
			this.button_nameApply.UseVisualStyleBackColor = true;
			this.button_nameApply.Click += new System.EventHandler(this.button_nameApply_Click);
			// 
			// panel_SelectedRotation
			// 
			this.panel_SelectedRotation.BackColor = System.Drawing.Color.DarkGray;
			this.panel_SelectedRotation.Controls.Add(this.label_SelectedRotation);
			this.panel_SelectedRotation.Controls.Add(this.trackBar_RotationX);
			this.panel_SelectedRotation.Controls.Add(this.trackBar_RotationZ);
			this.panel_SelectedRotation.Controls.Add(this.trackBar_RotationY);
			this.panel_SelectedRotation.Location = new System.Drawing.Point(6, 185);
			this.panel_SelectedRotation.Name = "panel_SelectedRotation";
			this.panel_SelectedRotation.Size = new System.Drawing.Size(90, 85);
			this.panel_SelectedRotation.TabIndex = 11;
			// 
			// label_SelectedRotation
			// 
			this.label_SelectedRotation.AutoSize = true;
			this.label_SelectedRotation.Font = new System.Drawing.Font("Pericles Light", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_SelectedRotation.Location = new System.Drawing.Point(3, 0);
			this.label_SelectedRotation.Name = "label_SelectedRotation";
			this.label_SelectedRotation.Size = new System.Drawing.Size(87, 19);
			this.label_SelectedRotation.TabIndex = 6;
			this.label_SelectedRotation.Text = "Rotation: ";
			// 
			// trackBar_RotationX
			// 
			this.trackBar_RotationX.AutoSize = false;
			this.trackBar_RotationX.Location = new System.Drawing.Point(9, 18);
			this.trackBar_RotationX.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_RotationX.Maximum = 360;
			this.trackBar_RotationX.Name = "trackBar_RotationX";
			this.trackBar_RotationX.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_RotationX.Size = new System.Drawing.Size(24, 67);
			this.trackBar_RotationX.TabIndex = 9;
			this.trackBar_RotationX.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_RotationX.Scroll += new System.EventHandler(this.trackBar_Rotation_Scroll);
			// 
			// trackBar_RotationZ
			// 
			this.trackBar_RotationZ.AutoSize = false;
			this.trackBar_RotationZ.Location = new System.Drawing.Point(62, 19);
			this.trackBar_RotationZ.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_RotationZ.Maximum = 360;
			this.trackBar_RotationZ.Name = "trackBar_RotationZ";
			this.trackBar_RotationZ.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_RotationZ.Size = new System.Drawing.Size(24, 67);
			this.trackBar_RotationZ.TabIndex = 9;
			this.trackBar_RotationZ.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_RotationZ.Scroll += new System.EventHandler(this.trackBar_Rotation_Scroll);
			// 
			// trackBar_RotationY
			// 
			this.trackBar_RotationY.AutoSize = false;
			this.trackBar_RotationY.LargeChange = 1;
			this.trackBar_RotationY.Location = new System.Drawing.Point(35, 19);
			this.trackBar_RotationY.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_RotationY.Maximum = 360;
			this.trackBar_RotationY.Name = "trackBar_RotationY";
			this.trackBar_RotationY.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_RotationY.Size = new System.Drawing.Size(24, 67);
			this.trackBar_RotationY.TabIndex = 9;
			this.trackBar_RotationY.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_RotationY.Scroll += new System.EventHandler(this.trackBar_Rotation_Scroll);
			// 
			// panel_SelectedScale
			// 
			this.panel_SelectedScale.BackColor = System.Drawing.Color.DarkGray;
			this.panel_SelectedScale.Controls.Add(this.checkBox_uniformScale);
			this.panel_SelectedScale.Controls.Add(this.trackBar_ScaleX);
			this.panel_SelectedScale.Controls.Add(this.trackBar_ScaleZ);
			this.panel_SelectedScale.Controls.Add(this.label_SelectedScale);
			this.panel_SelectedScale.Controls.Add(this.trackBar_ScaleY);
			this.panel_SelectedScale.Location = new System.Drawing.Point(6, 276);
			this.panel_SelectedScale.Name = "panel_SelectedScale";
			this.panel_SelectedScale.Size = new System.Drawing.Size(90, 103);
			this.panel_SelectedScale.TabIndex = 10;
			// 
			// checkBox_uniformScale
			// 
			this.checkBox_uniformScale.AutoSize = true;
			this.checkBox_uniformScale.Location = new System.Drawing.Point(5, 81);
			this.checkBox_uniformScale.Name = "checkBox_uniformScale";
			this.checkBox_uniformScale.Size = new System.Drawing.Size(62, 17);
			this.checkBox_uniformScale.TabIndex = 10;
			this.checkBox_uniformScale.Text = "Uniform";
			this.checkBox_uniformScale.UseVisualStyleBackColor = true;
			this.checkBox_uniformScale.CheckedChanged += new System.EventHandler(this.checkBox_uniformScale_CheckedChanged);
			// 
			// trackBar_ScaleX
			// 
			this.trackBar_ScaleX.AutoSize = false;
			this.trackBar_ScaleX.Location = new System.Drawing.Point(7, 19);
			this.trackBar_ScaleX.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_ScaleX.Maximum = 1000;
			this.trackBar_ScaleX.Minimum = 1;
			this.trackBar_ScaleX.Name = "trackBar_ScaleX";
			this.trackBar_ScaleX.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_ScaleX.Size = new System.Drawing.Size(24, 66);
			this.trackBar_ScaleX.TabIndex = 9;
			this.trackBar_ScaleX.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_ScaleX.Value = 1;
			this.trackBar_ScaleX.Scroll += new System.EventHandler(this.trackBar_Scale_Scroll);
			// 
			// trackBar_ScaleZ
			// 
			this.trackBar_ScaleZ.AutoSize = false;
			this.trackBar_ScaleZ.Location = new System.Drawing.Point(62, 19);
			this.trackBar_ScaleZ.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_ScaleZ.Maximum = 1000;
			this.trackBar_ScaleZ.Minimum = 1;
			this.trackBar_ScaleZ.Name = "trackBar_ScaleZ";
			this.trackBar_ScaleZ.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_ScaleZ.Size = new System.Drawing.Size(24, 66);
			this.trackBar_ScaleZ.TabIndex = 9;
			this.trackBar_ScaleZ.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_ScaleZ.Value = 1;
			this.trackBar_ScaleZ.Scroll += new System.EventHandler(this.trackBar_Scale_Scroll);
			// 
			// label_SelectedScale
			// 
			this.label_SelectedScale.AutoSize = true;
			this.label_SelectedScale.Font = new System.Drawing.Font("Pericles Light", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_SelectedScale.Location = new System.Drawing.Point(6, 0);
			this.label_SelectedScale.Name = "label_SelectedScale";
			this.label_SelectedScale.Size = new System.Drawing.Size(57, 19);
			this.label_SelectedScale.TabIndex = 7;
			this.label_SelectedScale.Text = "Scale: ";
			// 
			// trackBar_ScaleY
			// 
			this.trackBar_ScaleY.AutoSize = false;
			this.trackBar_ScaleY.Location = new System.Drawing.Point(35, 19);
			this.trackBar_ScaleY.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_ScaleY.Maximum = 1000;
			this.trackBar_ScaleY.Minimum = 1;
			this.trackBar_ScaleY.Name = "trackBar_ScaleY";
			this.trackBar_ScaleY.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_ScaleY.Size = new System.Drawing.Size(24, 66);
			this.trackBar_ScaleY.TabIndex = 9;
			this.trackBar_ScaleY.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_ScaleY.Value = 1;
			this.trackBar_ScaleY.Scroll += new System.EventHandler(this.trackBar_Scale_Scroll);
			// 
			// textBox_SelectedName
			// 
			this.textBox_SelectedName.Location = new System.Drawing.Point(5, 159);
			this.textBox_SelectedName.Name = "textBox_SelectedName";
			this.textBox_SelectedName.Size = new System.Drawing.Size(88, 20);
			this.textBox_SelectedName.TabIndex = 8;
			this.textBox_SelectedName.TextChanged += new System.EventHandler(this.textBox_SelectedName_TextChanged);
			// 
			// label_selectedName
			// 
			this.label_selectedName.AutoSize = true;
			this.label_selectedName.Location = new System.Drawing.Point(3, 143);
			this.label_selectedName.Name = "label_selectedName";
			this.label_selectedName.Size = new System.Drawing.Size(38, 13);
			this.label_selectedName.TabIndex = 5;
			this.label_selectedName.Text = "Name:";
			// 
			// label_FGM
			// 
			this.label_FGM.AutoSize = true;
			this.label_FGM.Location = new System.Drawing.Point(2, 31);
			this.label_FGM.Name = "label_FGM";
			this.label_FGM.Size = new System.Drawing.Size(79, 13);
			this.label_FGM.TabIndex = 4;
			this.label_FGM.Text = "Loaded models";
			// 
			// CameraDropBox
			// 
			this.CameraDropBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.CameraDropBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.CameraDropBox.FormattingEnabled = true;
			this.CameraDropBox.Location = new System.Drawing.Point(3, 5);
			this.CameraDropBox.Name = "CameraDropBox";
			this.CameraDropBox.Size = new System.Drawing.Size(110, 21);
			this.CameraDropBox.TabIndex = 3;
			this.CameraDropBox.SelectedIndexChanged += new System.EventHandler(this.CameraDropBox_SelectedIndexChanged);
			// 
			// LoadedResources_name
			// 
			this.LoadedResources_name.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.LoadedResources_name.BackColor = System.Drawing.Color.Gray;
			this.LoadedResources_name.ForeColor = System.Drawing.Color.White;
			this.LoadedResources_name.FormattingEnabled = true;
			this.LoadedResources_name.IntegralHeight = false;
			this.LoadedResources_name.Location = new System.Drawing.Point(3, 47);
			this.LoadedResources_name.Name = "LoadedResources_name";
			this.LoadedResources_name.Size = new System.Drawing.Size(151, 89);
			this.LoadedResources_name.Sorted = true;
			this.LoadedResources_name.TabIndex = 1;
			this.LoadedResources_name.SelectedIndexChanged += new System.EventHandler(this.CurrentObjChanged);
			// 
			// splitter1
			// 
			this.splitter1.BackColor = System.Drawing.Color.PaleGreen;
			this.splitter1.Dock = System.Windows.Forms.DockStyle.Right;
			this.splitter1.Location = new System.Drawing.Point(593, 24);
			this.splitter1.Name = "splitter1";
			this.splitter1.Size = new System.Drawing.Size(4, 386);
			this.splitter1.TabIndex = 3;
			this.splitter1.TabStop = false;
			this.splitter1.SplitterMoved += new System.Windows.Forms.SplitterEventHandler(this.splitter1_SplitterMoved);
			// 
			// FlyEdit
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(754, 410);
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
			((System.ComponentModel.ISupportInitialize)(this.RenderLockPictureBox)).EndInit();
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.ToolContainer.ResumeLayout(false);
			this.ToolContainer.PerformLayout();
			this.panel_SelectedRotation.ResumeLayout(false);
			this.panel_SelectedRotation.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationX)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationZ)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationY)).EndInit();
			this.panel_SelectedScale.ResumeLayout(false);
			this.panel_SelectedScale.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleX)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleZ)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleY)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Panel RenderWin;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem geometryToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadGeomrtyToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem outputWindowToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem shutdownEngineToolStripMenuItem;
		private System.Windows.Forms.PictureBox RenderLockPictureBox;
		private System.Windows.Forms.Panel ToolContainer;
		private System.Windows.Forms.Splitter splitter1;
		private System.Windows.Forms.ListBox LoadedResources_name;
		private System.Windows.Forms.ComboBox CameraDropBox;
		private System.Windows.Forms.ToolStripMenuItem NewtoolStripMenuItem;
		private System.Windows.Forms.Label label_FGM;
		private System.Windows.Forms.Label label_SelectedScale;
		private System.Windows.Forms.Label label_SelectedRotation;
		private System.Windows.Forms.Label label_selectedName;
		private System.Windows.Forms.Panel panel_SelectedRotation;
		private System.Windows.Forms.TrackBar trackBar_RotationX;
		private System.Windows.Forms.TrackBar trackBar_RotationZ;
		private System.Windows.Forms.TrackBar trackBar_RotationY;
		private System.Windows.Forms.Panel panel_SelectedScale;
		private System.Windows.Forms.TrackBar trackBar_ScaleX;
		private System.Windows.Forms.TrackBar trackBar_ScaleZ;
		private System.Windows.Forms.TrackBar trackBar_ScaleY;
		private System.Windows.Forms.TextBox textBox_SelectedName;
		private System.Windows.Forms.CheckBox checkBox_uniformScale;
		private System.Windows.Forms.Button button_nameApply;
	}
}


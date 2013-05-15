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
			this.panel_Camera = new System.Windows.Forms.Panel();
			this.label_CameraSpeed = new System.Windows.Forms.Label();
			this.CameraSpeedSelector = new System.Windows.Forms.NumericUpDown();
			this.CameraDropBox = new System.Windows.Forms.ComboBox();
			this.label_Camera = new System.Windows.Forms.Label();
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
			this.label_selectedName = new System.Windows.Forms.Label();
			this.label_Mesh = new System.Windows.Forms.Label();
			this.LoadedMesh_name = new System.Windows.Forms.ListBox();
			this.splitter1 = new System.Windows.Forms.Splitter();
			this.comboBox_Lights = new System.Windows.Forms.ComboBox();
			this.comboBox_pickups = new System.Windows.Forms.ComboBox();
			this.panel_Pickups = new System.Windows.Forms.Panel();
			this.label_Pickups = new System.Windows.Forms.Label();
			this.label_Lights = new System.Windows.Forms.Label();
			this.panel_Lights = new System.Windows.Forms.Panel();
			((System.ComponentModel.ISupportInitialize)(this.RenderLockPictureBox)).BeginInit();
			this.menuStrip1.SuspendLayout();
			this.ToolContainer.SuspendLayout();
			this.panel_Camera.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.CameraSpeedSelector)).BeginInit();
			this.panel_SelectedRotation.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationX)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationZ)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationY)).BeginInit();
			this.panel_SelectedScale.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleX)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleZ)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleY)).BeginInit();
			this.panel_Pickups.SuspendLayout();
			this.panel_Lights.SuspendLayout();
			this.SuspendLayout();
			// 
			// RenderWin
			// 
			this.RenderWin.BackColor = System.Drawing.SystemColors.ActiveCaption;
			this.RenderWin.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.RenderWin.Dock = System.Windows.Forms.DockStyle.Fill;
			this.RenderWin.Location = new System.Drawing.Point(0, 24);
			this.RenderWin.Name = "RenderWin";
			this.RenderWin.Size = new System.Drawing.Size(786, 465);
			this.RenderWin.TabIndex = 0;
			// 
			// RenderLockPictureBox
			// 
			this.RenderLockPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.RenderLockPictureBox.BackColor = System.Drawing.Color.Transparent;
			this.RenderLockPictureBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
			this.RenderLockPictureBox.Image = global::FlyEditUI.Properties.Resources.locked;
			this.RenderLockPictureBox.Location = new System.Drawing.Point(980, 0);
			this.RenderLockPictureBox.Name = "RenderLockPictureBox";
			this.RenderLockPictureBox.Size = new System.Drawing.Size(18, 24);
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
			this.menuStrip1.Size = new System.Drawing.Size(998, 24);
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
			this.loadGeomrtyToolStripMenuItem.Size = new System.Drawing.Size(149, 22);
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
			this.ToolContainer.Controls.Add(this.panel_Lights);
			this.ToolContainer.Controls.Add(this.panel_Pickups);
			this.ToolContainer.Controls.Add(this.panel_Camera);
			this.ToolContainer.Controls.Add(this.panel_SelectedRotation);
			this.ToolContainer.Controls.Add(this.panel_SelectedScale);
			this.ToolContainer.Controls.Add(this.label_selectedName);
			this.ToolContainer.Controls.Add(this.label_Mesh);
			this.ToolContainer.Controls.Add(this.LoadedMesh_name);
			this.ToolContainer.Dock = System.Windows.Forms.DockStyle.Right;
			this.ToolContainer.Location = new System.Drawing.Point(790, 24);
			this.ToolContainer.Name = "ToolContainer";
			this.ToolContainer.Size = new System.Drawing.Size(208, 465);
			this.ToolContainer.TabIndex = 2;
			// 
			// panel_Camera
			// 
			this.panel_Camera.BackColor = System.Drawing.Color.DarkGray;
			this.panel_Camera.Controls.Add(this.label_CameraSpeed);
			this.panel_Camera.Controls.Add(this.CameraSpeedSelector);
			this.panel_Camera.Controls.Add(this.CameraDropBox);
			this.panel_Camera.Controls.Add(this.label_Camera);
			this.panel_Camera.Location = new System.Drawing.Point(5, 250);
			this.panel_Camera.Name = "panel_Camera";
			this.panel_Camera.Size = new System.Drawing.Size(194, 80);
			this.panel_Camera.TabIndex = 13;
			// 
			// label_CameraSpeed
			// 
			this.label_CameraSpeed.AutoSize = true;
			this.label_CameraSpeed.Location = new System.Drawing.Point(8, 51);
			this.label_CameraSpeed.Name = "label_CameraSpeed";
			this.label_CameraSpeed.Size = new System.Drawing.Size(38, 13);
			this.label_CameraSpeed.TabIndex = 12;
			this.label_CameraSpeed.Text = "Speed";
			// 
			// CameraSpeedSelector
			// 
			this.CameraSpeedSelector.Location = new System.Drawing.Point(60, 49);
			this.CameraSpeedSelector.Name = "CameraSpeedSelector";
			this.CameraSpeedSelector.Size = new System.Drawing.Size(54, 20);
			this.CameraSpeedSelector.TabIndex = 11;
			this.CameraSpeedSelector.ValueChanged += new System.EventHandler(this.CameraSpeedSelector_ValueChanged);
			// 
			// CameraDropBox
			// 
			this.CameraDropBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.CameraDropBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.CameraDropBox.FormattingEnabled = true;
			this.CameraDropBox.Location = new System.Drawing.Point(4, 22);
			this.CameraDropBox.Name = "CameraDropBox";
			this.CameraDropBox.Size = new System.Drawing.Size(110, 21);
			this.CameraDropBox.TabIndex = 3;
			this.CameraDropBox.SelectedIndexChanged += new System.EventHandler(this.CameraDropBox_SelectedIndexChanged);
			// 
			// label_Camera
			// 
			this.label_Camera.AutoSize = true;
			this.label_Camera.Font = new System.Drawing.Font("Pericles Light", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_Camera.Location = new System.Drawing.Point(7, 0);
			this.label_Camera.Name = "label_Camera";
			this.label_Camera.Size = new System.Drawing.Size(65, 19);
			this.label_Camera.TabIndex = 10;
			this.label_Camera.Text = "Camera";
			// 
			// panel_SelectedRotation
			// 
			this.panel_SelectedRotation.BackColor = System.Drawing.Color.DarkGray;
			this.panel_SelectedRotation.Controls.Add(this.label_SelectedRotation);
			this.panel_SelectedRotation.Controls.Add(this.trackBar_RotationX);
			this.panel_SelectedRotation.Controls.Add(this.trackBar_RotationZ);
			this.panel_SelectedRotation.Controls.Add(this.trackBar_RotationY);
			this.panel_SelectedRotation.Location = new System.Drawing.Point(5, 141);
			this.panel_SelectedRotation.Name = "panel_SelectedRotation";
			this.panel_SelectedRotation.Size = new System.Drawing.Size(94, 103);
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
			this.trackBar_RotationX.Location = new System.Drawing.Point(6, 32);
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
			this.trackBar_RotationZ.Location = new System.Drawing.Point(59, 33);
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
			this.trackBar_RotationY.Location = new System.Drawing.Point(32, 33);
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
			this.panel_SelectedScale.Location = new System.Drawing.Point(105, 141);
			this.panel_SelectedScale.Name = "panel_SelectedScale";
			this.panel_SelectedScale.Size = new System.Drawing.Size(94, 103);
			this.panel_SelectedScale.TabIndex = 10;
			// 
			// checkBox_uniformScale
			// 
			this.checkBox_uniformScale.AutoSize = true;
			this.checkBox_uniformScale.Location = new System.Drawing.Point(12, 24);
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
			this.trackBar_ScaleX.Location = new System.Drawing.Point(5, 34);
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
			this.trackBar_ScaleZ.Location = new System.Drawing.Point(60, 34);
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
			this.label_SelectedScale.Location = new System.Drawing.Point(15, 0);
			this.label_SelectedScale.Name = "label_SelectedScale";
			this.label_SelectedScale.Size = new System.Drawing.Size(57, 19);
			this.label_SelectedScale.TabIndex = 7;
			this.label_SelectedScale.Text = "Scale: ";
			// 
			// trackBar_ScaleY
			// 
			this.trackBar_ScaleY.AutoSize = false;
			this.trackBar_ScaleY.Location = new System.Drawing.Point(33, 34);
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
			// label_selectedName
			// 
			this.label_selectedName.AutoSize = true;
			this.label_selectedName.Location = new System.Drawing.Point(6, 113);
			this.label_selectedName.Name = "label_selectedName";
			this.label_selectedName.Size = new System.Drawing.Size(52, 13);
			this.label_selectedName.TabIndex = 5;
			this.label_selectedName.Text = "Selected:";
			// 
			// label_Mesh
			// 
			this.label_Mesh.AutoSize = true;
			this.label_Mesh.Location = new System.Drawing.Point(2, 5);
			this.label_Mesh.Name = "label_Mesh";
			this.label_Mesh.Size = new System.Drawing.Size(79, 13);
			this.label_Mesh.TabIndex = 4;
			this.label_Mesh.Text = "Loaded models";
			// 
			// LoadedMesh_name
			// 
			this.LoadedMesh_name.BackColor = System.Drawing.Color.DarkGray;
			this.LoadedMesh_name.ForeColor = System.Drawing.Color.White;
			this.LoadedMesh_name.FormattingEnabled = true;
			this.LoadedMesh_name.IntegralHeight = false;
			this.LoadedMesh_name.Location = new System.Drawing.Point(3, 21);
			this.LoadedMesh_name.Name = "LoadedMesh_name";
			this.LoadedMesh_name.Size = new System.Drawing.Size(196, 89);
			this.LoadedMesh_name.Sorted = true;
			this.LoadedMesh_name.TabIndex = 1;
			this.LoadedMesh_name.SelectedIndexChanged += new System.EventHandler(this.CurrentObjChanged);
			// 
			// splitter1
			// 
			this.splitter1.BackColor = System.Drawing.Color.PaleGreen;
			this.splitter1.Dock = System.Windows.Forms.DockStyle.Right;
			this.splitter1.Location = new System.Drawing.Point(786, 24);
			this.splitter1.Name = "splitter1";
			this.splitter1.Size = new System.Drawing.Size(4, 465);
			this.splitter1.TabIndex = 3;
			this.splitter1.TabStop = false;
			this.splitter1.SplitterMoved += new System.Windows.Forms.SplitterEventHandler(this.splitter1_SplitterMoved);
			// 
			// comboBox_Lights
			// 
			this.comboBox_Lights.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBox_Lights.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.comboBox_Lights.FormattingEnabled = true;
			this.comboBox_Lights.Location = new System.Drawing.Point(5, 22);
			this.comboBox_Lights.Name = "comboBox_Lights";
			this.comboBox_Lights.Size = new System.Drawing.Size(110, 21);
			this.comboBox_Lights.TabIndex = 13;
			// 
			// comboBox_pickups
			// 
			this.comboBox_pickups.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboBox_pickups.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.comboBox_pickups.FormattingEnabled = true;
			this.comboBox_pickups.Location = new System.Drawing.Point(3, 22);
			this.comboBox_pickups.Name = "comboBox_pickups";
			this.comboBox_pickups.Size = new System.Drawing.Size(110, 21);
			this.comboBox_pickups.TabIndex = 14;
			// 
			// panel_Pickups
			// 
			this.panel_Pickups.BackColor = System.Drawing.Color.DarkGray;
			this.panel_Pickups.Controls.Add(this.label_Pickups);
			this.panel_Pickups.Controls.Add(this.comboBox_pickups);
			this.panel_Pickups.Location = new System.Drawing.Point(5, 336);
			this.panel_Pickups.Name = "panel_Pickups";
			this.panel_Pickups.Size = new System.Drawing.Size(194, 54);
			this.panel_Pickups.TabIndex = 15;
			// 
			// label_Pickups
			// 
			this.label_Pickups.AutoSize = true;
			this.label_Pickups.Font = new System.Drawing.Font("Pericles Light", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_Pickups.Location = new System.Drawing.Point(7, 0);
			this.label_Pickups.Name = "label_Pickups";
			this.label_Pickups.Size = new System.Drawing.Size(72, 19);
			this.label_Pickups.TabIndex = 11;
			this.label_Pickups.Text = "Pickups: ";
			// 
			// label_Lights
			// 
			this.label_Lights.AutoSize = true;
			this.label_Lights.Font = new System.Drawing.Font("Pericles Light", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_Lights.Location = new System.Drawing.Point(7, 0);
			this.label_Lights.Name = "label_Lights";
			this.label_Lights.Size = new System.Drawing.Size(64, 19);
			this.label_Lights.TabIndex = 11;
			this.label_Lights.Text = "Lights: ";
			// 
			// panel_Lights
			// 
			this.panel_Lights.BackColor = System.Drawing.Color.DarkGray;
			this.panel_Lights.Controls.Add(this.label_Lights);
			this.panel_Lights.Controls.Add(this.comboBox_Lights);
			this.panel_Lights.Location = new System.Drawing.Point(5, 402);
			this.panel_Lights.Name = "panel_Lights";
			this.panel_Lights.Size = new System.Drawing.Size(194, 54);
			this.panel_Lights.TabIndex = 16;
			// 
			// FlyEdit
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(998, 489);
			this.Controls.Add(this.RenderLockPictureBox);
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
			this.panel_Camera.ResumeLayout(false);
			this.panel_Camera.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.CameraSpeedSelector)).EndInit();
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
			this.panel_Pickups.ResumeLayout(false);
			this.panel_Pickups.PerformLayout();
			this.panel_Lights.ResumeLayout(false);
			this.panel_Lights.PerformLayout();
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
		private System.Windows.Forms.ListBox LoadedMesh_name;
		private System.Windows.Forms.ComboBox CameraDropBox;
		private System.Windows.Forms.ToolStripMenuItem NewtoolStripMenuItem;
		private System.Windows.Forms.Label label_Mesh;
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
		private System.Windows.Forms.CheckBox checkBox_uniformScale;
		private System.Windows.Forms.Panel panel_Camera;
		private System.Windows.Forms.NumericUpDown CameraSpeedSelector;
		private System.Windows.Forms.Label label_Camera;
		private System.Windows.Forms.Label label_CameraSpeed;
		private System.Windows.Forms.Panel panel_Lights;
		private System.Windows.Forms.Label label_Lights;
		private System.Windows.Forms.ComboBox comboBox_Lights;
		private System.Windows.Forms.Panel panel_Pickups;
		private System.Windows.Forms.Label label_Pickups;
		private System.Windows.Forms.ComboBox comboBox_pickups;
	}
}


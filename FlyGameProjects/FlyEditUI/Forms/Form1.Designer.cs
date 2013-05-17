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
			System.Windows.Forms.TreeNode treeNode21 = new System.Windows.Forms.TreeNode("Mesh");
			System.Windows.Forms.TreeNode treeNode22 = new System.Windows.Forms.TreeNode("Pickups");
			System.Windows.Forms.TreeNode treeNode23 = new System.Windows.Forms.TreeNode("Lights");
			System.Windows.Forms.TreeNode treeNode24 = new System.Windows.Forms.TreeNode("Camera");
			System.Windows.Forms.TreeNode treeNode25 = new System.Windows.Forms.TreeNode("Terrain");
			this.RenderWin = new System.Windows.Forms.Panel();
			this.RenderLockPictureBox = new System.Windows.Forms.PictureBox();
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.NewtoolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.loadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.shutdownEngineToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolContainer = new System.Windows.Forms.Panel();
			this.panel_Lights = new System.Windows.Forms.Panel();
			this.panel_Pickups = new System.Windows.Forms.Panel();
			this.panel_Terrain = new System.Windows.Forms.Panel();
			this.button_LoadTerrain = new System.Windows.Forms.Button();
			this.panel_Geometry = new System.Windows.Forms.Panel();
			this.button_LoadGeometry = new System.Windows.Forms.Button();
			this.ResourceTree = new System.Windows.Forms.TreeView();
			this.panel_Camera = new System.Windows.Forms.Panel();
			this.button_CreateCamera = new System.Windows.Forms.Button();
			this.label_CameraSpeed = new System.Windows.Forms.Label();
			this.CameraSpeedSelector = new System.Windows.Forms.NumericUpDown();
			this.panel_SelectedScale = new System.Windows.Forms.Panel();
			this.label_SelectedRotation = new System.Windows.Forms.Label();
			this.checkBox_uniformScale = new System.Windows.Forms.CheckBox();
			this.trackBar_RotationX = new System.Windows.Forms.TrackBar();
			this.trackBar_ScaleX = new System.Windows.Forms.TrackBar();
			this.trackBar_RotationZ = new System.Windows.Forms.TrackBar();
			this.trackBar_ScaleZ = new System.Windows.Forms.TrackBar();
			this.trackBar_RotationY = new System.Windows.Forms.TrackBar();
			this.label_SelectedScale = new System.Windows.Forms.Label();
			this.trackBar_ScaleY = new System.Windows.Forms.TrackBar();
			this.label_selectedName = new System.Windows.Forms.Label();
			this.splitter1 = new System.Windows.Forms.Splitter();
			((System.ComponentModel.ISupportInitialize)(this.RenderLockPictureBox)).BeginInit();
			this.menuStrip1.SuspendLayout();
			this.ToolContainer.SuspendLayout();
			this.panel_Terrain.SuspendLayout();
			this.panel_Geometry.SuspendLayout();
			this.panel_Camera.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.CameraSpeedSelector)).BeginInit();
			this.panel_SelectedScale.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationX)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleX)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationZ)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleZ)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationY)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleY)).BeginInit();
			this.SuspendLayout();
			// 
			// RenderWin
			// 
			this.RenderWin.BackColor = System.Drawing.SystemColors.ActiveCaption;
			this.RenderWin.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.RenderWin.Dock = System.Windows.Forms.DockStyle.Fill;
			this.RenderWin.Location = new System.Drawing.Point(0, 24);
			this.RenderWin.Name = "RenderWin";
			this.RenderWin.Size = new System.Drawing.Size(764, 473);
			this.RenderWin.TabIndex = 0;
			// 
			// RenderLockPictureBox
			// 
			this.RenderLockPictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.RenderLockPictureBox.BackColor = System.Drawing.Color.Transparent;
			this.RenderLockPictureBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
			this.RenderLockPictureBox.Image = global::FlyEditUI.Properties.Resources.locked;
			this.RenderLockPictureBox.Location = new System.Drawing.Point(1014, 0);
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
            this.fileToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(1032, 24);
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
			// ToolContainer
			// 
			this.ToolContainer.BackColor = System.Drawing.Color.Gray;
			this.ToolContainer.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
			this.ToolContainer.Controls.Add(this.panel_Geometry);
			this.ToolContainer.Controls.Add(this.panel_Lights);
			this.ToolContainer.Controls.Add(this.panel_Terrain);
			this.ToolContainer.Controls.Add(this.panel_Pickups);
			this.ToolContainer.Controls.Add(this.ResourceTree);
			this.ToolContainer.Controls.Add(this.panel_Camera);
			this.ToolContainer.Controls.Add(this.panel_SelectedScale);
			this.ToolContainer.Dock = System.Windows.Forms.DockStyle.Right;
			this.ToolContainer.Location = new System.Drawing.Point(768, 24);
			this.ToolContainer.Name = "ToolContainer";
			this.ToolContainer.Size = new System.Drawing.Size(264, 473);
			this.ToolContainer.TabIndex = 2;
			// 
			// panel_Lights
			// 
			this.panel_Lights.BackColor = System.Drawing.Color.DarkGray;
			this.panel_Lights.Location = new System.Drawing.Point(526, 229);
			this.panel_Lights.Name = "panel_Lights";
			this.panel_Lights.Size = new System.Drawing.Size(254, 240);
			this.panel_Lights.TabIndex = 16;
			this.panel_Lights.Visible = false;
			// 
			// panel_Pickups
			// 
			this.panel_Pickups.BackColor = System.Drawing.Color.DarkGray;
			this.panel_Pickups.Location = new System.Drawing.Point(266, 475);
			this.panel_Pickups.Name = "panel_Pickups";
			this.panel_Pickups.Size = new System.Drawing.Size(254, 240);
			this.panel_Pickups.TabIndex = 16;
			this.panel_Pickups.Visible = false;
			// 
			// panel_Terrain
			// 
			this.panel_Terrain.BackColor = System.Drawing.Color.DarkGray;
			this.panel_Terrain.Controls.Add(this.button_LoadTerrain);
			this.panel_Terrain.Location = new System.Drawing.Point(6, 475);
			this.panel_Terrain.Name = "panel_Terrain";
			this.panel_Terrain.Size = new System.Drawing.Size(254, 240);
			this.panel_Terrain.TabIndex = 15;
			this.panel_Terrain.Visible = false;
			// 
			// button_LoadTerrain
			// 
			this.button_LoadTerrain.FlatAppearance.BorderColor = System.Drawing.Color.Black;
			this.button_LoadTerrain.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button_LoadTerrain.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.button_LoadTerrain.Location = new System.Drawing.Point(23, 12);
			this.button_LoadTerrain.Margin = new System.Windows.Forms.Padding(0);
			this.button_LoadTerrain.Name = "button_LoadTerrain";
			this.button_LoadTerrain.Size = new System.Drawing.Size(74, 17);
			this.button_LoadTerrain.TabIndex = 15;
			this.button_LoadTerrain.Text = "Load Terrain";
			this.button_LoadTerrain.UseVisualStyleBackColor = true;
			this.button_LoadTerrain.Click += new System.EventHandler(this.button_LoadTerrain_Click);
			// 
			// panel_Geometry
			// 
			this.panel_Geometry.BackColor = System.Drawing.Color.DarkGray;
			this.panel_Geometry.Controls.Add(this.button_LoadGeometry);
			this.panel_Geometry.Location = new System.Drawing.Point(266, 229);
			this.panel_Geometry.Name = "panel_Geometry";
			this.panel_Geometry.Size = new System.Drawing.Size(254, 240);
			this.panel_Geometry.TabIndex = 14;
			this.panel_Geometry.Visible = false;
			// 
			// button_LoadGeometry
			// 
			this.button_LoadGeometry.FlatAppearance.BorderColor = System.Drawing.Color.Black;
			this.button_LoadGeometry.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.button_LoadGeometry.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.button_LoadGeometry.Location = new System.Drawing.Point(7, 9);
			this.button_LoadGeometry.Margin = new System.Windows.Forms.Padding(0);
			this.button_LoadGeometry.Name = "button_LoadGeometry";
			this.button_LoadGeometry.Size = new System.Drawing.Size(94, 17);
			this.button_LoadGeometry.TabIndex = 7;
			this.button_LoadGeometry.Text = "Load Geometry";
			this.button_LoadGeometry.UseVisualStyleBackColor = true;
			this.button_LoadGeometry.Click += new System.EventHandler(this.button_LoadGeometry_Click);
			// 
			// ResourceTree
			// 
			this.ResourceTree.BackColor = System.Drawing.Color.DarkGray;
			this.ResourceTree.Font = new System.Drawing.Font("Moire", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.ResourceTree.HotTracking = true;
			this.ResourceTree.Indent = 15;
			this.ResourceTree.ItemHeight = 20;
			this.ResourceTree.LineColor = System.Drawing.Color.WhiteSmoke;
			this.ResourceTree.Location = new System.Drawing.Point(6, 6);
			this.ResourceTree.Name = "ResourceTree";
			treeNode21.Name = "Node_GameMesh";
			treeNode21.Text = "Mesh";
			treeNode22.Name = "Node_Pickups";
			treeNode22.Text = "Pickups";
			treeNode23.Name = "Node_Lights";
			treeNode23.Text = "Lights";
			treeNode24.Name = "Node_Camera";
			treeNode24.Text = "Camera";
			treeNode25.Name = "Node_Terrain";
			treeNode25.Text = "Terrain";
			this.ResourceTree.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode21,
            treeNode22,
            treeNode23,
            treeNode24,
            treeNode25});
			this.ResourceTree.ShowNodeToolTips = true;
			this.ResourceTree.Size = new System.Drawing.Size(154, 217);
			this.ResourceTree.TabIndex = 14;
			this.ResourceTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.ResourceTree_AfterSelect);
			// 
			// panel_Camera
			// 
			this.panel_Camera.BackColor = System.Drawing.Color.DarkGray;
			this.panel_Camera.Controls.Add(this.button_CreateCamera);
			this.panel_Camera.Controls.Add(this.label_CameraSpeed);
			this.panel_Camera.Controls.Add(this.CameraSpeedSelector);
			this.panel_Camera.Location = new System.Drawing.Point(6, 229);
			this.panel_Camera.Name = "panel_Camera";
			this.panel_Camera.Size = new System.Drawing.Size(254, 240);
			this.panel_Camera.TabIndex = 13;
			this.panel_Camera.Visible = false;
			// 
			// button_CreateCamera
			// 
			this.button_CreateCamera.Location = new System.Drawing.Point(6, 38);
			this.button_CreateCamera.Name = "button_CreateCamera";
			this.button_CreateCamera.Size = new System.Drawing.Size(95, 23);
			this.button_CreateCamera.TabIndex = 13;
			this.button_CreateCamera.Text = "Create camera";
			this.button_CreateCamera.UseVisualStyleBackColor = true;
			// 
			// label_CameraSpeed
			// 
			this.label_CameraSpeed.AutoSize = true;
			this.label_CameraSpeed.Location = new System.Drawing.Point(3, 14);
			this.label_CameraSpeed.Name = "label_CameraSpeed";
			this.label_CameraSpeed.Size = new System.Drawing.Size(38, 13);
			this.label_CameraSpeed.TabIndex = 12;
			this.label_CameraSpeed.Text = "Speed";
			// 
			// CameraSpeedSelector
			// 
			this.CameraSpeedSelector.Location = new System.Drawing.Point(47, 12);
			this.CameraSpeedSelector.Name = "CameraSpeedSelector";
			this.CameraSpeedSelector.Size = new System.Drawing.Size(54, 20);
			this.CameraSpeedSelector.TabIndex = 11;
			this.CameraSpeedSelector.ValueChanged += new System.EventHandler(this.CameraSpeedSelector_ValueChanged);
			// 
			// panel_SelectedScale
			// 
			this.panel_SelectedScale.BackColor = System.Drawing.Color.DarkGray;
			this.panel_SelectedScale.Controls.Add(this.label_SelectedRotation);
			this.panel_SelectedScale.Controls.Add(this.checkBox_uniformScale);
			this.panel_SelectedScale.Controls.Add(this.trackBar_RotationX);
			this.panel_SelectedScale.Controls.Add(this.trackBar_ScaleX);
			this.panel_SelectedScale.Controls.Add(this.trackBar_RotationZ);
			this.panel_SelectedScale.Controls.Add(this.trackBar_ScaleZ);
			this.panel_SelectedScale.Controls.Add(this.trackBar_RotationY);
			this.panel_SelectedScale.Controls.Add(this.label_SelectedScale);
			this.panel_SelectedScale.Controls.Add(this.trackBar_ScaleY);
			this.panel_SelectedScale.Location = new System.Drawing.Point(166, 6);
			this.panel_SelectedScale.Name = "panel_SelectedScale";
			this.panel_SelectedScale.Size = new System.Drawing.Size(94, 217);
			this.panel_SelectedScale.TabIndex = 10;
			// 
			// label_SelectedRotation
			// 
			this.label_SelectedRotation.AutoSize = true;
			this.label_SelectedRotation.Font = new System.Drawing.Font("Pericles Light", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_SelectedRotation.Location = new System.Drawing.Point(11, 124);
			this.label_SelectedRotation.Name = "label_SelectedRotation";
			this.label_SelectedRotation.Size = new System.Drawing.Size(69, 19);
			this.label_SelectedRotation.TabIndex = 6;
			this.label_SelectedRotation.Text = "Rotate: ";
			// 
			// checkBox_uniformScale
			// 
			this.checkBox_uniformScale.AutoSize = true;
			this.checkBox_uniformScale.Location = new System.Drawing.Point(12, 32);
			this.checkBox_uniformScale.Name = "checkBox_uniformScale";
			this.checkBox_uniformScale.Size = new System.Drawing.Size(62, 17);
			this.checkBox_uniformScale.TabIndex = 10;
			this.checkBox_uniformScale.Text = "Uniform";
			this.checkBox_uniformScale.UseVisualStyleBackColor = true;
			this.checkBox_uniformScale.CheckedChanged += new System.EventHandler(this.checkBox_uniformScale_CheckedChanged);
			// 
			// trackBar_RotationX
			// 
			this.trackBar_RotationX.AutoSize = false;
			this.trackBar_RotationX.Location = new System.Drawing.Point(8, 143);
			this.trackBar_RotationX.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_RotationX.Maximum = 360;
			this.trackBar_RotationX.Name = "trackBar_RotationX";
			this.trackBar_RotationX.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_RotationX.Size = new System.Drawing.Size(24, 67);
			this.trackBar_RotationX.TabIndex = 9;
			this.trackBar_RotationX.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_RotationX.MouseDown += new System.Windows.Forms.MouseEventHandler(this.RotationSliderMouseDownEvent);
			this.trackBar_RotationX.MouseUp += new System.Windows.Forms.MouseEventHandler(this.RotationSliderMoseUpEven);
			// 
			// trackBar_ScaleX
			// 
			this.trackBar_ScaleX.AutoSize = false;
			this.trackBar_ScaleX.Location = new System.Drawing.Point(7, 51);
			this.trackBar_ScaleX.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_ScaleX.Maximum = 4000;
			this.trackBar_ScaleX.Minimum = 1;
			this.trackBar_ScaleX.Name = "trackBar_ScaleX";
			this.trackBar_ScaleX.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_ScaleX.Size = new System.Drawing.Size(24, 66);
			this.trackBar_ScaleX.TabIndex = 9;
			this.trackBar_ScaleX.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_ScaleX.Value = 1;
			this.trackBar_ScaleX.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ScaleSliderMouseDownEvent);
			this.trackBar_ScaleX.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ScaleSliderMouseUpEvent);
			// 
			// trackBar_RotationZ
			// 
			this.trackBar_RotationZ.AutoSize = false;
			this.trackBar_RotationZ.Location = new System.Drawing.Point(61, 144);
			this.trackBar_RotationZ.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_RotationZ.Maximum = 360;
			this.trackBar_RotationZ.Name = "trackBar_RotationZ";
			this.trackBar_RotationZ.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_RotationZ.Size = new System.Drawing.Size(24, 67);
			this.trackBar_RotationZ.TabIndex = 9;
			this.trackBar_RotationZ.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_RotationZ.MouseDown += new System.Windows.Forms.MouseEventHandler(this.RotationSliderMouseDownEvent);
			this.trackBar_RotationZ.MouseUp += new System.Windows.Forms.MouseEventHandler(this.RotationSliderMoseUpEven);
			// 
			// trackBar_ScaleZ
			// 
			this.trackBar_ScaleZ.AutoSize = false;
			this.trackBar_ScaleZ.Location = new System.Drawing.Point(62, 51);
			this.trackBar_ScaleZ.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_ScaleZ.Maximum = 4000;
			this.trackBar_ScaleZ.Minimum = 1;
			this.trackBar_ScaleZ.Name = "trackBar_ScaleZ";
			this.trackBar_ScaleZ.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_ScaleZ.Size = new System.Drawing.Size(24, 66);
			this.trackBar_ScaleZ.TabIndex = 9;
			this.trackBar_ScaleZ.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_ScaleZ.Value = 1;
			this.trackBar_ScaleZ.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ScaleSliderMouseDownEvent);
			this.trackBar_ScaleZ.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ScaleSliderMouseUpEvent);
			// 
			// trackBar_RotationY
			// 
			this.trackBar_RotationY.AutoSize = false;
			this.trackBar_RotationY.LargeChange = 1;
			this.trackBar_RotationY.Location = new System.Drawing.Point(34, 144);
			this.trackBar_RotationY.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_RotationY.Maximum = 360;
			this.trackBar_RotationY.Name = "trackBar_RotationY";
			this.trackBar_RotationY.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_RotationY.Size = new System.Drawing.Size(24, 67);
			this.trackBar_RotationY.TabIndex = 9;
			this.trackBar_RotationY.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_RotationY.MouseDown += new System.Windows.Forms.MouseEventHandler(this.RotationSliderMouseDownEvent);
			this.trackBar_RotationY.MouseUp += new System.Windows.Forms.MouseEventHandler(this.RotationSliderMoseUpEven);
			// 
			// label_SelectedScale
			// 
			this.label_SelectedScale.AutoSize = true;
			this.label_SelectedScale.Font = new System.Drawing.Font("Pericles Light", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_SelectedScale.Location = new System.Drawing.Point(15, 8);
			this.label_SelectedScale.Name = "label_SelectedScale";
			this.label_SelectedScale.Size = new System.Drawing.Size(57, 19);
			this.label_SelectedScale.TabIndex = 7;
			this.label_SelectedScale.Text = "Scale: ";
			// 
			// trackBar_ScaleY
			// 
			this.trackBar_ScaleY.AutoSize = false;
			this.trackBar_ScaleY.Location = new System.Drawing.Point(35, 51);
			this.trackBar_ScaleY.Margin = new System.Windows.Forms.Padding(0);
			this.trackBar_ScaleY.Maximum = 4000;
			this.trackBar_ScaleY.Minimum = 1;
			this.trackBar_ScaleY.Name = "trackBar_ScaleY";
			this.trackBar_ScaleY.Orientation = System.Windows.Forms.Orientation.Vertical;
			this.trackBar_ScaleY.Size = new System.Drawing.Size(24, 66);
			this.trackBar_ScaleY.TabIndex = 9;
			this.trackBar_ScaleY.TickStyle = System.Windows.Forms.TickStyle.None;
			this.trackBar_ScaleY.Value = 1;
			this.trackBar_ScaleY.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ScaleSliderMouseDownEvent);
			this.trackBar_ScaleY.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ScaleSliderMouseUpEvent);
			// 
			// label_selectedName
			// 
			this.label_selectedName.AutoSize = true;
			this.label_selectedName.Font = new System.Drawing.Font("Meiryo", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.label_selectedName.ForeColor = System.Drawing.Color.DarkRed;
			this.label_selectedName.Location = new System.Drawing.Point(770, 4);
			this.label_selectedName.Name = "label_selectedName";
			this.label_selectedName.Size = new System.Drawing.Size(63, 18);
			this.label_selectedName.TabIndex = 5;
			this.label_selectedName.Text = "Selected:";
			// 
			// splitter1
			// 
			this.splitter1.BackColor = System.Drawing.Color.PaleGreen;
			this.splitter1.Dock = System.Windows.Forms.DockStyle.Right;
			this.splitter1.Location = new System.Drawing.Point(764, 24);
			this.splitter1.Name = "splitter1";
			this.splitter1.Size = new System.Drawing.Size(4, 473);
			this.splitter1.TabIndex = 3;
			this.splitter1.TabStop = false;
			this.splitter1.SplitterMoved += new System.Windows.Forms.SplitterEventHandler(this.splitter1_SplitterMoved);
			// 
			// FlyEdit
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1032, 497);
			this.Controls.Add(this.label_selectedName);
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
			this.panel_Terrain.ResumeLayout(false);
			this.panel_Geometry.ResumeLayout(false);
			this.panel_Camera.ResumeLayout(false);
			this.panel_Camera.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.CameraSpeedSelector)).EndInit();
			this.panel_SelectedScale.ResumeLayout(false);
			this.panel_SelectedScale.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationX)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleX)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationZ)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_ScaleZ)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.trackBar_RotationY)).EndInit();
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
		private System.Windows.Forms.ToolStripMenuItem shutdownEngineToolStripMenuItem;
		private System.Windows.Forms.PictureBox RenderLockPictureBox;
		private System.Windows.Forms.Panel ToolContainer;
		private System.Windows.Forms.Splitter splitter1;
		private System.Windows.Forms.ToolStripMenuItem NewtoolStripMenuItem;
		private System.Windows.Forms.Label label_SelectedScale;
		private System.Windows.Forms.Label label_SelectedRotation;
		private System.Windows.Forms.Label label_selectedName;
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
		private System.Windows.Forms.Label label_CameraSpeed;
		private System.Windows.Forms.Button button_LoadGeometry;
		private System.Windows.Forms.TreeView ResourceTree;
		private System.Windows.Forms.Button button_LoadTerrain;
		private System.Windows.Forms.Button button_CreateCamera;
		private System.Windows.Forms.Panel panel_Terrain;
		private System.Windows.Forms.Panel panel_Geometry;
		private System.Windows.Forms.Panel panel_Lights;
		private System.Windows.Forms.Panel panel_Pickups;
	}
}


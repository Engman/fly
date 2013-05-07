namespace FlyEditUI
{
	partial class Output
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
			this.OutputWin = new System.Windows.Forms.RichTextBox();
			this.ClearBtn = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// OutputWin
			// 
			this.OutputWin.BackColor = System.Drawing.Color.Black;
			this.OutputWin.Dock = System.Windows.Forms.DockStyle.Fill;
			this.OutputWin.ForeColor = System.Drawing.Color.Green;
			this.OutputWin.Location = new System.Drawing.Point(0, 0);
			this.OutputWin.Name = "OutputWin";
			this.OutputWin.ReadOnly = true;
			this.OutputWin.Size = new System.Drawing.Size(337, 182);
			this.OutputWin.TabIndex = 0;
			this.OutputWin.Text = "";
			this.OutputWin.MouseEnter += new System.EventHandler(this.MouseEnterOutputLable);
			// 
			// ClearBtn
			// 
			this.ClearBtn.BackColor = System.Drawing.Color.Black;
			this.ClearBtn.Font = new System.Drawing.Font("MS Gothic", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.ClearBtn.ForeColor = System.Drawing.Color.Green;
			this.ClearBtn.Location = new System.Drawing.Point(279, 12);
			this.ClearBtn.Name = "ClearBtn";
			this.ClearBtn.Size = new System.Drawing.Size(46, 21);
			this.ClearBtn.TabIndex = 1;
			this.ClearBtn.Text = "Clear";
			this.ClearBtn.UseVisualStyleBackColor = false;
			this.ClearBtn.Click += new System.EventHandler(this.ButtonClearOutput_Click);
			this.ClearBtn.MouseLeave += new System.EventHandler(this.MouseLeaveOutputBTN);
			// 
			// Output
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(337, 182);
			this.ControlBox = false;
			this.Controls.Add(this.ClearBtn);
			this.Controls.Add(this.OutputWin);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
			this.Name = "Output";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
			this.Text = "OutputWindow";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FromClosing);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.RichTextBox OutputWin;
		private System.Windows.Forms.Button ClearBtn;
	}
}
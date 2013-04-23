namespace FlyEditUI
{
	partial class Form1
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
			this.SuspendLayout();
			// 
			// RenderWin
			// 
			this.RenderWin.BackColor = System.Drawing.SystemColors.ControlLight;
			this.RenderWin.Location = new System.Drawing.Point(47, 60);
			this.RenderWin.Name = "RenderWin";
			this.RenderWin.Size = new System.Drawing.Size(508, 355);
			this.RenderWin.TabIndex = 0;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(602, 427);
			this.Controls.Add(this.RenderWin);
			this.Name = "Form1";
			this.Text = "Form1";
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel RenderWin;
	}
}


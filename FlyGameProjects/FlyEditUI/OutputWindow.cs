using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FlyEditUI
{
	public partial class Output : Form
	{
		public Output()
		{
			InitializeComponent();
		}


		private void MouseEnterOutputLable(object sender, EventArgs e)
		{
			this.ClearBtn.Visible = true;
		}
		private void MouseLeaveOutputBTN(object sender, EventArgs e)
		{
			this.ClearBtn.Visible = false;
		}
		private void ButtonClearOutput_Click(object sender, EventArgs e)
		{
			this.OutputWin.Text = "";
		}

		public void addText(String text)
		{
			this.OutputWin.Text += DateTime.Now.ToLongTimeString() + " - " + text + "\n--------------------------------------------------------------\n\n";
		}

		private void FromClosing(object sender, FormClosingEventArgs e)
		{
			this.Hide();
			e.Cancel = true;
			return;
		}
	}
}

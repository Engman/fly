using System;
using System.IO;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using ScintillaNET;

namespace FlyWindEditor
{
	public partial class LuaEdit :Form
	{
		private SaveFileDialog saveDialog = null;
		OpenFileDialog openDialog = null;
		private Panel codeControl;
		private Scintilla LuaCodeEditor = null;
		private Label ScriptSaveStatus;
		String lastPath = "";
		
		public bool UnsavedChanges
		{
			get { return this.LuaCodeEditor.Modified; }
		}
		public Scintilla LuaCode
		{
			get { return LuaCodeEditor; }
		}

		public LuaEdit()
		{
			InitializeComponent();
			this.saveDialog = new SaveFileDialog();
			this.saveDialog.Filter = "All files|*.*";
			this.saveDialog.DefaultExt = ".lua";
			this.saveDialog.AddExtension = true;
			this.openDialog = new OpenFileDialog();
			this.openDialog.Filter = "LUA files|*.lua|Text files|*.txt|All files|*.*";
			this.LuaCodeEditor.Styles.LineNumber.BackColor = System.Drawing.Color.Silver;
		}

		private void InitializeComponent()
		{
			this.LuaCodeEditor = new ScintillaNET.Scintilla();
			this.codeControl = new System.Windows.Forms.Panel();
			this.ScriptSaveStatus = new System.Windows.Forms.Label();
			((System.ComponentModel.ISupportInitialize)(this.LuaCodeEditor)).BeginInit();
			this.codeControl.SuspendLayout();
			this.SuspendLayout();
			// 
			// LuaCodeEditor
			// 
			this.LuaCodeEditor.Annotations.Visibility = ScintillaNET.AnnotationsVisibility.Standard;
			this.LuaCodeEditor.BackColor = System.Drawing.SystemColors.Control;
			this.LuaCodeEditor.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.LuaCodeEditor.Caret.CurrentLineBackgroundAlpha = 50;
			this.LuaCodeEditor.Caret.CurrentLineBackgroundColor = System.Drawing.Color.Silver;
			this.LuaCodeEditor.Caret.HighlightCurrentLine = true;
			this.LuaCodeEditor.ConfigurationManager.CustomLocation = "ScintillaLanguage.xml";
			this.LuaCodeEditor.ConfigurationManager.Language = "lua";
			this.LuaCodeEditor.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LuaCodeEditor.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.LuaCodeEditor.ForeColor = System.Drawing.Color.Black;
			this.LuaCodeEditor.LineWrapping.IndentSize = 4;
			this.LuaCodeEditor.Location = new System.Drawing.Point(0, 32);
			this.LuaCodeEditor.LongLines.EdgeColor = System.Drawing.Color.Black;
			this.LuaCodeEditor.Margins.Left = 5;
			this.LuaCodeEditor.Margins.Margin0.IsFoldMargin = true;
			this.LuaCodeEditor.Margins.Margin0.IsMarkerMargin = true;
			this.LuaCodeEditor.Margins.Margin0.Width = 25;
			this.LuaCodeEditor.Margins.Margin1.Type = ScintillaNET.MarginType.Fore;
			this.LuaCodeEditor.Margins.Margin1.Width = 1;
			this.LuaCodeEditor.Name = "LuaCodeEditor";
			this.LuaCodeEditor.Padding = new System.Windows.Forms.Padding(3);
			this.LuaCodeEditor.Scrolling.HorizontalWidth = 1;
			this.LuaCodeEditor.Size = new System.Drawing.Size(284, 230);
			this.LuaCodeEditor.Snippets.ActiveSnippetIndicatorStyle = ScintillaNET.IndicatorStyle.Plain;
			this.LuaCodeEditor.Snippets.InactiveSnippetIndicatorStyle = ScintillaNET.IndicatorStyle.Plain;
			this.LuaCodeEditor.Styles.BraceBad.Size = 12F;
			this.LuaCodeEditor.Styles.BraceLight.BackColor = System.Drawing.Color.White;
			this.LuaCodeEditor.Styles.BraceLight.Size = 12F;
			this.LuaCodeEditor.Styles.ControlChar.Size = 12F;
			this.LuaCodeEditor.Styles.Default.BackColor = System.Drawing.SystemColors.Control;
			this.LuaCodeEditor.Styles.Default.Size = 12F;
			this.LuaCodeEditor.Styles.IndentGuide.Size = 12F;
			this.LuaCodeEditor.Styles.LastPredefined.Size = 12F;
			this.LuaCodeEditor.Styles.LineNumber.BackColor = System.Drawing.Color.Silver;
			this.LuaCodeEditor.Styles.LineNumber.CharacterSet = ScintillaNET.CharacterSet.ShiftJis;
			this.LuaCodeEditor.Styles.LineNumber.Size = 10F;
			this.LuaCodeEditor.Styles.Max.Size = 12F;
			this.LuaCodeEditor.TabIndex = 1;
			this.LuaCodeEditor.ModifiedChanged += new System.EventHandler(this.ScriptModified);
			this.LuaCodeEditor.KeyDown += new System.Windows.Forms.KeyEventHandler(this.KeyDownEvent);
			// 
			// codeControl
			// 
			this.codeControl.BackColor = System.Drawing.Color.Silver;
			this.codeControl.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.codeControl.Controls.Add(this.ScriptSaveStatus);
			this.codeControl.Dock = System.Windows.Forms.DockStyle.Top;
			this.codeControl.Location = new System.Drawing.Point(0, 0);
			this.codeControl.Name = "codeControl";
			this.codeControl.Size = new System.Drawing.Size(284, 32);
			this.codeControl.TabIndex = 2;
			// 
			// ScriptSaveStatus
			// 
			this.ScriptSaveStatus.AutoSize = true;
			this.ScriptSaveStatus.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.ScriptSaveStatus.ForeColor = System.Drawing.Color.Red;
			this.ScriptSaveStatus.Location = new System.Drawing.Point(262, 6);
			this.ScriptSaveStatus.Name = "ScriptSaveStatus";
			this.ScriptSaveStatus.Size = new System.Drawing.Size(17, 24);
			this.ScriptSaveStatus.TabIndex = 0;
			this.ScriptSaveStatus.Text = "*";
			this.ScriptSaveStatus.Visible = false;
			// 
			// LuaEdit
			// 
			this.AccessibleRole = System.Windows.Forms.AccessibleRole.Window;
			this.ClientSize = new System.Drawing.Size(284, 262);
			this.ControlBox = false;
			this.Controls.Add(this.LuaCodeEditor);
			this.Controls.Add(this.codeControl);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
			this.Name = "LuaEdit";
			this.Text = "LuaEdit";
			((System.ComponentModel.ISupportInitialize)(this.LuaCodeEditor)).EndInit();
			this.codeControl.ResumeLayout(false);
			this.codeControl.PerformLayout();
			this.ResumeLayout(false);

		}

		public void KeyDownEvent(object sender, KeyEventArgs e)
		{
			if (e.Control && e.KeyCode.Equals(Keys.S))
			{
				if (this.LuaCodeEditor.Modified)
				{
					if (this.lastPath == "")
						if (this.saveDialog.ShowDialog() != System.Windows.Forms.DialogResult.OK)
							return;
						else
							this.lastPath = this.saveDialog.FileName;

					StreamWriter a = new StreamWriter(this.lastPath);
					a.Write(this.LuaCodeEditor.Text);
					a.Close();
					this.LuaCodeEditor.Modified = false;
					this.ScriptSaveStatus.Hide();
				}
			}
			else if (e.Control && e.Shift && e.KeyCode.Equals(Keys.S))
			{
				if (this.saveDialog.ShowDialog() != System.Windows.Forms.DialogResult.OK)
					return;

				this.lastPath = this.saveDialog.FileName;
				StreamWriter a = new StreamWriter(this.lastPath);
				a.Write(this.LuaCodeEditor.Text);
				a.Close();
				this.LuaCodeEditor.Modified = false;
				this.ScriptSaveStatus.Hide();
			}
			else if (e.Control && e.KeyCode.Equals(Keys.O))
			{
				if (this.openDialog.ShowDialog() != System.Windows.Forms.DialogResult.OK)
					return;

				StreamReader sr = new StreamReader(this.openDialog.FileName);
				this.LuaCodeEditor.InsertText(sr.ReadToEnd());
				sr.Close();
			}
		}

		private void ScriptModified(object sender, EventArgs e)
		{
			if (this.LuaCodeEditor.Modified)	this.ScriptSaveStatus.Show();
			else								this.ScriptSaveStatus.Hide();
		}
	}
}

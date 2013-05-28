﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Interop;

namespace FlyEditUI
{
	class LevelGenerator
	{
		Dictionary<string, int> meshResource = new Dictionary<string, int>();
		Dictionary<string, int> lights = new Dictionary<string, int>();
		Dictionary<string, int> pickups = new Dictionary<string, int>();
		Dictionary<string, int> cameras = new Dictionary<string, int>();

		
		public Dictionary<string, int> MeshResource
		{
			get { return meshResource; }
			set { meshResource = value; }
		}
		public Dictionary<string, int> Lights
		{
			get { return lights; }
			set { lights = value; }
		}
		public Dictionary<string, int> Pickups
		{
			get { return pickups; }
			set { pickups = value; }
		}
		public Dictionary<string, int> Cameras
		{
			get { return cameras; }
			set { cameras = value; }
		}


		public LevelGenerator()
		{

		}
	}
}

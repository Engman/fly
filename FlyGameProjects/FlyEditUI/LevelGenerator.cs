using System;
using System.CodeDom.Compiler;
using System.Reflection;

namespace FlyEditUI
{
	class LevelGenerator
	{
		public static void GenerateLevel()
		{
			System.CodeDom.Compiler.CompilerParameters parameters = new CompilerParameters();
			parameters.GenerateExecutable = false;
			parameters.OutputAssembly = "AutoGen.dll";

			CompilerResults r = CodeDomProvider.CreateProvider("C#").CompileAssemblyFromSource(parameters, "public class B {public static int k=7;}");

			//verify generation
			Console.WriteLine(Assembly.LoadFrom("AutoGen.dll").GetType("B").GetField("k").GetValue(null));
		}
	}
}

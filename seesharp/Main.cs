#region Includes

using System;
using System.Dynamic;
using System.Runtime.InteropServices;

#endregion

namespace SeeSharp
{
    public class Main
    {
		static ScriptHandler scripts;
		static bool isInit;

		public static int CanInvoke(string method)
		{
			method = method.Split(' ')[0];

			if (method == "init" || method == "rehash")
				return 1;

			if (scripts == null)
				return -1;

			return scripts.CanInvoke(method) ? 1 : 0;
		}

        public static int Invoke(string method)
        {
			var msplit = method.Split(' ');
			var args = "";

			if (msplit.Length > 1)
				args = method.Substring(msplit[0].Length + 1);

			method = msplit[0];

			if (method == "init")
			{
				if (isInit)
					Game.PrintToChat("", "Script system already initialised.  Please use /rehash to reload scripts.", ChatColours.SunsetOrange2);
				else
				{
					Game.PrintToChat("", "--------------------------------", ChatColours.SweetCorn);
					Game.PrintToChat("", ".:: CABAL Online Ruby Scripting System ::.", ChatColours.Coral);
					Game.PrintToChat("", "Author: Yamachi", ChatColours.Coral);
					Game.PrintToChat("", "--------------------------------", ChatColours.SweetCorn);
					Game.PrintToChat("", "Initialising script system...", ChatColours.Coral);
					LoadScripts();
					Game.PrintToChat("", "Script system initialised!", ChatColours.Coral);
					Game.PrintToChat("", "You may now start entering commands (use /help for more information).", ChatColours.Coral);
					Game.PrintToChat("", "--------------------------------", ChatColours.SweetCorn);
					isInit = true;
				}
			}
			else if (method == "rehash")
			{
				Game.PrintToChat("", "Reloading scripts...", ChatColours.Coral);
				Rehash();
				Game.PrintToChat("", "Scripts reloaded!", ChatColours.Coral);
			}
			else
			{
				if (args == "")
					scripts.Invoke(method);
				else
					scripts.Invoke(method, args);
			}

			return 0;
        }

		static bool Rehash()
		{
			try
			{
				scripts.Invoke("_uninit_");
				LoadScripts();
			}
			catch (Exception e)
			{
				Game.PrintToChat("", "ERROR: " + e.Message, ChatColours.SunsetOrange2);
				return false;
			}

			return true;
		}

		static void LoadScripts()
		{
			var sound = new System.Media.SoundPlayer();
			sound.Stream = Properties.Resources.coin_flip;

			scripts = new ScriptHandler();
			scripts.Concatenate("Main", new string[] { "mscorlib" });
			scripts.Run("Main");
			scripts.CreateInstance("Main");
			scripts.SetGlobal("scripts", scripts);
			scripts.SetGlobal("sound", sound);
			dynamic result = scripts.Invoke("_init_");
		}
    }
}

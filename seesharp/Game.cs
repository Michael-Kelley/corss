#region Includes

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using IronRuby.Builtins;

#endregion

namespace SeeSharp
{
	public class Game
	{
		[DllImport("bootstrap.dll")]
		public static extern void eSysWindowSay([MarshalAs(UnmanagedType.LPStr)]string message, int colour);

		[DllImport("bootstrap.dll", EntryPoint = "SendPacket")]
		public static extern void eSendPacket([MarshalAs(UnmanagedType.LPArray)]byte[] packet, int size);

		[UnmanagedFunctionPointer(CallingConvention.StdCall)]
		delegate void dChatWindowSay([MarshalAs(UnmanagedType.LPStr)]string name, [MarshalAs(UnmanagedType.LPStr)]string message, int colour);
		
		static dChatWindowSay ChatWindowSay = (dChatWindowSay)Marshal.GetDelegateForFunctionPointer((IntPtr)0x004C1F20, typeof(dChatWindowSay));

		public static void PrintToChat(string name, string message, ChatColours colour)
			{ ChatWindowSay(name, message, (int)colour); }

		public static void PrintToSystem(string message, ChatColours colour)
			{ eSysWindowSay(message, (int)colour); }

		public static void SendPacket(RubyArray packet, int size)
		{
			var list = new List<byte>();

			foreach (var p in packet)
				if (p is int)
					list.Add((byte)(int)p);
				else
					list.Add((byte)p);

			eSendPacket(list.ToArray(), size);
		}
	}
}
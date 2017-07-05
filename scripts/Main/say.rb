def say(msg)
	mode = msg.split(' ')[0]
	packet = [ 0xE2, 0xB7, -1, 0, 0, 0, 0, 0, 0x89, 0x01, -1, 0xFE, 0xFE, -1, 1, 1 ]
	
	if mode == "cap" or mode == "pro" or mode == "na" or mode == "gm" then
		msg = msg[mode.length..msg.length - 1]
		msg = msg.strip
		packet[2] = 0x10 + msg.length
		packet[10] = 0x05 + msg.length
	end
	
	if mode == "cap" then
		packet[13] = 0x10
	elsif mode == "pro" then
		packet[13] = 0x20
	elsif mode == "na" then
		packet[13] = 0xB0
	elsif mode == "gm" then
		packet[13] = 0xA1
	end
	
	packet += System::Text::Encoding.ASCII.get_bytes msg
	Game.send_packet packet, packet.length if packet[2] != -1
end
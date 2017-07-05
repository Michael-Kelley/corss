def help()
	Game.print_to_chat("", "Available commands:", ChatColours.coral)
	Game.print_to_chat("", "  /rehash", ChatColours.coral)
	(self.class.instance_methods - self.class.superclass.instance_methods).each { |method| Game.print_to_chat("", "  /" + method.to_s, ChatColours.coral) if method[0] != '_'	}
end
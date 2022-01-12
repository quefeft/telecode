all: telecode

telecode:
	gcc main.c -o telecode

install:
	gcc main.c -o telecode
	sudo mv telecode /usr/bin
	sudo mkdir /usr/share/telecode
	sudo cp help.txt /usr/share/telecode/
	chown root:root /usr/bin/telecode
	chmod +x /usr/bin/telecode

clean:
	-tm telecode



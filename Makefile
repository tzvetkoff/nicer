all: nicer chown chgrp setuid setgid

nicer: nicer.c
	$(RM) nicer
	$(CC) $(CFLAGS) -o nicer nicer.c

chown: nicer
	sudo chown `id -u root` nicer

chgrp: nicer
	sudo chgrp `id -g root` nicer

setuid: nicer
	sudo chmod u+s nicer

setgid:
	sudo chmod g+s nicer

clean:
	$(RM) -f nicer

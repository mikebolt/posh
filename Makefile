SRCDIR=src
BINDIR=bin

all: posh-core utils

posh-core: posh dup seq setenv unsetenv clearenv pipe read write sub

posh: $(SRCDIR)/posh_main.c $(SRCDIR)/posh.c
	$(CC) $(CFLAGS) $< $(SRCDIR)/posh.c -o $(BINDIR)/$@

dup: $(SRCDIR)/posh_dup.c $(SRCDIR)/posh.c
	$(CC) $(CFLAGS) $< $(SRCDIR)/posh.c -o $(BINDIR)/$@

seq: $(SRCDIR)/posh_seq.c $(SRCDIR)/posh.c
	$(CC) $(CFLAGS) $< $(SRCDIR)/posh.c -o $(BINDIR)/$@

setenv: $(SRCDIR)/posh_setenv.c $(SRCDIR)/posh.c
	$(CC) $(CFLAGS) $< $(SRCDIR)/posh.c -o $(BINDIR)/$@

unsetenv: $(SRCDIR)/posh_unsetenv.c $(SRCDIR)/posh.c
	$(CC) $(CFLAGS) $< $(SRCDIR)/posh.c -o $(BINDIR)/$@

clearenv: $(SRCDIR)/posh_clearenv.c $(SRCDIR)/posh.c
	$(CC) $(CFLAGS) $< $(SRCDIR)/posh.c -o $(BINDIR)/$@

pipe: $(SRCDIR)/posh_pipe.c $(SRCDIR)/posh.c
	$(CC) $(CFLAGS) $< $(SRCDIR)/posh.c -o $(BINDIR)/$@

read: $(SRCDIR)/posh_read.c $(SRCDIR)/posh.c
	$(CC) $(CFLAGS) $< $(SRCDIR)/posh.c -o $(BINDIR)/$@

write: $(SRCDIR)/posh_write.c $(SRCDIR)/posh.c
	$(CC) $(CFLAGS) $< $(SRCDIR)/posh.c -o $(BINDIR)/$@

sub: $(SRCDIR)/posh_sub.c $(SRCDIR)/posh.c
	$(CC) $(CFLAGS) $< $(SRCDIR)/posh.c -o $(BINDIR)/$@

utils: concat

concat: $(SRCDIR)/concat.c
	$(CC) $(CFLAGS) $< -o $(BINDIR)/$@

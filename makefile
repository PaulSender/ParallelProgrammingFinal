FCC = gcc
LDC = gcc
LD_FLAGS = -g 
FLAGS = -g
PROG = quick.cx
RM = /bin/rm
OBJS = quick.o

#all rule
all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $(LD_FLAGS) $(OBJS) -o $(PROG)

%.o: %.c
	$(FCC) $(FLAGS) -c $<

clean:
	$(RM) -rf *.o $(PROG) *.mod

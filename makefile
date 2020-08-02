FCC = gcc
LDC = gcc
LD_FLAGS = 
FLAGS = 
PROG = bubble.cx
RM = /bin/rm
OBJS = bubble.o

#all rule
all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $(LD_FLAGS) $(OBJS) -o $(PROG)

%.o: %.c
	$(FCC) $(FLAGS) -c $<

clean:
	$(RM) -rf *.o $(PROG) *.mod

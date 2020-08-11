FCC = mpicc
LDC = mpicc
LD_FLAGS =  
FLAGS =
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
	$(RM) -rf *.o $(PROG) *.o*

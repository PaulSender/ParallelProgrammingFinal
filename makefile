FCC = mpicc
LDC = mpicc
LD_FLAGS = 
FLAGS = 
PROG = qp.cx
RM = /bin/rm
OBJS = qp.o

#all rule
all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $(LD_FLAGS) $(OBJS) -o $(PROG)

%.o: %.c
	$(FCC) $(FLAGS) -c $<

clean:
	$(RM) -rf *.o $(PROG) *.mod out.*

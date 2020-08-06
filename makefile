FCC = mpicc
LDC = mpicc
LD_FLAGS = 
FLAGS = 
<<<<<<< HEAD
PROG = mp.cx
RM = /bin/rm
OBJS = mp.o
=======
PROG = qp.cx
RM = /bin/rm
OBJS = qp.o
>>>>>>> 335d4006dff1b4f09064ff52084c476a72661892

#all rule
all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $(LD_FLAGS) $(OBJS) -o $(PROG)

%.o: %.c
	$(FCC) $(FLAGS) -c $<

clean:
	$(RM) -rf *.o $(PROG) *.mod out.*

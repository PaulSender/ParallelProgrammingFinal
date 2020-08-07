FCC = tau_cc.sh
LDC = tau_cc.sh
LD_FLAGS =
FLAGS =
RM = /bin/rm
PROG = rp.cx
RM = /bin/rm
OBJS = rp.o

#all rule
all: $(PROG)

$(PROG): $(OBJS)
	$(LDC) $(LD_FLAGS) $(OBJS) -o $(PROG)

%.o: %.c
	$(FCC) $(FLAGS) -c $<

clean:
	$(RM) -rf *.o $(PROG) *.mod out.*

CC		= 	g++
CFLAGS		= 	-g -Wall

TARGET		=	csh4lmp

SRC_DIRS	?=	./src

SRCS 	:=	$(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)

OBJS	= 	$(SRCS:.c=.o)


INC		= 	-I/src
LFLAGS	= 
LIBS	= 	-lm

MKFILE	= 	Makefile 


$(TARGET) : $(OBJS)
		$(CC) $(CFLAGS) $(INC) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
		$(CC) $(CFLAGS) $(INC) -c $< -o $@


.PHONY: clean

clean:
		rm -f $(TARGET) $(patsubst %,$(SRCDIR)/%.o,$(basename $(SRC)))

depend: $(SRCS)
		makedepend $(INC) $^

#

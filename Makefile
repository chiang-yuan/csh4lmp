CC		= 	gcc
CFLAGS	= 	-g -Wall -O3

TARGET	=	csh4lmp.exe

SRCS	= 	csh4lmp.cpp \
			error.cpp \
			system.cpp read_data.cpp write_data.cpp modify.cpp modify_h2o.cpp change_box.cpp \
			atom.cpp bond.cpp angle.cpp \
			affine_transform.cpp

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
		rm -f $(OBJS) $(TARGET)

depend: $(SRCS)
		makedepend $(INC) $^

#
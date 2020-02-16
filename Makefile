
FILE =  TheMind.c vote.c main.c 
SOVERSION=1


 
# -- Paths ----------
SRC_PATH = src
OBJ_PATH = obj
EXE_PATH = exe
INC_PATH = include
LIB_PATH = mqtt_lib/lib

CONFIG = CLI

# -- Macros ----------
CC = gcc
AR = ar -rc

# -- Flags ----------
C_DEBUG_FLAGS = -O0 -g
C_OPTIMISATION_FLAGS = -O3 -fstrict-aliasing

C_INC_FLAGS = -I$(INC_PATH)
C_LIB_FLAGS = -L$(LIB_PATH)

CFLAGS =  $(C_DEBUG_FLAGS)         $(C_LIB_FLAGS) $(C_INC_FLAGS)
#CFLAGS = $(C_OPTIMISATION_FLAGS)  $(C_LIB_FLAGS) $(C_INC_FLAGS)
LDFLAGS = $(C_OPTIMISATION_FLAGS)  $(C_LIB_FLAGS) $(C_INC_FLAGS) -lm -lmosquitto

# -- Final product ----------
PRODUCT = TheMind

# -- src and obj List ----------
SRC = $(addprefix ${SRC_PATH}/, $(FILE))
OBJ = $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(FILE))))

# -- Base rules ----------
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

#-----Main rule ----------
$(EXE_PATH)/$(PRODUCT): $(OBJ)
	 $(CC) -o $@ $^ $(LDFLAGS) $(OPTFLAGS) $(INC) $(LIB) 

# -- Other stuff ----------
depend:
	makedepend $(CFLAGS) -Y $(SRC)

clean:
	rm -f $(OBJ)
	rm -f ${EXE_PATH}/${PRODUCT}
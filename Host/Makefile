
FILE 		= mqtt_callback.c main.c 
SHARED_FILE = TheMind.c vote.c
SOVERSION=1

 
# -- Paths ----------
SRC_PATH = src
OBJ_PATH = obj
EXE_PATH = exe
INC_PATH = inc
LIB_PATH = ../mqtt_lib/lib

SHARED_INC_PATH = ../shared_inc
SHARED_SRC_PATH = ../shared_src
SHARED_OBJ_PATH = ../shared_obj


# -- Macros ----------
CC = gcc
AR = ar -rc

# -- Flags ----------
C_DEBUG_FLAGS = -O0 -g
C_OPTIMISATION_FLAGS = -O3 -fstrict-aliasing

C_INC_FLAGS = -I$(INC_PATH) -I$(SHARED_INC_PATH)
C_LIB_FLAGS = -L$(LIB_PATH)

CFLAGS =  $(C_DEBUG_FLAGS)         $(C_LIB_FLAGS) $(C_INC_FLAGS)
#CFLAGS = $(C_OPTIMISATION_FLAGS)  $(C_LIB_FLAGS) $(C_INC_FLAGS)
LDFLAGS = $(C_OPTIMISATION_FLAGS)  $(C_LIB_FLAGS) $(C_INC_FLAGS) -lm -lmosquitto -lpthread

# -- Final product ----------
PRODUCT = Host

# -- src and obj List ----------
SRC 	   = $(addprefix ${SRC_PATH}/, $(FILE))
OBJ 	   = $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(FILE))))
SHARED_SRC = $(addprefix ${SHARED_SRC_PATH}/, $(SHARED_FILE))
SHARED_OBJ = $(addprefix ${SHARED_OBJ_PATH}/, $(addsuffix .o, $(basename $(SHARED_FILE))))

# -- Base rules ----------
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SHARED_OBJ_PATH)/%.o : $(SHARED_SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


#-----Main rule ----------
$(EXE_PATH)/$(PRODUCT): $(OBJ) $(SHARED_OBJ)
	 $(CC) -o $@ $^ $(LDFLAGS) $(OPTFLAGS) $(INC) $(LIB) 

# -- Other stuff ----------
depend:
	makedepend $(CFLAGS) -Y $(SRC) $(SHARED_SRC)

clean:
	rm -f $(OBJ)
	rm -f $(SHARED_OBJ)
	rm -f ${EXE_PATH}/${PRODUCT}
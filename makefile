#UNCOMMENT THESE TO COMPILE TERMINAL GAME
#CLIENTDIR := examples/Aquar/client
#SERVERDIR := examples/Aquar/server
#COMMONDIR := examples/simpleChat/common

#UNCOMMENT THESE TO COMPILE WHIT GRAPHIC
CLIENTDIR := examples/AllegroAquar/client
SERVERDIR := examples/AllegroAquar/server
COMMONDIR := examples/AllegroAquar/common

CC := gcc -std=c99
RM := rm -f
MK := mkdir -p
#Uncomment above for intense debugging
#CFLAGS := -Wall -Werror -Wconversion -Wextra
LDLIB := -lm -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf

OUTPUTDIR := bin
LIBDIR := lib

CLIENTNAME := CLIENTDIR
SERVERNAME := SERVERDIR

EXT := c
INC := -I lib -I $(COMMONDIR)

CLIENTSOURCES := $(shell find $(CLIENTDIR) -type f -name *.$(EXT))
SERVERSOURCES := $(shell find $(SERVERDIR) -type f -name *.$(EXT))
LIBSOURCES := $(shell find $(LIBDIR) -type f -name *.$(EXT))
COMMONSOURCES := $(shell find $(COMMONDIR) -type f -name *.$(EXT))


CLIENTOBJS := $(subst .$(EXT),.o,$(CLIENTSOURCES))
SERVEROBJS := $(subst .$(EXT),.o,$(SERVERSOURCES))
LIBOBJS := $(subst .$(EXT),.o,$(LIBSOURCES))
COMMONOBJS := $(subst .$(EXT),.o,$(COMMONSOURCES))

all: mkdirs buildServer buildClient clean

server: mkdirs buildServer clean runServer

client: mkdirs buildClient clean runClient

buildClient: $(LIBOBJS) $(CLIENTOBJS) $(COMMONOBJS)
	@echo "\n  Linking $(CLIENTNAME)..."
	$(CC) -o $(OUTPUTDIR)/$(CLIENTNAME) $(LIBOBJS) $(COMMONOBJS) $(CLIENTOBJS) $(LDLIB) $(CFLAGS)
	@echo "\n"

buildServer: $(LIBOBJS) $(SERVEROBJS) $(COMMONOBJS)
	@echo "\n  Linking $(SERVERNAME)..."
	$(CC) -o $(OUTPUTDIR)/$(SERVERNAME) $(LIBOBJS) $(COMMONOBJS) $(SERVEROBJS) $(LDLIB) $(CFLAGS)
	@echo "\n"

%.o : %.$(EXT)	
	$(CC) -c $< -o $@ $(INC) $(CFLAGS)

mkdirs:
	$(MK) $(OUTPUTDIR)

clean:
	@echo "  Cleaning..."
	$(RM) $(LIBOBJS) $(CLIENTOBJS) $(SERVEROBJS) $(COMMONOBJS)

runClient:
	@echo "\n  Starting to run $(CLIENTNAME)...\n"; ./$(OUTPUTDIR)/$(CLIENTNAME)

runServer:
	@echo "\n  Starting to run $(SERVERNAME)...\n"; ./$(OUTPUTDIR)/$(SERVERNAME) 

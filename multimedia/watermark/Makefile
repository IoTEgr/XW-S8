GCCPATH = C:\MinGW-4.7.1\bin

LIB     = .\libwatermark.a

CC         = $(GCCPATH)/or1k-elf-gcc.exe  -DOR1K_NEWCPU -mno-delay
AS         = $(GCCPATH)/or1k-elf-as.exe
AR         = $(GCCPATH)/or1k-elf-ar.exe



C_FLAGS    = -I./ -I../header -Os -Wall -mhard-div -c
CAS_FLAGS  = -I./ -I../header -c 
AR_FLAGS   = ru


# ȡ�õ�ǰĿ¼���г����ļ�
files := $(wildcard ./*.c ./*.S )
files := $(basename $(files))

objs  := $(addsuffix .o, $(files))

OBJECTS := $(objs)

%.a : $(OBJECTS)
	$(AR) $(AR_FLAGS) $@ $^

%.o : %.c
	$(CC) $(C_FLAGS) $< -o $@

%.o : %.s
	$(AS) $(AS_FLAGS) $< -o $@

%.o : %.S
	$(CC) $(CAS_FLAGS) $< -o $@


all : $(LIB)
	copy .\$(LIB) ..\..\bwlib\$(LIB)

clean :
	del $(LIB)


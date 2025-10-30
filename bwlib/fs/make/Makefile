GCCPATH = C:\MinGW-4.7.1\bin

LIB     = .\libfs.a

CC         = $(GCCPATH)/or1k-elf-gcc.exe  -DOR1K_NEWCPU -mno-delay
AS         = $(GCCPATH)/or1k-elf-as.exe
AR         = $(GCCPATH)/or1k-elf-ar.exe



C_FLAGS    = -I./ -I../header -Os -Wall -mhard-div -c --std=c99
CAS_FLAGS  = -I./ -I../header -c 
AR_FLAGS   = ru


# 取得当前目录所有程序文件
files := $(wildcard ../src/*.c ../src/*.S )
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
	copy .\$(LIB) ..\..\$(LIB)

clean :
	del *.bak
	del *.a
	del /Q objs
	del $(LIB)


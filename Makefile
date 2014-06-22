PROJECT		= rgb2yuv-bench

#
# Flags
#

BINDIR 		= bin
OBJDIR 		= obj
VPATH		= src

C_FILES		= main rgb2yuv_wiki rgb2yuv_novell_ch rgb2yuv_tables rgb2yuv_tables64 rgb2yuv_sse rgb2yuv_sse2 rgb2yuv_gstreamer
OBJS		= $(addprefix $(OBJDIR)/, $(addsuffix .o, $(C_FILES)))

CFLAGS		= -fmessage-length=0 -fstack-protector -O2 -Wall -D_FORTIFY_SOURCE=2 -funwind-tables -fasynchronous-unwind-tables -g \
			  -Wall -Wextra -Wno-unused-parameter -Wformat-y2k -Winit-self -msse4.1 \
			  -Wstrict-prototypes -Winline -Wnested-externs -Wbad-function-cast -Wshadow -std=gnu11
LDFLAGS		= -lrt -lgomp

#
# Targets
#

.PHONY: clean all prepare

prepare:
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)

clean:
	rm -f $(BINDIR)/$(PROJECT)
	rm -r $(BINDIR)
	rm -f $(OBJS)
	rm -r $(OBJDIR)

all: prepare $(OBJS)
	$(CC) $(OBJS) -o $(BINDIR)/$(PROJECT) $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

# TODO reduce the number of magic names in this file
# autodetect the user's ImageMagick/MagickWand libraries
magick_libs = $(shell pkg-config --cflags --libs MagickWand)
gcc_opts = -Wall
# list of all files to be cleaned
objs = image.o cli.o
clean_files = $(objs) htpewpew debug

htpewpew: $(objs) main.c
	@echo "gcc $(gcc_opts) main.c $(objs) -o htpewpew"
	@gcc $(gcc_opts) main.c $(objs) -o htpewpew $(magick_libs)

debug: $(objs) main.c
	@echo "gcc $(gcc_opts) -g main.c $(objs) -o debug"
	@gcc $(gcc_opts) -g main.c $(objs) -o debug $(magick_libs)

# don't print the magick_libs garbage, please use this as a template
image.o: image.c
	@echo "gcc $(gcc_opts) -c image.c -o image.o"
	@gcc $(gcc_opts) -c image.c -o image.o $(magick_libs)

cli.o: cli.c
	gcc $(gcc_opts) -c cli.c -o cli.o

clean:
	rm -f $(clean_files)

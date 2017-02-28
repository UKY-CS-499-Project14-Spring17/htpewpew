# TODO reduce the number of magic names in this file
# autodetect the user's ImageMagick/MagickWand libraries
magick_libs = $(shell pkg-config --cflags --libs MagickWand)
gcc_opts = "-Wall"
# list of all files to be cleaned
clean_files = image.o

# don't print the magick_libs garbage, please use this as a template
image.o: image.c
	@echo "gcc $(gcc_opts) image.c -o image.o"
	@gcc $(gcc_opts) image.c -o image.o $(magick_libs)

clean:
	rm -f $(clean_files)

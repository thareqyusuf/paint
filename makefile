all: paint

myPlane: cannon.c game.c keypress.c main.c stack.c color.c graphics.c line.c rotate.c physics.c grafika.h
	gcc -o myPlane cannon.c game.c keypress.c main.c stack.c color.c graphics.c line.c rotate.c physics.c -lpthread -lm -std=gnu99

myPlane-fast: cannon.c game.c keypress.c main.c stack.c color.c graphics.c line.c rotate.c physics.c grafika.h
	gcc -o myPlane-fast cannon.c game.c keypress.c main.c stack.c color.c graphics.c line.c rotate.c physics.c -O2 -lpthread -lm -std=gnu99

paint: paint.c color.c point.c pointqueue.c grafika.c geometry.c transform.c game.c filling.c physics.c clipping.c framebuffer.c keypress.c
	gcc -o paint paint.c color.c point.c pointqueue.c  grafika.c geometry.c transform.c  game.c filling.c physics.c clipping.c framebuffer.c keypress.c -lpthread -O2 -lm -std=gnu99 -w

clean:
	$(RM) myPlane
	$(RM) myPlane-fast
	$(RM) paint

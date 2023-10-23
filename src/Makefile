CC = g++
VER = -std=c++17
GCOVFLAGS = -lgtest
GCOVFLAGSLIN = --coverage
GTKFLAG = `pkg-config gtkmm-3.0 --cflags --libs`
ERRFLAGS = -Wall -Werror -Wextra
ILLEGAL = -D private=public
#ASAN = -fsanitize=address
FLAGC = -c
FLAGO = -o
TEST_C = tests/*.cc
maze_C = maze_model/*.cc cave_model/*.cc 
maze_H = maze_model/*.h cave_model/*.h
cont_H = control/*.h
cont_GCDA = covers/*.gcda
gui_C = gui/*.cc
gui_H = gui/*.h
lib_worker = -Lmodel -l:maze.a 

all: install

install: uninstall
	mkdir ready_to_go
	$(CC) $(VER) $(ASAN) $(maze_C) $(gui_C) $(FLAGO) ready_to_go/maze $(GTKFLAG)

uninstall: clean
	rm -rf ready_to_go

dist:
	tar -czvf maze.tar.gz ready_to_go

dvi:
	open ref_doc.pdf	

run:
	./ready_to_go/maze

clean:
	rm -rf ready_to_go a.out
	rm -rf covers html
	rm -rf maze.tar.gz
	rm -rf *.gcno
	rm -rf *.gcov

tests: clean 
	$(CC) $(VER) $(ASAN) $(ERRFLAGS) $(GCOVFLAGSLIN) $(maze_C) $(cont_C) $(TEST_C) $(FLAGO) cont_test $(GCOVFLAGS)
	./cont_test
	mkdir covers
	mv *.gcda covers
	mv *.gcno covers

gcov_report: tests
	mkdir html
	gcov $(cont_GCDA)
	gcovr
	gcovr --html-details html/coverage.html
	mv *.gcov covers
	open html/coverage.html

check:
	cp ../materials/linters/.clang-format ../src
	clang-format -i $(cont_H) $(gui_C) $(gui_H) $(TEST_C) $(maze_C) $(maze_H)
	rm .clang-format
	# $(CC) $(VER) $(ERRFLAGS) $(maze_C) $(TEST_C) $(GCOVFLAGS)
	# valgrind --leak-check=full ./ready_to_go/maze
	# CK_FORK=no leaks --atExit -- ./a.out

download_aps:
	sudo apt install libgtkmm-3.0-dev
	sudo atp install libgstreamermm-1.0-dev
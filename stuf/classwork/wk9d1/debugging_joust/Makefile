GCCPLATFORM = $(shell g++ --version 2>&1 | grep Apple | cut -d' ' -f1)

ifeq "$(GCCPLATFORM)" "Apple"
  $(error Please use Linux g++, not MacOS)
endif

new_project_joust: Knight.h Knight.cpp Weapon.h Weapon.cpp Weapon.h Random.cpp project_joust.cpp 
	g++ -std=c++11 Random.cpp project_joust.cpp Knight.cpp Weapon.cpp Weapon.cpp -o new_project_joust

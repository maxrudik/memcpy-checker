INCLUDE1 = /home/max/llvm-project/clang/include
INCLUDE2 = /home/max/llvm-project/llvm/include
INCLUDE3 = /home/max/llvm-project/build/include
INCLUDE4 = /home/max/llvm-project/build/tools/clang/include

CC1_INCLUDE1 = /usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0
CC1_INCLUDE2 = /usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/x86_64-linux-gnu/c++/7.5.0
CC1_INCLUDE3 = /usr/lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/backward
CC1_INCLUDE4 = /usr/local/include
CC1_INCLUDE5 = /home/max/llvm-project/build/lib/clang/13.0.0/include
CC1_INCLUDE6 = /usr/include/x86_64-linux-gnu
CC1_INCLUDE7 = /usr/include
CC1_INCLUDE = -I $(CC1_INCLUDE1) -I $(CC1_INCLUDE2) -I $(CC1_INCLUDE3) -I $(CC1_INCLUDE4) -I $(CC1_INCLUDE5) -I $(CC1_INCLUDE6) -I $(CC1_INCLUDE7)

.PHONY: all clean
	
all: MainCallCheckerMax.so
	
clean:
			rm MainCallCheckerMax.so

build: MemcpyCheck.cpp
	g++ MemcpyCheck.cpp -o MemcpyCheck.so -shared -fPIC -fno-rtti -I $(INCLUDE1) -I $(INCLUDE2) -I $(INCLUDE3) -I $(INCLUDE4)

load:
	bash -c "clang++ -cc1 -x c++ -load ./MemcpyCheck.so -analyze -analyzer-checker=example.MemcpyChecker $(CC1_INCLUDE)  ./main.cpp"

load_report:
	bash -c "clang++ -cc1 -x c++ -load ./MemcpyCheck.so -analyze -analyzer-checker=example.MemcpyChecker $(CC1_INCLUDE) -analyzer-output=html -o ./reports/rep1 ./main.cpp"

main: main.cpp
	g++ -o main ./main.cpp

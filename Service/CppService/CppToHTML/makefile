.PHONY:clean
complier = g++
target = generator
cflags = -std=c++14 -Wall
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.cpp")
objs = $(src:%.cpp=%.o) 

$(target): $(objs) ; $(complier) $(cflags) -o $(target) $(objs) 

%.o: %.cpp $(deps) ; $(complier) $(cflags) -o $@ -c $< 

clean: ; rm -rf $(objs) $(target)
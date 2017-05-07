.PHONY:clean
complier = g++-4.9
target = dependencyAnalyzer
cflags = -std=c++14 -DTEST_FILEMGR -lpthread
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.cpp")
objs = $(src:%.cpp=%.o) 
$(target): $(objs) ; $(complier) $(cflags) -o $(target) $(objs) 

%.o: %.cpp $(deps) ; $(complier) $(cflags) -o $@ -c $< 

clean: ; rm -rf $(objs) $(target)

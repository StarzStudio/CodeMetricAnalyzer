.PHONY:clean
complier = g++
target = metricsAnalyzer
cflags = -std=c++14 -DTEST_METRICSEXECUTIVE
deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.cpp")
objs = $(src:%.cpp=%.o) 
$(target): $(objs) ; $(complier) $(cflags) -o $(target) $(objs) 

%.o: %.cpp $(deps) ; $(complier) $(cflags) -o $@ -c $< 

clean: ; rm -rf $(objs) $(target)

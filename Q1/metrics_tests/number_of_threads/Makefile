CC      := gcc
CCFLAGS := -pthread

TARGET1 := rw_lock_test_metrics.c 
TARGET2 := s_lock_test_metrics.c
TARGET3 := hoh_lock_test_metrics.c

.PHONY: workload


help:
	@echo make build : To build all the files 
	@echo make exec : To execute the build files 
	@echo make plot : To generate plot 
	@echo make workload : To generate workload
	@echo make clean : To clean the build files

build: 
	@echo "Building ..."
	@$(CC) -o rw_lock $(TARGET1) $(CCFLAGS)
	@$(CC) -o s_lock $(TARGET2) $(CCFLAGS)
	@$(CC) -o hoh_lock $(TARGET3) $(CCFLAGS)
	@echo "Build Complete" 

exec:
	@echo "Executing the build files ... "

	@ ./rw_lock
	@ ./s_lock
	@ ./hoh_lock

plot:
	@echo "Creating Plots ..."
	@python3 plot.py 
	@echo "Plots sucessfully created" 
	
workload:
	@python3 workload_gen.py

clean: 
	rm rw_lock s_lock hoh_lock

all: 
	@echo cleaning ....
	@make clean
	@make workload
	@make build
	@make exec 
	@make plot


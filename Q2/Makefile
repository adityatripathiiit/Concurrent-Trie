CC      := gcc
CCFLAGS := -lm

TARGETS := main.c policy.c workload.c fort.c gen_csv.c 

help:
	@echo make build : To build all the files 
	@echo make exec : To execute the files after build
	@echo make plot : To generate the plots for performace comparision
	@echo make clean : To remove the build files 

build: 
	@echo "Building ..."
	@$(CC) -o policies $(TARGETS) $(CCFLAGS)
	@echo "Build Complete" 

exec:
	@echo "Executing the build files ... "

	@ ./policies

plot:
	@echo "Creating Plots ..."
	@python3 plot.py 
	@echo "Plots sucessfully created" 

clean: 
	rm policies 








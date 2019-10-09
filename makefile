CC=gcc
CFLAGS=-I.
DEPS=const.h service.h
ODIR:=obj
BDIR:=bin

all : $(BDIR)/hmi_output $(BDIR)/hmi_input $(BDIR)/ecu $(BDIR)/camera $(BDIR)/throttle $(BDIR)/brake $(BDIR)/steer $(BDIR)/park $(BDIR)/sur_cameras

$(ODIR)/%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/hmi_input : $(ODIR)/hmi_input.o $(ODIR)/service.o 
	$(CC) -o $@ $^ $(CFLAGS)

$(BDIR)/hmi_output : $(ODIR)/hmi_output.o $(ODIR)/service.o 
	$(CC) -o $@ $^ $(CFLAGS) 

$(BDIR)/ecu :  $(ODIR)/ecu.o $(ODIR)/service.o 
	$(CC) -o $@ $^ $(CFLAGS) 

$(BDIR)/camera : $(ODIR)/camera.o $(ODIR)/service.o
	$(CC) -o $@ $^ $(CFLAGS) 

$(BDIR)/throttle : $(ODIR)/throttle.o $(ODIR)/service.o
	$(CC) -o $@ $^ $(CFLAGS) 

$(BDIR)/brake : $(ODIR)/brake.o $(ODIR)/service.o
	$(CC) -o $@ $^ $(CFLAGS) 

$(BDIR)/steer : $(ODIR)/steer.o $(ODIR)/service.o
	$(CC) -o $@ $^ $(CFLAGS) 

$(BDIR)/park : $(ODIR)/park.o $(ODIR)/service.o
	$(CC) -o $@ $^ $(CFLAGS)

$(BDIR)/sur_cameras : $(ODIR)/sur_cameras.o $(ODIR)/service.o
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean

clean : 
	rm -f $(ODIR)/*.o






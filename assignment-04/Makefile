obj-m += main.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	cp ./kbd-rule.rules /etc/udev/rules.d/

clean:
	rmmod main
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

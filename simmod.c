#include <linux/module.h>
#include <linux/kernel.h>

int frost_init(void){
	printk(KERN_INFO "a trip to the linux's core!\n");
	
	return 0;

}
void frost_exit(void){
	printk(KERN_INFO "It's time to leave.\n");

}

module_init( frost_init );
module_exit( frost_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("FROST");

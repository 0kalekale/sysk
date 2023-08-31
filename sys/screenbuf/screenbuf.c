/*
 * screenbuf.c: create a linux char block device to get screen buffer pixels  
 *
 * USAGE: cat /dev/screenbuf > screenshot.bmp
 * 
 * Compile with `make`. Load with `sudo insmod chardev.ko`. Check `dmesg | tail`
 * 
 * Under AGPL3
 *
 *
 * note: temp copied from https://gist.github.com/brenns10/65d1ee6bb8419f96d2ae693eb7a66cc0
 * now using it as a base of operation 
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h> 

MODULE_LICENSE("AGPL3");

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "screenbuf"

static int major;
static int Device_Open = 0;
static char *pixbuf;

static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
};

int init_module(void) {
  	major = register_chrdev(0, DEVICE_NAME, &fops);

  	if (major < 0) {
    		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
    		return major;
 	 }

  	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
  	printk(KERN_INFO "the driver, create a dev file with\n");
  	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
  	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
  	printk(KERN_INFO "the device file.\n");
  	printk(KERN_INFO "Remove the device file and module when done.\n");
	
	return SUCCESS;
}

void cleanup_module(void) {
  	unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *filp) {
  	static int counter = 0;

  	if (Device_Open)
    		return -EBUSY;

  	Device_Open++;
  	try_module_get(THIS_MODULE);

  	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *filp) {
	Device_Open--;
	module_put(THIS_MODULE);
	return SUCCESS;
}

/*
	get the size of the screen(how?)
	alloc buffer			
	write pixels to buffer(how??)
	write buffer to user 
*/
static ssize_t device_read(struct file *filp, char *buffer, size_t length, off_t *offset) {	
	int bytes_read = 0;
	
	/*
		alloc pixbuf
		and fill pixbuf
	*/
	
	if (*pixbuf == 0)
    		return 0;

  	while (length && pixbuf) {
 		put_user(*(pixbuf++), buffer++);
    		length--;
    		bytes_read++;
  	}
	return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buf, size_t len, loff_t *off) {
  	printk(KERN_ALERT "/dev/screenbuf: operation not allowed.\n");
  	return -EINVAL;
}

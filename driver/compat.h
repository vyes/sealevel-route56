#ifndef __COMPAT_H__
#define __COMPAT_H__
#include <linux/version.h>   //just in case

 // ---------------------------------------------------------------------------
 // The latest versions of the kernel replace SA_SHIRQ with IRQF_SHARED
 // ---------------------------------------------------------------------------
 #ifndef IRQF_SHARED
 #define IRQF_SHARED SA_SHIRQ
 #endif

 // ---------------------------------------------------------------------------
 // Early 2.4 kernels don't appear to have this
 // ---------------------------------------------------------------------------
 #ifndef __devexit_p
   #define __devexit_p(x) x
 #endif
 
 // ---------------------------------------------------------------------------
 // The License macro isn't defined in some earlier 2.4 kernels
 // ---------------------------------------------------------------------------
 #ifndef MODULE_LICENSE 
   #define MODULE_LICENSE(x)
 #endif

 // ---------------------------------------------------------------------------
 // min macros aren't defined in some earlier 2.4 kernels
 // ---------------------------------------------------------------------------
 #ifndef min_t
   #define min(x,y) ({ \
	const typeof(x) _x = (x);	\
	const typeof(y) _y = (y);	\
	(void) (&_x == &_y);		\
	_x < _y ? _x : _y; })
   #define min_t(type,x,y) \
	({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })
 #endif

 // ---------------------------------------------------------------------------
 // These header files are new to 2.6.x 
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 0)
   // We don't want to include the non-existent header files.
 #else  
   #include <linux/dma-mapping.h>
   #include <linux/workqueue.h>
 #endif

 // ---------------------------------------------------------------------------
 // These qualifiers are new to 2.4.30
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 30)
   #define __user 
   #define __kernel 
 #else
   // Already there.
 #endif

 // ---------------------------------------------------------------------------
 // The interrupt routine has a new interface beyond 2.4.23
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 23)
   #define irqreturn_t void
   #define IRQ_NONE 
   #define IRQ_HANDLED 
 #else
   // Already defined after 2.4.23
 #endif
 
 // ---------------------------------------------------------------------------
 // The new tty_io inteface two builtin ioctls that we must emulate
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 0)
   int cpat_tiocm(void *info, unsigned int cmd, unsigned long arg)
   {
	struct tty_struct temptty;
	temptty.driver_data = info;

	switch(cmd)
	{
		case TIOCMGET:
			return tiocmget(&temptty, NULL);
		case TIOCMBIS:
			return tiocmset(&temptty, NULL, arg, 0);
		case TIOCMBIC:
			return tiocmset(&temptty, NULL, 0, arg);
		case TIOCMSET:
			return tiocmset(&temptty, NULL, arg, ~arg);
		default:
			return -1;
	}
   }
 #else
   // Will already be handled, so just ignore it
   int cpat_tiocm(void *info, unsigned int cmd, unsigned long arg)
   {
	   return -1;
   }
 #endif

 // ---------------------------------------------------------------------------
 // The new tty_io inteface internally provides locking and refcounts.
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 0)
   static int serial_refcount;
   static struct tty_struct *serial_table[MAX_TOTAL_DEVICES];
   static struct termios *serial_termios[MAX_TOTAL_DEVICES];
   static struct termios *serial_termios_locked[MAX_TOTAL_DEVICES];

   void cpat_setup_serial(struct tty_driver *driver)
   {
     // First clear out our struct arrays
     memset(serial_table, 0, sizeof(struct tty_struct*)*MAX_TOTAL_DEVICES);
     memset(serial_termios, 0, sizeof(struct termios*)*MAX_TOTAL_DEVICES);
     memset(serial_termios_locked, 0,sizeof(struct termios*)*MAX_TOTAL_DEVICES);

     driver->refcount = &serial_refcount;
     driver->table = serial_table;
     driver->termios = serial_termios;
     driver->termios_locked = serial_termios_locked;
   }
 #else
   // Everything is internal to 2.6.x, except there is an added owner in tty
   void cpat_setup_serial(struct tty_driver *driver)
   {
	driver->owner =  THIS_MODULE;
   }
 #endif

 // ---------------------------------------------------------------------------
 // 2.6.20 brings a new kernel termios
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 20)
   #define ktermios termios
 #else
   //already up to date
 #endif
 
// ---------------------------------------------------------------------------
 // The old write interface required 4 arguments and not just 3.
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 10)
   #define r56_write(a,b,c) r56_write(a, int from_user, b, c)
 #else
   //already up to date
 #endif

 // ---------------------------------------------------------------------------
 // There is a new tty interface in 2.6.x; need to provide it for 2.4.x 
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 0)
   struct tty_operations {
	int  (*open)(struct tty_struct * tty, struct file * filp);
	void (*close)(struct tty_struct * tty, struct file * filp);
	int  (*write)(struct tty_struct * tty, int from_user,
		      const unsigned char *buf, int count);
	void (*put_char)(struct tty_struct *tty, unsigned char ch);
	void (*flush_chars)(struct tty_struct *tty);
	int  (*write_room)(struct tty_struct *tty);
	int  (*chars_in_buffer)(struct tty_struct *tty);
	int  (*ioctl)(struct tty_struct *tty, struct file * file,
		    unsigned int cmd, unsigned long arg);
	void (*set_termios)(struct tty_struct *tty, struct ktermios * old);
	void (*throttle)(struct tty_struct * tty);
	void (*unthrottle)(struct tty_struct * tty);
	void (*stop)(struct tty_struct *tty);
	void (*start)(struct tty_struct *tty);
	void (*hangup)(struct tty_struct *tty);
	void (*break_ctl)(struct tty_struct *tty, int state);
	void (*flush_buffer)(struct tty_struct *tty);
	void (*set_ldisc)(struct tty_struct *tty);
	void (*wait_until_sent)(struct tty_struct *tty, int timeout);
	void (*send_xchar)(struct tty_struct *tty, char ch);
	int (*read_proc)(char *page, char **start, off_t off,
			  int count, int *eof, void *data);
	int (*write_proc)(struct file *file, const char __user *buffer,
			  unsigned long count, void *data);
	int (*tiocmget)(struct tty_struct *tty, struct file *file);
	int (*tiocmset)(struct tty_struct *tty, struct file *file,
			unsigned int set, unsigned int clear);
   };

   // Allocation of tty_driver
   struct tty_driver *alloc_tty_driver(int lines)
   {
	struct tty_driver *driver;

	driver = kmalloc(sizeof(struct tty_driver), GFP_KERNEL);
	if (driver) {
		memset(driver, 0, sizeof(struct tty_driver));
		driver->magic = TTY_DRIVER_MAGIC;
		driver->num = lines;
		/* later we'll move allocation of tables here */
	}
	return driver;
   }

   // Deallocation of a tty_driver struct
   void put_tty_driver(struct tty_driver *driver)
   {
	kfree( (void*)driver );
   }

   // Set the values previously assigned
   void tty_set_operations(struct tty_driver *driver,
			   const struct tty_operations *op)
   {
	driver->open = op->open;
	driver->close = op->close;
	driver->write = op->write;
	driver->put_char = op->put_char;
	driver->flush_chars = op->flush_chars;
	driver->write_room = op->write_room;
	driver->chars_in_buffer = op->chars_in_buffer;
	driver->ioctl = op->ioctl;
	driver->set_termios = op->set_termios;
	driver->throttle = op->throttle;
	driver->unthrottle = op->unthrottle;
	driver->stop = op->stop;
	driver->start = op->start;
	driver->hangup = op->hangup;
	driver->break_ctl = op->break_ctl;
	driver->flush_buffer = op->flush_buffer;
	driver->set_ldisc = op->set_ldisc;
	driver->wait_until_sent = op->wait_until_sent;
	driver->send_xchar = op->send_xchar;
	driver->read_proc = op->read_proc;
	driver->write_proc = op->write_proc;
//	driver->tiocmget = op->tiocmget;  //not in tty_driver prior to 2.6.x
//	driver->tiocmset = op->tiocmset;  //ditto
   }
 #else
   // Already provided in 2.6.x
 #endif

 // ---------------------------------------------------------------------------
 // Some work/task related functions not yet provided in 2.4.x
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 0)
   #define work_struct tq_struct
   #define schedule_work(a) queue_task(a, &tq_immediate); mark_bh(IMMEDIATE_BH)
 #else 
   // Already provided in 2.6.x
 #endif

 // ---------------------------------------------------------------------------
 // Note the difference in spelling of cannonicalize (1 n vs 2)
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 0)
   #define irq_canonicalize(a) irq_cannonicalize(a)
 #else 
   // Already uses the new spelling
 #endif

 // ---------------------------------------------------------------------------
 // 2.6.x brings a dma interface that has to be emulated in 2.4.x
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 0)
   #define cpat_dma_alloc(a,b,c) kmalloc(a, c | GFP_DMA)
   #define cpat_dma_free(a,b,c) kfree(b)
   #define cpat_phys_addr(a) virt_to_bus(a->buffer_list)
 #else 
   #define cpat_dma_alloc(a,b,c) dma_alloc_coherent(NULL, a, b, c)
   #define cpat_dma_free(a,b,c) dma_free_coherent(NULL, a, b, c)
   #define cpat_phys_addr(a) (u32)(a->buffer_list_dma_addr)
 #endif

 // ---------------------------------------------------------------------------
 // We need a pointer to the driver struct, but in 2.4.x it is a member
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 0)
   #define cpat_pdriver(a) &(a->driver)
 #else 
   #define cpat_pdriver(a) (a->driver)
 #endif

 // ---------------------------------------------------------------------------
 // The driver ops pointers used to be directly accessible, but now are burried
 // one layer down in an ops structure...
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 26)
   #define cpat_pdriver_ops(a) &(a->driver)
 #else 
   #define cpat_pdriver_ops(a) (a->driver->ops)
 #endif

 // ---------------------------------------------------------------------------
 // 2.6.x stores the devname for you, while in 2.4.x you have to use kdevname()
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 0)
   #define cpat_name(a) (char*) kdevname( (kdev_t)((a)->device) )
 #else 
   #define cpat_name(a) (a->name)
 #endif

 // ---------------------------------------------------------------------------
 // Device's index.  Stored in 2.6.x, must calculate in 2.4.x
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 0)
   #define cpat_line(a) ( (MINOR(a->device)) - (a->driver.minor_start) )
 #else 
   #define cpat_line(a) (a->index)
 #endif

 // ---------------------------------------------------------------------------
 // After 2.6.10 there is a new module parameter interface 
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 10)
   #define module_param(a,b,c) MODULE_PARM(a,"i")
   #define module_param_array(a,b,c,d) \
	   MODULE_PARM(a, "1-" __MODULE_STRING(MAX_TOTAL_DEVICES) "i")
 #else
   //already up to date
 #endif

 // ---------------------------------------------------------------------------
 // These functions are available in >2.4.29 and in >2.6.10 
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 4, 29) || (LINUX_VERSION_CODE > \
       KERNEL_VERSION(2, 5, 0) && LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 10))
   static inline unsigned int jiffies_to_msecs(const unsigned long j)
   {
   	#if HZ <= 1000 && !(1000 % HZ)
		return (1000 / HZ) * j;
	#elif HZ > 1000 && !(HZ % 1000)
		return (j + (HZ / 1000) - 1)/(HZ / 1000);
	#else
		return (j * 1000) / HZ;
	#endif
   }
   static inline unsigned long msecs_to_jiffies(const unsigned int m)
   {
	if (m > jiffies_to_msecs(MAX_JIFFY_OFFSET))
		return MAX_JIFFY_OFFSET;
	#if HZ <= 1000 && !(1000 % HZ)
		return (m + (1000 / HZ) - 1) / (1000 / HZ);
	#elif HZ > 1000 && !(HZ % 1000)
		return m * (HZ / 1000);
	#else
		return (m * HZ + 999) / 1000;
	#endif
   }
 #else
   //already included in kernel
 #endif

 // ---------------------------------------------------------------------------
 // Before 2.6.10 there was no interface to set sleep interruptible 
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 10)
   #define msleep_interruptible(a) set_current_state(TASK_INTERRUPTIBLE); \
				     schedule_timeout(msecs_to_jiffies((a)))
 #else
   //already up to date
 #endif

 // ---------------------------------------------------------------------------
 // >2.6.10 brings a new ldisc interface.  Access is different now.
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 10)
   #define cpat_ldisc_receive_buf(a,b,c,d) \
	if (a && a->ldisc.receive_buf) a->ldisc.receive_buf(a, b, c, d)
   #define tty_ldisc_flush(a) \
	if(a && a->ldisc.flush_buffer) a->ldisc.flush_buffer(a)
   #define tty_wakeup(a) if(a && ((a->flags)&(1<<TTY_DO_WRITE_WAKEUP)) \
        && a->ldisc.write_wakeup) (a->ldisc.write_wakeup)(a)
 #elif LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 27)
   static void cpat_ldisc_receive_buf(struct tty_struct *tty,
			      const __u8 *data, char *flags, int count)
   {
	struct tty_ldisc *ld;
	if (!tty)
		return;
	ld = tty_ldisc_ref(tty);
	if (ld) {
		if (ld->receive_buf)
			ld->receive_buf(tty, data, flags, count);
		tty_ldisc_deref(ld);
	}
   }
 #else
   static void cpat_ldisc_receive_buf(struct tty_struct *tty,
			      const __u8 *data, char *flags, int count)
   {
	struct tty_ldisc *ld;
	if (!tty)
		return;
	ld = tty_ldisc_ref(tty);
	if (ld) {
		if (ld->ops->receive_buf)
			ld->ops->receive_buf(tty, data, flags, count);
		tty_ldisc_deref(ld);
	}
   }
 #endif
 
 // ---------------------------------------------------------------------------
 // TTY interface changes beyond 2.6.16... this is all we need to fix it.
 // HDLC interface change... removal of setcarrier()
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16)
   int cpat_insert_flip_char(struct tty_struct *tty, u8 data, u8 flag)
   {
	   if (!tty || tty->flip.count >= TTY_FLIPBUF_SIZE) return 0;
	   
	   tty->flip.count++;
	   *tty->flip.flag_buf_ptr++ = flag;
	   *tty->flip.char_buf_ptr++ = data;

	   return 1;
   }
 #else
   int cpat_insert_flip_char(struct tty_struct *tty, u8 data, u8 flag)
   {
	   return tty_insert_flip_char(tty, data, flag);
   }
 #endif

 // --------------------------------------------------------------------------- 
 // INIT_WORK() Macro Drops the third arg in > 2.6.20
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 5, 0)
   #define CPAT_INIT_WORK(a,b,c) (*a).sync = 0; (*a).routine = b; (*a).data = c
 #elif LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 20)
   #define CPAT_INIT_WORK(a, b, c) INIT_WORK(a, b, c)
 #else
   #define CPAT_INIT_WORK(a, b, c) INIT_WORK(a, b)
 #endif

 // ---------------------------------------------------------------------------
 // 2.6.20 has a new task pointer type
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 20)
   typedef void* r56_pContext;
 #else
   typedef struct work_struct* r56_pContext;
 #endif

 // ---------------------------------------------------------------------------
 // container_of() is a function introduced in 2.6.20
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 20)
   #define cpat_contain(a,b,c) (b*)a
 #else
   #define cpat_contain(a,b,c) container_of(a,b,c)
 #endif

 // ---------------------------------------------------------------------------
 // 2.6.20 drops the third arg in interrupt routine 
 // ---------------------------------------------------------------------------
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 20)
   //already up to date
 #else
   #define r56_interrupt(a, b, c) r56_interrupt(a, b)
 #endif

#endif //__Z16C32_H__

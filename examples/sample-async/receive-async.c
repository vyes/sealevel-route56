/*
 * sample program to receive async data
 *
 * $Id: receive-async.c,v 1.1 2007/04/13 13:17:56 thomasw Exp $
 *
 * Sealevel and Route56 are registered trademarks
 * of Sealevel Systems, Inc.
 *
 * This code is released under the GNU General Public License (GPL)
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <termios.h>
#include <errno.h>

#include "route56.h"

int main(int argc, char* argv[])
{
	int fd;
	int rc;
	int disc = N_TTY;
	R56_PARAMS params;
	int sigs;
	FILE *fp = NULL;
	int size = 4096;
	int count;
	unsigned char *buf = malloc(size);
	char *devname;
	struct termios termios;

	if (argc > 1)
		devname = argv[1];
	else
		devname = "/dev/ttySL1";

	printf("receive raw data on %s\n", devname);

	if (!buf ) {
		printf("can't allocate buffer\n");
		return ENOMEM;
	}

	/* open file to capture received data */
	fp = fopen("data", "wb");
	if (fp == NULL) {
		printf("fopen(data) error=%d %s\n",
		       errno, strerror(errno));
		return errno;
	}

	/* open device with O_NONBLOCK to ignore DCD */
	fd = open(devname, O_RDWR | O_NONBLOCK, 0);
	if (fd < 0) {
		printf("open of %s error=%d %s\n",
		       devname, errno, strerror(errno));
		return errno;
	}

	/* set N_TTY (standard async) line discipline */
	rc = ioctl(fd, TIOCSETD, &disc);
	if(rc < 0) {
		printf("ERROR, can't set line discipline error=%d %s\n",
		       errno, strerror(errno));
		return rc;
	}

	/* get current device parameters */
	rc = ioctl(fd, R56_IOCGPARAMS, &params);
	if (rc < 0) {
		printf("ioctl(R56_IOCGPARAMS) on device %s"
		       " failed with err=%d %s\n",
		       devname, errno, strerror(errno));
		return rc;
	}

	/* modify device parameters */

	params.mode = R56_MODE_ASYNC;
	params.loopback = 0;

	/* set current device parameters */
	rc = ioctl(fd, R56_IOCSPARAMS, &params);
	if (rc < 0) {
		printf("ioctl(R56_IOCSPARAMS) on device %s"
		       " failed with err=%d %s\n",
		       devname, errno, strerror(errno));
		return rc;
	}


	/* set async parameters with termios functions */

	rc = tcgetattr(fd, &termios);
	if (rc < 0) {
		printf("tcgetattr() error=%d %s\n", errno, strerror(errno));
		return rc;
	}

	termios.c_iflag = 0;
	termios.c_oflag = 0;
	termios.c_cflag = CREAD | CS8 | HUPCL | CLOCAL;
	termios.c_lflag = 0;
	termios.c_cc[VTIME] = 0;
	termios.c_cc[VMIN]  = 1;
	cfsetospeed(&termios, B9600);
	cfsetispeed(&termios, B9600);

	rc = tcsetattr(fd, TCSANOW, &termios);
	if (rc < 0) {
		printf("tcsetattr() error=%d %s\n", errno, strerror(errno));
		return rc;
	}

	printf("Turning on RTS and DTR\n");
	sigs = TIOCM_RTS + TIOCM_DTR;
	rc = ioctl(fd, TIOCMBIS, &sigs);
	if(rc < 0) {
		printf("can't assert DTR/RTS error=%d %s\n",
		       errno, strerror(errno));
		return rc;
	}

	/* set device to blocking mode for reads and writes */
	fcntl(fd, F_SETFL, fcntl(fd,F_GETFL) & ~O_NONBLOCK);


	for (;;) {
		memset(buf, 0, size);

		rc = read(fd, buf, size);
		if (rc < 0) {
			printf("read() error=%d %s\n",
			       errno, strerror(errno));
			break;
		}
		if (rc == 0) {
			printf("read() returned with no data\n");
			continue;
		}

		printf("received %d bytes of data\n", rc);

		/* write received data to capture file */
		count = fwrite(buf, sizeof(char), rc, fp);
		if (count != rc) {
			printf("data file write error=%d (%s)\n",
			       errno, strerror(errno));
			break;
		}
		fflush(fp);
	}

	printf("Turning off RTS and DTR\n");
	sigs = TIOCM_RTS + TIOCM_DTR;
	rc = ioctl(fd, TIOCMBIC, &sigs);
	if (rc < 0) {
		printf("can't negate DTR/RTS error=%d %s\n",
		       errno, strerror(errno));
		return rc;
	}

	return 0;
}



/*
 * i2c.c
 *
 * Copyright (C) 2015 Miguelsoft NETwork
 *
 *  Created on: 28 may. 2015
 *      Author: Miguel Angel Abella Gonzalez
 *
 * See the file LICENSE, included with the source code, for details about
 * copyright and restrictions.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

/*
 * For more details about i2c, see:
 * https://www.kernel.org/doc/Documentation/i2c/dev-interface
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "i2c.h"


/*
 * Open a i2c device node represented by it's block device (i.e: /dev/i2c-<port>)
 *
 * Returns a handle for the i2c device node. This handle is the result of
 * calling open() with O_RDWR flag and it's value must be checked by the user.
 */
int i2c_open(int i2c_port)
{
	char filename[20];
	snprintf(filename, 19, "/dev/i2c-%d", i2c_port);
	return open(filename, O_RDWR);
}

/*
 * Close the handle of an i2c port previously opened.
 *
 * Returns 0 on success.
 */
int i2c_close(int i2c_port)
{
	return close(i2c_port);
}

/*
 * Initialize an I2C device.
 *
 * Internally, it is just a wrapper for the call to open().
 *
 * Returns 0 on success.
 * When this function fails, the variable errno will be set.
 */
int i2c_init(i2c_ctl_t * i2c)
{
	int ret = i2c_open(i2c->port);
	i2c->port_fd = ret;
	return ret < 0 ? ret : 0;
}

/*
 * Release an I2C device.
 *
 * All calls to any i2c_* function will fail after releasing the device.
 * A subsequent call to i2c_init may succeed.
 *
 * Internally it is just a wrapper for the call to close().
 *
 * Returns 0 on success.
 * When this function fails, the variable errno will be set.
 */
int i2c_release(i2c_ctl_t * i2c)
{
	return i2c_close(i2c->port_fd);
}

/*
 * Selects a slave device attached to the I2C.
 *
 * The "slave address" field must be provided for this call to succeed.
 *
 * Returns 0 on success.
 * When this function fails, the variable errno will be set.
 * Errno is also returned as "-errno".
 */
int i2c_select_slave(const i2c_ctl_t i2c)
{
	if (ioctl(i2c.port_fd, I2C_SLAVE, i2c.slave_address) < 0)
		return -errno;
	return 0;
}

/*
 * Reads data from a slave device.
 *
 * The return value is -1 on error or the actual data on success.
 */
int i2c_read(const i2c_ctl_t i2c, __u8 i2c_slave_register)
{
	return i2c_smbus_read_byte_data(i2c.port_fd, i2c_slave_register);
}

/*
 * Writes data to a slave device.
 *
 * The slave register address in which to write must be passed in "i2c_register"
 * The buffer must be a valid buffer able to hold "len" elements.
 *
 * Returns the number of written bytes into the buffer.
 */
int i2c_write(const i2c_ctl_t i2c, __u8 i2c_register, const __u8 * buffer, __u8 len)
{
	return i2c_smbus_write_i2c_block_data(i2c.port_fd, i2c_register, len, buffer);
}

int i2c_write_byte(const i2c_ctl_t i2c, __u8 i2c_register, const __u8 data)
{
	return i2c_smbus_write_byte_data(i2c.port_fd, i2c_register, data);
}
